#include "audio.h"

Audio_data init_audio() {
    // initializing Audio
    InitAudioDevice();

    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    int sample_step = 0;

    AudioStream stream = LoadAudioStream(SAMPLE_RATE, 32, 1);
    SetAudioStreamPan(stream, 0.0f);
    SetAudioStreamVolume(stream, 0.3f);
    PlayAudioStream(stream);

    return (Audio_data) {
        .stream = stream,
        .buffer = { 0 },
        .sample_step = sample_step };
}

void sample_audio_buffer(Audio_data *audio) {
    const int freq = 440;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        int wavelength = SAMPLE_RATE / freq;
        // square wave
        audio->buffer[i] = sinf(
            2* PI * freq * audio->sample_step/SAMPLE_RATE) > 0 ? 1 : -1;
        audio->sample_step++;
        if (audio->sample_step >= wavelength) audio->sample_step = 0;
    }
}