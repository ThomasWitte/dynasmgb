#include <sys/mman.h>
#include "core.h"

void free_block(gb_block *block) {
    munmap(block->mem, block->size);
}

bool init_vm(gb_vm *vm, const char *filename) {
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
    
    vm->state.flags = 0x0;
    
    vm->state.inst_count = 0;
    vm->state.ly_count = 0;
    vm->state.tima_count = 0;
    vm->state.div_count = 0;
    
    vm->state.ime = true;
    vm->state.halt = false;
    
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
	
	for(int block = 0; block < 4; ++block)
	    for(int i = 0; i < 0x4000; ++i) {
	        vm->compiled_blocks[block][i].exec_count = 0;
	        vm->compiled_blocks[block][i].func = 0;
	    }

    // init lcd
    vm->win = init_window();
    if(!vm->win)
        return false;

    // init sound
    if(!init_sound(&vm->sound, &vm->memory))
        return false;

	return true;
}

bool run_vm(gb_vm *vm) {
    uint16_t prev_pc = vm->state.last_pc;
    vm->state.last_pc = vm->state.pc;

    // compile next block / get cached block
    if(vm->state.pc < 0x4000) { // first block
        if(vm->compiled_blocks[0][vm->state.pc].exec_count == 0) {
            if(!compile(&vm->compiled_blocks[0][vm->state.pc], &vm->memory, vm->state.pc))
                goto compile_error;
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "execute function @%#x (count %i)\n", vm->state.pc,
               vm->compiled_blocks[0][vm->state.pc].exec_count);
        vm->compiled_blocks[0][vm->state.pc].exec_count++;
        vm->state.pc = vm->compiled_blocks[0][vm->state.pc].func(&vm->state);
//        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "finished\n");
    } else if(vm->state.pc < 0x8000) { // execute function in rom
        uint8_t bank = vm->memory.current_rom_bank;
        if(vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count == 0) {
            if(!compile(&vm->compiled_blocks[bank][vm->state.pc-0x4000], &vm->memory, vm->state.pc))
                goto compile_error;
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "execute function @%#x (count %i)\n", vm->state.pc,
               vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count);
        vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count++;
        vm->state.pc = vm->compiled_blocks[bank][vm->state.pc-0x4000].func(&vm->state);
    } else { // execute function in ram
        gb_block temp = {0};
        if(!compile(&temp, &vm->memory, vm->state.pc))
            goto compile_error;
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "execute function in ram\n");
        vm->state.pc = temp.func(&vm->state);
        free_block(&temp);
    }
    
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ioregs: STAT=%02x LY=%02x IE=%02x\n", vm->memory.mem[0xff41], vm->memory.mem[0xff44], vm->memory.mem[0xffff]);
    if(vm->state._sp < 0xfff0)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "stack: %02x %02x |%02x %02x %02x %02x\n",
                     vm->memory.mem[vm->state._sp-2], vm->memory.mem[vm->state._sp-1],
                     vm->memory.mem[vm->state._sp], vm->memory.mem[vm->state._sp+1],
                     vm->memory.mem[vm->state._sp+2], vm->memory.mem[vm->state._sp+3]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "register: A=%02x, BC=%02x%02x, DE=%02x%02x, HL=%02x%02x, SP=%04x\n",
           vm->state.a, vm->state.b, vm->state.c, vm->state.d, vm->state.e,
           vm->state.h, vm->state.l, vm->state._sp);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "previous address: %#x\n", prev_pc);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "next address: %#x\n", vm->state.pc);

#ifdef DEBUG_CG
    printf("%i\n", vm->state.pc);
#endif

    do {
        // check interrupts
        update_ioregs(&vm->state);

        uint16_t interrupt_addr = start_interrupt(&vm->state);
        if(interrupt_addr) {
#ifdef DEBUG_CG
            printf("interrupt from %i to %i\n", vm->state.pc, interrupt_addr);
#endif

            // end halt mode
            vm->state.halt = false;
        
            // save PC to stack
            vm->state._sp -= 2;
            *(uint16_t*)(&vm->state.mem->mem[vm->state._sp]) = vm->state.pc;
            // jump to interrupt address
            vm->state.pc = interrupt_addr;

            if(interrupt_addr == 0x40) { // VBLANK interrupt
                render_frame(vm->win);
            }
        }
        
        if(vm->state.halt) {
            vm->state.inst_count += 16;
        }
    } while(vm->state.halt);
    
    return true;
    
compile_error:
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "an error occurred while compiling the function @%#x.\n", vm->state.pc);

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ioregs: STAT=%02x LY=%02x IE=%02x\n", vm->memory.mem[0xff41], vm->memory.mem[0xff44], vm->memory.mem[0xffff]);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "register: A=%02x, BC=%02x%02x, DE=%02x%02x, HL=%02x%02x, SP=%04x\n",
           vm->state.a, vm->state.b, vm->state.c, vm->state.d, vm->state.e,
           vm->state.h, vm->state.l, vm->state._sp);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "previous address: %#x\n", prev_pc);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "next address: %#x\n", vm->state.pc);

    return false;
}

bool free_vm(gb_vm *vm) {
	for(int block = 0; block < 4; ++block)
	    for(int i = 0; i < 0x4000; ++i)
            if(vm->compiled_blocks[block][i].exec_count > 0)
	            free_block(&vm->compiled_blocks[block][i]);

    // destroy sound
    deinit_sound(&vm->sound);

    // destroy window
    deinit_window(vm->win);

    return gb_memory_free(&vm->memory);
}
