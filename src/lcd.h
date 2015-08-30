/*
    This file is part of dynasmgb.

    dynasmgb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    dynasmgb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with dynasmgb.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright 2014, 2015 Thomas Witte
*/

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
