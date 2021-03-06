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
    uint8_t* ram_banks;
    const char* filename;
    int fd;
    enum {MBC_NONE = 0x00,
          MBC1 = 0x01,
          MBC1_RAM_BAT = 0x03,
          MBC2 = 0x05,
          MBC2_BAT = 0x06,
          MBC3_TIMER_RAM_BAT = 0x10,
          MBC3 = 0x11,
          MBC3_RAM_BAT = 0x13,
          MBC5 = 0x19,
          MBC5_RAM_BAT = 0x1b
         } mbc;
    uint8_t mbc_mode;
    uint8_t mbc_data;
    uint8_t current_rom_bank;
    uint8_t current_ram_bank;
    bool rtc_access;
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
    bool f_subtract;
    uint16_t _sp;
    uint16_t pc;
    uint64_t flags;
    
    uint16_t last_pc;

    // keys
    gb_keys keys;

    // instruction count
    uint64_t inst_count;
    
    uint64_t ly_count;
    uint64_t tima_count;
    uint64_t div_count;

    uint64_t next_update;

    // interrupt timers etc
    bool ime;
    
    // cpu is in halt state
    uint32_t halt;
    uint8_t halt_arg;
    
    // flag to trace callstack
    enum {REASON_OTHER = 0,
          REASON_CALL = 1,
          REASON_RST = 2,
          REASON_INT = 4,
          REASON_RET = 8} trap_reason;
} gb_state;

// emulate write through mbc
void gb_memory_write(gb_state *state, uint64_t addr, uint64_t value);

// initialize memory layout and map file filename
bool gb_memory_init(gb_memory *mem, const char *filename);

// change rom bank to bank if supported
void gb_memory_change_rom_bank(gb_memory *mem, int bank);

// change ram bank to bank if supported
void gb_memory_change_ram_bank(gb_memory *mem, int bank);

// reload current ram bank if supported
void gb_memory_reload_ram_bank(gb_memory *mem);

// prepare rtc read
void gb_memory_access_rtc(gb_memory *mem, int bank);

// update rtc time
void gb_memory_update_rtc_time(gb_memory *mem, int value);

// free memory again
bool gb_memory_free(gb_memory *mem);

// print rom header information
void print_header_info(gb_memory *mem);

#endif
