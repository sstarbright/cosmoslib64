#include "costrans.h"

Module* trans3Dmodule_create() {
    Module* new_module = module_create("Trans3D");

    Trans3DModule* new_trans = malloc(sizeof(Trans3DModule));
    t3d_mat4_identity(&new_trans->transform);

    new_module->data = new_trans;
    new_module->death = trans3Dmodule_death;
    module_init(new_module);
    return new_module;
}

void trans3Dmodule_death(Module* self) {
    Trans3DModule* trans_module = (Trans3DModule*)self->data;
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