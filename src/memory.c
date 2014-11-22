#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "memory.h"

// emulate write through mbc
void gb_memory_write(gb_memory *mem, uint64_t addr, uint64_t value) {
    addr &= 0xffff;
    value &= 0xff;

    if(addr < 0x8000) {
        LOG_DEBUG("write to rom @address %#lx\n", addr);
    } else if(addr == 0xff05) {
        LOG_DEBUG("Memory write to %#lx, reset to 0\n", addr);
        mem->mem[addr] = 0;
    } else if(addr == 0xff46) { // DMA Transfer to OAM RAM
        // TODO: Sprünge in den RAM detektieren und DMA optimieren
        LOG_DEBUG("DMA Transfer started.\n");
        mem->mem[addr] = value;
        memcpy(&mem->mem[0xfe00], &mem->mem[value << 8], 0xa0);
    } else {
        LOG_DEBUG("Memory write to %#lx, value is %#lx\n", addr, value);
        mem->mem[addr] = value;
    }
}

// initialize memory layout and map file filename
bool gb_memory_init(gb_memory *mem, const char *filename) {
    //TODO: memory aliasing
    //TODO: memory banking

    int fd = open(filename, O_RDONLY);
    if(fd < 0) {
        printf("Could not open file! (%i)\n", errno);
        return false;
    }
    
    mem->mem = mmap((void*)0x1000000, 0x8000, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mem->mem == MAP_FAILED) {
        printf("Map failed! (%i)\n", errno);
        return false;
    }
    
    if(mmap(mem->mem + 0x8000, 0x8000, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED) {
        printf("Allocating memory failed! (%i)\n", errno);
        return false;
    }
    close(fd);

    mem->filename = filename;
    mem->mbc = mem->mem[0x0147];
    
    mem->current_rom_bank = 1;
    mem->current_ram_bank = 0;
    
    return true;
}

// change rom bank to bank if supported
bool gb_memory_change_rom_bank(gb_memory *mem, int bank) {
    return false;
}

// change ram bank to bank if supported
bool gb_memory_change_ram_bank(gb_memory *mem, int bank) {
    return false;
}

// free memory again
bool gb_memory_free(gb_memory *mem) {
    if(munmap(mem->mem, 0x8000) != 0 ||
       munmap(mem->mem + 0x8000, 0x8000) != 0)
    {
        printf("munmap failed (%i)\n", errno);
        return false;
    }
    return true;
}

void print_header_info(gb_memory *mem) {
    printf("rom information for file %s:\n"
           "==================================================\n", mem->filename);
    printf("Title: %s\n", mem->mem + 0x134);
    printf("Manufacturer: %s\n", mem->mem + 0x13f);
    printf("Cartridge Type: %#2x\n", mem->mem[0x147]);
    printf("ROM Size: %i kByte\n", 32 << mem->mem[0x148]);
    printf("RAM Size: %i kByte\n", mem->mem[0x149] > 0 ? 1 << (mem->mem[0x149]*2-1) : 0);
	printf("==================================================\n");
}
