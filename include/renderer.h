#ifndef RENDERER_H
#define RENDERER_H
#include "types.h"

void draw(IO *io, Regs *regs);
void draw_debug(IO *io, Regs *regs);
void clear_display(bool display[]);
void load_debugger_inst_array(Regs *regs);
void free_debugger_inst_array();
bool drawPixel(int x, int y, bool isBitOn, bool display[]);

#endif