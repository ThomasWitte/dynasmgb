#ifndef DYNASMGB_MEMINSPECT_H
#define DYNASMGB_MEMINSPECT_H

#include "memory.h"
#include <SDL2/SDL.h>

typedef struct {
    gb_memory *mem;
    SDL_Window *win;
    SDL_Window *win2;
    
    SDL_Texture *bitmapTex;
    uint32_t *imgbuf;

    SDL_Texture *bitmapTex2;
    uint32_t *imgbuf2;
} memory_inspector_t;

void memory_inspector_init(memory_inspector_t *inspector, gb_memory *mem);
void memory_inspector_update(memory_inspector_t *inspector);
void memory_inspector_free(memory_inspector_t *inspector);

#endif
