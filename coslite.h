#ifndef __COSLITE_H
#define __COSLITE_H
#include <libdragon.h>
#include "cosmesh.h"

// A structure that stores a Directional Light's data.
// A structure that adds a Directional Light to a T3D Scene being rendered.
// Can draw a Directional Light.
typedef struct DirLite3DM DirLite3DM;
// A structure that adds a Point Light to a T3D Scene being rendered.
// Can draw a Point Light.
typedef struct PntLite3DM PntLite3DM;

struct DirLite3DM {
    // A render module.
    Render3DM render;
    // The direction this light is pointing.
    T3DVec3 direction;
};

// Create a DirectionalLight3D module, initializing its members.
void dirlite3dm_create(Stage* stage, DirLite3DM* module);
// A basic function to be called up DirLite3DM matrix update.
void dirlite3dm_matup(Trans3DM* self, const T3DMat4* global_mat);
// A basic function to be called upon DirLite3DM draw.
void dirlite3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon DirLite3DM death.
void dirlite3dm_death(Module* self);

struct PntLite3DM {
    // A render module.
    Render3DM render;
    // The position in space this light resides at.
    T3DVec3 position;
    // The size of this light (Must be from 0.0-1.0).
    float size;
};

// Create a PointLight3D module, initializing its members.
void pntlite3dm_create(Stage* stage, PntLite3DM* module);
// A basic function to be called up PntLite3DM matrix update.
void pntlite3dm_matup(Trans3DM* self, const T3DMat4* global_mat);
// A basic function to be called upon PntLite3DM draw.
void pntlite3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon PntLite3DM death.
void pntlite3dm_death(Module* self);

#endif