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

typedef struct {
    gb_state state;
    gb_memory memory;
    gb_block compiled_blocks[2][0x4000]; // bank, start address
    SDL_Window *win;
} gb_vm;

void free_block(gb_block *block);

bool init_vm(gb_vm *vm, const char *filename);
bool run_vm(gb_vm *vm);
bool free_vm(gb_vm *vm);

#endif
