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

#include <sys/mman.h>
#include "core.h"

void free_block(gb_block *block) {
    munmap(block->mem, block->size);
}

bool init_vm(gb_vm *vm, const char *filename, int opt_level, bool init_io) {
    if(!gb_memory_init(&vm->memory, filename))
        return false;
    
    print_header_info(&vm->memory);
    
    vm->state.mem = &vm->memory;
    vm->state.a = 0x01;
    vm->state.b = 0x00;
    vm->state.c = 0x13;
    vm->state.d = 0x00;
    vm->state.e = 0xd8;
    vm->state.h = 0x01;
    vm->state.l = 0x4d;
    vm->state._sp = 0xfffe;
    vm->state.pc = 0x100;
    vm->state.f_subtract = false;
    
    vm->state.flags = 0x0;
    
    vm->state.inst_count = 0;
    vm->state.ly_count = 0;
    vm->state.tima_count = 0;
    vm->state.div_count = 0;
    vm->state.next_update = 0;
    
    vm->state.ime = true;
    vm->state.halt = false;
    
    vm->state.trap_reason = 0;
    
    vm->memory.mem[0xff05] = 0x00;
	vm->memory.mem[0xff06] = 0x00;
	vm->memory.mem[0xff07] = 0x00;
   	vm->memory.mem[0xff10] = 0x80;
	vm->memory.mem[0xff11] = 0xBF;
	vm->memory.mem[0xff12] = 0xF3;
	vm->memory.mem[0xff14] = 0xBF;
	vm->memory.mem[0xff16] = 0x3F;
	vm->memory.mem[0xff17] = 0x00;
	vm->memory.mem[0xff19] = 0xBF;
	vm->memory.mem[0xff1a] = 0x7F;
	vm->memory.mem[0xff1b] = 0xFF;
	vm->memory.mem[0xff1c] = 0x9F;
	vm->memory.mem[0xff1e] = 0xbf;
	vm->memory.mem[0xff20] = 0xff;
	vm->memory.mem[0xff21] = 0x00;
	vm->memory.mem[0xff22] = 0x00;
	vm->memory.mem[0xff23] = 0xbf;
	vm->memory.mem[0xff24] = 0x77;
	vm->memory.mem[0xff25] = 0xf3;
	vm->memory.mem[0xff26] = 0xf1;
	vm->memory.mem[0xff40] = 0x91;
	vm->memory.mem[0xff42] = 0x00;
	vm->memory.mem[0xff43] = 0x00;
	vm->memory.mem[0xff45] = 0x00;
	vm->memory.mem[0xff47] = 0xfc;
	vm->memory.mem[0xff48] = 0xff;
	vm->memory.mem[0xff49] = 0xff;
	vm->memory.mem[0xff4a] = 0x00;
	vm->memory.mem[0xff4b] = 0x00;
	vm->memory.mem[0xffff] = 0x00;
	
	for(int block = 0; block < MAX_ROM_BANKS; ++block)
	    for(int i = 0; i < 0x4000; ++i) {
	        vm->compiled_blocks[block][i].exec_count = 0;
	        vm->compiled_blocks[block][i].func = 0;
	    }

    for(int i = 0; i < 0x80; ++i) {
	    vm->highmem_blocks[i].exec_count = 0;
	    vm->highmem_blocks[i].func = 0;
	}

    if(init_io) {
        // init lcd
        if(!init_window(&vm->lcd))
            return false;

        vm->draw_frame = true;
        vm->next_frame_time = SDL_GetTicks();
        vm->time_busy = 0;
        vm->last_time = 0;
        vm->frame_cnt = 0;

        vm->opt_level = opt_level;

        // init sound
        if(!init_sound(&vm->sound, &vm->memory))
            return false;
    }

	return true;
}

