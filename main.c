#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include "types.h"
#include "stack.h"
#include "renderer.h"
#include "instructions.h"
#include "audio.h"
#include "style_amber.h"

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 4096

int load_rom(Regs *regs, char *file_name);
void load_font(Regs *regs);
void init_audio_buffer(float buffer[], int *sineIndex);
int map_key(int key);
void handle_input(IO *io);
void main_cycle(IO *io, Regs *regs, Audio_data *audio, GuiVars *gui_vars);
void close_services(Audio_data *audio);

int main(int argc, char *argv[])
{
    #ifdef DEBUG_ON
    InitWindow(
        DISPLAY_WIDTH * SCALE + 380,
        DISPLAY_HEIGHT * SCALE + 200,
        "Chip-8 Emu");
    #else
    InitWindow(DISPLAY_WIDTH * SCALE,
        DISPLAY_HEIGHT * SCALE,
        "Chip-8 Emu");
    #endif
    GuiLoadStyleAmber();

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
        .keys_down = { false },
        .last_key_pressed = NO_KEY,
        .display_wait = false,
    };

    Audio_data audio = init_audio();
    GuiVars gui_vars = init_gui_vars();

    const int frame_time = 17; // amount of time between frames in miliseconds

    // Insert ROM gui pre-loop
    while (true) {
        if (IsFileDropped()) {
            FilePathList dropped_rom = LoadDroppedFiles();
            if ((dropped_rom.count > 0) &&
                 IsFileExtension(dropped_rom.paths[0], ".ch8")) {
                load_rom(&regs, dropped_rom.paths[0]);
                #ifdef DEBUG_ON
                load_debugger_inst_array(&regs, &gui_vars);
                #endif
                break;
            }
        }
        draw(&io, &regs, &gui_vars);
        usleep(frame_time * 1000);

        if (WindowShouldClose()) {
            close_services(&audio);
            return 0 ;
        }
    }

    load_font(&regs);

    while (!WindowShouldClose()) {

        // pause hack
        if (IsKeyPressed(KEY_P)) gui_vars.is_paused = !gui_vars.is_paused;

        main_cycle(&io, &regs, &audio, &gui_vars);

        draw(&io, &regs, &gui_vars);
        usleep(frame_time * 1000);
    }

    #ifdef DEBUG_ON
    free_debugger_inst_array(gui_vars.inst_list);
    #endif
    close_services(&audio);

    return 0;
}

//unload all resources and close window
void close_services(Audio_data *audio) {
    UnloadAudioStream(audio->stream);
    CloseAudioDevice();
    CloseWindow();
}

void main_cycle(IO *io, Regs *regs, Audio_data *audio, GuiVars *gui_vars) {
    if (IsAudioStreamProcessed(audio->stream)) {
        sample_audio_buffer(audio);
        UpdateAudioStream(audio->stream, audio->buffer, BUFFER_SIZE);
    }

    if (regs->delay > 0) regs->delay -= 1;
    if (regs->sound > 0) regs->sound -= 1;

    // audio
    if (regs->sound == 0) PauseAudioStream(audio->stream);
    if (regs->sound) ResumeAudioStream(audio->stream);

    handle_input(io);

    const int IPF = 11;        // instructions per frame
    for (int i = 0; i < IPF; i++) {
        //if (io.display_wait) break; // comment to disable screen wait

        while (gui_vars->is_paused) {
            // step 1 instruction
            if (gui_vars->is_step) {
                printf("step!");
                gui_vars->is_step = false;
                break;
            }
            if (WindowShouldClose()) return;
            draw(io, regs, gui_vars);
        }

        FDE(regs, io);
        update_scroll(regs, gui_vars);
    }
}

void init_audio_buffer(float buffer[], int *sineIndex) {
    const int sineFrequency = 440;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        int wavelength = SAMPLE_RATE / sineFrequency;
        buffer[i] = sinf(2* PI * (*sineIndex)/wavelength) > 0 ? 1 : -1;
        //if (sinf(2*PI*sineIndex/wavelength) > 0.0f) {
        //    buffer[i] = 1.0f;
        //}
        //else {
        //    buffer[i] = -1.0f;
        //}
        (*sineIndex)++;
        if ((*sineIndex) >= wavelength) (*sineIndex) = 0;
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