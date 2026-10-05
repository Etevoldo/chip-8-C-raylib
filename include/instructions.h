#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "types.h"

typedef struct {
    bool is_display_wait;
    bool is_8xy6e_shift_vy_into_vx;
    bool is_bnnn_vx;
    bool is_fx565_change_I;
} Quirks;

void FDE(Regs *regs, IO *io, Quirks quirks);
void op_DXYN(u8 x_index, u8 y_index, int n, Regs *regs, bool display[]);

void op_8000(u8 x_index, u8 y_index, int logical_type,
    Regs *regs, Quirks quirks);

void op_FX00(u8 x_index, int other_type,
    Regs *regs, IO *io, Quirks quirks);

#endif