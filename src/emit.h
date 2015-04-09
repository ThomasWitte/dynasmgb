#ifndef DYNASMGB_EMIT_H
#define DYNASMGB_EMIT_H

#define YYSTYPE GList*
#define DATA(X) ((gb_instruction*)(X)->data)

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <glib-2.0/glib.h>
#include "memory.h"
#include "optimize.tab.h"

typedef struct {
    enum yytokentype opcode;
    enum {NONE, REG_A, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L /*=7*/,
          REG_AF, REG_BC, REG_DE, REG_HL, REG_SP, IMM8, IMM16 /*=14*/,
          MEM_BC, MEM_DE, MEM_HL, MEM_16, MEM_8, MEM_C, MEM_INC_HL, MEM_DEC_HL /*=22*/,
          CC_Z, CC_C, CC_NZ, CC_NC,
          MEM_0x00, MEM_0x08, MEM_0x10, MEM_0x18, MEM_0x20, MEM_0x28, MEM_0x30, MEM_0x38,
          BIT_0, BIT_1, BIT_2, BIT_3, BIT_4, BIT_5, BIT_6, BIT_7
         } op1, op2;
    uint8_t *args;
	uint16_t address;
    uint8_t bytes;
    uint8_t cycles, alt_cycles;
    enum {INST_FLAG_NONE            = 0x00,
          INST_FLAG_PRESERVE_CC     = 0x01,
          INST_FLAG_PERS_WRITE      = 0x02,
          INST_FLAG_USES_CC         = 0x04,
          INST_FLAG_AFFECTS_CC      = 0x08,
          INST_FLAG_ENDS_BLOCK      = 0x10,
          INST_FLAG_SAVE_CC         = 0x20,
          INST_FLAG_RESTORE_CC      = 0x40
    } flags;
} gb_instruction;

typedef struct {
    uint16_t(*func)(gb_state*);
    unsigned exec_count;
    uint16_t end_address;
    size_t size;
    void *mem;
} gb_block;

bool emit(gb_block *block, GList *inst);

#endif
