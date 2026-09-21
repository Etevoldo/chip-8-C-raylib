#include "raylib.h"
#include "types.h"
#include "renderer.h"

void draw(IO *io, Regs *regs)
{
    io->display_wait = false;
    io->last_key_pressed = NO_KEY;
    BeginDrawing();
    ClearBackground(BLACK);

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int index = x + y * DISPLAY_WIDTH;
            if (!io->display[index]) continue;
 
            DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, WHITE);
        }
    }

    #ifdef DEBUG
    const char *text;
    for (int i = 0; i < N_OF_KEYS; i++) {
        text = TextFormat(
            "%0.1X = %s", i, io->keys_down[i] ? "DOWN" : "UP");
        DrawText(text, 10, 10 * i + 10, 5, RED);
    }

    for (int i = 0; i < 16; i++) {
        text = TextFormat("V%0.1X = %0.2X", i, regs->v[i]);
        DrawText(text, DISPLAY_WIDTH * SCALE - 50, 10 * i + 10, 5, RED);
    }
    #endif

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
    int index = x + y * DISPLAY_WIDTH;
    bool collision = false;

    if (isBitOn) {
        if (display[index]) {
            display[index] = false;
            collision = true;
        }
        else if (!display[index]) {
            display[index] = true;
        }
    }

    return collision;
}
