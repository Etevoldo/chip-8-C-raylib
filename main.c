#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "raylib.h"
#include "types.h"
#include "stack.h"
#include "renderer.h"
#include "instructions.h"

#define DEBUG

int load_rom(Regs *, char *);
bool drawPixel(int x, int y, bool isBitOn, bool display[]);

int main(void)
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
        .last_key_pressed = NO_KEY
    };

    load_rom(&regs, "./tests/4-flags.ch8");

    InitWindow(DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE, "Chip-8 Emu");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        draw(io.display);
        printf("%.2X ", regs.pc);
        FDE(&regs, &io);
    }

    return 0;
}

int load_rom(Regs *regs, char *file_name)
{
    FILE *rom;
    if ((rom = fopen(file_name, "rb")) == NULL)
        return 0;

    int rom_size = fread(
        regs->ram + PC_START,
        1,
        RAM_SIZE - PC_START,
        rom);

    #ifdef DEBUG
    for (int i = PC_START; i < PC_START + rom_size; i++) {
        printf("%.2X ", regs->ram[i]);
    }
    #endif

    fclose(rom);
    return 1;
}