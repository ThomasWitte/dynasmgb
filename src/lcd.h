#ifndef DYNASMGB_LCD_H
#define DYNASMGB_LCD_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *win;
    SDL_mutex *vblank_mutex;
    SDL_cond *vblank_cond;
    SDL_Thread *thread;
    bool exit;
} gb_lcd; 

bool init_window(gb_lcd* lcd);
void deinit_window(gb_lcd* lcd);
void render_frame(gb_lcd* lcd);
void update_line(uint8_t *mem);

#endif
