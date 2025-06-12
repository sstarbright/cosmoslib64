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
typedef struct actor_scr_o_t actor_scr_o_t;
typedef struct actor_o_t actor_o_t;
typedef struct bs_mesh_t bs_mesh_t;
typedef struct sk_mesh_t sk_mesh_t;
typedef struct so_mesh_t so_mesh_t;

void load_ctx(context_o_t* ctx, void* data);
void unload_ctx(context_o_t* ctx);
void req_ctx(context_o_t* ctx, int entry, void* data);
void unreq_ctx(context_o_t* ctx, int entry);

void load_scn(scene_o_t* scn, const char* path, int acts, void* data);
void unload_scn(scene_o_t* scn);

void load_act(actor_scr_o_t* act, const char* path, int size, int max, void* data);
void unload_act(actor_scr_o_t* act);
actor_o_t* new_act(actor_scr_o_t* act, void* data);
void update_act(actor_scr_o_t* act, float delta, int buffer);
void kill_act(actor_o_t* act);

void load_scr(script_o_t* script, const char* path, bool auto_init, void* data);
void unload_scr(script_o_t* script);

struct script_o_t {
    void* dso;
    void (*up)(float delta, int buffer, void* data);
};

struct context_o_t {
    script_o_t script;
    bool loaded;
    char* path;
    int depends;
    bool auto_close;
};

struct scene_o_t {
    script_o_t script;
    actor_scr_o_t* actors;
};

struct actor_scr_o_t {
    script_o_t script;
    void* inst;
    int max_inst;
    int last_empty;
    int size;
    void (*new)(actor_o_t* self, void* data);
    void (*kill)(actor_o_t* self);
};

struct actor_o_t {
    actor_scr_o_t* base;
    bool exists;
    int index;
};

struct mesh_t {
    T3DModel* model;
    color_t color;
    rspq_block_t* block;
    T3DMat4FP* mat_buffer;
};
struct sk_mesh_t {
    T3DModel* model;
    color_t color;
    rspq_block_t* block;
    T3DMat4FP* mat_buffer;
    T3DSkeleton skel;
    T3DSkeleton b_skel;
};
struct so_mesh_t {
    T3DObject** layers;
    color_t color;
    T3DMat4FP* mat_buffer;
};

#endif