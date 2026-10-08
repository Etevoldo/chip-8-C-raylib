#pragma once
#include "types.h"

#define INST_TO_DISPLAY RAM_SIZE / 2

GuiVars init_gui_vars();
void free_debugger_inst_array(char **inst_list);
void draw(IO *io, Regs *regs, GuiVars *gui_vars);
void draw_debug(Regs *regs, GuiVars *gui_vars, bool keys_down[]);
void draw_gui(Regs *regs, GuiVars *gui_vars);
void clear_display(bool display[]);
void update_scroll(Regs *regs, GuiVars *gui_vars);
void load_debugger_inst_array(Regs *regs, GuiVars *gui_vars);
void free_debugger_inst_array(char **inst_list);
bool drawPixel(int x, int y, bool isBitOn, bool display[]);
