#include "cossnd.h"

void cossnd_init(const int frequency, int buffers, int channels) {
    audio_init(frequency, buffers);
    mixer_init(channels);
}
void cossnd_life() {
    if (audio_can_write()) {
        // Select an audio buffer that we can write to
        short *buf = audio_write_begin();
        // Write to the audio buffer from the mixer
        mixer_poll(buf, audio_get_buffer_length());
        // Tell the audio system that the buffer has
        // been filled and is ready for playback
        audio_write_end();
    }
}