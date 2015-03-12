#include "lcd.h"

void
render_back(uint32_t *buf, uint8_t* addr_sp)
{
    int pal_grey[] = {0xffffff, 0xaaaaaa, 0x555555, 0x000000};

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
            buf[i] = pal_grey[(addr_sp[0xff47]>>(indx<<1))&3];
			//back_col[i][addr_sp[0xff44]]=indx;
			i++;
		}
		j=0;
		shftr=7;
	}

    // TODO: support 8x16 sprites
    // TODO: support sprite color palettes
    // TODO: support flags
    // TODO: sprite priorities
    if(addr_sp[0xff40] & 0x02) {
        int y = addr_sp[0xff44]+addr_sp[0xff42];
        int x_offset = addr_sp[0xff43];
        for(int sprite = 0; sprite < 40; ++sprite) {
            int sposy = addr_sp[0xfe00 + 4*sprite] - 8;
            int sposx = addr_sp[0xfe01 + 4*sprite] - 8 - addr_sp[0xff43];
            uint8_t tile_idx = addr_sp[0xfe02 + 4*sprite];
            uint8_t flags = addr_sp[0xfe03 + 4*sprite];

            if(sposy > y && sposy <= y+8) {
                // sprite wird in zeile angezeigt
                for(int x = 0; x < 8; ++x) {
                    int col = (addr_sp[0x8000 + 16*tile_idx + 2*(y+8-sposy)] >> (7-x)) % 2 +
                              (addr_sp[0x8001 + 16*tile_idx + 2*(y+8-sposy)] >> (7-x)) % 2;
                    if(col != 0) {
                        buf[y*160 + (flags & 0x20 ? 7-x : x) + sposx + x_offset] = pal_grey[col];
                    }
                }
            }
        }
    }
}

int render_thread_function(void *ptr) {
    gb_lcd* lcd = (gb_lcd*) ptr;

    SDL_Init(SDL_INIT_VIDEO);
    
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

    SDL_LockMutex(lcd->vblank_mutex);
    while(!lcd->exit) {
        SDL_CondWait(lcd->vblank_cond, lcd->vblank_mutex);
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
    
    SDL_LockMutex(lcd->vblank_mutex);
        
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

uint32_t imgbuf[160*144];

void update_line(uint8_t *mem) {
    render_back(imgbuf, mem);
}

void render_frame(gb_lcd* lcd) {
    SDL_Renderer *renderer = SDL_GetRenderer(lcd->win);
    static SDL_Texture *bitmapTex = NULL;
    if(!bitmapTex) {
        bitmapTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 160, 144);
    }
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(bitmapTex, NULL, imgbuf, 160*sizeof(uint32_t));
    SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
    SDL_RenderPresent(renderer);
}
