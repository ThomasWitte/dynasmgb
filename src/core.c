#include "core.h"

void free_block(gb_block *block) {
//    free(block->func);
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
    
    vm->state.inst_count = 0;
    
    vm->state.ime = true;
    
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
	
	for(int block = 0; block < 2; ++block)
	    for(int i = 0; i < 0x4000; ++i) {
	        vm->compiled_blocks[block][i].exec_count = 0;
	        vm->compiled_blocks[block][i].func = 0;
	    }
	return true;
}

bool run_vm(gb_vm *vm) {
    // check interrupts
    update_ioregs(&vm->state, &vm->memory);
    
    // compile next block / get cached block
    if(vm->state.pc < 0x4000) { // first block
        if(vm->compiled_blocks[0][vm->state.pc].exec_count == 0) {
            if(!compile(&vm->compiled_blocks[0][vm->state.pc], &vm->memory, vm->state.pc))
                goto compile_error;
        }
        printf("execute function @%#x (count %i)\n", vm->state.pc,
               vm->compiled_blocks[0][vm->state.pc].exec_count);
        vm->compiled_blocks[0][vm->state.pc].exec_count++;
        vm->state.pc = vm->compiled_blocks[0][vm->state.pc].func(&vm->state);
    } else if(vm->state.pc < 0x8000) { // execute function in rom
        int bank = vm->memory.current_rom_bank;
        if(vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count == 0) {
            if(!compile(&vm->compiled_blocks[bank][vm->state.pc-0x4000], &vm->memory, vm->state.pc))
                goto compile_error;
        }
        printf("execute function @%#x (count %i)\n", vm->state.pc,
               vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count);
        vm->compiled_blocks[bank][vm->state.pc-0x4000].exec_count++;
        vm->state.pc = vm->compiled_blocks[bank][vm->state.pc-0x4000].func(&vm->state);
    } else { // execute function in ram
        gb_block temp = {0};
        if(!compile(&temp, &vm->memory, vm->state.pc))
            goto compile_error;
        printf("execute function in ram\n");
        vm->state.pc = temp.func(&vm->state);
        free_block(&temp);
    }
    
    printf("ioregs: LY=%02x\n", vm->memory.mem[0xff44]);
    printf("register: A=%02x, BC=%02x%02x, DE=%02x%02x, HL=%02x%02x, SP=%04x\n",
           vm->state.a, vm->state.b, vm->state.c, vm->state.d, vm->state.e,
           vm->state.h, vm->state.l, vm->state._sp);
    printf("next address: %#x\n", vm->state.pc);
    
    return true;
    
compile_error:
    printf("an error occurred while compiling the function @%#x.\n", vm->state.pc);
    return false;
}

bool free_vm(gb_vm *vm) {
	for(int block = 0; block < 2; ++block)
	    for(int i = 0; i < 0x4000; ++i)
	        free_block(&vm->compiled_blocks[block][i]);

    return gb_memory_free(&vm->memory);
}
