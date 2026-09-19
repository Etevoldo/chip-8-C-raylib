#include "raylib.h"
#include "types.h"
#include "renderer.h"

void draw(bool display[])
{
    BeginDrawing();
    ClearBackground(BLACK);

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int index = x + y * DISPLAY_WIDTH;
            if (!display[index]) continue;
 
            DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, WHITE);
        }
    }

    EndDrawing();
}

void clear_display(bool display[])
{
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int index = x + y * DISPLAY_WIDTH;
            display[index] = false;
        }
    }
}

bool drawPixel(int x, int y, bool isBitOn, bool display[]) {
    if (!isBitOn) return false;
    int index = x + y * DISPLAY_WIDTH;
    bool collision;

    if (display[index]) {
        display[index] = false;
        collision = true;
    }
    else {
        display[index] = true;
        collision = false;
    }
    return collision;
}