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

#include "lcd.h"

void
render_back(uint32_t *buf, uint8_t* addr_sp)
{
    uint32_t pal_grey[] = {0xffffff, 0xaaaaaa, 0x555555, 0x000000};

	int i, j;
	uint8_t *ptr_data;
	uint8_t *ptr_map;
	uint8_t indx, shftr, x, y, x1;
	int16_t tile_num;
	
	/* Point to tile map */
	if (addr_sp[0xff40]&0x8)
		ptr_map=addr_sp+0x9c00;
	else
		ptr_map=addr_sp+0x9800;
	
	/* Current line + SCROLL Y */
	y = addr_sp[0xff44]+addr_sp[0xff42];
	/* SCROLL X */
	j = addr_sp[0xff43];
	x1 = j>>3;
	
	/* Advance to row in tile map */
	ptr_map += ((y>>3)<<5)&0x3ff;
	
    i=addr_sp[0xff44]*160; //0;
	j &= 7;
	x = 8-j;
    shftr=((uint8_t)(~j))%8; // shift factor
	for (; x<168; x+=8) {
		tile_num = ptr_map[x1++&0x1f];
		if (!(addr_sp[0xff40]&0x10))
			tile_num = 256 + (signed char)tile_num;
		ptr_data = addr_sp+0x8000+(tile_num<<4); // point to tile; each tile is 8*8*2=128 bits=16 bytes
		ptr_data+=(y&7)<<1; // point to row in tile depending on LY and SCROLL Y; each row is 8*2=16 bits=2 bytes
		for (; j<8 && (x+j)<168; shftr--, j++) {
			indx = ((ptr_data[0]>>shftr)&1)|((((ptr_data[1]>>shftr))&1)<<1);
			// if bit 0 in LCDC is not set, screen is blank
            buf[i] = addr_sp[0xff40]&0x01 ? pal_grey[(addr_sp[0xff47]>>(indx<<1))&3] : (unsigned)-1;
			//back_col[i][addr_sp[0xff44]]=indx;
			i++;
		}
		j=0;
		shftr=7;
	}

    if(addr_sp[0xff40] & 0x20) {
        uint8_t wx = addr_sp[0xff4b]-7;
        uint8_t wy = addr_sp[0xff4a];

        int y = addr_sp[0xff44]; // current line to update
        uint8_t *tile_map_ptr = addr_sp + (addr_sp[0xff40]&0x08 ? 0x9800 : 0x9c00) + (y-wy)/8 * 32;
        uint8_t *tile_data_ptr = addr_sp + (addr_sp[0xff40]&0x10 ? 0x8000 : 0x9000);
        int i = y*160;
        
        for(int x = 0; x < 160; ++x) {
            if(x < wx || y < wy)
                continue;
        
            uint8_t *tile = tile_data_ptr + 16*(addr_sp[0xff40]&0x10 ? tile_map_ptr[(x-wx)/8] : (int8_t)tile_map_ptr[(x-wx)/8]);
            tile += (y-wy)%8 * 2;
            
            int col = ((*tile >> (7-(x-wx)%8)) & 1) + (((*(tile+1) >> (7-(x-wx)%8)) & 1) << 1);
            
            buf[i+x] = pal_grey[(addr_sp[0xff47]>>(col<<1))&3];
        }
    }

    // TODO: sprite priorities
    if(addr_sp[0xff40] & 0x02) {
        bool sprite_8x16_mode = addr_sp[0xff40] & 0x04 ? true : false;
        int y = addr_sp[0xff44];//+addr_sp[0xff42];
        for(int sprite = 0; sprite < 40; ++sprite) {
            int sposy = addr_sp[0xfe00 + 4*sprite] - 16;
            int sposx = addr_sp[0xfe01 + 4*sprite] - 8;
            
            // TODO: support 8x16 sprites
            uint8_t flags = addr_sp[0xfe03 + 4*sprite];
            uint8_t tile_idx = sprite_8x16_mode ?
                (flags & 0x40 ? addr_sp[0xfe02 + 4*sprite]|0x01 : addr_sp[0xfe02 + 4*sprite]&~0x01) :
                addr_sp[0xfe02 + 4*sprite];
            uint8_t obp = (flags & 0x10 ? addr_sp[0xff49] : addr_sp[0xff48]);

            if(sposy > y-8 && sposy <= y) {
                // sprite wird in zeile angezeigt
                for(int x = 0; x < 8; ++x) {
                    int px_x = (flags & 0x20 ? 7-x : x) + sposx;
                    int px_y = (flags & 0x40 ? 7-y+sposy : y-sposy);

                    int col = ((addr_sp[0x8000 + 16*tile_idx + 2*px_y] >> (7-x)) & 1) +
                              (((addr_sp[0x8001 + 16*tile_idx + 2*px_y] >> (7-x)) & 1) << 1);
                              
                    if(col != 0 && px_x >= 0 && px_x < 160) {
                        if(!(flags & 0x80) || buf[y*160 + px_x] == pal_grey[0]) {
                            buf[y*160 + px_x] = pal_grey[obp>>(col<<1)&3];
                        }
                    }
                }
            }
            
            if(sprite_8x16_mode && sposy > y-16 && sposy <= y-8) {
                tile_idx = (flags & 0x40 ?
                    addr_sp[0xfe02 + 4*sprite]&~0x01 :
                    addr_sp[0xfe02 + 4*sprite]|0x01);
                // sprite wird in zeile angezeigt
                for(int x = 0; x < 8; ++x) {
                    int px_x = (flags & 0x20 ? 7-x : x) + sposx;
                    int px_y = (flags & 0x40 ? 15-y+sposy : y-8-sposy);

                    int col = ((addr_sp[0x8000 + 16*tile_idx + 2*px_y] >> (7-x)) & 1) +
                              (((addr_sp[0x8001 + 16*tile_idx + 2*px_y] >> (7-x)) & 1) << 1);
                              
                    if(col != 0 && px_x >= 0 && px_x < 160) {
                        if(!(flags & 0x80) || buf[y*160 + px_x] == pal_grey[0]) {
                            buf[y*160 + px_x] = pal_grey[obp>>(col<<1)&3];
                        }
                    }
                }
            }
        }
    }
}

