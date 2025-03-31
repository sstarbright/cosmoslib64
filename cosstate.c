#include "cosstate.h"

#define MAX(a,b) (a) > (b) ? (a) : (b)
#define MIN(a,b) (a) < (b) ? (a) : (b)
#define CLAMP(x, min, max) MIN(MAX((x), (min)), (max))

void statem_create(StateM* module, int state_count, int state_size) {
    m_create((Module*)module);
    
    ((Module*)module)->life = statem_life;
    ((Module*)module)->death = statem_death;

    module->current_state = NULL;
    module->target_state = 0;
    module->state_count = state_count;
    module->main_skel = NULL;
    module->blend_skel = NULL;

    module->states = malloc(sizeof(void*) * state_count);
    BasicSt** states = module->states;
    for (int i = 0; i < module->state_count; i++) {
        states[i] = NULL;
    }
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
            StateTr* transition;

            while (found_trans < 0 && ++current_trans < trans_count)
                if (trans_ids[current_trans] == target_state) {
                    found_trans = current_trans;
                    transition = (StateTr*)(&current_state->transitions[found_trans]);
                }
            if (found_trans >= 0 && transition->param(transition->from, transition->to)) {
                current_state = (BasicSt*)transition;
                module->current_state = current_state;
                module->target_state = -1;
                current_state->entry(current_state, transition->time);
                
                if (transition->time > .0f) {
                    transition->elapsed = .0f;
                } else {
                    module->current_state = transition->to;
                    current_state = module->current_state;
                }
            }
        }
        current_state->life(current_state, delta, true, 1.f);
    } else {
        current_state = module->states[module->target_state];
        module->current_state = current_state;
        module->target_state = -1;
        current_state->entry(current_state, .0f);
        current_state->life(current_state, delta, true, 1.f);
    }
}
void statem_death(Module* self) {
    statem_simple_death(((StateM*)self));

    free(self);
}
void statem_simple_death(StateM* self) {
    if (self->state_count > 0) {
        BasicSt** states = self->states;
        for (int i = 0; i < self->state_count; i++) {
            if (states[i]) {
                states[i]->death(states[i]);
                free(states[i]);
            }
        }
        free(self->states);
    }
}


void basicst_create(StateM* machine, BasicSt* state, int slot, int trans_count) {
    machine->states[slot] = state;
    state->module = machine;
    state->id = slot;
    state->entry = basicst_entry;
    state->life = basicst_life;
    state->exit = basicst_exit;
    state->death = basicst_death;
    state->leaving = true;

    state->trans_count = trans_count;
    if (trans_count > 0) {
        state->transitions = malloc(sizeof(StateTr)*trans_count);
        state->trans_ids = malloc(sizeof(int)*trans_count);
        int* trans_ids = state->trans_ids;
        for (int i=0; i < trans_count; i++) {
            trans_ids[i] = -1;
        }
    } else {
        state->transitions = NULL;
        state->trans_ids = NULL;
    }
}
void basicst_entry(BasicSt* state, float time) {
    state->leaving = false;
}
void basicst_life(BasicSt* state, float delta, bool is_first, float strength) {

}
void basicst_exit(BasicSt* state, bool has_time) {
    state->leaving = true;
}
void basicst_death(BasicSt* state) {
    if (state->trans_count > 0) {
        free(state->transitions);
        free(state->trans_ids);
    }
}

void statetr_create(StateM* machine, int from, int to, float time) {
    BasicSt* from_state = machine->states[from];
    if (from_state->module) {
        int found_index = -1;
        int current_index = -1;
        int trans_count = from_state->trans_count;
        int* trans_ids = from_state->trans_ids;
        while(found_index < 0 && ++current_index < trans_count) {
            if (trans_ids[current_index] < 0) {
                found_index = current_index;
                trans_ids[current_index] = to;
            }
        }
        if (found_index >= 0) {
            StateTr* transition = &from_state->transitions[found_index];
            transition->from = from_state;
            transition->to = machine->states[to];
            transition->param = statetr_param;
            transition->time = time;
            transition->elapsed = .0f;
            BasicSt* state = &transition->state;
            state->module = machine;
            state->id = -1;
            state->trans_count = 0;
            state->trans_ids = NULL;
            state->transitions = NULL;
            state->leaving = false;
            state->entry = statetr_entry;
            state->life = statetr_life;
            state->exit = basicst_exit;
            state->death = basicst_death;
        }
    }
}
bool statetr_param(BasicSt* from, BasicSt* to) {
    return true;
}
void statetr_entry(BasicSt* state, float time) {
    StateTr* transition = (StateTr*)state;
    transition->from->exit(transition->from, transition->time > .0f);
    transition->to->entry(transition->to, transition->time);
}
void statetr_life(BasicSt* state, float delta, bool is_first, float strength) {
    StateTr* transition = (StateTr*)state;
    float elapsed = transition->elapsed;
    float time = transition->time;
    elapsed += delta;
    float phase = CLAMP(elapsed/time, .0f, 1.f);
    transition->from->life(transition->from, delta, true, 1.f-phase);
    transition->to->life(transition->to, delta, false, phase);
    if (elapsed >= time) {
        StateM* module = state->module;
        module->current_state = transition->to;
    } else {
        transition->elapsed = elapsed;
    }
}