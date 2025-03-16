#ifndef __COSAMS_H
#define __COSAMS_H

#include "coshash.h"
#include "coslink.h"
#include "cosdebug.h"
#include <libdragon.h>

// A structure that holds Actors in a linked list.
// Allows important Actors to be indexed for quick access.
typedef struct Stage Stage;
// A structure that holds Modules in a linked list.
// Allows important Modules to be indexed for quick access.
typedef struct Actor Actor;
// A structure that holds a tag string.
typedef struct Tag Tag;
// A structure that allows for different behavioural code to be attached to actors.
// Has basic function pointers that may be "overloaded" for varying behaviours
typedef struct Module Module;

#ifndef STAGE_NAME_SIZE
// Defines the size of a Stage's name.
#define STAGE_NAME_SIZE 50
#endif

#ifndef STAGE_ACTORTABLE_SIZE
// Defines the number of Actors that may be indexed per Stage.
#define STAGE_ACTORTABLE_SIZE 30
// Defines the size of each Stage's Actor Hash Table.
#define STAGE_ACTORTABLE_MEMSIZE STAGE_ACTORTABLE_SIZE*sizeof(void*)
#endif

struct Stage {
    // The name of this Stage.
    char name[STAGE_NAME_SIZE];
    // The first Actor in the linked list.
    Actor* actor;
    // The hashtable that stores indexed Actors.
    Actor* actor_table[STAGE_ACTORTABLE_SIZE];
};

// Initialize a Stage with basic data.
void stage_init(Stage* stage, const char* name);
// Add an Actor to this Stage's linked list (and hash table if indexed)
void stage_add_actor(Stage* stage, Actor* actor, bool indexed);
// Update a Stage and its Actors.
void stage_life(Stage* self, float delta);
// Kill a Stage and its Actors.
void stage_kill(Stage* stage);

#ifndef ACTOR_NAME_SIZE
// Defines the size of an Actor's name.
#define ACTOR_NAME_SIZE 25
#endif

#ifndef ACTOR_MODULETABLE_SIZE
// Defines the number of Modules that may be indexed per Actor.
#define ACTOR_MODULETABLE_SIZE 10
// Defines the size of each Actor's Module Hash Table.
#define ACTOR_MODULETABLE_MEMSIZE ACTOR_MODULETABLE_SIZE*sizeof(void*)
#endif

#ifndef ACTOR_TAGTABLE_SIZE
// Defines the number of Tags that may be attached per Actor.
#define ACTOR_TAGTABLE_SIZE 10
// Defines the size of each Actor's Tag Hash Table.
#define ACTOR_TAGTABLE_MEMSIZE ACTOR_TAGTABLE_SIZE*sizeof(void*)
#endif

#ifndef ACTOR_TAG_SIZE
// Defines the size of each Tag attached to an Actor.
#define ACTOR_TAG_SIZE 15
#endif

struct Tag {
    char tag[ACTOR_TAG_SIZE];
};

struct Actor {
    // The name of this Actor.
    char name[ACTOR_NAME_SIZE];
    // The Stage that this Actor is on.
    Stage* stage;
    // Is this Actor active?
    bool enabled;
    // Is this Actor indexed on Stage?
    bool indexed;
    // The first Module in the linked list.
    Module* module;
    // The hashtable that stores indexed Modules.
    Module* module_table[ACTOR_MODULETABLE_SIZE];
    // The hashtable that stores indexed Tags.
    Tag* tags[ACTOR_TAGTABLE_SIZE];
    // The previous Actor in the linked list.
    Actor* prev;
    // The next Actor in the linked list.
    Actor* next;
};

// Initialize an Actor with basic data.
void actor_init(Actor* actor, const char* name);
// Add a Module to this Actor's linked list (and hash table if indexed)
void actor_add_module(Actor* actor, Module* module, bool indexed);
// Get an indexed Module from this Actor's hash table.
Module* actor_get_indexed_module(Actor* actor, const char* name);
// Add a tag to this Actor's tag table.
bool actor_add_tag(Actor* actor, const char* tag);
// Check if a tag is present in this Actor's tag table.
bool actor_has_tag(Actor* actor, const char* tag);
// Remove a tag from this actor's tag table.
void actor_pop_tag(Actor* actor, const char* tag);
// Update an Actor and its Modules.
void actor_life(Actor* actor, float delta);
// Kill an Actor and its Modules.
void actor_kill(Actor* actor);
// Kill an Actor and its Modules, without updating the linked list or hash table.
void actor_simple_kill(void* actor_pointer);

#ifndef MAX_MODULE_NAME_SIZE
// Defines the max size of a Module's name.
#define MAX_MODULE_NAME_SIZE 25
#endif

struct Module {
    // The name of this Module.
    char name[MAX_MODULE_NAME_SIZE];
    // The Actor this Module is attached to.
    Actor* actor;
    // Is this Module active?
    bool enabled;
    // Is this Module indexed on Actor?
    bool indexed;
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
void m_create(Module* module, const char* name);
// Enable this module. (Modules are enabled by default)
void m_enable(Module* module);
// Disable this module.
void m_disable(Module* module);
// Kill a module.
void m_kill(Module* module);
// Kill a module without updating the linked list or hash table.
void m_simple_kill(void* module_pointer);

// A basic function to be called upon Module activation after being deactivated.
void m_active(Module* self);
// A basic function to be called upon Module update.
void m_life(Module* self, float delta);
// A basic function to be called upon Module deactivation.
void m_inactive(Module* self);
// A basic function to be called upon Module death.
void m_death(Module* self);

#endif