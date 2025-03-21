#ifndef __COSSTATE_H
#define __COSSTATE_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3danim.h>

typedef struct StateM StateM;
typedef struct BasicSt BasicSt;
typedef struct BasicTr BasicTr;

struct StateM {
    Module module;
    BasicSt* current_state;
    BasicSt* states;
    int state_count;
};

struct BasicSt {
    StateM* module;
    int id;
    BasicTr* transitions;
    void (*entry)(BasicSt* state);
    void (*life)(BasicSt* state, float delta);
    void (*exit)(BasicSt* state);
};

struct BasicTr {
    BasicSt state;
    BasicSt* from;
    BasicSt* to;
    float time;
    bool (*param)(BasicSt* from, BasicTr* to);
};

void statem_create(StateM* module, int state_count, int state_size);
void statem_life(Module* self, float delta);
void statem_death(Module* self);
void statem_simple_death(StateM* self);

void basicst_create(StateM* machine, int slot);
void basicst_entry(BasicSt* state);
void basicst_life(BasicSt* state, float delta);
void basicst_exit(BasicSt* state);

void basictr_create(StateM* machine, int from, int to);
void basictr_param(BasicSt* from, BasicSt* to);
void basictr_entry(BasicSt* state);
void basictr_life(BasicSt* state, float delta);

#endif