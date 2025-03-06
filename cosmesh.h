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
typedef struct SMesh3DModule SMesh3DModule;

struct CachedModel {
    T3DModel* model;
    int uses;
    char name[MODEL_CACHE_NAME_SIZE];
};

void cosmesh_init();
void model_cache_create(uint32_t size);
void model_cache_clear();

struct Mesh3DModule {
    Trans3DModule* transform;
    T3DMat4FP* matrix_buffer;
    T3DModel* model;
    rspq_block_t* block;
};

Module* mesh3D_module_create(const char* name);
void mesh3D_module_birth(Module* self)
void mesh3D_module_draw(Module* self, float delta, uint32_t frame_buffer);
void mesh3D_module_death(Module* self);

struct SMesh3DModule {
    Trans3DModule* transform;
    T3DMat4FP* matrix_buffer;
    T3DModel* model;
    rspq_block_t* block;
};

Module* smesh3D_module_create();
void smesh3D_module_draw(Module* self, float delta, uint32_t frame_buffer);
void smesh3D_module_death(Module* self);

#endif