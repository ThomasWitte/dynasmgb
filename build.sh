#!/bin/sh
lua dynasm/dynasm.lua $@ -I src -o src/emit.c src/emit.dasc
gcc -std=gnu11 -g -Wall -Wextra -Wno-unused-parameter $@ -o dynasmgb src/*.c -lSDL2
#gcc -std=gnu11 -g -Wall -Wextra -lSDL2 -o dynasmgb src/*.c
