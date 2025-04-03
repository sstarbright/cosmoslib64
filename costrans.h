#ifndef __COSTRANS_H
#define __COSTRANS_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>

#define ONE_SCALE 1.f/64.f

// A structure that stores 3D transformations, transforms them into 4x4 Matrices, and propagates changes down to each of their children.
// If changes are made to the local transformations of a parent and their children, only the parent needs to 
typedef struct Trans3DM Trans3DM;
// A structure that stores 2D transformations.
typedef struct Trans2DM Trans2DM;

struct Trans3DM {
    // The base module.
    Module module;
    // The local position of this module.
    T3DVec3 position;
    // The local scale of this module.
    T3DVec3 scale;
    // The local rotation of this module.
    T3DQuat rotation;
    // The global matrix (fixed-point) of this module.
    T3DMat4FP* fp_matrix;
    // The global matrix of this module.
    T3DMat4* matrix;
    // The parent Trans3D of this module.
    Trans3DM* parent;
    // The child Trans3D linked list of this module.
    Trans3DM* child;
    // The previous Trans3D in this linked list.
    Trans3DM* prev;
    // The next Trans3D in this linked list.
    Trans3DM* next;
    // The function called when this Trans3D module's matrix is updated.
    void (*matup)(Trans3DM* self, const T3DMat4* global_mat);
};

// Create a Trans3D module, initializing its members.
void trans3dm_create(Trans3DM* module);
// Add a child Trans3D to the parent Trans3D's linked child list.
void trans3dm_add_child(Trans3DM* parent, Trans3DM* child);
// Remove a child Trans3D from its parent's linked child list.
void trans3dm_pop_child(Trans3DM* child);
// Updates a Trans3D module based on its 3D transformations and its parent's matrix.
// This update is propagated to children, so calling it is unneeded for children if parent is going to be updated.
void trans3dm_update_matrix(Trans3DM* module);
// Updates a Trans3D module based on its 3D transformations and a reference matrix.
void trans3dm_update_matrix_from_ref(Trans3DM* module, T3DMat4* ref_mat);
// A basic function to be called upon Trans3DM matrix update.
void trans3dm_module_matup(Trans3DM* self, const T3DMat4* global_mat);


// A basic function to be called upon Trans3DM death.
void trans3dm_death(Module* self);
// A basic function to be called upon Trans3DM death (without freeing the module itself).
void trans3dm_simple_death(Trans3DM* module);

struct Trans2DM {
    Module module;
    T3DVec3 position;
    T3DVec3 scale;
    float rotation;
};

void trans2dm_create(Trans2DM* module);
void trans2dm_death(Module* self);
void trans2dm_simple_death(Trans2DM* module);

#endif