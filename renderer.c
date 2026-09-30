#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "types.h"
#include "renderer.h"
#include "raygui.h"

#define ON_COLOR CLITERAL(Color){ 155, 188, 15, 255 }
#define OFF_COLOR CLITERAL(Color){ 15, 56, 15, 255 }

void draw(IO *io, Regs *regs)
{
    io->display_wait = false;
    io->last_key_pressed = NO_KEY;

    BeginDrawing();

        ClearBackground(BLACK);

        // CHIP-8 display
        DrawRectangle(0, 0, DISPLAY_WIDTH * SCALE, 
            DISPLAY_HEIGHT * SCALE, OFF_COLOR);

        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            for (int x = 0; x < DISPLAY_WIDTH; x++) {
                int index = x + y * DISPLAY_WIDTH;
                if (!io->display[index]) continue;
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, ON_COLOR);
            }
        }
        draw_debug(io, regs);


    EndDrawing();
}

// boooooo global variables
const int inst_to_display = 200;
int inst_active = inst_to_display / 2;
int inst_focus = -1;
int inst_scrollIndex = inst_to_display / 2 - 10;

void draw_debug(IO *io, Regs *regs) {

    const char *text;

    const int debug_x = DISPLAY_WIDTH * SCALE + 10;
    const int debug_y = 10;
    const int label_width = 60;
    const int label_height = 50;
    const int label_spacing = 15;
    const int padding = 10;

    // keys state
    GuiGroupBox((Rectangle) {debug_x, debug_y, 70, 280 }, "Keys");
    for (int i = 0; i < N_OF_KEYS; i++) {
        text = TextFormat(
            "%0.1X = %s", i, io->keys_down[i] ? "DOWN" : "UP");
        GuiLabel(
            (Rectangle) {
                debug_x + padding,
                label_spacing * i,
                label_width,
                label_height },
            text);
    }

    // Registers V 0 to F
    GuiGroupBox((Rectangle) {debug_x + 69, debug_y, 70, 280 }, "V.Regs");
    for (int i = 0; i < 16; i++) {
        text = TextFormat("V%0.1X = %0.2X", i, regs->v[i]);
        GuiLabel(
            (Rectangle) {debug_x + 79, label_spacing * i, label_width, label_height },
            text);
    }

    // Instructions
    char *inst_list[inst_to_display];

    for (int i = 0; i < inst_to_display; i++) {
        inst_list[i] = (char *) malloc(12);
        if (inst_list[i] == NULL) i--;
    }

    GuiGroupBox(
        (Rectangle) {debug_x + 138, debug_y, 200, 620 },
        "Instructions");
    int i = 0;
    for (int j = regs->pc - inst_to_display;
         j < regs->pc + inst_to_display;
         j += 2) {
        // idea to investigate this is reclycling memory values
        sprintf(inst_list[i], "%.4X = %.2X%.2X\n",
            j, regs->ram[j], regs->ram[j + 1]);

        i++;
    }
    GuiListViewEx(
        (Rectangle) { 
            debug_x + 148,
            debug_y + 10,
            180,
            600},
        inst_list,
        inst_to_display,
        &inst_scrollIndex,
        &inst_active,
        &inst_focus
    );

    for (int i = 0; i < inst_to_display; i++) {
        free(inst_list[i]);
    }

    // stack
    u16 address;
    GuiGroupBox((Rectangle) {debug_x, debug_y + 340, 70, 280 }, "Stack");
    for (int i = 0; i < 16; i++) {
        address = regs->stack.arr[i];
        text = TextFormat("%0.4X", address);
        GuiLabel(
            (Rectangle) {
                debug_x + padding,
                debug_y + 340 + label_spacing * i,
                label_width,
                label_height },
            text);
    }

    // other Registers
    GuiGroupBox(
        (Rectangle) {debug_x + 69, debug_y + 340, 70, 280 },
        "Other\nRegs.");
    // I registers
    GuiLabel(
        (Rectangle) {
            debug_x + 69 + padding,
            debug_y + 340 + padding,
            label_width,
            label_height },
        TextFormat("I  = %0.4X", regs->index));
    // PC
    GuiLabel(
        (Rectangle) {
            debug_x + 69 + padding,
            debug_y + 340 + padding + label_spacing,
            label_width,
            label_height },
        TextFormat("PC = %0.4X", regs->pc));
    // Sound
    GuiLabel(
        (Rectangle) {
            debug_x + 69 + padding,
            debug_y + 340 + padding + label_spacing*2,
            label_width,
            label_height },
        TextFormat("ST = %0.4X", regs->sound));
    // Delay
    GuiLabel(
        (Rectangle) {
            debug_x + 69 + padding,
            debug_y + 340 + padding + label_spacing*3,
            label_width,
            label_height },
        TextFormat("DT = %0.4X", regs->delay));
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
