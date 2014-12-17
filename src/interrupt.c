#include "interrupt.h"
#include "lcd.h"

void update_ioregs(gb_state* state) {
    uint8_t *mem = state->mem->mem;

    // tima-register 0xff05
    int clock[] = {256, 4, 16, 64};
    int cl = clock[mem[0xff07] & 0x3];

    if(state->inst_count > state->tima_count + cl) {
        state->tima_count = state->inst_count;
        if(mem[0xff07] & 0x4) {
            mem[0xff05]++;
            if(mem[0xff05] == 0) {
                mem[0xff05] = mem[0xff06];
                // timer interrupt selected
                mem[0xff0f] |= 0x04;
            }
        }
    }

    // div-register 0xff04
    if(state->inst_count > state->div_count + cl) {
        state->div_count = state->inst_count;
        mem[0xff04]++;
    }

    // reset the coincidence flag
    mem[0xff41] &= ~0x04;

    // ly-register 0xff44
    if(state->inst_count > state->ly_count + 114) {
        state->ly_count = state->inst_count;
        mem[0xff44]++;
        mem[0xff44] %= 153;

        if(mem[0xff44] < 144) {
            update_line(mem);
        }

        if(mem[0xff45] == mem[0xff44]) {
            // Set the coincidence flag
            mem[0xff41] |= 0x04;
            
            // Coincidence interrupt selected
            if(mem[0xff41] & 0x40)
                // stat interrupt occurs
                mem[0xff0f] |= 0x02;
        }

        // if-register 0xff0f
        if(mem[0xff44] == 144) {
            // VBLANK-Interrupt ist anhängig
            mem[0xff0f] |= 0x01;

            // mode 1 interrupt selected
            if(mem[0xff41] & 0x10 && (mem[0xff41] & 0x03) != 1)
                // stat interrupt occurs
                mem[0xff0f] |= 0x02;

            // LCDC Stat mode 1
            mem[0xff41] &= ~0x03;
            mem[0xff41] |= 0x01;
        }
    }

    if(mem[0xff44] < 144) {
        // if not VBLANK
        if(state->inst_count-state->ly_count < 20) {
            // mode 2 interrupt selected
            if(mem[0xff41] & 0x20 && (mem[0xff41] & 0x03) != 2)
                // stat interrupt occurs
                mem[0xff0f] |= 0x02;

            // LCDC Stat mode 2
            mem[0xff41] &= ~0x03;
            mem[0xff41] |= 0x02;
        } else if(state->inst_count-state->ly_count < 63) {
            // LCDC Stat mode 3
            mem[0xff41] &= ~0x03;
            mem[0xff41] |= 0x03;
        } else {
            // mode 0 interrupt selected
            if(mem[0xff41] & 0x08 && (mem[0xff41] & 0x03) != 0)
                // stat interrupt occurs
                mem[0xff0f] |= 0x02;

            // LCDC Stat mode 0
            mem[0xff41] &= ~0x03;
        }
    }
}

uint16_t start_interrupt(gb_state* state) {
    if(state->ime == true) { // interrupt master enable is set

        uint8_t *mem = state->mem->mem;
        uint8_t interrupts = mem[0xffff] & mem[0xff0f];

        if(interrupts & 0x01) { // VBLANK on
            LOG_DEBUG("VBLANK interrupt!\n");
            state->ime = 0;       // disable interrupts
            mem[0xff0f] &= ~0x01; // reset VBLANK interrupt
            return 0x40;          // return interrupt start address
        }
        
        if(interrupts & 0x02) { // LCD stat on
            LOG_DEBUG("STAT interrupt!\n");
            state->ime = 0;
            mem[0xff0f] &= ~0x02; // reset STAT interrupt
            return 0x48;          // return interrupt start address
        }
        
        if(interrupts & 0x04) { // Timer
            LOG_DEBUG("TIMER interrupt!\n");
            state->ime = 0;
            mem[0xff0f] &= ~0x04; // reset TIMER interrupt
            return 0x50;          // return interrupt start address
        }
        
        if(interrupts & 0x08) { // Serial
            state->ime = 0;
        }
        
        if(interrupts & 0x10) { // Joypad
            state->ime = 0;
        }
    }
    return 0;
}
