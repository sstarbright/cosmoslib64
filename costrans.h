#ifndef __COSTRANS_H
#define __COSTRANS_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>

typedef struct Trans3DM Trans3DM;
typedef struct Trans2DM Trans2DM;

struct Trans3DM {
    Module module;
    T3DVec3 position;
    T3DVec3 scale;
    T3DVec3 rotation;
    T3DMat4FP* matrix;
    Trans3DM* parent;
    Trans3DM* child;
    Trans3DM* prev;
    Trans3DM* next;
    void (*matup)(Trans3DM* self, const T3DMat4* global_mat);
};

void trans3dm_create(Trans3DM* module, const char* name);
void trans3dm_add_child(Trans3DM* parent, Trans3DM* child);
void trans3dm_pop_child(Trans3DM* child);
void trans3dm_get_matrix(Trans3DM* module, T3DMat4* matRes);
void trans3dm_update_matrix(Trans3DM* module);
void trans3dm_update_matrix_from_ref(Trans3DM* module, T3DMat4* ref_mat);
void trans3dm_module_matup(Trans3DM* self, const T3DMat4* global_mat);

void trans3dm_death(Module* self);
void trans3dm_simple_death(Trans3DM* module);

struct Trans2DM {
    Module module;
    T3DVec3 position;
    T3DVec3 scale;
    float rotation;
};

void trans2dm_create(Trans2DM* module, const char* name);
void trans2dm_death(Module* self);
void trans2dm_simple_death(Trans2DM* module);

#endif