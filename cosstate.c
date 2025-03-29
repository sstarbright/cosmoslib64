#include "cosstate.h"

void statem_create(StateM* module, int state_count, int state_size) {
    m_create((Module*)module);
    
    ((Module*)module)->life = statem_life;
    ((Module*)module)->death = statem_death;

    module->current_state = NULL;
    module->target_state = -1;
    module->state_count = state_count;

    module->states = malloc(state_size * state_count);
}
void statem_life(Module* self, float delta) {
    StateM* module = (StateM*)self;
    BasicSt* current_state = module->current_state;
    if (current_state) {
        int target_state = module->target_state;
        if (target_state >= 0 && current_state->trans_count > 0) {
            int found_trans = -1;
            int current_trans = -1;
            int trans_count = current_state->trans_count;
            int* trans_ids = current_state->trans_ids;

            while (found_trans < 0 && ++current_trans < trans_count)
                if (trans_ids[current_trans] == target_state)
                    found_trans = current_trans;
            if (found_trans >= 0) {
                module->current_state = current_state->transitions[found_trans];
                current_state = module->current_state;
                module->target_state = -1;
            }
        }
        current_state->life(current_state, delta, true, 1.f);
    }
}
void statem_death(Module* self) {
    statem_simple_death(((StateM*)self));

    free(self);
}
void statem_simple_death(StateM* self) {
    free(self->states);
}


void basicst_create(StateM* machine, int slot, int trans_count) {
    BasicSt* state = &machine->states[slot];
    state->module = machine;
    state->id = slot;
    state->entry = basicst_entry;
    state->life = basicst_life;
    state->exit = basicst_exit;

    state->trans_count = trans_count;
    state->transitions = malloc(sizeof(StateTr)*trans_count);
    state->trans_ids = malloc(sizeof(int)*trans_count);
}
void basicst_entry(BasicSt* state) {

}
void basicst_life(BasicSt* state, float delta, bool is_first, float strength) {

}
void basicst_exit(BasicSt* state) {

}

void statetr_create(StateM* machine, int from, int to) {

}
void statetr_param(BasicSt* from, BasicSt* to) {

}
void statetr_entry(BasicSt* state) {

}
void statetr_life(BasicSt* state, float delta, bool is_first, float strength) {

}