#include "memory_inspector.h"

void draw_tile(memory_inspector_t *inspector, uint16_t base_addr, int pos_x, int pos_y, int zoom) {
    uint8_t *mem = inspector->mem->mem;

    for(int x = 0; x < 8*zoom; ++x) {
        for(int y = 0; y < 8*zoom; ++y) {
            int col = (mem[base_addr + (y/zoom)*2] >> (7-(x/zoom))) % 2 +
            2*((mem[base_addr + (y/zoom)*2 + 1] >> (7-(x/zoom))) % 2);

            inspector->imgbuf[640 * (pos_y + y) + pos_x + x]
                = 0xffffff - 0x555555 * col;
        }
    }
}

void memory_inspector_init(memory_inspector_t *inspector, gb_memory *mem) {
    SDL_Init(SDL_INIT_VIDEO);

    inspector->win = SDL_CreateWindow(
        "memory_inspector",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 640, SDL_WINDOW_OPENGL);

    if(!inspector->win) {
        printf("Could not create window!\n");
        exit(-1);
    }

    inspector->imgbuf = malloc(640 * 640 * sizeof(uint32_t));
    inspector->mem = mem;

    SDL_Renderer *renderer = SDL_CreateRenderer(inspector->win, -1, SDL_RENDERER_ACCELERATED);

    inspector->bitmapTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STATIC, 640, 640);
}

void memory_inspector_update(memory_inspector_t *inspector) {
    for(int i = 0; i <= 255; ++i) {
        int tile_x = i % 32;
        int tile_y = i / 32;

        draw_tile(inspector, 0x8000 + 16*i, 2 + tile_x*20, 2 + 20*tile_y, 2);
        draw_tile(inspector, 0x8800 + 16*i, 2 + tile_x*20, 182 + 20*tile_y, 2);
    }

    int tile_table_address = inspector->mem->mem[0xff40]&0x10 ? 0x8000 : 0x9000;
    for(int i = 0; i < 32; ++i) {
        for(int j = 0; j < 32; ++j) {
            int idx = ((inspector->mem->mem[0xff40]&0x10) ? inspector->mem->mem[0x9800 + 32*i + j] : (int8_t)inspector->mem->mem[0x9800 + 32*i + j]);
            draw_tile(inspector, tile_table_address + 16*idx, 100 + 8*j , 360 + 8*i, 1);
        }
    }

    SDL_Renderer *renderer = SDL_GetRenderer(inspector->win);

    SDL_RenderClear(renderer);
    SDL_UpdateTexture(inspector->bitmapTex, NULL, inspector->imgbuf, 640*sizeof(uint32_t));
    SDL_RenderCopy(renderer, inspector->bitmapTex, NULL, NULL);
    SDL_RenderPresent(renderer);    
}

void memory_inspector_free(memory_inspector_t *inspector) {
    free(inspector->imgbuf);

    SDL_Renderer *renderer = SDL_GetRenderer(inspector->win);
    SDL_DestroyTexture(inspector->bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(inspector->win);
}
