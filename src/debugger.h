#ifndef DYNASMGB_DEBUG_H
#define DYNASMGB_DEBUG_H

#include <stdbool.h>
#include "core.h"

typedef struct {
    gb_vm* vm;
    int breakpoint;
    bool enabled;
} gb_debug;

void gb_debug_init(gb_debug* dbg, gb_vm* vm);
void gb_debug_free(gb_debug* dbg);

void gb_debug_set_enabled(gb_debug* dbg, bool mode);
bool gb_debug_is_enabled(gb_debug* dbg);
bool gb_debug_prompt(gb_debug *dbg);
void gb_debug_set_breakpoint(gb_debug *dbg, int breakpoint);

bool run_vm_debug(gb_debug *dbg);

#endif
