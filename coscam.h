#ifndef __COSCAM_H
#define __COSCAM_H

#include <libdragon.h>
#include "cosmesh.h"

typedef struct Camera3DModule Camera3DModule;

struct Camera3DModule {
    Render3DModule render;
    T3DViewport viewport;
    T3DMat4 matrix;
    float fov;
    float near;
    float far;
};

// Create a Camera3D module, initializing its members.
void camera3d_module_create(Camera3DModule* module, const char* name);
// A basic function to be called upon Camera3DModule initialization.
void camera3d_module_birth(Module* self);
void camera3d_module_life(Module* self, float delta);
// A basic function to be called upon Camera3DModule draw.
void camera3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon Camera3DModule death.
void camera3d_module_death(Module* self);
// A basic function to be called upon Camera3DModule death.
void camera3d_simple_module_death(Camera3DModule* self);

#endif