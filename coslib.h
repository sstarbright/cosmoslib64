#ifndef __COSLIB_H
#define __COSLIB_H

#include <libdragon.h>
//#include <rspq_profile.h>
#include <t3d/t3ddebug.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>

void coslib_init(int asset_compress, resolution_t resolution, bitdepth_t color_depth, int num_buffers, gamma_t gamma_correct, filter_options_t filter);
void coslib_end();

typedef struct script_o_t script_o_t;
typedef struct context_o_t context_o_t;
typedef struct scene_o_t scene_o_t;
typedef struct mesh3d_t mesh3d_t;
typedef struct sk_mesh3d_t sk_mesh3d_t;
typedef struct so_mesh3d_t so_mesh3d_t;

struct script_o_t {
    void* dso;
    void (*up)(script_o_t* self, float delta, int buffer);
};

struct scene_o_t {
    script_o_t script;
    context_o_t* context;
};

struct context_o_t {
    script_o_t script;
    scene_o_t scene;
};

struct mesh3d_t {
    T3DModel* model;
    color_t color;
    rspq_block_t* block;
    T3DMat4FP* mat_buffer;
};
struct sk_mesh3d_t {
    T3DModel* model;
    color_t color;
    rspq_block_t* block;
    T3DMat4FP* mat_buffer;
    T3DSkeleton skel;
    T3DSkeleton b_skel;
};
struct so_mesh3d_t {
    T3DObject** layers;
    color_t color;
    T3DMat4FP* mat_buffer;
};

#endif