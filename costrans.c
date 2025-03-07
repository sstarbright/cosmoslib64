#include "costrans.h"

Module* trans3D_module_create() {
    Module* new_module = module_create("Trans3D");

    Trans3DModule* new_trans = malloc(sizeof(Trans3DModule));
    *new_trans = (Trans3DModule){
        .position = {{0.f,0.f,0.f}},
        .scale = {{1.f,1.f,1.f}},
        .rotation = {{0.f,0.f,0.f}},
        .matrix = malloc_uncached(sizeof(T3DMat4FP))
    };
    t3d_mat4fp_identity(new_trans->matrix);
    new_trans->parent = NULL;
    new_trans->child = NULL;
    new_trans->module = new_module;
    new_trans->prev = new_trans;
    new_trans->next = new_trans;

    new_module->data = new_trans;
    new_module->life = trans3D_module_life;
    new_module->death = trans3D_module_death;
    return new_module;
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

void trans3D_module_life(Module* self, float _) {
    Trans3DModule* trans_module = (Trans3DModule*)self->data;

    t3d_mat4fp_from_srt_euler(trans_module->matrix, trans_module->scale.v, trans_module->rotation.v, trans_module->position.v);
}
void trans3D_module_death(Module* self) {
    Trans3DModule* trans_module = (Trans3DModule*)self->data;
    trans3D_pop_child(trans_module);

    free_uncached(trans_module->matrix);
    free(trans_module);
}

Module* trans2Dmodule_create() {
    Module* new_module = module_create("Trans2D");

    Trans2DModule* new_trans = malloc(sizeof(Trans2DModule));
    *new_trans = (Trans2DModule) {
        .position = {{0.f,0.f,0.f}},
        .scale = {{1.f,1.f,1.f}},
        .rotation = 0.f,
        .module = new_module
    };

    new_module->data = new_trans;
    new_module->death = trans2Dmodule_death;
    return new_module;
}
void trans2Dmodule_death(Module* self) {
    Trans2DModule* trans_module = (Trans2DModule*)self->data;
    free(trans_module);
}