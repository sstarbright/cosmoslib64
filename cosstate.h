#ifndef __COSSTATE_H
#define __COSSTATE_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3danim.h>

typedef struct StateM StateM;
typedef struct BasicSt BasicSt;
typedef struct StateTr StateTr;

struct StateM {
    Module module;
    BasicSt* current_state;
    BasicSt* states;
    int target_state;
    int state_count;
};

struct BasicSt {
    StateM* module;
    int id;
    StateTr* transitions;
    int* trans_ids;
    int trans_count;
    void (*entry)(BasicSt* state);
    void (*life)(BasicSt* state, float delta, bool is_first, float strength);
    void (*exit)(BasicSt* state);
};

struct StateTr {
    BasicSt state;
    BasicSt* from;
    BasicSt* to;
    float time;
    bool (*param)(BasicSt* from, StateTr* to);
};

void statem_create(StateM* module, int state_count, int state_size);
void statem_life(Module* self, float delta);
void statem_death(Module* self);
void statem_simple_death(StateM* self);

void basicst_create(StateM* machine, int slot, int trans_count);
void basicst_entry(BasicSt* state);
void basicst_life(BasicSt* state, float delta, bool is_first, float strength);
void basicst_exit(BasicSt* state);

void statetr_create(StateM* machine, int from, int to);
void statetr_param(BasicSt* from, BasicSt* to);
void statetr_entry(BasicSt* state);
void statetr_life(BasicSt* state, float delta, bool is_first, float strength);

#endif