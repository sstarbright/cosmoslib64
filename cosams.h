#ifndef __COSAMS_H
#define __COSAMS_H

#include "coslink.h"
#include <libdragon.h>
#include <t3d/t3d.h>

// A structure that holds Actors in a linked list.
// Allows important Actors to be indexed for quick access.
typedef struct Stage Stage;
// A structure that holds Modules in a linked list.
// Allows important Modules to be indexed for quick access.
typedef struct Actor Actor;
// A structure that allows for different behavioural code to be attached to actors.
// Has basic function pointers that may be "overloaded" for varying behaviours
typedef struct Module Module;
// A structure that stores 3D transformations, transforms them into 4x4 Matrices, and propagates changes down to each of their children.
// If changes are made to the local transformations of a parent and their children, only the parent needs to 
typedef struct Trans3DM Trans3DM;
// A structure that stores 2D transformations.
typedef struct Trans2DM Trans2DM;
// A structure that allows for different behavioural code to be called when drawing a renderable module.
// Has a basic function pointer for draw that may be "overloaded" for varying behaviours
typedef struct Render3DM Render3DM;
// A structure that manages and sets a Viewport for T3D.
// Can draw a Viewport.
typedef struct Camera3DM Camera3DM;

struct Stage {
    bool enabled;
    bool visible;
    // The first Actor in the linked list.
    Actor* actor;
    Camera3DM* camera;
    Render3DM* light;
    uint32_t light_count;
    uint32_t current_light;
    Render3DM* draw;
    // The hashtable that stores indexed Actors.
    Actor** actor_table;
    color_t ambient_color;
    color_t fog_color;
    bool is_fog;
    float fog_start;
    float fog_end;
    Stage* prev;
    Stage* next;
};

// Initialize a Stage with basic data.
void stage_init(Stage* stage, int index_size);
// Add an Actor to this Stage's linked list (and Actor Table if slot > -1)
void stage_add_actor(Stage* stage, Actor* actor, int slot);
void stage_add_draw(Stage* stage, Render3DM* draw);
// Update a Stage and its Actors.
void stage_life(Stage* self, float delta);
void stage_predraw(Stage* self, float delta, uint32_t matrix_id);
void stage_set_fog(Stage* self);
void stage_draw(Stage* self, float delta, uint32_t matrix_id);
// Kill a Stage and its Actors.
void stage_kill(Stage* stage);
void stage_simple_kill(Stage* stage);

struct Actor {
    // The Stage that this Actor is on.
    Stage* stage;
    // Is this Actor active?
    bool enabled;
    // The index slot this Actor occupies. -1 if not indexed.
    int index_slot;
    // The first Module in the linked list.
    Module* module;
    // The hashtable that stores indexed Modules.
    Module** module_table;
    // The number of modules this Actor may index.
    int index_size;
    // An integer used to store extra attributes to attach to an actor through bitwise.
    int attributes;
    // The previous Actor in the linked list.
    Actor* prev;
    // The next Actor in the linked list.
    Actor* next;
};

// Initialize an Actor with basic data.
void actor_init(Actor* actor, int index_size);
// Add a Module to this Actor's linked list (and Module Table if slot > -1)
void actor_add_module(Actor* actor, Module* module, int slot);
// Update an Actor and its Modules.
void actor_life(Actor* actor, float delta);
// Kill an Actor and its Modules.
void actor_kill(Actor* actor);
// Kill an Actor and its Modules, without updating the linked list or Actor Table.
void actor_simple_kill(void* actor_pointer);

struct Module {
    // The Actor this Module is attached to.
    Actor* actor;
    // Is this Module active?
    bool enabled;
    // The index slot this Module occupies. -1 if not indexed.
    int index_slot;
    // The function called when this Module is activated.
    void (*active)(Module* self);
    // The function called when this Module is updated.
    void (*life)(Module* self, float delta);
    // The backup for life function when this Module is disabled.
    void (*d_life)(Module* self, float delta);
    // The function called when this Module is deactivated.
    void (*inactive)(Module* self);
    // The function called when this Module is killed.
    void (*death)(Module* self);
    // The previous Module in the linked list.
    Module* prev;
    // The next Module in the linked list.
    Module* next;
};

// Create a module, initializing its members.
void m_create(Module* module);
// Enable this module. (Modules are enabled by default)
void m_enable(Module* module);
// Disable this module.
void m_disable(Module* module);
// Kill a module.
void m_kill(Module* module);
// Kill a module without updating the linked list or Module Table.
void m_simple_kill(void* module_pointer);

// A basic function to be called upon Module activation after being deactivated.
void m_active(Module* self);
// A basic function to be called upon Module update.
void m_life(Module* self, float delta);
// A basic function to be called upon Module deactivation.
void m_inactive(Module* self);
// A basic function to be called upon Module death.
void m_death(Module* self);

struct Trans3DM {
    // The base module.
    Module module;
    // The local position of this module.
    T3DVec3 position;
    // The local scale of this module.
    T3DVec3 scale;
    // The local rotation of this module.
    T3DQuat rotation;
    // The global matrix (fixed-point) of this module.
    T3DMat4FP* fp_matrix;
    // The global matrix of this module.
    T3DMat4* matrix;
    // The parent Trans3D of this module.
    Trans3DM* parent;
    // The child Trans3D linked list of this module.
    Trans3DM* child;
    // The previous Trans3D in this linked list.
    Trans3DM* prev;
    // The next Trans3D in this linked list.
    Trans3DM* next;
    // The function called when this Trans3D module's matrix is updated.
    void (*matup)(Trans3DM* self, const T3DMat4* ref_mat);
};

struct Render3DM {
    // The 3D transform of this Render3D module.
    Trans3DM transform;
    // The output color of this Render3D module.
    color_t color;
    // The function to be called just before drawing begins.
    // (Useful in collectively updating everything related to rendering before drawing pipeline begins)
    void (*predraw)(Render3DM* self, float delta, uint32_t frame_buffer);
    // The function called when this Render3D Module is drawn.
    void (*draw)(Render3DM* self, float delta, uint32_t frame_buffer);
    Render3DM* prev;
    Render3DM* next;
};

#endif