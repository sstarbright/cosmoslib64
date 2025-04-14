#ifndef __COSAUD_H
#define __COSAUD_H

#include "cosmesh.h"
#include <libdragon.h>

typedef struct AudChannel AudChannel;
typedef struct AudEv AudEv;

struct AudChannel {
    uint32_t id;
    wav64_t* last_sound;
    bool is_reserved;
};

struct AudEv {
    uint32_t* channel;
    wav64_t sound;
    float vol;
};

void cosaud_init(const int frequency, int buffers, int channels);
AudChannel* cosaud_reserve(int count);
void cosaud_release(AudChannel* channel, int count);
void cosaud_play(wav64_t* sound, int channel, float vol, float pan);
void audev_action(AnimSt* state, AnimEv* event);
void cosaud_kill();

#endif