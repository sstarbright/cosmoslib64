#include "cosstate.h"

void statem_create(StateM* module, const char* name, uint32_t state_count, uint32_t state_size) {
    m_create((Module*)module, name);
    ((Module*)module)->life = statem_life;
    ((Module*)module)->death = statem_death;

    module->current_state = NULL;
    module->state_count = state_count;
    module->state_size = state_size;

    module->states = malloc(state_size * state_count);
}
void statem_life(Module* self, float delta) {
    BasicState* current_state = (StateM*)self->current_state;
    if (current_state) {
        current_state->life(current_state, delta);
    }
}
void statem_death(Module* self) {
    statem_simple_death(self);

    free((StateM*)self);
}
void statem_simple_death(Module* self) {
    free((BasicState*)((StateM*)self->states));
    (StateM*)self->current_state = NULL;
}