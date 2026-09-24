#ifndef AUDIO_H
#define AUDIO_H

#include <math.h>
#include "types.h"
#include "raylib.h"

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 4096

typedef struct {
    AudioStream stream;
    float buffer[BUFFER_SIZE];
    int sample_step;
} Audio_data;

Audio_data init_audio();
void sample_audio_buffer(Audio_data *audio);

#endif