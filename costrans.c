#include "costrans.h"

void trans3d_module_create(Trans3DModule* module, const char* name) {
    module_create((Module*)module, name);
    ((Module*)module)->death = trans3d_module_death;
    module->matup = trans3d_module_matup;

    module->position.x = 0.f;
    module->position.y = 0.f;
    module->position.z = 0.f;
    module->scale.x = 1.f;
    module->scale.y = 1.f;
    module->scale.z = 1.f;
    module->rotation.x = 0.f;
    module->rotation.y = 0.f;
    module->rotation.z = 0.f;

    module->matrix = malloc(sizeof(T3DMat4FP));
    t3d_mat4fp_identity(module->matrix);

    module->parent = NULL;
    module->child = NULL;
    module->prev = module;
    module->next = module;
}
void trans3d_add_child(Trans3DModule* parent, Trans3DModule* child) {
    if (parent->child) {
        linked_add_to_list(parent->child->prev, parent->child, child, offsetof(Trans3DModule, prev), offsetof(Trans3DModule, next));
    } else {
        parent->child = child;
    }
}
void trans3d_pop_child(Trans3DModule* child) {
    Trans3DModule* next_child = child->next;
    if (linked_pop_from_list(child, offsetof(Trans3DModule, prev), offsetof(Trans3DModule, next))) {
        if (child->parent)
            child->parent->child = NULL;
    } else {
        if (child->parent)
            child->parent->child = next_child;
    }
}
void trans3d_get_matrix(Trans3DModule* module, T3DMat4* matRes) {
    T3DMat4 local_mat;
    T3DMat4 parent_mat;
    if (module->parent)
        trans3d_get_matrix(module->parent, &parent_mat);
    else
        t3d_mat4_identity(&parent_mat);
    t3d_mat4_from_srt_euler(&local_mat, module->scale.v, module->rotation.v, module->position.v);
    t3d_mat4_mul(matRes, &parent_mat, &local_mat);
}
void trans3d_update_matrix(Trans3DModule* module) {
    T3DMat4 parent_mat;
    if (module->parent)
        trans3d_get_matrix(module->parent, &parent_mat);
    else
        t3d_mat4_identity(&parent_mat);
    trans3d_update_matrix_from_ref(module, &parent_mat);
}
void trans3d_update_matrix_from_ref(Trans3DModule* module, T3DMat4* parent_mat) {
    T3DMat4 local_mat;
    T3DMat4 global_mat;
    t3d_mat4_from_srt_euler(&local_mat, module->scale.v, module->rotation.v, module->position.v);
    t3d_mat4_mul(&global_mat, parent_mat, &local_mat);

    Trans3DModule* start_child_module = module->child;
    if (start_child_module) {
        trans3d_update_matrix_from_ref(start_child_module, &global_mat);
        Trans3DModule* child_module = start_child_module->next;
        while (child_module && start_child_module != child_module) {
            trans3d_update_matrix_from_ref(child_module, &global_mat);
            child_module = child_module->next;
        }
    }
    module->matup(module, &global_mat);
    t3d_mat4_to_fixed(module->matrix, &global_mat);
}
void trans3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat) {

}

void trans3d_module_death(Module* self) {
    trans3d_simple_module_death((Trans3DModule*)self);
    
    free((Trans3DModule*)self);
}
void trans3d_simple_module_death(Trans3DModule* module) {
    trans3d_pop_child((Trans3DModule*)module);

    free(((Trans3DModule*)module)->matrix);
}

void trans2d_module_create(Trans2DModule* module, const char* name) {
    module_create((Module*)module, name);
    ((Module*)module)->death = trans2d_module_death;

    module->position.x = 0.f;
    module->position.y = 0.f;
    module->position.z = 0.f;
    module->scale.x = 1.f;
    module->scale.y = 1.f;
    module->scale.z = 1.f;
    module->rotation = 0.f;
}
void trans2d_module_death(Module* self) {
    trans2d_simple_module_death((Trans2DModule*)self);
    
    free((Trans2DModule*)self);
}
void trans2d_simple_module_death(Trans2DModule* module) {

}