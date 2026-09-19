#ifndef RENDERER_H
#define RENDERER_H
#include "types.h"

void draw(bool []);
void clear_display(bool []);
bool drawPixel(int x, int y, bool isBitOn, bool display[]);

#endif