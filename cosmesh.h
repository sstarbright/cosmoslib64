#ifndef __COSMESH_H
#define __COSMESH_H

#include <libdragon.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include "costrans.h"

#ifndef MESH_MAT_SEGMENT_PLACEHOLDER
// Defines the segment placeholder for Mesh Matrices.
#define MESH_MAT_SEGMENT_PLACEHOLDER 1
#endif

#ifndef MODEL_CACHE_NAME_SIZE
// Defines the size of a CachedModel's name
#define MODEL_CACHE_NAME_SIZE 50
#endif
#ifndef ANIMATION_NAME_SIZE
// Defines the size of a SkinnedAnimation's name
#define ANIMATION_NAME_SIZE 30
#endif

// A structure that stores T3DModel information, to be held within a ModelCache.
typedef struct CachedModel CachedModel;
typedef struct Mesh3DM Mesh3DM;
// A structure that stores SkinnedAnimation information, to be held within a Mesh3DM.
typedef struct SkinnedAnimation SkinnedAnimation;
typedef struct MeshSkeleton MeshSkeleton;
typedef struct Bone3DM Bone3DM;
typedef struct LagBone3DM LagBone3DM;
// A structure that allows for different behavioural code to be called when drawing a renderable module.
// Has a basic function pointer for draw that may be "overloaded" for varying behaviours
typedef struct Render3DM Render3DM;

struct CachedModel {
    T3DModel* model;
    int uses;
    char name[MODEL_CACHE_NAME_SIZE];
};

struct SkinnedAnimation {
    T3DAnim animation;
    char name[ANIMATION_NAME_SIZE];
};

struct MeshSkeleton {
    T3DSkeleton skeleton;
    BoneTransform* bone_trans;
};

struct Bone3DM {
    Trans3DM transform;
    T3DMat4* bone_matrix;
};

struct LagBone3DM {
    Trans3DM transform;
    T3DBone* bone;
}

void cosmesh_init();
void model_cache_create(uint32_t size);
CachedModel* load_model_into_cache(const char* location, const char* name);
void model_cache_clear();

struct Render3DM {
    // The 3D transform of this Render3D module.
    Trans3DM transform;
    // The output color of this Render3D module.
    color_t color;
    // The function called when this Module is drawn.
    void (*draw)(Render3DM* self, float delta, uint32_t frame_buffer);
};

// Create a Render3D module, initializing its members.
void render3dm_create(Render3DM* module, const char* name);
// A basic function to be called upon Render3DM draw.
void render3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon Render3DM death.
void render3dm_death(Module* self);

struct Mesh3DM {
    Render3DM render;
    T3DMat4FP* matrix_buffer;

    CachedModel* model;

    bool has_skeleton;
    int max_skeletons;
    int num_skeletons;
    T3DSkeleton* skeletons;

    int max_animations;
    int num_animations;
    SkinnedAnimation* animations;
    SkinnedAnimation* looping;
    SkinnedAnimation* oneshot;

    rspq_block_t* block;
    uint32_t frame_buffer;
};

void mesh3dm_create(Mesh3DM* module, const char* name, uint32_t skeleton_count, uint32_t animation_count);
void mesh3dm_create(Module* self, float _);
void mesh3dm_draw(Render3DM* self, float delta, uint32_t frame_buffer);
void mesh3dm_death(Module* self);
void mesh3dm_simple_death(Mesh3DM* self);

#endif