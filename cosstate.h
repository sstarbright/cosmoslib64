#ifndef __COSSTATE_H
#define __COSSTATE_H

#include <libdragon.h>
#include <t3d/t3danim.h>

typedef struct BasicState BasicState;
typedef struct BasicTrans BasicTrans;
typedef struct AnimState AnimState;
typedef struct AnimTrans AnimTrans;

struct BasicState {
    BasicTrans* current_transition;
    BasicTrans* transitions;
    void (*birth)(BasicState* state);
    void (*life)(BasicState* state, float delta, uint32_t frame_buffer);
    void (*death)(BasicState* state);
};

struct BasicTrans {
    BasicState* target;
    void (*entry)(BasicState* from, BasicTrans* state);
    bool (*param)(BasicState* from, BasicTrans* state);
};

struct AnimState {
    BasicState state;
    T3DAnim animation;
};

struct AnimTrans {
    BasicTrans transition;
    float time;
};

#endif