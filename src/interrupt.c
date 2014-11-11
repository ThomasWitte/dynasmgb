#include "interrupt.h"

void update_ioregs(gb_state* state, gb_memory* memory) {
    // ly-register
    while(state->inst_count > 144) {
        state->inst_count -= 144;
        memory->mem[0xff44]++;
        memory->mem[0xff44] %= 153;
    }
}
