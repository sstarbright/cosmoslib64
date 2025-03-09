#ifndef __COSMESH_H
#define __COSMESH_H

#include <libdragon.h>
#include <t3d/t3dmodel.h>
#include "costrans.h"

#ifndef MESH_MAT_SEGMENT_PLACEHOLDER
// Defines the segment placeholder for Mesh Matrices.
#define MESH_MAT_SEGMENT_PLACEHOLDER 1
#endif

#ifndef MODEL_CACHE_NAME_SIZE
// Defines the size of a CachedModel's name
#define MODEL_CACHE_NAME_SIZE 50
#endif

typedef struct CachedModel CachedModel;
typedef struct CachedSModel CachedSModel;
typedef struct Mesh3DModule Mesh3DModule;
// A structure that allows for different behavioural code to be called when drawing a renderable module.
// Has a basic function pointer for draw that may be "overloaded" for varying behaviours
typedef struct Render3DModule Render3DModule;

struct CachedModel {
    T3DModel* model;
    int uses;
    char name[MODEL_CACHE_NAME_SIZE];
};

void cosmesh_init();
void model_cache_create(uint32_t size);
CachedModel* load_model_into_cache(const char* location, const char* name);
void model_cache_clear();

struct Render3DModule {
    // The 3D transform of this Render3D module.
    Trans3DModule transform;
    // The function called when this Module is drawn.
    void (*draw)(Render3DModule* self, float delta, uint32_t frame_buffer);
};

// Create a Render3D module, initializing its members.
void render3d_module_create(Render3DModule* module, const char* name);
// A basic function to be called upon Render3DModule initialization.
void render3d_module_birth(Module* self);
// A basic function to be called upon Render3DModule draw.
void render3d_module_draw(Render3DModule* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon Render3DModule death.
void render3d_module_death(Module* module);

struct Mesh3DModule {
    Render3DModule render;
    T3DMat4FP* matrix_buffer;
    CachedModel* model;
    rspq_block_t* block;
    uint32_t frame_buffer;
};

void mesh3D_module_create(Mesh3DModule* module, const char* name);
void mesh3D_module_birth(Module* self);
void mesh3D_module_draw(Render3DModule* self, float delta, uint32_t frame_buffer);
void mesh3D_module_death(Module* self);

#endif