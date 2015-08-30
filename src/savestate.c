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
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "savestate.h"

bool savestate_load(gb_vm *vm, const char* filename) {
    SDL_LockMutex(vm->lcd.vblank_mutex);

    gb_vm temp = *vm;

    int fd = open(filename, O_RDONLY);
    
    if(fd < 0) {
        printf("Could not open file! (%i)\n", errno);
        return false;
    }
        
    int bytes_to_read = sizeof(gb_vm);
    while(bytes_to_read > 0) {
        bytes_to_read -= read(fd, (char*) vm, bytes_to_read);
    }
    
//    if(bytes_to_read != 0)
//        return false;
    
    bytes_to_read = 0x8000;
    while(bytes_to_read > 0) {
        bytes_to_read -= read(fd, temp.memory.mem + 0x8000, bytes_to_read);
    }
    
//    if(bytes_to_read != 0)
//        return false;
    
    close(fd);

    // repair pointers
    
    for(int i = 0; i < MAX_ROM_BANKS; ++i)
        for(int j = 0; j < 0x4000; ++j) {
            vm->compiled_blocks[i][j] = temp.compiled_blocks[i][j];
        }
    
    for(int i = 0; i < 0x80; ++i)
        vm->highmem_blocks[i] = temp.highmem_blocks[i];
    
    vm->memory.mem = temp.memory.mem;
    vm->memory.filename = temp.memory.filename;
    vm->memory.fd = temp.memory.fd;
    
    vm->state.mem = &vm->memory;
    vm->sound.memory = &vm->memory;
    
    vm->lcd.win = temp.lcd.win;
    vm->lcd.vblank_mutex = temp.lcd.vblank_mutex;
    vm->lcd.vblank_cond = temp.lcd.vblank_cond;
    vm->lcd.thread = temp.lcd.thread;
    
    vm->next_frame_time = SDL_GetTicks();
    
    // load memory bank
    
    gb_memory_change_rom_bank(&vm->memory, vm->memory.current_rom_bank);
    gb_memory_change_ram_bank(&vm->memory, vm->memory.current_ram_bank);

    SDL_UnlockMutex(vm->lcd.vblank_mutex);
    return true;
}

bool savestate_save(gb_vm *vm, const char* filename) {
    int fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    
    if(fd < 0) {
        printf("Could not open file! (%i)\n", errno);
        return false;
    }
        
    int bytes_to_write = sizeof(gb_vm);
    while(bytes_to_write > 0) {
        bytes_to_write -= write(fd, (char*) vm, bytes_to_write);
    }
    
//    if(bytes_to_write != 0)
//        return false;

    bytes_to_write = 0x8000;
    while(bytes_to_write > 0) {
        bytes_to_write -= write(fd, vm->memory.mem + 0x8000, bytes_to_write);
    }
    
//    if(bytes_to_write != 0)
//        return false;

    close(fd);
    
    return true;
}
