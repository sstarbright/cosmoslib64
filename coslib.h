#ifndef __COSLIB_H
#define __COSLIB_H

#include <libdragon.h>
//#include <rspq_profile.h>
#include <t3d/t3ddebug.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>

// Initialize the library with various parameters.
void coslib_init(int asset_compress, resolution_t resolution, bitdepth_t color_depth, int num_buffers, gamma_t gamma_correct, filter_options_t filter);
// Uninitialize the library.
void coslib_end();

typedef struct script_o_t script_o_t;
typedef struct context_o_t context_o_t;
typedef struct scene_o_t scene_o_t;
typedef struct actor_scr_o_t actor_scr_o_t;
typedef struct actor_o_t actor_o_t;
typedef struct bs_mesh_t bs_mesh_t;
typedef struct sk_mesh_t sk_mesh_t;
typedef struct so_mesh_t so_mesh_t;

// Load a context into a context script object.
void load_ctx(context_o_t* ctx, void* data);
// Unload a context from a context script object.
void unload_ctx(context_o_t* ctx);
// Request a context for use in a script.
// Each script that requests a context will add to it's depends count.
// If the context's script isn't loaded, a request will cause the script to be loaded in.
void req_ctx(context_o_t* ctx, int entry, void* data);
// Release a context from being depended upon by a script.
// Each script that unrequests a context will subtract from it's depends count.
// If the context's script is loaded, with auto_close set to true, and depends becomes zero, unrequesting will unload the script.
void unreq_ctx(context_o_t* ctx, int entry);

// Load a scene into a scene script object, using path as the file location.
void load_scn(scene_o_t* scn, const char* path, int acts, void* data);
// Unload a scene from a scene script object.
void unload_scn(scene_o_t* scn);

// Load an actor into an actor script object, using path as the file location.
void load_act(actor_scr_o_t* act, const char* path, int size, int max, void* data);
// Unload an actor from an actor script object.
void unload_act(actor_scr_o_t* act);
// Instance a new actor on an actor script object.
// Returns NULL if all instances are taken up.
actor_o_t* new_act(actor_scr_o_t* act, void* data);

// Update an actor script, which updates each individual actor instance.
void update_act(actor_scr_o_t* act, float delta, int buffer);
// Kill an actor instance.
void kill_act(actor_o_t* act);

// Load a script into a script object, using path as the file location.
// The Initialization function of the script will be called automatically if auto_init is true.
void load_scr(script_o_t* script, const char* path, bool auto_init, void* data);
// Unload a script from a script object.
void unload_scr(script_o_t* script);

// An object that holds the basic script data.
struct script_o_t {
    // The handle for the loaded DSO.
    void* dso;
    // The update function for this script.
    void (*up)(float delta, int buffer, void* data);
};

// An object that holds context script data.
struct context_o_t {
    // The basic script data of this context.
    script_o_t script;
    // Whether this context is loaded or not.
    bool loaded;
    // The path to the .dso file of this context.
    char* path;
    // The number of scripts that depend on this context.
    int depends;
    // Whether this context should automatically close when there are no dependant scripts.
    bool auto_close;
};

// An object that holds scene script data.
struct scene_o_t {
    // The basic script data of this scene.
    script_o_t script;
    // An array of actor scripts that this scene will use.
    actor_scr_o_t* actors;
};

// An object that holds actor script data.
struct actor_scr_o_t {
    // The basic script data of this actor script.
    script_o_t script;
    // An array of actor instances stored in this script.
    void* inst;
    // The max number of actors this script can instance.
    int max_inst;
    // The last empty instance.
    // Used when deciding what index to setup new instances within.
    int last_empty;
    // The size of each actor object within this script.
    int size;
    // The index of the latest actor this script has instanced.
    // Used when updating instances, to hopefully trim the number of instances being queried on each update.
    int used;
    // The function called when new actors are instanced with this script.
    void (*new)(actor_o_t* self, void* data);
    // The function called when actors are killed with this script.
    void (*kill)(actor_o_t* self);
};

// An object that holds basic actor data.
struct actor_o_t {
    // A pointer to this actor's base script.
    actor_scr_o_t* base;
    // Whether this actor object has been instanced or not.
    bool exists;
    // This actor's index within the base script's instance array
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