bool run_vm(gb_vm *vm) {
    uint16_t prev_pc = vm->state.last_pc;
    vm->state.last_pc = vm->state.pc;

    // compile next block / get cached block
    if(vm->state.pc < 0x4000) { // first block
        if(vm->compiled_blocks[0][vm->state.pc].exec_count == 0) {
            if(!compile(&vm->compiled_blocks[0][vm->state.pc], &vm->memory, vm->state.pc, vm->opt_level))
                goto compile_error;
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "execute function @%#x (count %i)\n", vm->state.pc,
               vm->compiled_blocks[0][vm->state.pc].exec_count);
        vm->compiled_blocks[0][vm->state.pc].exec_count++;
        vm->state.pc = vm->compiled_blocks[0][vm->state.pc].func(&vm->state);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "finished\n");
    } else if(vm->state.pc < 0x8000) { // execute function in rom
        uint8_t bank = vm->memory.current_rom_bank;
        if(vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count == 0) {
            if(!compile(&vm->compiled_blocks[bank][vm->state.pc-0x4000], &vm->memory, vm->state.pc, vm->opt_level))
                goto compile_error;
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "execute function @%#x (count %i)\n", vm->state.pc,
               vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count);
        vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count++;
        vm->state.pc = vm->compiled_blocks[bank][vm->state.pc-0x4000].func(&vm->state);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "finished\n");
    } else if(vm->state.pc >= 0xff80) { // execute function in internal ram, e.g. for dma
        if(vm->highmem_blocks[vm->state.pc-0xff80].exec_count == 0) {
            if(!compile(&vm->highmem_blocks[vm->state.pc-0xff80], &vm->memory, vm->state.pc, vm->opt_level))
                goto compile_error;
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "execute function @%#x (count %i)\n", vm->state.pc,
               vm->highmem_blocks[vm->state.pc-0xff80].exec_count);
        vm->highmem_blocks[vm->state.pc-0xff80].exec_count++;
        vm->state.pc = vm->highmem_blocks[vm->state.pc-0xff80].func(&vm->state);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "finished\n");
    } else { // execute function in ram
        gb_block temp = {0};
        if(!compile(&temp, &vm->memory, vm->state.pc, vm->opt_level))
            goto compile_error;
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "execute function in ram\n");
        vm->state.pc = temp.func(&vm->state);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "finished\n");
        free_block(&temp);
    }
    
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ioregs: STAT=%02x LY=%02x IF=%02x IE=%02x\n", vm->memory.mem[0xff41], vm->memory.mem[0xff44], vm->memory.mem[0xff0f], vm->memory.mem[0xffff]);
//    if(vm->state._sp < 0xfff0)
//        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "stack: %02x %02x |%02x %02x %02x %02x\n",
//                     vm->memory.mem[vm->state._sp-2], vm->memory.mem[vm->state._sp-1],
//                     vm->memory.mem[vm->state._sp], vm->memory.mem[vm->state._sp+1],
//                     vm->memory.mem[vm->state._sp+2], vm->memory.mem[vm->state._sp+3]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "register: A=%02x, BC=%02x%02x, DE=%02x%02x, HL=%02x%02x, SP=%04x\n",
           vm->state.a, vm->state.b, vm->state.c, vm->state.d, vm->state.e,
           vm->state.h, vm->state.l, vm->state._sp);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "previous address: %#x\n", prev_pc);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "next address: %#x\n", vm->state.pc);

#ifdef DEBUG_CG
    printf("%i\n", vm->state.pc);
