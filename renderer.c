#include "raylib.h"
#include "types.h"
#include "renderer.h"

#define ON_COLOR CLITERAL(Color){ 155, 188, 15, 255 }
#define OFF_COLOR CLITERAL(Color){ 15, 56, 15, 255 }

void draw(IO *io, Regs *regs)
{
    io->display_wait = false;
    io->last_key_pressed = NO_KEY;
    BeginDrawing();
    ClearBackground(OFF_COLOR);

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int index = x + y * DISPLAY_WIDTH;
            if (!io->display[index]) continue;
 
            DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, ON_COLOR);
        }
    }

    #ifdef DEBUG_ON
    const char *text;
    const int debug_anchor = DISPLAY_WIDTH * SCALE; // right letterbox
    for (int i = 0; i < N_OF_KEYS; i++) {
        text = TextFormat(
            "%0.1X = %s", i, io->keys_down[i] ? "DOWN" : "UP");
        DrawText(text, debug_anchor + 10, 10 * i + 10, 5, WHITE);
    }

    for (int i = 0; i < 16; i++) {
        text = TextFormat("V%0.1X = %0.2X", i, regs->v[i]);
        DrawText(text, debug_anchor + 80, 10 * i + 10, 5, WHITE);
    }

    int i = 0;
    for (int j = regs->pc - 15; j < regs->pc + 15; j++) {
        text = TextFormat("ram[%0.2X]:\t0x%0.2X%0.2X",
            j, regs->ram[j], regs->ram[j + 1]);
        Color tint;
        if (j == regs->pc) tint = RED;
        else tint = WHITE;

        DrawText(
            text,
            debug_anchor + 150,
            10 * i + 10,
            10.0f,
            tint);
        i++;
    }

    // stack
    u16 address;
    DrawText(
        "Stack:", 
        debug_anchor + 10,
        DISPLAY_HEIGHT * SCALE - 30,
        10,
        RED);
    for (int i = 0; i < 16; i++) {
        address = regs->stack.arr[i];
        text = TextFormat("%0.4X ", address);
        DrawText(text, debug_anchor + 10 + (30 * i), DISPLAY_HEIGHT * SCALE - 20, 10, WHITE);
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
