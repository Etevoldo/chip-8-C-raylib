#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include "stack.h"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define SCALE 10
#define PC_START 0x200
#define RAM_SIZE 4096
#define V_QTY 16

typedef struct {
    u8    v[V_QTY];
    u16   index;
    u8    delay;
    u8    sound;
    u8    ram[RAM_SIZE];
    Stack stack;
    u16   pc;
} Regs;

#endif