#include "cosstate.h"

void statem_create(StateM* module, int state_count, int state_size) {
    m_create((Module*)module);
    
    ((Module*)module)->life = statem_life;
    ((Module*)module)->death = statem_death;

    module->current_state = NULL;
    module->state_count = state_count;

    module->states = malloc(state_size * state_count);
}
void statem_life(Module* self, float delta) {
    BasicSt* current_state = ((StateM*)self)->current_state;
    if (current_state) {
        current_state->life(current_state, delta);
    }
}
void statem_death(Module* self) {
    statem_simple_death(((StateM*)self));

    free((StateM*)self);
}
void statem_simple_death(StateM* self) {
    free((BasicSt*)(self->states));
}

void basicst_create(StateM* machine, int slot) {
    
}
void basicst_entry(BasicSt* state) {

}
void basicst_life(BasicSt* state, float delta) {

}
void basicst_exit(BasicSt* state) {

}

void basictr_create(StateM* machine, int from, int to) {

}
void basictr_param(BasicSt* from, BasicSt* to) {

}
void basictr_entry(BasicSt* state) {

}
void basictr_life(BasicSt* state, float delta) {

}