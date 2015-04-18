#ifndef DYNASMGB_OPTIMIZE_H
#define DYNASMGB_OPTIMIZE_H

#include <stdbool.h>
#include "emit.h"

bool optimize_block(GList** instructions, int opt_level);

#endif
