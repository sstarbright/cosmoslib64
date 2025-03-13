#ifndef __COSCAM_H
#define __COSCAM_H

#include <libdragon.h>
#include "cosmesh.h"

// A structure that manages and sets a Viewport for T3D.
// Can draw a Viewport.
typedef struct Camera3DModule Camera3DModule;

struct Camera3DModule {
    // A render module.
    Render3DModule render;
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
void camera3d_module_create(Camera3DModule* module, const char* name);
// A basic function to be called upon Camera3DModule life.
void camera3d_module_life(Module* self, float delta);
// A basic function to be called upon Camera3DModule matrix update.
void camera3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat);
// A basic function to be called upon Camera3DModule draw.
void camera3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon Camera3DModule death.
void camera3d_module_death(Module* self);
// A basic function to be called upon Camera3DModule death.
void camera3d_simple_module_death(Camera3DModule* self);

#endif