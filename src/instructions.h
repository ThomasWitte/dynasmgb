#ifndef DYNASMGB_INSTRUCTIONS_H
#define DYNASMGB_INSTRUCTIONS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include "emit.h"

extern GList* opt_result;

// compiles block starting at start_address to gb_block
bool compile(gb_block *block, gb_memory *mem, uint16_t start_address);
int yylex();
void yyerror (char const *s);

#endif
