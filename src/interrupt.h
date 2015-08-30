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

#ifndef DYNASMGB_INTERRUPT_H
#define DYNASMGB_INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "memory.h"
#include "emit.h"

uint64_t next_update_time(gb_state* state);

void update_ioregs(gb_state* state);
uint16_t start_interrupt(gb_state* state);

#endif
