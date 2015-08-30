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

#ifndef DYNASMGB_SOUND_H
#define DYNASMGB_SOUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

#define SAMPLE_FREQUENCY 22050

typedef struct {
    gb_memory *memory;
    struct sound_detail *snd;
} gb_sound;

void sound_reg_write(gb_sound *sound, uint16_t addr, uint8_t val, uint64_t time);

bool init_sound(gb_sound *sound, gb_memory *memory);

void pause_sound(gb_sound *sound, bool pause);

bool deinit_sound(gb_sound *sound);

#endif
