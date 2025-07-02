// Make sure to include the main header file, so this scene can access your contexts.
#include "../../src/main.h"

// The actor script object for this script.
scene_o_t* scn_self;

// Called before this scene script is initialized.
// Expects a return value for however many actor scripts this scene will use
int acts() {
    return 0;
}

// Called when this scene script is initialized.
void init(script_o_t* self, void* data) {
    scn_self = (scene_o_t*)self;
}

// Called when the scene is updated.
void up(float delta, int buffer, void* data) {
}

// Called when this scene script is closed.
void close() {
}