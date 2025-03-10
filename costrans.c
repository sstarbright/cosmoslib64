#include "costrans.h"

void trans3D_module_create(Trans3DModule* module, const char* name) {
    module_create((Module*)module, name);
    ((Module*)module)->death = trans3D_module_death;

    module->position.x = 0.f;
    module->position.y = 0.f;
    module->position.z = 0.f;
    module->scale.x = 1.f;
    module->scale.y = 1.f;
    module->scale.z = 1.f;
    module->rotation.x = 0.f;
    module->rotation.y = 0.f;
    module->rotation.z = 0.f;

    module->matrix = malloc_uncached(sizeof(T3DMat4FP));
    t3d_mat4fp_identity(module->matrix);

    module->parent = NULL;
    module->child = NULL;
    module->prev = module;
    module->next = module;
}
void trans3D_add_child(Trans3DModule* parent, Trans3DModule* child) {
    if (parent->child) {
        linked_add_to_list(parent->child->prev, parent->child, child, offsetof(Trans3DModule, prev), offsetof(Trans3DModule, next));
    } else {
        parent->child = child;
    }
}
void trans3D_pop_child(Trans3DModule* child) {
    Trans3DModule* next_child = child->next;
    if (linked_pop_from_list(child, offsetof(Trans3DModule, prev), offsetof(Trans3DModule, next))) {
        if (child->parent)
            child->parent->child = NULL;
    } else {
        if (child->parent)
            child->parent->child = next_child;
    }
}
void trans3D_get_matrix(Trans3DModule* module, T3DMat4* matRes) {
    T3DMat4 local_mat;
    T3DMat4 parent_mat;
    if (module->parent)
        trans3D_get_matrix(module->parent, &parent_mat);
    else
        t3d_mat4_identity(&parent_mat);
    t3d_mat4_from_srt_euler(&local_mat, module->scale.v, module->rotation.v, module->position.v);
    t3d_mat4_mul(matRes, &parent_mat, &local_mat);
}
void trans3D_update_matrix(Trans3DModule* module) {
    T3DMat4 parent_mat;
    if (module->parent)
        trans3D_get_matrix(module->parent, &parent_mat);
    else
        t3d_mat4_identity(&parent_mat);
    trans3D_update_matrix_from_ref(module, &parent_mat);
}
void trans3D_update_matrix_from_ref(Trans3DModule* module, T3DMat4* parent_mat) {
    T3DMat4 local_mat;
    T3DMat4 global_mat;
    t3d_mat4_from_srt_euler(&local_mat, module->scale.v, module->rotation.v, module->position.v);
    t3d_mat4_mul(&global_mat, parent_mat, &local_mat);

    Trans3DModule* start_child_module = module->child;
    if (start_child_module) {
        trans3D_update_matrix_from_ref(start_child_module, &global_mat);
        Trans3DModule* child_module = start_child_module->next;
        while (child_module && start_child_module != child_module) {
            trans3D_update_matrix_from_ref(child_module, &global_mat);
            child_module = child_module->next;
        }
    }

    t3d_mat4_to_fixed(module->matrix, &global_mat);
}
void trans3D_module_death(Module* self) {
    trans3D_simple_module_death((Trans3DModule*)self);
    
    free((Trans3DModule*)self);
}
void trans3D_simple_module_death(Trans3DModule* module) {
    trans3D_pop_child((Trans3DModule*)module);

    free_uncached(((Trans3DModule*)module)->matrix);
}

void trans2D_module_create(Trans2DModule* module, const char* name) {
    module_create((Module*)module, name);
    ((Module*)module)->death = trans2D_module_death;

    module->position.x = 0.f;
    module->position.y = 0.f;
    module->position.z = 0.f;
    module->scale.x = 1.f;
    module->scale.y = 1.f;
    module->scale.z = 1.f;
    module->rotation = 0.f;
}
void trans2D_module_death(Module* self) {
    trans2D_simple_module_death((Trans2DModule*)self);
    free((Trans2DModule*)self);
}
void trans2D_simple_module_death(Trans2DModule* module) {
    
}