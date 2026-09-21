#ifndef RENDERER_H
#define RENDERER_H
#include "types.h"

void draw(IO *io, Regs *regs);
void clear_display(bool display[]);
bool drawPixel(int x, int y, bool isBitOn, bool display[]);

#endif