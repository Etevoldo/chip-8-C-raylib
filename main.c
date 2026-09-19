#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "raylib.h"
#include "types.h"
#include "stack.h"
#include "renderer.h"

#define DEBUG
int load_rom(Regs *, char *);
void FDE(Regs *, bool[]);
void DXYN(u8, u8, int, Regs *, bool[]);
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
        .pc = PC_START
    };
    bool display[DISPLAY_HEIGHT * DISPLAY_WIDTH] = { false };

    load_rom(&regs, "./tests/1-chip8-logo.ch8");

    InitWindow(DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE, "Chip-8 Emu");
    SetTargetFPS(5);

    while (!WindowShouldClose()) {
        draw(display);
        printf("%.2X ", regs.pc);
        FDE(&regs, display);
    }

    return 0;
}

void FDE(Regs *regs, bool display[]) {

    // for short
    u16 pc = regs->pc;
    // fetch

    // current instruction
    u16 instruc = (regs->ram[pc] << 8) + regs->ram[pc + 1];
    regs->pc += 2;

    // decode

    const u8 x   = (instruc & 0x0F00) >> 8; // 2nd nibble, V selector
    const u8 y   = (instruc & 0x00F0) >> 4; // 3rd nibble, V selector
    const int n   = (instruc & 0x000F);      // 4th nibble, 4 bit number
    const int kk  = (instruc & 0x00FF);      // 3rd & 4th nibble, 8 bit number
    const int nnn = (instruc & 0x0FFF);      // 2-4th nibble, memory address

    // for general instructions
    const int type = instruc & 0xF000;
    // for 8VX instructions
    const int logical_type = instruc & 0x000F;
    // for E000 and F000
    const int other_types = instruc & 0x00FF;

    // execute

    switch (type)
    {
    case 0x0000:
        if (other_types == 0x0E0) {
            clear_display(display);
        }
        else if (other_types == 0x00EE) {
            regs->pc = stackPop(&(regs->stack));
        }
        break;
    case 0x1000:
        regs->pc = nnn;
        break;
    case 0x6000:
        regs->v[x] = kk;
        break;
    case 0x7000:
        // check for overflow
        regs->v[x] += kk;
        break;
    case 0xA000:
        regs->index = nnn;
        break;
    case 0xD000:
        DXYN(x, y, n, regs, display);
        break;

    default:
        break;
    }
}

void DXYN(u8 x_index, u8 y_index, int n, Regs *regs, bool display[]) {
    const u16 spriteIndex = regs->index;
    const u8 xAnchor = regs->v[x_index] % DISPLAY_WIDTH;
    const u8 yAnchor = regs->v[y_index] % DISPLAY_HEIGHT;
    regs->v[0xF] = 0;

    u8 x = xAnchor;
    u8 y = yAnchor;

    for (int byteN = 0; byteN < n; byteN++) {
        x = xAnchor;
        int mask = 0b10000000; // no digit separator in C :(
        int byte = regs->ram[spriteIndex + byteN];
        for (int bitN = 0; bitN < 8; bitN++) {
            bool isBitOn = (byte & mask) > 0;
            bool isCollision = drawPixel(x, y, isBitOn, display);

            if (isCollision) regs->v[0xF] = 1;

            mask = mask >> 1;
            x++;

            if (x >= DISPLAY_WIDTH) break;
        }
        y++;
        if (y >= DISPLAY_HEIGHT) break;
    }
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