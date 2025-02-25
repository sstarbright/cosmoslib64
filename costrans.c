#include "costrans.h"

Module* trans3Dmodule_create() {
    Module* new_module = module_create("Trans3D");

    Trans3DModule* new_trans = malloc(sizeof(Trans3DModule));
    *new_trans = (Trans3DModule){
        .position = {{0.f,0.f,0.f}},
        .scale = {{1.f,1.f,1.f}},
        .rotation = {{0.f,0.f,0.f}},
        .matrix = malloc_uncached(sizeof(T3DMat4FP))
    };
    t3d_mat4fp_identity(new_trans->matrix);

    new_module->data = new_trans;
    new_module->life = trans3Dmodule_life;
    new_module->death = trans3Dmodule_death;
    module_init(new_module);
    return new_module;
}
void trans3Dmodule_life(Module* self, float _) {
    Trans3DModule* trans_module = (Trans3DModule*)self->data;
    t3d_mat4fp_from_srt_euler(trans_module->matrix, trans_module->scale.v, trans_module->rotation.v, trans_module->position.v);
}
void trans3Dmodule_death(Module* self) {
    Trans3DModule* trans_module = (Trans3DModule*)self->data;
    free_uncached(trans_module->matrix);
    free(trans_module);
}

Module* trans2Dmodule_create() {
    Module* new_module = module_create("Trans2D");

    Trans2DModule* new_trans = malloc(sizeof(Trans2DModule));
    *new_trans = (Trans2DModule) {
        .position = {{0.f,0.f,0.f}},
        .scale = {{1.f,1.f,1.f}},
        .rotation = 0.f
    };

    new_module->data = new_trans;
    new_module->death = trans2Dmodule_death;
    module_init(new_module);
    return new_module;
}
void trans2Dmodule_death(Module* self) {
    Trans2DModule* trans_module = (Trans2DModule*)self->data;
    free(trans_module);
}