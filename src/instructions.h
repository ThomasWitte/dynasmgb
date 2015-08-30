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

#ifndef DYNASMGB_INSTRUCTIONS_H
#define DYNASMGB_INSTRUCTIONS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include "emit.h"
#include "optimize.h"

extern GList* opt_result;

// compiles block starting at start_address to gb_block
bool compile(gb_block *block, gb_memory *mem, uint16_t start_address, int opt_level);
int yylex();
void yyerror (char const *s);

#endif
