// Make sure to include the context's header file so this script may define it's global functions.
#include "context_template.h"

// The context script object for this script.
context_o_t* ctx_self;

// Called when this context script is initialized.
void init(script_o_t* self, void* data) {
    ctx_self = (context_o_t*)self;
}

// Called when this context script is updated.
void up(float delta, int buffer, void* data) {
}

// Called when this context script is closed.
void close() {

}