#ifndef __COSTRANS_H
#define __COSTRANS_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

typedef struct Trans3DModule Trans3DModule;
typedef struct Trans2DModule Trans2DModule;

struct Trans3DModule {
    T3DVec3 position;
    T3DVec3 scale;
    T3DVec3 rotation;
    T3DMat4FP* matrix;
};

Module* trans3Dmodule_create();
void trans3Dmodule_life(Module* self, float delta);
void trans3Dmodule_death(Module* self);

struct Trans2DModule {
    T3DVec3 position;
    T3DVec3 scale;
    float rotation;
};

Module* trans2Dmodule_create();
void trans2Dmodule_death(Module* self);

#endif