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

#ifndef DYNASMGB_CORE_H
#define DYNASMGB_CORE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "instructions.h"
#include "interrupt.h"
#include "lcd.h"
#include "memory_inspector.h"
#include "sound.h"

#define MAX_ROM_BANKS 256
#define MAX_RAM_BANKS 4

typedef struct {
    gb_state state;
    gb_memory memory;
    gb_sound sound;
    gb_block compiled_blocks[MAX_ROM_BANKS][0x4000]; // bank, start address
    gb_block highmem_blocks[0x80];
    gb_lcd lcd;
    bool draw_frame;
    unsigned next_frame_time;
    
    int frame_cnt;
    unsigned time_busy;
    unsigned last_time;
    
    int opt_level;
} gb_vm;

void free_block(gb_block *block);

bool init_vm(gb_vm *vm, const char *filename, int opt_level, bool init_io);
bool run_vm(gb_vm *vm);
bool free_vm(gb_vm *vm);

#endif
