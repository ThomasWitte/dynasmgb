#ifndef DYNASMGB_DEBUG_H
#define DYNASMGB_DEBUG_H

#include <stdbool.h>
#include "core.h"

typedef struct {
    uint16_t sp;
    uint16_t pc;
    uint8_t trap_reason;
} gb_stack_frame;

typedef struct {
    gb_stack_frame* data;
    unsigned size;
    unsigned max_size;    
} gb_stack;

typedef struct {
    gb_vm* vm;
    int breakpoint;
    int watchpoint;
    bool enabled;
    uint8_t watchpoint_value;
    gb_stack stack;
    memory_inspector_t* meminspector;
} gb_debug;

void gb_debug_init(gb_debug* dbg, gb_vm* vm);
void gb_debug_free(gb_debug* dbg);

void gb_debug_set_enabled(gb_debug* dbg, bool mode);
bool gb_debug_is_enabled(gb_debug* dbg);
bool gb_debug_prompt(gb_debug *dbg);
void gb_debug_set_breakpoint(gb_debug *dbg, int breakpoint);
void gb_debug_set_watchpoint(gb_debug *dbg, int watchpoint);

bool run_vm_debug(gb_debug *dbg);

#endif
