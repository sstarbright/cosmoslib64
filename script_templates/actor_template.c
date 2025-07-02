// Make sure to include the main header file, so actors can access your contexts.
#include "../../src/main.h"

// The actor script object for this script.
actor_scr_o_t* act_self;

// Called when this actor script is initialized.
void init(script_o_t* self, void* data) {
    act_self = (actor_scr_o_t*)self;
}

// Called when a new actor is instanced from this script.
void new(actor_o_t* self, void* data) {
}

// Called when an actor is updated.
// data contains a pointer to the actor_o_t being updated.
void up(float delta, int buffer, void* data) {
    actor_o_t* self = (actor_o_t*)data;
}

// Called when an actor is killed.
void kill(actor_o_t* self) {
}

// Called when this actor script is closed.
void close() {
}