#endif

    do {
        if(vm->state.inst_count >= vm->state.next_update) {
            // check interrupts
            update_ioregs(&vm->state);

            if(vm->memory.mem[0xff44] == 144) {
                if(vm->draw_frame) {
                    unsigned time = SDL_GetTicks();
                    if(!SDL_TICKS_PASSED(time, vm->next_frame_time)) {
                        SDL_Delay(vm->next_frame_time - time);
                    }

                    vm->time_busy += time - vm->last_time;
                    vm->last_time = SDL_GetTicks();

                    if(++(vm->frame_cnt) == 60) {
                        vm->frame_cnt = 0;
                        float load = (vm->time_busy) / (60*17.0);
                        char title[15];
                        sprintf(title, "load: %.2f", load);
                        SDL_SetWindowTitle(vm->lcd.win, title);
                        vm->time_busy = 0;
                    }

                    vm->next_frame_time += 17; // 17ms until next frame
                    SDL_CondBroadcast(vm->lcd.vblank_cond);
                    vm->draw_frame = false;
                }
            } else {
                vm->draw_frame = true;
            }

            uint16_t interrupt_addr = start_interrupt(&vm->state);
            if(interrupt_addr) {
    #ifdef DEBUG_CG
                printf("interrupt from %i to %i\n", vm->state.pc, interrupt_addr);
    #endif

                // end halt mode
                if(vm->state.halt == 1)
                    vm->state.halt = 0;

                // save PC to stack
                vm->state._sp -= 2;
                *(uint16_t*)(&vm->state.mem->mem[vm->state._sp]) = vm->state.pc;
                // jump to interrupt address
                vm->state.pc = interrupt_addr;

                //if(interrupt_addr == 0x40) { // VBLANK interrupt
                //    render_frame(vm->win);
                //}
            }
            
            if(vm->state.halt == WAIT_STAT3 && (vm->memory.mem[0xff41] & 0x3) == 0x3) {
                // end wait for stat mode 3
                vm->state.halt = 0;
            }
            
            if(vm->state.halt == WAIT_LY && vm->memory.mem[0xff44] == vm->state.halt_arg) {
                // end wait for ly
                vm->state.halt = 0;
            }
            
            vm->state.next_update = next_update_time(&vm->state);
        }
        
        if(vm->state.halt != 0) {
            vm->state.inst_count = (vm->state.inst_count < vm->state.next_update ? vm->state.next_update : vm->state.inst_count + 16);
        }
    } while(vm->state.halt != 0);
    
    return true;
    
compile_error:
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "an error occurred while compiling the function @%#x.\n", vm->state.pc);

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ioregs: STAT=%02x LY=%02x IF=%02x IE=%02x\n", vm->memory.mem[0xff41], vm->memory.mem[0xff44], vm->memory.mem[0xff0f], vm->memory.mem[0xffff]);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "register: A=%02x, BC=%02x%02x, DE=%02x%02x, HL=%02x%02x, SP=%04x\n",
           vm->state.a, vm->state.b, vm->state.c, vm->state.d, vm->state.e,
           vm->state.h, vm->state.l, vm->state._sp);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "previous address: %#x\n", prev_pc);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "next address: %#x\n", vm->state.pc);

    return false;
}

void show_statistics(gb_vm *vm) {
	printf("some statistics:\n");

	uint64_t compiled_functions = 0;
	uint64_t most_executed = 0;
	uint64_t most_executed_addr = 0;
	uint64_t total_executed = 0;

	for(int block = 0; block < MAX_ROM_BANKS; ++block)
	    for(int i = 0; i < 0x4000; ++i) {
			if(vm->compiled_blocks[block][i].exec_count > 0) {
				++compiled_functions;
				if(vm->compiled_blocks[block][i].exec_count > most_executed) {
					most_executed_addr = block * 0x4000 + i;
					most_executed = vm->compiled_blocks[block][i].exec_count;
				}
				total_executed += vm->compiled_blocks[block][i].exec_count;
			}
		}
	
	printf("\ttotal compiled rom functions: %lu\n", compiled_functions);
	printf("\thottest: block @%#lx, %lu times executed\n", most_executed_addr, most_executed);
	int cnt = vm->compiled_blocks[0][0x40].exec_count ? vm->compiled_blocks[0][0x40].exec_count : 1;
	printf("\texecuted blocks total / per frame: %lu/%lu\n", total_executed, total_executed / cnt);
	printf("\tframes: %u\n", vm->compiled_blocks[0][0x40].exec_count);
}

bool free_vm(gb_vm *vm) {
	show_statistics(vm);

	for(int block = 0; block < MAX_ROM_BANKS; ++block)
	    for(int i = 0; i < 0x4000; ++i)
            if(vm->compiled_blocks[block][i].exec_count > 0)
	            free_block(&vm->compiled_blocks[block][i]);

    for(int i = 0; i < 0x80; ++i)
        if(vm->highmem_blocks[i].exec_count > 0)
	        free_block(&vm->highmem_blocks[i]);


    // destroy sound
    deinit_sound(&vm->sound);

    // destroy window
    deinit_window(&vm->lcd);

    return gb_memory_free(&vm->memory);
}
