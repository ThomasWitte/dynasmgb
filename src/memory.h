#ifndef DYNASMGB_MEMORY_H
#define DYNASMGB_MEMORY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef DEBUG
#define LOG_DEBUG(...) printf(__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

typedef struct {
    uint8_t* mem;
    const char* filename;
    enum {MBC_NONE = 0x00, MBC1 = 0x01, MBC2 = 0x05, MBC3 = 0x11, MBC5 = 0x19} mbc;
    int current_rom_bank;
    int current_ram_bank;
} gb_memory;

typedef struct {
    enum {GB_KEY_RIGHT  = 0x01,
          GB_KEY_LEFT   = 0x02,
          GB_KEY_UP     = 0x04,
          GB_KEY_DOWN   = 0x08,
          GB_KEY_A      = 0x10,
          GB_KEY_B      = 0x20,
          GB_KEY_SELECT = 0x40,
          GB_KEY_START  = 0x80} state;
} gb_keys;

typedef struct {
    // memory
    gb_memory *mem;

    // register
    uint8_t a, b, c, d, e, h, l;
    uint16_t _sp;
    uint16_t pc;
    
    uint16_t last_pc;

    // keys
    gb_keys keys;

    // instruction count
    uint64_t inst_count;
    
    uint64_t ly_count;
    uint64_t tima_count;

    // interrupt timers etc
    bool ime;
} gb_state;

// emulate write through mbc
void gb_memory_write(gb_state *state, uint64_t addr, uint64_t value);

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
