#include "costrans.h"

Module* trans3Dmodule_create() {
    Module* new_module = module_create("Trans3D");

    Trans3DModule* new_trans = malloc(sizeof(Trans3DModule));
    new_trans->matrix = malloc_uncached(sizeof(T3DMat4FP));
    new_trans->position = {0f,0f,0f};
    new_trans->scale = {1f,1f,1f};
    t3d_quat_identity(&new_trans->rotation);
    t3d_mat4fp_identity(new_trans->matrix);

    new_module->data = new_trans;
    new_module->life = trans3Dmodule_life;
    new_module->death = trans3Dmodule_death;
    module_init(new_module);
    return new_module;
}
void trans3Dmodule_life(Module* self, float _) {
    Trans3DModule* trans_module = (Trans3DModule*)self->data;
    t3d_mat4fp_from_srt_euler(trans_module->matrix, trans_module->scale, trans_module->rotation, trans_module->position);
}
void trans3Dmodule_death(Module* self) {
    Trans3DModule* trans_module = (Trans3DModule*)self->data;
    free_uncached(trans_module->matrix);
    free(trans_module);
}

Module* trans2Dmodule_create() {
    Module* new_module = module_create("Trans2D");

    Trans2DModule* new_trans = malloc(sizeof(Trans2DModule));
    new_trans->position = {0f,0f,0f};
    new_trans->scale = {1f,1f,1f};
    new_trans->rotation = 0f;

    new_module->data = new_trans;
    new_module->death = trans2Dmodule_death;
    module_init(new_module);
    return new_module;
}
void trans2Dmodule_death(Module* self) {
    Trans2DModule* trans_module = (Trans2DModule*)self->data;
    free(trans_module);
}