#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "raylib.h"
#include "types.h"
#include "stack.h"
#include "renderer.h"
#include "instructions.h"

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 4096

int load_rom(Regs *regs, char *file_name);
void load_font(Regs *regs);
void init_audio_buffer(float buffer[]);
int map_key(int key);
void handle_input(IO *io);

int main(int argc, char *argv[])
{
    Stack s = (Stack) {0, { 0 }};
    Regs regs = (Regs) {
        .v = { 0 },
        .index = 0,
        .delay = 0,
        .sound = 0,
        .ram = { 0 },
        .stack = s,
        .pc = PC_START,
    };

    IO io = (IO) {
        .display = { false },
        .keys_down= { false },
        .last_key_pressed = NO_KEY,
        .display_wait = false
    };

    if (argc < 2) {
        printf("Not enought Arguments");
        return 0;
    }

    load_rom(&regs, argv[1]);
    load_font(&regs);

    InitWindow(DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE, "Chip-8 Emu");

    // initializing Audio
    InitAudioDevice();

    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    float buffer[BUFFER_SIZE] = { };

    AudioStream stream = LoadAudioStream(SAMPLE_RATE, 32, 1);
    SetAudioStreamPan(stream, 0.0f);
    PlayAudioStream(stream);
    init_audio_buffer(buffer);

    const int IPF = 11;        // instructions per frame
    const int frame_time = 17; // amount of time between frames in miliseconds
    while (!WindowShouldClose()) {
        if (IsAudioStreamProcessed(stream)) {
            UpdateAudioStream(stream, buffer, BUFFER_SIZE);
        }

        if (regs.delay > 0) regs.delay -= 1;
        if (regs.sound > 0) regs.sound -= 1;

        // audio
        if (regs.sound == 0) {
            PauseAudioStream(stream);
        }
        if (regs.sound) {
            ResumeAudioStream(stream); 
        }

        handle_input(&io);

        for (int i = 0; i < IPF; i++) {
            //if (io.display_wait) break; // comment to disable screen wait

            FDE(&regs, &io);
        }

        draw(&io, &regs);
        usleep(frame_time * 1000);
    }

    UnloadAudioStream(stream);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void init_audio_buffer(float buffer[]) {
    const int sineFrequency = 440;
    int sineIndex = 0;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        int wavelength = SAMPLE_RATE / sineFrequency;
        buffer[i] = sinf(2* PI * sineIndex/wavelength);
        //if (sinf(2*PI*sineIndex/wavelength) > 0.0f) {
        //    buffer[i] = 1.0f;
        //}
        //else {
        //    buffer[i] = -1.0f;
        //}
        sineIndex++;
        //if (sineIndex >= wavelength) sineIndex = 0;
    }
}

void handle_input(IO *io)
{
    int key_codes[N_OF_KEYS] = {
        KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_Q, KEY_W, KEY_E,
        KEY_R, KEY_A, KEY_S, KEY_D, KEY_F, KEY_Z, KEY_X, KEY_C, KEY_V,
    };

    int key_index;
    for (int i = 0; i < N_OF_KEYS; i++) {
        key_index = map_key(key_codes[i]);
        if (IsKeyDown(key_codes[i]) && !io->keys_down[key_index]) {
            io->keys_down[key_index] = true;
        }
        if (IsKeyUp(key_codes[i]) && io->keys_down[key_index]) {
            io->keys_down[key_index] = false;
            io->last_key_pressed = key_index;
        }
    }

}

int map_key(int key) {
    switch (key){
        case KEY_ONE:   return 0x1;
        case KEY_TWO:   return 0x2;
        case KEY_THREE: return 0x3;
        case KEY_FOUR:  return 0xC;
        case KEY_Q:     return 0x4;
        case KEY_W:     return 0x5; 
        case KEY_E:     return 0x6; 
        case KEY_R:     return 0xD; 
        case KEY_A:     return 0x7; 
        case KEY_S:     return 0x8; 
        case KEY_D:     return 0x9; 
        case KEY_F:     return 0xE; 
        case KEY_Z:     return 0xA; 
        case KEY_X:     return 0x0; 
        case KEY_C:     return 0xB; 
        case KEY_V:     return 0xF;
        default:        return NO_KEY;
    }
}

int load_rom(Regs *regs, char *file_name)
{
    FILE *rom;
    if ((rom = fopen(file_name, "rb")) == NULL)
        return 0;

    fread(regs->ram + PC_START, 1, RAM_SIZE - PC_START, rom);

    fclose(rom);
    return 1;
}

void load_font(Regs *regs)
{
    u8 font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80}; // F
    
    const int font_length = sizeof(font) / sizeof(font[0]);

    for (int i = 0; i < font_length; i++) {
        regs->ram[FONT_START + i] = font[i];
    }
}