#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#define RAM_SIZE 4096
#define V_QTY 16

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define SCALE 10
#define PC_START 0x200

#define DEBUG

typedef uint8_t u8;
typedef uint16_t u16;

typedef struct {
    u8  index;
    u16 arr[16];
} Stack;

typedef struct {
    u8    v[V_QTY];
    u16   index;
    u8    delay;
    u8    sound;
    u8    ram[RAM_SIZE];
    Stack stack;
    u16   pc;
} Regs;

// standard stack data structure
void stackPush(Stack *, u16);
u16 stackPop(Stack *);
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

void stackPush(Stack *stack, u16 value)
{
    stack->arr[stack->index] = value;
    stack->index++;
}

u16 stackPop(Stack *stack)
{
    stack->index--;
    return stack->arr[stack->index];
}
