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

    inspector->win2 = SDL_CreateWindow(
        "address space",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        512, 512, SDL_WINDOW_OPENGL);

    if(!inspector->win) {
        printf("Could not create window!\n");
        exit(-1);
    }
    
    if(!inspector->win2) {
        printf("Could not create window!\n");
        exit(-1);
    }

    inspector->imgbuf = malloc(640 * 640 * sizeof(uint32_t));
    memset(inspector->imgbuf, 0, 640 * 640 * sizeof(uint32_t));

    inspector->imgbuf2 = malloc(512 * 512 * sizeof(uint32_t));
    memset(inspector->imgbuf2, 0, 512 * 512 * sizeof(uint32_t));
    
    inspector->mem = mem;

    SDL_Renderer *renderer = SDL_CreateRenderer(inspector->win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer *renderer2 = SDL_CreateRenderer(inspector->win2, -1, SDL_RENDERER_ACCELERATED);

    inspector->bitmapTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
                                             SDL_TEXTUREACCESS_STATIC, 640, 640);
    inspector->bitmapTex2 = SDL_CreateTexture(renderer2, SDL_PIXELFORMAT_RGB888,
                                             SDL_TEXTUREACCESS_STATIC, 512, 512);
}

void update_aspace_view(memory_inspector_t *inspector) {
    uint8_t *mem = inspector->mem->mem;
    for(int y = 0; y < 256; ++y)
        for(int x = 0; x < 128; ++x) {
            uint16_t addr = 0x8000 + (255-y)*128 + x;
            uint8_t val = mem[addr];
            if((inspector->imgbuf2[y*2*512 + 2*x] & 0xff000000) >> 24 != val) {
                inspector->imgbuf2[y*2*512 + 2*x] = val << 24 | 0xffffff;
                inspector->imgbuf2[y*2*512 + 2*x+1] = val << 24 | 0xffffff;
                inspector->imgbuf2[(y*2+1)*512 + 2*x] = val << 24 | 0xffffff;
                inspector->imgbuf2[(y*2+1)*512 + 2*x+1] = val << 24 | 0xffffff;
            } else {
                int pattern = 0;
            
                if(addr < 0xa000) { // tile ram  -> orange
                    if((inspector->imgbuf2[y*2*512 + 2*x] & 0xff) > 0x0)
                        pattern = 0x030405;
                } else if(addr < 0xc000) { // cart ram   -> green
                    if((inspector->imgbuf2[y*2*512 + 2*x] & 0xff0000) > 0x0)
                        pattern = 0x050404;
                } else if(addr < 0xe000) { // ram banks  -> blue
                    if((inspector->imgbuf2[y*2*512 + 2*x] & 0xff00) > 0x0)
                        pattern = 0x050503;
                } else if(addr < 0xfe00) { // echo ram   -> red
                    if((inspector->imgbuf2[y*2*512 + 2*x] & 0xff) > 0x0)
                        pattern = 0x020505;
                } else if(addr < 0xff00) { // oam        -> orange
                    if((inspector->imgbuf2[y*2*512 + 2*x] & 0xff) > 0x0)
                        pattern = 0x030405;
                } else {                   // hardware io, high mem -> pink
                    if((inspector->imgbuf2[y*2*512 + 2*x] & 0xff00) > 0x0)
                        pattern = 0x030503;
                }

                inspector->imgbuf2[y*2*512 + 2*x] -= pattern;
                inspector->imgbuf2[y*2*512 + 2*x+1] -= pattern;
                inspector->imgbuf2[(y*2+1)*512 + 2*x] -= pattern;
                inspector->imgbuf2[(y*2+1)*512 + 2*x+1] -= pattern;
            }
        }
    
    // show rom banks:
    int num_banks = 2 << mem[0x148];
    for(int y = 0; y <= num_banks / 16; ++y) {
        for(int x = 0; x < 16; ++x) {
            if(16*y + x >= num_banks)
                break;
            
            int col = 0x777777;
            if(16*y + x == inspector->mem->current_rom_bank || (y == 0 && x == 0)) {
                col = 0xffffff;
            } else {
                col = (inspector->imgbuf2[(256+16*y+1)*512 + 256+16*x+1] & 0xffff00) - 0x010100;
                if(col < 0x202000) col = 0x202000;
            }
            
            for(int u = 0; u < 14; ++u)
                for(int v = 0; v < 14; ++v)
                    inspector->imgbuf2[(256+16*y+u+1)*512 + 256+16*x+v+1] = col;
        }
    }
    
    // show ram banks
    num_banks = mem[0x149] > 0 ? 1 << (mem[0x149]-1) : 0;
    for(int y = 0; y <= num_banks / 16; ++y) {
        for(int x = 0; x < 16; ++x) {
            if(16*y + x >= num_banks)
                break;
            
            int col = 0x777777;
            if(16*y + x == inspector->mem->current_ram_bank) {
                col = 0xffffff;
            } else {
                col = (inspector->imgbuf2[(220+16*y+1)*512 + 256+16*x+1] & 0xffff) - 0x0101;
                if(col < 0x2020) col = 0x2020;
            }
            
            for(int u = 0; u < 14; ++u)
                for(int v = 0; v < 14; ++v)
                    inspector->imgbuf2[(220+16*y+u+1)*512 + 256+16*x+v+1] = col;
        }
    }
    
    // show io registers
    for(int y = 0; y < 3; ++y) {
        for(int x = 0; x < 16; ++x) {
            if(16*y + x >= 75)
                break;
            
            int col = 0x777777;
            if(mem[0xff00 + 16*y+x] != (inspector->imgbuf2[(16*y+1)*512 + 256+16*x+1] & 0xff000000) >> 24) {
                col = 0xffffff | (mem[0xff00 + 16*y+x] << 24);
            } else {
                col = (inspector->imgbuf2[(16*y+1)*512 + 256+16*x+1] & 0xffff00ff) - 0x010001;
                if((col&0xffffff) < 0x300030) {
                    col &= ~0xffffff;
                    col |= 0x300030;
                }
            }
            
            for(int u = 0; u < 14; ++u)
                for(int v = 0; v < 14; ++v)
                    inspector->imgbuf2[(16*y+u+1)*512 + 256+16*x+v+1] = col;
        }
    }
    
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

    update_aspace_view(inspector);

    SDL_Renderer *renderer = SDL_GetRenderer(inspector->win);
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(inspector->bitmapTex, NULL, inspector->imgbuf, 640*sizeof(uint32_t));
    SDL_RenderCopy(renderer, inspector->bitmapTex, NULL, NULL);
    SDL_RenderPresent(renderer);    

    SDL_Renderer *renderer2 = SDL_GetRenderer(inspector->win2);
    SDL_RenderClear(renderer2);
    SDL_UpdateTexture(inspector->bitmapTex2, NULL, inspector->imgbuf2, 512*sizeof(uint32_t));
    SDL_RenderCopy(renderer2, inspector->bitmapTex2, NULL, NULL);
    SDL_RenderPresent(renderer2);
}

void memory_inspector_free(memory_inspector_t *inspector) {
    free(inspector->imgbuf);
    free(inspector->imgbuf2);

    SDL_Renderer *renderer = SDL_GetRenderer(inspector->win);
    SDL_DestroyTexture(inspector->bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(inspector->win);
    
    SDL_Renderer *renderer2 = SDL_GetRenderer(inspector->win2);
    SDL_DestroyTexture(inspector->bitmapTex2);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(inspector->win2);
}
