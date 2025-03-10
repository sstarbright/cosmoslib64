#ifndef __COSTRANS_H
#define __COSTRANS_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>

typedef struct Trans3DModule Trans3DModule;
typedef struct Trans2DModule Trans2DModule;

struct Trans3DModule {
    Module module;
    T3DVec3 position;
    T3DVec3 scale;
    T3DVec3 rotation;
    T3DMat4 matrixf;
    T3DMat4FP* matrix;
    Trans3DModule* parent;
    Trans3DModule* child;
    Trans3DModule* prev;
    Trans3DModule* next;
};

void trans3D_module_create(Trans3DModule* module, const char* name);
void trans3D_add_child(Trans3DModule* parent, Trans3DModule* child);
void trans3D_pop_child(Trans3DModule* child);
void trans3D_get_matrix(Trans3DModule* module, T3DMat4* matRes);
void trans3D_update_matrix(Trans3DModule* module);
void trans3D_update_matrix_from_ref(Trans3DModule* module, T3DMat4* ref_mat);

void trans3D_module_death(Module* self);
void trans3D_simple_module_death(Trans3DModule* module);

struct Trans2DModule {
    Module module;
    T3DVec3 position;
    T3DVec3 scale;
    float rotation;
};

void trans2D_module_create(Trans2DModule* module, const char* name);
void trans2D_module_death(Module* self);
void trans2D_simple_module_death(Trans2DModule* module);

#endif