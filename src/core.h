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

typedef struct {
    gb_state state;
    gb_memory memory;
    gb_sound sound;
    gb_block compiled_blocks[4][0x4000]; // bank, start address
    gb_block highmem_blocks[0x80];
    gb_lcd lcd;
    bool draw_frame;
    unsigned next_frame_time;
    
    int frame_cnt;
    unsigned time_busy;
    unsigned last_time;
} gb_vm;

void free_block(gb_block *block);

bool init_vm(gb_vm *vm, const char *filename);
bool run_vm(gb_vm *vm);
bool free_vm(gb_vm *vm);

#endif
