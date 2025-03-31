#ifndef __COSSTATE_H
#define __COSSTATE_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>

typedef struct StateM StateM;
typedef struct BasicSt BasicSt;
typedef struct StateTr StateTr;

struct StateM {
    Module module;
    T3DSkeleton* main_skel;
    T3DSkeleton* blend_skel;
    BasicSt* current_state;
    BasicSt** states;
    int target_state;
    int state_count;
};

struct BasicSt {
    StateM* module;
    int id;
    StateTr* transitions;
    int* trans_ids;
    int trans_count;
    bool leaving;
    void (*entry)(BasicSt* state, float time);
    void (*life)(BasicSt* state, float delta, bool is_first, float strength);
    void (*exit)(BasicSt* state, bool has_time);
    void (*death)(BasicSt* state);
};

struct StateTr {
    BasicSt state;
    BasicSt* from;
    BasicSt* to;
    float time;
    float elapsed;
    bool (*param)(BasicSt* from, BasicSt* to);
};

void statem_create(StateM* module, int state_count, int state_size);
void statem_life(Module* self, float delta);
void statem_death(Module* self);
void statem_simple_death(StateM* self);

void basicst_create(StateM* machine, BasicSt* state, int slot, int trans_count);
void basicst_entry(BasicSt* state, float time);
void basicst_life(BasicSt* state, float delta, bool is_first, float strength);
void basicst_exit(BasicSt* state, bool has_time);
void basicst_death(BasicSt* state);

void statetr_create(StateM* machine, int from, int to, float time);
bool statetr_param(BasicSt* from, BasicSt* to);
void statetr_entry(BasicSt* state, float time);
void statetr_life(BasicSt* state, float delta, bool is_first, float strength);

#endif