uint32_t imgbuf[2][160*144];
int cur_imgbuf = 0;

int render_thread_function(void *ptr) {
    gb_lcd* lcd = (gb_lcd*) ptr;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_LockMutex(lcd->vblank_mutex);

    lcd->win = SDL_CreateWindow(
        "dynasmgb",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        160*3, 144*3, SDL_WINDOW_OPENGL);

    if(!lcd->win) {
        printf("Could not create window!\n");
        return false;
    }

    SDL_CreateRenderer(lcd->win, -1, SDL_RENDERER_ACCELERATED);

    while(!lcd->exit) {
        SDL_CondWait(lcd->vblank_cond, lcd->vblank_mutex);
        cur_imgbuf = (cur_imgbuf+1)%2;
        render_frame(lcd);
    }
    SDL_UnlockMutex(lcd->vblank_mutex);
    
    SDL_Renderer *renderer = SDL_GetRenderer(lcd->win);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(lcd->win);
    
    return 0;
}

bool init_window(gb_lcd* lcd) {
    lcd->vblank_mutex = SDL_CreateMutex();
    lcd->vblank_cond = SDL_CreateCond();

    lcd->exit = false;

    lcd->thread = SDL_CreateThread(render_thread_function, "Render Thread", (void*)lcd);
    
    return true;
}

void deinit_window(gb_lcd* lcd) {
    lcd->exit = true;
    SDL_CondBroadcast(lcd->vblank_cond);
    
    SDL_WaitThread(lcd->thread, 0);

    SDL_DestroyCond(lcd->vblank_cond);
    SDL_DestroyMutex(lcd->vblank_mutex);
    
    SDL_Quit();
}


void update_line(uint8_t *mem) {
    render_back(imgbuf[cur_imgbuf], mem);
}

void render_frame(gb_lcd* lcd) {
    SDL_Renderer *renderer = SDL_GetRenderer(lcd->win);
    static SDL_Texture *bitmapTex = NULL;
    if(!bitmapTex) {
        bitmapTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    }
    //SDL_RenderClear(renderer);
    SDL_UpdateTexture(bitmapTex, NULL, imgbuf[(cur_imgbuf+1)%2], 160*sizeof(uint32_t));
    //void *pixels = NULL;
    //int pitch = 0;
    //SDL_LockTexture(bitmapTex, NULL, &pixels, &pitch);
    //memcpy(pixels, imgbuf[(cur_imgbuf+1)%2], 160*144*sizeof(uint32_t));
    //SDL_UnlockTexture(bitmapTex);
    
    SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
    SDL_RenderPresent(renderer);
}
