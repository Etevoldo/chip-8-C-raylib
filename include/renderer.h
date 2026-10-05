#ifndef RENDERER_H
#define RENDERER_H
#include "types.h"

#define INST_TO_DISPLAY RAM_SIZE / 2

typedef struct {
    const int inst_to_display;
    int inst_active;
    int inst_focus;
    int inst_scrollIndex;
    char **inst_list;
    bool is_paused;
    bool is_step;
    float audio_vol_slider_value;
    float audio_freq;
    int IPF;
    bool is_display_wait;
    bool is_8xy6e_shift;
    bool is_bnnn_vx;
    bool is_fx565_change_I;
} GuiVars;

GuiVars init_gui_vars();
void free_debugger_inst_array(char **inst_list);
void draw(IO *io, Regs *regs, GuiVars *gui_vars);
void draw_debug(IO *io, Regs *regs, GuiVars *gui_vars);
void draw_gui(IO *io, Regs *regs, GuiVars *gui_vars);
void clear_display(bool display[]);
void update_scroll(Regs *regs, GuiVars *gui_vars);
void load_debugger_inst_array(Regs *regs, GuiVars *gui_vars);
void free_debugger_inst_array(char **inst_list);
bool drawPixel(int x, int y, bool isBitOn, bool display[]);

#endif