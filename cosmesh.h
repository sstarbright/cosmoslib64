#ifndef __COSMESH_H
#define __COSMESH_H

#include <libdragon.h>
#include "cosstate.h"
#include "costrans.h"

#ifndef MESH_MAT_SEGMENT_PLACEHOLDER
// Defines the segment placeholder for Mesh Matrices.
#define MESH_MAT_SEGMENT_PLACEHOLDER 1
#endif

// A structure that stores T3DModel information, to be held within a ModelCache.
typedef struct CachedModel CachedModel;
// A structure that extracts a T3DBone's transform into a Trans3D Module.
// For use in parenting other Trans3Ds to a T3DBone.
typedef struct Bone3DM Bone3DM;
// A structure that delays a T3DBone's transformations.
// For use in making "streaking" effects, often for weapons or fast-moving objects.
typedef struct LagBone3DM LagBone3DM;
typedef struct AnimSt AnimSt;
typedef struct AnimEv AnimEv;
// A structure that uses a Render3D Module to draw a mesh with Primitive Color and 3D transformations.
typedef struct Mesh3DM Mesh3DM;

struct CachedModel {
    // The Tiny3D model that this cache stores.
    T3DModel* model;
    // The number of uses this Model currently has.
    int uses;
};

struct Bone3DM {
    // The 3D transform of this Bone3D module.
    Trans3DM transform;
    // The Tiny3D Bone's matrix, which gets pushed to the transform.
    T3DMat4* bone_matrix;
};

struct LagBone3DM {
    // The 3D transform of this LagBone3D module.
    Trans3DM transform;
    // The Tiny3D Bone to manipulate.
    T3DBone* t3d_bone;
    // The speed to catch the bone's position up with it's "intended" position.
    float speed;
};

// Initializes Cosmos Mesh module.
void cosmesh_init();
// Creates the Cosmos Mesh model cache, with a specific number of models.
void model_cache_create(int size);
// Loads a model into the model cache, at a specific slot.
CachedModel* load_model_into_cache(const char* location, int slot, bool unshaded);
// Clears the model cache.
void model_cache_clear();

// Create a Render3D module, initializing its members.
void render3dm_create(Render3DM* module);
// A basic function to be called upon Render3DM predraw.
void render3dm_predraw(Render3DM* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon Render3DM draw.
void render3dm_draw(Render3DM* module, float delta, uint32_t frame_buffer);
// A basic function to be called upon Render3DM death.
void render3dm_death(Module* self);

struct AnimSt {
    BasicSt state;
    T3DAnim anim;
    T3DAnim blend_anim;
    AnimEv* events;
    int event_count;
    int next_event;
    float time;
    float time_offset;
};

AnimSt* animst_create(StateM* machine, AnimSt* anim, int slot, int trans_count, int ev_count, T3DModel* source, const char* name);
void animst_entry(BasicSt* state, float time);
void animst_life(BasicSt* state, float delta, bool is_first, float strength);
void animst_exit(BasicSt* state, bool has_time);
void animst_death(BasicSt* state);

struct AnimEv {
    float time;
    bool enabled;
    void (*action)(AnimSt* state, AnimEv* event);
    void* data;
    bool free_data;
};

AnimEv* animev_create(AnimSt* state, int slot, float time);
void animev_action(AnimSt* state, AnimEv* event);

void transev_action(AnimSt* state, AnimEv* event);

struct Mesh3DM {
    // The 3D renderer of this Mesh3D module.
    Render3DM render;
    // The Matrix Buffer for this Mesh3D module.
    T3DMat4FP* matrix_buffer;

    // The model that this module renders.
    CachedModel* model;

    // Does this module have a T3D skeleton?
    bool has_skeleton;
    // The number of skeletons this module can have.
    int max_skeletons;
    // The number of skeletons this module currently has.
    int num_skeletons;
    // The list of skeletons this module has.
    T3DSkeleton* skeletons;

    // The currently running drawing block for this module. (Base by default)
    rspq_block_t* block;
    // The base drawing block that was created for this module.
    rspq_block_t* base_block;
    // The transparent drawing block created for this module.
    rspq_block_t* trans_block;
};

// Create a Mesh3D module, initializing its members.
// Set trans_or and trans_xor to customize the color combiner for the trans_block.
void mesh3dm_create(Stage* stage, Mesh3DM* module, int model_slot, int skeleton_count);
// A basic function to be called upon Mesh3DM predraw.
void mesh3dm_predraw(Render3DM* self, float delta, uint32_t frame_buffer);
// A basic function to be called upon Mesh3DM draw.
void mesh3dm_draw(Render3DM* self, float delta, uint32_t frame_buffer);
void no_fog_draw(Render3DM* self, float delta, uint32_t frame_buffer);
void no_depth_draw(Render3DM* self, float delta, uint32_t frame_buffer);
void no_fog_or_depth_draw(Render3DM* self, float delta, uint32_t frame_buffer);
// A basic function to be called upon Mesh3DM death.
void mesh3dm_death(Module* self);
// A basic function to be called upon Mesh3DM death (without freeing the module itself).
void mesh3dm_simple_death(Mesh3DM* self);

void bone3dm_create(Bone3DM* module, T3DSkeleton* skeleton, const char* bone);
void bone3dm_life(Module* self, float delta);
void bone3dm_matup(Trans3DM* self, const T3DMat4* ref_mat);

#endif