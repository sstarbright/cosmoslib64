#ifndef __COSSTATE_H
#define __COSSTATE_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3danim.h>

#ifndef STATE_NAME_SIZE
// Defines the size of a BasicState's name
#define STATE_NAME_SIZE 30
#endif

typedef struct StateM StateM;
typedef struct BasicState BasicState;
typedef struct BasicTrans BasicTrans;

struct StateM {
    Module module;
    BasicState* current_state;
    BasicState* states;
    uint32_t state_count;
    uint32_t state_size;
};

struct BasicState {
    StateM* module;
    char name[STATE_NAME_SIZE];
    BasicTrans* transitions;
    void (*entry)(BasicState* state);
    void (*life)(BasicState* state, float delta);
    void (*exit)(BasicState* state);
};

struct BasicTrans {
    BasicState state;
    BasicState* from;
    BasicState* to;
    float time;
    bool (*param)(BasicState* from, BasicTrans* to);
};

void statem_create(StateM* module, const char* name, uint32_t state_count, uint32_t state_size);
void statem_life(StateM* self, float delta);
void statem_death(StateM* self);
void statem_simple_death(StateM* self);

#endif