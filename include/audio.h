#pragma once

#include <math.h>
#include "types.h"
#include "raylib.h"

AudioStream init_audio();
void sample_audio_buffer(Audio_data *audio);
void init_audio_buffer(float buffer[], int *sineIndex);
Audio_data init_audio_vars(AudioStream stream);
