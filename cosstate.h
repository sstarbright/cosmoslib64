#ifndef __COSSTATE_H
#define __COSSTATE_H

#include <libdragon.h>
#include "cosams.h"
#include <t3d/t3danim.h>

typedef struct StateMachM StateMachM;
typedef struct BasicState BasicState;
typedef struct BasicTrans BasicTrans;

struct StateMachM {
    Module module;
    BasicState* current_state;
}

struct BasicState {
    StateMachM* module;
    BasicTrans* transitions;
    void (*entry)(BasicState* state);
    void (*life)(BasicState* state, float delta, uint32_t frame_buffer);
    void (*exit)(BasicState* state);
};

struct BasicTrans {
    BasicState state;
    BasicState* target;
    float time;
    bool (*param)(BasicState* from, BasicTrans* to);
};

#endif