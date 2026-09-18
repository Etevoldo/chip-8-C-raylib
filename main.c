#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;

#include "raylib.h"
#include "types.h"
#include "stack.h"

#define DEBUG
int load_rom(Regs *, char *);

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
        .pc = 0x200
    };
    bool display[DISPLAY_HEIGHT * DISPLAY_WIDTH] = { false };

    load_rom(&regs, "./tests/1-chip8-logo.ch8");

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