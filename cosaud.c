#include "cosaud.h"
#include <libdragon.h>

AudChannel* channels;
int channel_count;
int last_channel;

void cosaud_init(const int frequency, int buffers, int num_channels) {
    audio_init(frequency, buffers);
    mixer_init(num_channels);
    channels = malloc(sizeof(AudChannel)*num_channels);
    channel_count = num_channels;

    AudChannel* current_channel;
    for (int i = 0; i < num_channels; i++) {
        current_channel = &channels[i];
        current_channel->id = i;
        current_channel->last_sound = NULL;
        current_channel->is_reserved = false;
    }
}

AudChannel* cosaud_reserve(int count) {
    int current_channel = 0;
    int start_channel = 0;
    int consec_channels = 0;

    while (current_channel < channel_count && consec_channels < count) {
        if (channels[current_channel].is_reserved) {
            start_channel++;
            consec_channels = 0;
        } else {
            consec_channels++;
        }
        current_channel++;
    }

    if (consec_channels == count) {
        for (int i = 0; i < count; i++) {
            channels[i+start_channel].is_reserved = true;
        }
        return &channels[start_channel];
    } else
        return NULL;
}
void cosaud_release(AudChannel* channel, int count) {
    for (int i = channel->id; i < count; i++) {
        channels[i].is_reserved = false;
    }
}
void cosaud_play(wav64_t* sound, int channel, float vol, float pan) {
    if (channel < 0) {
        int try_channel = (last_channel + 1)%channel_count;

        while (try_channel != last_channel && channel < 0) {
            if (!channels[try_channel].is_reserved)
                channel = try_channel;
            else
                try_channel = (try_channel + 1)%channel_count;
        }
        
        last_channel = channel;

        if (channel < 0)
            debugf("Sound %p failed to play! (All channels reserved!)", sound);
        else {
            wav64_play(sound, channel);
            mixer_ch_set_vol_pan(channel, vol, pan);
        }
    } else {
        wav64_play(sound, channel);
        mixer_ch_set_vol_pan(channel, vol, pan);
    }
}

void audev_action(AnimSt* state, AnimEv* event) {
    AudEv* aud_ev = (AudEv*)event->data;
    cosaud_play(&aud_ev->sound, aud_ev->channel ? *aud_ev->channel : -1, aud_ev->vol, .5f);
}

void cosaud_kill() {
    free(channels);
}