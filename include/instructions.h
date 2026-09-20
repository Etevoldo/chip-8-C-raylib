#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "types.h"

void FDE(Regs *regs, IO *io);
void op_DXYN(u8 x_index, u8 y_index, int n, Regs *regs, bool display[]);
void op_8000(u8 x_index, u8 y_index, int logical_type, Regs *regs);
void op_FX00(u8 x_index, int other_type, Regs *regs, IO *io);

#endif