#ifndef RENDERER_H
#define RENDERER_H
#include "types.h"

void draw(bool display[]);
void clear_display(bool display[]);
bool drawPixel(int x, int y, bool isBitOn, bool display[]);

#endif