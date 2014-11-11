#ifndef DYNASMGB_INTERRUPT_H
#define DYNASMGB_INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "memory.h"
#include "emit.h"

void update_ioregs(gb_state* state, gb_memory* memory);

#endif
