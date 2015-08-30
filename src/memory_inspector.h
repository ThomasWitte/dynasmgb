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

#ifndef DYNASMGB_MEMINSPECT_H
#define DYNASMGB_MEMINSPECT_H

#include "memory.h"
#include <SDL2/SDL.h>

typedef struct {
    gb_memory *mem;
    SDL_Window *win;
    SDL_Window *win2;
    
    SDL_Texture *bitmapTex;
    uint32_t *imgbuf;

    SDL_Texture *bitmapTex2;
    uint32_t *imgbuf2;
} memory_inspector_t;

void memory_inspector_init(memory_inspector_t *inspector, gb_memory *mem);
void memory_inspector_update(memory_inspector_t *inspector);
void memory_inspector_free(memory_inspector_t *inspector);

#endif
