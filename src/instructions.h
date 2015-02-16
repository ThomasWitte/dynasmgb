#ifndef DYNASMGB_INSTRUCTIONS_H
#define DYNASMGB_INSTRUCTIONS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include "emit.h"

// compiles block starting at start_address to gb_block
bool compile(gb_block *block, gb_memory *mem, uint16_t start_address);

#endif
