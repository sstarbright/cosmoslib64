#include "costrans.h"

void trans3dm_create(Trans3DM* module) {
    m_create((Module*)module);
    ((Module*)module)->death = trans3dm_death;
    module->matup = trans3dm_matup;

    module->position = (T3DVec3){{0.f, 0.f, 0.f}};
    module->scale = (T3DVec3){{1.f, 1.f, 1.f}};
    t3d_quat_identity(&module->rotation);

    module->fp_matrix = malloc(sizeof(T3DMat4FP));
    module->matrix = malloc(sizeof(T3DMat4));
    t3d_mat4fp_identity(module->fp_matrix);
    t3d_mat4_identity(module->matrix);

    module->parent = NULL;
    module->child = NULL;
    module->prev = module;
    module->next = module;
}
void trans3dm_add_child(Trans3DM* parent, Trans3DM* child) {
    Trans3DM* first_child = parent->child;
    if (first_child)
        linked_add_to_list(first_child->prev, first_child, child, offsetof(Trans3DM, prev), offsetof(Trans3DM, next));
    else
        parent->child = child;
    child->parent = parent;
}
void trans3dm_pop_child(Trans3DM* child) {
    Trans3DM* next_child = child->next;
    Trans3DM* parent = child->parent;
    if (linked_pop_from_list(child, offsetof(Trans3DM, prev), offsetof(Trans3DM, next))) {
        if (parent)
            parent->child = next_child;
    } else {
        if (parent)
            parent->child = NULL;
    }
    child->parent = NULL;
}
void trans3dm_update_matrix(Trans3DM* module) {
    T3DMat4 parent_mat;
    if (module->parent)
        memcpy(&parent_mat, module->parent->matrix, sizeof(T3DMat4));
    else
        t3d_mat4_identity(&parent_mat);
    module->matup(module, &parent_mat);
}
void trans3dm_matup(Trans3DM* self, const T3DMat4* ref_mat) {
    T3DMat4 local_mat;
    T3DMat4* global_mat = self->matrix;
    t3d_mat4_from_srt(&local_mat, self->scale.v, self->rotation.v, self->position.v);
    t3d_mat4_mul(global_mat, ref_mat, &local_mat);

    Trans3DM* start_child_module = self->child;
    if (start_child_module) {
        start_child_module->matup(start_child_module, global_mat);
        Trans3DM* child_module = start_child_module->next;
        while (child_module && start_child_module != child_module) {
            child_module->matup(child_module, global_mat);
            child_module = child_module->next;
        }
    }
    t3d_mat4_to_fixed(self->fp_matrix, global_mat);
}

void trans3dm_death(Module* self) {
    trans3dm_simple_death((Trans3DM*)self);
    
    free(self);
}
void trans3dm_simple_death(Trans3DM* module) {
    trans3dm_pop_child((Trans3DM*)module);

    free(((Trans3DM*)module)->fp_matrix);
}

void trans2dm_create(Trans2DM* module) {
    m_create((Module*)module);
    ((Module*)module)->death = trans2dm_death;

    module->position.x = 0.f;
    module->position.y = 0.f;
    module->position.z = 0.f;
    module->scale.x = 1.f;
    module->scale.y = 1.f;
    module->scale.z = 1.f;
    module->rotation = 0.f;
}
void trans2dm_death(Module* self) {
    trans2dm_simple_death((Trans2DM*)self);
    
    free(self);
}
void trans2dm_simple_death(Trans2DM* module) {

}