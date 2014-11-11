#ifndef DYNASMGB_MEMORY_H
#define DYNASMGB_MEMORY_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t* mem;
    const char* filename;
    enum {MBC_NONE = 0x00, MBC1 = 0x01, MBC2 = 0x05, MBC3 = 0x11, MBC5 = 0x19} mbc;
    int current_rom_bank;
    int current_ram_bank;
} gb_memory;

// emulate write through mbc
void gb_memory_write(gb_memory *mem, uint64_t addr, uint64_t value);

// initialize memory layout and map file filename
bool gb_memory_init(gb_memory *mem, const char *filename);

// change rom bank to bank if supported
bool gb_memory_change_rom_bank(gb_memory *mem, int bank);

// change ram bank to bank if supported
bool gb_memory_change_ram_bank(gb_memory *mem, int bank);

// free memory again
bool gb_memory_free(gb_memory *mem);

// print rom header information
void print_header_info(gb_memory *mem);

#endif
