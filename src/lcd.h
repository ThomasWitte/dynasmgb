#ifndef DYNASMGB_LCD_H
#define DYNASMGB_LCD_H

#include <SDL2/SDL.h>

SDL_Window *init_window();
void deinit_window(SDL_Window *win);
void render_frame(SDL_Window *win);
void update_line(uint8_t *mem);

#endif
