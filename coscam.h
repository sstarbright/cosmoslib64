#ifndef __COSCAM_H
#define __COSCAM_H

#include <libdragon.h>
#include "cosmesh.h"

struct Camera3DM {
    // A render module.
    Render3DM render;
    // The current viewport of this module.
    T3DViewport viewport;
    // The view matrix of this module.
    T3DMat4 matrix;
    // The field of view (in radians) of this camera's viewport.
    float fov;
    // The near clipping plane of this camera's viewport.
    float near;
    // The far clipping plane of this camera's viewport.
    float far;
};

// Create a Camera3D module, initializing its members.
void camera3dm_create(Camera3DM* module);
// A basic function to be called upon Camera3DM life.
void camera3dm_life(Module* self, float delta);
// A basic function to be called upon Camera3DM matrix update.
void camera3dm_matup(Trans3DM* self, const T3DMat4* ref_mat);
// A basic function to be called upon Camera3DM draw.
void camera3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon Camera3DM death.
void camera3dm_death(Module* self);
// A basic function to be called upon Camera3DM death.
void camera3dm_simple_death(Camera3DM* self);

#endif