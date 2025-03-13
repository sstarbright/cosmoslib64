#ifndef __COSLITE_H
#define __COSLITE_H
#include <libdragon.h>
#include "cosmesh.h"

// A structure that adds a Directional Light to a T3D Scene being rendered.
// Can draw a Directional Light.
typedef struct DirLite3DModule DirLite3DModule;
// A structure that adds a Point Light to a T3D Scene being rendered.
// Can draw a Point Light.
typedef struct PntLite3DModule PntLite3DModule;

// Fetch the number of Directional/Point lights currently active.
uint32_t coslite_get_count();
// Refreshes the drawn light counter.
void coslite_new_frame();

struct DirLite3DModule {
    // A render module.
    Render3DModule render;
    // The direction this light is pointing.
    T3DVec3 direction;
};

// Create a DirectionalLight3D module, initializing its members.
void dirlite3d_module_create(DirLite3DModule* module, const char* name);
// A basic function to be called up DirLite3DModule matrix update.
void dirlite3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat);
// A basic function to be called upon DirLite3DModule draw.
void dirlite3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon DirLite3DModule death.
void dirlite3d_module_death(Module* self);

struct PntLite3DModule {
    // A render module.
    Render3DModule render;
    // The position in space this light resides at.
    T3DVec3 position;
    // The size of this light (Must be from 0.0-1.0).
    float size;
};

// Create a PointLight3D module, initializing its members.
void pntlite3d_module_create(PntLite3DModule* module, const char* name);
// A basic function to be called up PntLite3DModule matrix update.
void pntlite3d_module_matup(Trans3DModule* self, const T3DMat4* global_mat);
// A basic function to be called upon PntLite3DModule draw.
void pntlite3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon PntLite3DModule death.
void pntlite3d_module_death(Module* self);

#endif