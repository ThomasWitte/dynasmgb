#ifndef DYNASMGB_SSTATE_H
#define DYNASMGB_SSTATE_H

#include "core.h"

bool savestate_load(gb_vm *vm, const char* filename);
bool savestate_save(gb_vm *vm, const char* filename);

#endif
