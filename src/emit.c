/*
** This file has been pre-processed with DynASM.
** http://luajit.org/dynasm.html
** DynASM version 1.3.0, DynASM x64 version 1.3.0
** DO NOT EDIT! The original file is in "src/emit.dasc".
*/

#line 1 "src/emit.dasc"
#include "emit.h"

#include "../dynasm/dasm_proto.h"
#include "../dynasm/dasm_x86.h"

#include <sys/mman.h>

    //|.arch x64
#if DASM_VERSION != 10300
#error "Version mismatch between DynASM and included encoding engine"
#endif
#line 9 "src/emit.dasc"

    //|.define    A,      r0b
    //|.define    xA,     r0
    //|.define    B,      r1b
    //|.define    xB,     r1
    //|.define    C,      r2b
    //|.define    xC,     r2
    //|.define    D,      r3b
    //|.define    xD,     r3
    //|.define    E,      r13b
    //|.define    xE,     r13
    //|.define    H,      r5b
    //|.define    xH,     r5
    //|.define    L,      r6b
    //|.define    xL,     r6
    //|.define    SP,     r7w
    //|.define    xSP,    r7
    //|.define    aMem,   r8
    //|.define    aState, r9
    //|.define    tmp1,   r10
    //|.define    tmp1b,  r10b
    //|.define    tmp1w,  r10w
    //|.define    tmp2,   r11
    //|.define    tmp2b,  r11b
    //|.define    tmp2w,  r11w
    //|.define    tmp3,   r12
    //|.define    tmp3b,  r12b
    //|.define    tmp3w,  r12w
    //|.define    rArg1,  rdi
    //|.define    rArg2,  rsi
    //|.define    rArg3,  rdx
    //|.define    rRet,   rax

    //|.type state, gb_state, aState
#define Dt1(_V) (int)(ptrdiff_t)&(((gb_state *)0)_V)
#line 43 "src/emit.dasc"

    //|.include dasm_macros.h
#line 1 "src/dasm_macros.h"
//|.macro write_byte, addr, value
    //| pushfq
	//| push r0
	//| push r1
	//| push r2
	//| push r6
	//| push r7
	//| push r8
	//| push r9
	//| push r10
	//| push r11
	//| mov rArg1, state->mem
    //| mov rArg2, addr
    //| mov rArg3, value
	//| mov rax, &gb_memory_write
	//| call rax
	//| .nop 1
	//| pop r11
	//| pop r10
	//| pop r9
	//| pop r8
	//| pop r7
	//| pop r6
	//| pop r2
	//| pop r1
	//| pop r0
	//| popfq
//|.endmacro

//|.macro print, text
    //| pushfq
	//| push r0
	//| push r1
	//| push r2
	//| push r6
	//| push r7
	//| push r8
	//| push r9
	//| push r10
	//| push r11
	//| mov rArg1, text
	//| mov rax, &puts
	//| call rax
	//| .nop 1
	//| pop r11
	//| pop r10
	//| pop r9
	//| pop r8
	//| pop r7
	//| pop r6
	//| pop r2
	//| pop r1
	//| pop r0
	//| popfq
//|.endmacro

void printnum(uint64_t addr) {
    printf("Debug: %#lx\n", addr);
}

//|.macro debug_a64, addr
    //| pushfq
	//| push r0
	//| push r1
	//| push r2
	//| push r6
	//| push r7
	//| push r8
	//| push r9
	//| push r10
	//| push r11
	//| mov rArg1, addr
	//| mov rax, &printnum
	//| call rax
	//| .nop 1
	//| pop r11
	//| pop r10
	//| pop r9
	//| pop r8
	//| pop r7
	//| pop r6
	//| pop r2
	//| pop r1
	//| pop r0
	//| popfq
//|.endmacro

//|.macro prologue
    //| push rbx
    //| push rsp
    //| push rbp
    //| push r12
    //| push r13
    //| push r14
    //| push r15
    //| mov aState, rArg1
    //| mov xA, 0
	//| mov A, state->a
    //| mov xB, 0
    //| mov B, state->b
    //| mov xC, 0
    //| mov C, state->c
    //| mov xD, 0
    //| mov D, state->d
    //| mov xE, 0
    //| mov E, state->e
    //| mov xH, 0
    //| mov H, state->h
    //| mov xL, 0
    //| mov L, state->l
    //| mov xSP, 0
    //| mov SP, state->_sp
	//| mov tmp1, state->mem
    //| mov aMem, [tmp1 + offsetof(gb_memory, mem)]
//|.endmacro
	
//|.macro return, addr
    //| mov state->a, A
    //| mov state->b, B
    //| mov state->c, C
    //| mov state->d, D
    //| mov state->e, E
    //| mov state->h, H
    //| mov state->l, L
    //| mov state->_sp, SP
	//| pop r15
    //| pop r14
    //| pop r13
    //| pop r12
    //| pop rbp
    //| pop rsp
    //| pop rbx
    //| mov rRet, addr
	//| ret
//|.endmacro

//|.macro inst1, opcode, op1
    //|| switch(op1) {
    //|| case REG_A:
    //|      opcode A
    //||     break;
    //|| case REG_B:
    //|      opcode B
    //||     break;
    //|| case REG_C:
    //|      opcode C
    //||     break;
    //|| case REG_D:
    //|      opcode D
    //||     break;
    //|| case REG_E:
    //|      opcode E
    //||     break;
    //|| case REG_H:
    //|      opcode H
    //||     break;
    //|| case REG_L:
    //|      opcode L
    //||     break;
    //|| default:
    //||     printf("Invalid operand to opcode\n");
    //||     return false;
    //|| }
//|.endmacro

//|.macro inst, opcode, op1, op2
    //|| switch(op1) {
    //|| case REG_A:
    //||     switch(op2) {
    //||     case REG_A:
    //|          opcode A, A
    //||         break;
    //||     case REG_B:
    //|          opcode A, B
    //||         break;
    //||     case REG_C:
    //|          opcode A, C
    //||         break;
    //||     case REG_D:
    //|          opcode A, D
    //||         break;
    //||     case REG_E:
    //|          opcode A, E
    //||         break;
    //||     case REG_H:
    //|          opcode A, H
    //||         break;
    //||     case REG_L:
    //|          opcode A, L
    //||         break;
    //||     case IMM8:
    //|          opcode A, inst->args[1]
    //||         break;
    //||     case MEM_8:
    //|          opcode A, [aMem + (0xff00+inst->args[1])]
    //||         break;
    //||     case MEM_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode A, [aMem + tmp1]
    //||         break;
    //||     case MEM_DEC_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode A, [aMem+tmp1]
    //|          dec tmp1
    //|          mov L, tmp1b
    //|          shr tmp1, 8
    //|          mov H, tmp1b
    //||         break;
    //||     case MEM_INC_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode A, [aMem+tmp1]
    //|          inc tmp1
    //|          mov L, tmp1b
    //|          shr tmp1, 8
    //|          mov H, tmp1b
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case REG_B:
    //||     switch(op2) {
    //||     case REG_A:
    //|          opcode B, A
    //||         break;
    //||     case REG_B:
    //|          opcode B, B
    //||         break;
    //||     case REG_C:
    //|          opcode B, C
    //||         break;
    //||     case REG_D:
    //|          opcode B, D
    //||         break;
    //||     case REG_E:
    //|          opcode B, E
    //||         break;
    //||     case REG_H:
    //|          opcode B, H
    //||         break;
    //||     case REG_L:
    //|          opcode B, L
    //||         break;
    //||     case MEM_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode B, [aMem + tmp1]
    //||         break;
    //||     case IMM8:
    //|          opcode B, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case REG_C:
    //||     switch(op2) {
    //||     case REG_A:
    //|          opcode C, A
    //||         break;
    //||     case REG_B:
    //|          opcode C, B
    //||         break;
    //||     case REG_C:
    //|          opcode C, C
    //||         break;
    //||     case REG_D:
    //|          opcode C, D
    //||         break;
    //||     case REG_E:
    //|          opcode C, E
    //||         break;
    //||     case REG_H:
    //|          opcode C, H
    //||         break;
    //||     case REG_L:
    //|          opcode C, L
    //||         break;
    //||     case MEM_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode C, [aMem + tmp1]
    //||         break;
    //||     case IMM8:
    //|          opcode C, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case REG_D:
    //||     switch(op2) {
    //||     case REG_A:
    //|          opcode D, A
    //||         break;
    //||     case REG_B:
    //|          opcode D, B
    //||         break;
    //||     case REG_C:
    //|          opcode D, C
    //||         break;
    //||     case REG_D:
    //|          opcode D, D
    //||         break;
    //||     case REG_E:
    //|          opcode D, E
    //||         break;
    //||     case REG_H:
    //|          opcode D, H
    //||         break;
    //||     case REG_L:
    //|          opcode D, L
    //||         break;
    //||     case MEM_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode D, [aMem + tmp1]
    //||         break;
    //||     case IMM8:
    //|          opcode D, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case REG_E:
    //||     switch(op2) {
    //||     case REG_A:
    //|          opcode E, A
    //||         break;
    //||     case REG_B:
    //|          opcode E, B
    //||         break;
    //||     case REG_C:
    //|          opcode E, C
    //||         break;
    //||     case REG_D:
    //|          opcode E, D
    //||         break;
    //||     case REG_E:
    //|          opcode E, E
    //||         break;
    //||     case REG_H:
    //|          opcode E, H
    //||         break;
    //||     case REG_L:
    //|          opcode E, L
    //||         break;
    //||     case MEM_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode E, [aMem + tmp1]
    //||         break;
    //||     case IMM8:
    //|          opcode E, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case REG_H:
    //||     switch(op2) {
    //||     case REG_A:
    //|          opcode H, A
    //||         break;
    //||     case REG_B:
    //|          opcode H, B
    //||         break;
    //||     case REG_C:
    //|          opcode H, C
    //||         break;
    //||     case REG_D:
    //|          opcode H, D
    //||         break;
    //||     case REG_E:
    //|          opcode H, E
    //||         break;
    //||     case REG_H:
    //|          opcode H, H
    //||         break;
    //||     case REG_L:
    //|          opcode H, L
    //||         break;
    //||     case MEM_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode H, [aMem + tmp1]
    //||         break;
    //||     case IMM8:
    //|          opcode H, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case REG_L:
    //||     switch(op2) {
    //||     case REG_A:
    //|          opcode L, A
    //||         break;
    //||     case REG_B:
    //|          opcode L, B
    //||         break;
    //||     case REG_C:
    //|          opcode L, C
    //||         break;
    //||     case REG_D:
    //|          opcode L, D
    //||         break;
    //||     case REG_E:
    //|          opcode L, E
    //||         break;
    //||     case REG_H:
    //|          opcode L, H
    //||         break;
    //||     case REG_L:
    //|          opcode L, L
    //||         break;
    //||     case MEM_HL:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          opcode L, [aMem + tmp1]
    //||         break;
    //||     case IMM8:
    //|          opcode L, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case MEM_HL:
    //||     switch(op2) {
    //||     case REG_A:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|.if 'opcode' == 'mov'
    //|          write_byte tmp1, xA
    //|.else
    //|          opcode [aMem + tmp1], A
    //|.endif    
    //||         break;
    //||     case REG_B:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|.if 'opcode' == 'mov'
    //|          write_byte tmp1, xB
    //|.else
    //|          opcode [aMem + tmp1], B
    //|.endif    
    //||         break;
    //||     case REG_C:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|.if 'opcode' == 'mov'
    //|          write_byte tmp1, xC
    //|.else
    //|          opcode [aMem + tmp1], C
    //|.endif    
    //||         break;
    //||     case REG_D:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|.if 'opcode' == 'mov'
    //|          write_byte tmp1, xD
    //|.else
    //|          opcode [aMem + tmp1], D
    //|.endif    
    //||         break;
    //||     case REG_E:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|.if 'opcode' == 'mov'
    //|          write_byte tmp1, xE
    //|.else
    //|          opcode [aMem + tmp1], E
    //|.endif    
    //||         break;
    //||     case REG_H:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|.if 'opcode' == 'mov'
    //|          write_byte tmp1, xH
    //|.else
    //|          opcode [aMem + tmp1], H
    //|.endif    
    //||         break;
    //||     case REG_L:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|.if 'opcode' == 'mov'
    //|          write_byte tmp1, xL
    //|.else
    //|          opcode [aMem + tmp1], L
    //|.endif    
    //||         break;
    //||     case IMM8:
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|.if 'opcode' == 'mov'
    //|          write_byte tmp1, inst->args[1]
    //|.else
    //|          opcode byte [aMem + tmp1], inst->args[1]
    //|.endif    
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case MEM_DEC_HL:
// TODO: load through mbc
    //||     if(op2 == REG_A) {
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          write_byte tmp1, inst->args[1]
    //|          dec tmp1
    //|          mov L, tmp1b
    //|          shr tmp1, 8
    //|          mov H, tmp1b
    //||     } else {
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case MEM_INC_HL:
// TODO: load through mbc
    //||     if(op2 == REG_A) {
    //|          and xL, 0xff
    //|          and xH, 0xff
    //|          mov tmp2, xH
    //|          shl tmp2, 8
    //|          mov tmp1, xL
    //|          add tmp1, tmp2
    //|          write_byte tmp1, inst->args[1]
    //|          inc tmp1
    //|          mov L, tmp1b
    //|          shr tmp1, 8
    //|          mov H, tmp1b
    //||     } else {
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case MEM_8:
    //||     if(op2 == REG_A) {
    //|          opcode [aMem + (0xff00+inst->args[1])], A
    //||     } else {
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case MEM_C:
    //||     if(op2 == REG_A) {
    //|          and xC, 0xff;
    //|          opcode byte [aMem + xC + 0xff00], A
    //||     } else {
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| case MEM_16:
    //||     if(op2 == REG_A) {
// TODO: implement static memory check at compile time
    //||         uint16_t addr = inst->args[2]*256+inst->args[1];
    //|          debug_a64 addr
    //|          write_byte addr, xA
    //||     } else {
    //||         printf("Unsupported operand op2=%i to opcode\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| default:
    //||     printf("Unsupported operand op1=%i to opcode\n", op1);
    //||     return false;
    //|| }
//|.endmacro
#line 45 "src/emit.dasc"

bool inst_nop(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "NOP"
    dasm_put(Dst, 0, "NOP", (ptrdiff_t)(puts));
#line 48 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool inst_jp(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "JP/CALL"
    dasm_put(Dst, 0, "JP/CALL", (ptrdiff_t)(puts));
#line 54 "src/emit.dasc"
    
    switch(inst->op1) {
    case NONE:
        break;
    case CC_NZ:
        //| jz >1
        dasm_put(Dst, 40);
#line 60 "src/emit.dasc"
        break;
    case CC_Z:
        //| jnz >1
        dasm_put(Dst, 45);
#line 63 "src/emit.dasc"
        break;
    case CC_NC:
        //| jc >1
        dasm_put(Dst, 50);
#line 66 "src/emit.dasc"
        break;
    case CC_C:
        //| jnc >1
        dasm_put(Dst, 55);
#line 69 "src/emit.dasc"
        break;
    default:
        printf("Invalid 1st operand to JP/CALL\n");
        return false;
    }

    if(inst->opcode == CALL) {
        //| and xSP, 0xffff
	    //| mov word [aMem+xSP], (uint16_t)(inst->address + inst->bytes)
	    //| dec SP
	    //| dec SP
	    dasm_put(Dst, 60, (uint16_t)(inst->address + inst->bytes));
#line 80 "src/emit.dasc"
    }

    //| add qword state->inst_count, *cycles + inst->cycles;
    dasm_put(Dst, 84, Dt1(->inst_count), *cycles + inst->cycles);
#line 83 "src/emit.dasc"

    switch(inst->op2) {
	case IMM8:
		//| return inst->address + (int8_t)inst->args[1] + 2
		dasm_put(Dst, 90, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp), inst->address + (int8_t)inst->args[1] + 2);
#line 87 "src/emit.dasc"
		break;
    case IMM16:    
        //| return inst->args[2]*256 + inst->args[1]
        dasm_put(Dst, 90, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp), inst->args[2]*256 + inst->args[1]);
#line 90 "src/emit.dasc"
        break;
    case MEM_HL:
        //| mov tmp1, xH
        //| shl tmp1, 8
        //| add tmp1, xL
        //| return tmp1
        dasm_put(Dst, 140, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp));
#line 96 "src/emit.dasc"
        break;
    default:
        printf("Invalid 2nd operand to JP/CALL\n");
        return false;
    }

    if(inst->op1 != NONE) {
        //| 1:
        dasm_put(Dst, 201);
#line 104 "src/emit.dasc"
    }
    
    *cycles += inst->alt_cycles;
    
    return true;
}

bool inst_ret(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "RET"
    dasm_put(Dst, 0, "RET", (ptrdiff_t)(puts));
#line 113 "src/emit.dasc"
    
    switch(inst->op1) {
    case NONE:
        break;
    case CC_NZ:
        //| jz >1
        dasm_put(Dst, 40);
#line 119 "src/emit.dasc"
        break;
    case CC_Z:
        //| jnz >1
        dasm_put(Dst, 45);
#line 122 "src/emit.dasc"
        break;
    case CC_NC:
        //| jc >1
        dasm_put(Dst, 50);
#line 125 "src/emit.dasc"
        break;
    case CC_C:
        //| jnc >1
        dasm_put(Dst, 55);
#line 128 "src/emit.dasc"
        break;
    default:
        printf("Invalid 1st operand to RET\n");
        return false;
    }

    //| add qword state->inst_count, *cycles + inst->cycles;
    //| inc SP
    //| inc SP
    //| and xSP, 0xffff
    //| mov tmp1, [aMem+xSP]
    //| and tmp1, 0xffff
    //| return tmp1
    dasm_put(Dst, 204, Dt1(->inst_count), *cycles + inst->cycles, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp));
#line 141 "src/emit.dasc"

    if(inst->op1 != NONE) {
        //| 1:
        dasm_put(Dst, 201);
#line 144 "src/emit.dasc"
    }
    
    *cycles += inst->alt_cycles;
    
    return true;
}

bool inst_ld16(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "LD16"
    dasm_put(Dst, 0, "LD16", (ptrdiff_t)(puts));
#line 153 "src/emit.dasc"
    switch(inst->op1) {
    case REG_BC:
        if(inst->op2 == IMM16) {
            //| mov B, inst->args[2];
            //| mov C, inst->args[1];
            dasm_put(Dst, 288, inst->args[2], inst->args[1]);
#line 158 "src/emit.dasc"
        } else {
            printf("Invalid 2nd operand to LD16\n");
            return false;
        }
        break;
    case REG_DE:
        if(inst->op2 == IMM16) {
            //| mov D, inst->args[2];
            //| mov E, inst->args[1];
            dasm_put(Dst, 293, inst->args[2], inst->args[1]);
#line 167 "src/emit.dasc"
        } else {
            printf("Invalid 2nd operand to LD16\n");
            return false;
        }
        break;
    case REG_HL:
        if(inst->op2 == IMM16) {
            //| mov H, inst->args[2];
            //| mov L, inst->args[1];
            dasm_put(Dst, 299, inst->args[2], inst->args[1]);
#line 176 "src/emit.dasc"
        } else {
            printf("Invalid 2nd operand to LD16\n");
            return false;
        }
        break;
    case REG_SP:
        if(inst->op2 == IMM16) {
            //| mov SP, (uint16_t)(inst->args[1] + 256*inst->args[2]);
            dasm_put(Dst, 306, (uint16_t)(inst->args[1] + 256*inst->args[2]));
#line 184 "src/emit.dasc"
        } else {
            printf("Invalid 2nd operand to LD16\n");
            return false;
        }
        break;
    default:
        printf("Invalid 1st operand to LD16\n");
        return false;
    }
    *cycles += inst->alt_cycles;
    return true;
}

bool inst_ld(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "LD"
	//| inst mov, inst->op1, inst->op2
	dasm_put(Dst, 0, "LD", (ptrdiff_t)(puts));
	 switch(inst->op1) {
	 case REG_A:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 310);
	         break;
	     case REG_B:
	dasm_put(Dst, 313);
	         break;
	     case REG_C:
	dasm_put(Dst, 316);
	         break;
	     case REG_D:
	dasm_put(Dst, 319);
	         break;
	     case REG_E:
	dasm_put(Dst, 322);
	         break;
	     case REG_H:
	dasm_put(Dst, 326);
	         break;
	     case REG_L:
	dasm_put(Dst, 330);
	         break;
	     case IMM8:
	dasm_put(Dst, 335, inst->args[1]);
	         break;
	     case MEM_8:
	dasm_put(Dst, 338, (0xff00+inst->args[1]));
	         break;
	     case MEM_HL:
	dasm_put(Dst, 343);
	         break;
	     case MEM_DEC_HL:
	dasm_put(Dst, 379);
	         break;
	     case MEM_INC_HL:
	dasm_put(Dst, 430);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_B:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 481);
	         break;
	     case REG_B:
	dasm_put(Dst, 484);
	         break;
	     case REG_C:
	dasm_put(Dst, 487);
	         break;
	     case REG_D:
	dasm_put(Dst, 490);
	         break;
	     case REG_E:
	dasm_put(Dst, 493);
	         break;
	     case REG_H:
	dasm_put(Dst, 498);
	         break;
	     case REG_L:
	dasm_put(Dst, 503);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 508);
	         break;
	     case IMM8:
	dasm_put(Dst, 544, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_C:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 547);
	         break;
	     case REG_B:
	dasm_put(Dst, 550);
	         break;
	     case REG_C:
	dasm_put(Dst, 553);
	         break;
	     case REG_D:
	dasm_put(Dst, 556);
	         break;
	     case REG_E:
	dasm_put(Dst, 559);
	         break;
	     case REG_H:
	dasm_put(Dst, 564);
	         break;
	     case REG_L:
	dasm_put(Dst, 569);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 574);
	         break;
	     case IMM8:
	dasm_put(Dst, 290, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_D:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 610);
	         break;
	     case REG_B:
	dasm_put(Dst, 613);
	         break;
	     case REG_C:
	dasm_put(Dst, 616);
	         break;
	     case REG_D:
	dasm_put(Dst, 619);
	         break;
	     case REG_E:
	dasm_put(Dst, 622);
	         break;
	     case REG_H:
	dasm_put(Dst, 627);
	         break;
	     case REG_L:
	dasm_put(Dst, 632);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 637);
	         break;
	     case IMM8:
	dasm_put(Dst, 673, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_E:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 676);
	         break;
	     case REG_B:
	dasm_put(Dst, 680);
	         break;
	     case REG_C:
	dasm_put(Dst, 684);
	         break;
	     case REG_D:
	dasm_put(Dst, 688);
	         break;
	     case REG_E:
	dasm_put(Dst, 692);
	         break;
	     case REG_H:
	dasm_put(Dst, 697);
	         break;
	     case REG_L:
	dasm_put(Dst, 702);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 707);
	         break;
	     case IMM8:
	dasm_put(Dst, 295, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_H:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 743);
	         break;
	     case REG_B:
	dasm_put(Dst, 747);
	         break;
	     case REG_C:
	dasm_put(Dst, 751);
	         break;
	     case REG_D:
	dasm_put(Dst, 755);
	         break;
	     case REG_E:
	dasm_put(Dst, 759);
	         break;
	     case REG_H:
	dasm_put(Dst, 764);
	         break;
	     case REG_L:
	dasm_put(Dst, 769);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 774);
	         break;
	     case IMM8:
	dasm_put(Dst, 810, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_L:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 814);
	         break;
	     case REG_B:
	dasm_put(Dst, 818);
	         break;
	     case REG_C:
	dasm_put(Dst, 822);
	         break;
	     case REG_D:
	dasm_put(Dst, 826);
	         break;
	     case REG_E:
	dasm_put(Dst, 830);
	         break;
	     case REG_H:
	dasm_put(Dst, 835);
	         break;
	     case REG_L:
	dasm_put(Dst, 840);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 845);
	         break;
	     case IMM8:
	dasm_put(Dst, 302, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_HL:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 881, Dt1(->mem), (ptrdiff_t)(gb_memory_write));
	         break;
	     case REG_B:
	dasm_put(Dst, 958, Dt1(->mem), (ptrdiff_t)(gb_memory_write));
	         break;
	     case REG_C:
	dasm_put(Dst, 1035, Dt1(->mem), (ptrdiff_t)(gb_memory_write));
	         break;
	     case REG_D:
	dasm_put(Dst, 1112, Dt1(->mem), (ptrdiff_t)(gb_memory_write));
	         break;
	     case REG_E:
	dasm_put(Dst, 1189, Dt1(->mem), (ptrdiff_t)(gb_memory_write));
	         break;
	     case REG_H:
	dasm_put(Dst, 1267, Dt1(->mem), (ptrdiff_t)(gb_memory_write));
	         break;
	     case REG_L:
	dasm_put(Dst, 1345, Dt1(->mem), (ptrdiff_t)(gb_memory_write));
	         break;
	     case IMM8:
	dasm_put(Dst, 1423, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_DEC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1501, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_INC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1594, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_8:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1687, (0xff00+inst->args[1]));
	     } else {
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_C:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1692, 0xff00);
	     } else {
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_16:
	     if(inst->op2 == REG_A) {
	         uint16_t addr = inst->args[2]*256+inst->args[1];
	dasm_put(Dst, 1707, addr, (ptrdiff_t)(printnum), Dt1(->mem), addr, (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to mov\n", inst->op2);
	         return false;
	     }
	     break;
	 default:
	     printf("Unsupported operand inst->op1=%i to mov\n", inst->op1);
	     return false;
	 }
#line 200 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool inst_inc(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "INC"
    //| inst1 inc, inst->op1
    dasm_put(Dst, 0, "INC", (ptrdiff_t)(puts));
     switch(inst->op1) {
     case REG_A:
    dasm_put(Dst, 1793);
         break;
     case REG_B:
    dasm_put(Dst, 1797);
         break;
     case REG_C:
    dasm_put(Dst, 1801);
         break;
     case REG_D:
    dasm_put(Dst, 1805);
         break;
     case REG_E:
    dasm_put(Dst, 1809);
         break;
     case REG_H:
    dasm_put(Dst, 1814);
         break;
     case REG_L:
    dasm_put(Dst, 1819);
         break;
     default:
         printf("Invalid operand to inc\n");
         return false;
     }
#line 207 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool inst_dec(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "DEC"
    //| inst1 dec, inst->op1
    dasm_put(Dst, 0, "DEC", (ptrdiff_t)(puts));
     switch(inst->op1) {
     case REG_A:
    dasm_put(Dst, 1824);
         break;
     case REG_B:
    dasm_put(Dst, 1828);
         break;
     case REG_C:
    dasm_put(Dst, 1832);
         break;
     case REG_D:
    dasm_put(Dst, 1836);
         break;
     case REG_E:
    dasm_put(Dst, 1840);
         break;
     case REG_H:
    dasm_put(Dst, 1845);
         break;
     case REG_L:
    dasm_put(Dst, 1850);
         break;
     default:
         printf("Invalid operand to dec\n");
         return false;
     }
#line 214 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool inst_dec16(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "DEC16"
    dasm_put(Dst, 0, "DEC16", (ptrdiff_t)(puts));
#line 220 "src/emit.dasc"
    switch(inst->op1) {
    case REG_BC:
        //| mov tmp2, xB
        //| shl tmp2, 8
        //| mov tmp1, xC
        //| add tmp1, tmp2
        //| dec tmp1
        //| mov C, tmp1b
        //| shr tmp1, 8
        //| mov B, tmp1b
        dasm_put(Dst, 1855);
#line 230 "src/emit.dasc"
        break;
    case REG_DE:
        //| mov tmp2, xD
        //| shl tmp2, 8
        //| mov tmp1, xE
        //| add tmp1, tmp2
        //| dec tmp1
        //| mov E, tmp1b
        //| shr tmp1, 8
        //| mov D, tmp1b
        dasm_put(Dst, 1884);
#line 240 "src/emit.dasc"
        break;
    case REG_HL:
        //| mov tmp2, xH
        //| shl tmp2, 8
        //| mov tmp1, xL
        //| add tmp1, tmp2
        //| dec tmp1
        //| mov L, tmp1b
        //| shr tmp1, 8
        //| mov H, tmp1b
        dasm_put(Dst, 1914);
#line 250 "src/emit.dasc"
        break;
    case REG_SP:
        //| dec SP
        dasm_put(Dst, 79);
#line 253 "src/emit.dasc"
        break;
    default:
        printf("Invalid operand to DEC16\n");
        return false;
    }
    *cycles += inst->cycles;
    return true;
}

bool inst_di(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
	//| print "DI"
	//| mov byte state->ime, 0
	dasm_put(Dst, 1945, "DI", (ptrdiff_t)(puts), Dt1(->ime));
#line 265 "src/emit.dasc"
    *cycles += inst->cycles;
	return true;
}

bool inst_ei(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
	//| print "EI"
	//| mov byte state->ime, 1
	dasm_put(Dst, 1990, "EI", (ptrdiff_t)(puts), Dt1(->ime));
#line 272 "src/emit.dasc"
    *cycles += inst->cycles;
	return true;
}

bool inst_cp(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "CP"
	//| inst cmp, inst->op1, inst->op2
	dasm_put(Dst, 0, "CP", (ptrdiff_t)(puts));
	 switch(inst->op1) {
	 case REG_A:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2035);
	         break;
	     case REG_B:
	dasm_put(Dst, 2038);
	         break;
	     case REG_C:
	dasm_put(Dst, 2041);
	         break;
	     case REG_D:
	dasm_put(Dst, 2044);
	         break;
	     case REG_E:
	dasm_put(Dst, 2047);
	         break;
	     case REG_H:
	dasm_put(Dst, 2051);
	         break;
	     case REG_L:
	dasm_put(Dst, 2055);
	         break;
	     case IMM8:
	dasm_put(Dst, 2060, inst->args[1]);
	         break;
	     case MEM_8:
	dasm_put(Dst, 2063, (0xff00+inst->args[1]));
	         break;
	     case MEM_HL:
	dasm_put(Dst, 2068);
	         break;
	     case MEM_DEC_HL:
	dasm_put(Dst, 2104);
	         break;
	     case MEM_INC_HL:
	dasm_put(Dst, 2155);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_B:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2206);
	         break;
	     case REG_B:
	dasm_put(Dst, 2209);
	         break;
	     case REG_C:
	dasm_put(Dst, 2212);
	         break;
	     case REG_D:
	dasm_put(Dst, 2215);
	         break;
	     case REG_E:
	dasm_put(Dst, 2218);
	         break;
	     case REG_H:
	dasm_put(Dst, 2223);
	         break;
	     case REG_L:
	dasm_put(Dst, 2228);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 2233);
	         break;
	     case IMM8:
	dasm_put(Dst, 2269, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_C:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2274);
	         break;
	     case REG_B:
	dasm_put(Dst, 2277);
	         break;
	     case REG_C:
	dasm_put(Dst, 2280);
	         break;
	     case REG_D:
	dasm_put(Dst, 2283);
	         break;
	     case REG_E:
	dasm_put(Dst, 2286);
	         break;
	     case REG_H:
	dasm_put(Dst, 2291);
	         break;
	     case REG_L:
	dasm_put(Dst, 2296);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 2301);
	         break;
	     case IMM8:
	dasm_put(Dst, 2337, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_D:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2342);
	         break;
	     case REG_B:
	dasm_put(Dst, 2345);
	         break;
	     case REG_C:
	dasm_put(Dst, 2348);
	         break;
	     case REG_D:
	dasm_put(Dst, 2351);
	         break;
	     case REG_E:
	dasm_put(Dst, 2354);
	         break;
	     case REG_H:
	dasm_put(Dst, 2359);
	         break;
	     case REG_L:
	dasm_put(Dst, 2364);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 2369);
	         break;
	     case IMM8:
	dasm_put(Dst, 2405, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_E:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2410);
	         break;
	     case REG_B:
	dasm_put(Dst, 2414);
	         break;
	     case REG_C:
	dasm_put(Dst, 2418);
	         break;
	     case REG_D:
	dasm_put(Dst, 2422);
	         break;
	     case REG_E:
	dasm_put(Dst, 2426);
	         break;
	     case REG_H:
	dasm_put(Dst, 2431);
	         break;
	     case REG_L:
	dasm_put(Dst, 2436);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 2441);
	         break;
	     case IMM8:
	dasm_put(Dst, 2477, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_H:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2483);
	         break;
	     case REG_B:
	dasm_put(Dst, 2487);
	         break;
	     case REG_C:
	dasm_put(Dst, 2491);
	         break;
	     case REG_D:
	dasm_put(Dst, 2495);
	         break;
	     case REG_E:
	dasm_put(Dst, 2499);
	         break;
	     case REG_H:
	dasm_put(Dst, 2504);
	         break;
	     case REG_L:
	dasm_put(Dst, 2509);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 2514);
	         break;
	     case IMM8:
	dasm_put(Dst, 2550, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_L:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2556);
	         break;
	     case REG_B:
	dasm_put(Dst, 2560);
	         break;
	     case REG_C:
	dasm_put(Dst, 2564);
	         break;
	     case REG_D:
	dasm_put(Dst, 2568);
	         break;
	     case REG_E:
	dasm_put(Dst, 2572);
	         break;
	     case REG_H:
	dasm_put(Dst, 2577);
	         break;
	     case REG_L:
	dasm_put(Dst, 2582);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 2587);
	         break;
	     case IMM8:
	dasm_put(Dst, 2623, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_HL:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2629);
	         break;
	     case REG_B:
	dasm_put(Dst, 2665);
	         break;
	     case REG_C:
	dasm_put(Dst, 2701);
	         break;
	     case REG_D:
	dasm_put(Dst, 2737);
	         break;
	     case REG_E:
	dasm_put(Dst, 2773);
	         break;
	     case REG_H:
	dasm_put(Dst, 2809);
	         break;
	     case REG_L:
	dasm_put(Dst, 2845);
	         break;
	     case IMM8:
	dasm_put(Dst, 2881, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_DEC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1501, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_INC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1594, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_8:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 2918, (0xff00+inst->args[1]));
	     } else {
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_C:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 2923, 0xff00);
	     } else {
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_16:
	     if(inst->op2 == REG_A) {
	         uint16_t addr = inst->args[2]*256+inst->args[1];
	dasm_put(Dst, 1707, addr, (ptrdiff_t)(printnum), Dt1(->mem), addr, (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to cmp\n", inst->op2);
	         return false;
	     }
	     break;
	 default:
	     printf("Unsupported operand inst->op1=%i to cmp\n", inst->op1);
	     return false;
	 }
#line 279 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool inst_or(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "OR"
	//| inst or, inst->op1, inst->op2
	dasm_put(Dst, 0, "OR", (ptrdiff_t)(puts));
	 switch(inst->op1) {
	 case REG_A:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 2938);
	         break;
	     case REG_B:
	dasm_put(Dst, 2941);
	         break;
	     case REG_C:
	dasm_put(Dst, 2944);
	         break;
	     case REG_D:
	dasm_put(Dst, 2947);
	         break;
	     case REG_E:
	dasm_put(Dst, 2950);
	         break;
	     case REG_H:
	dasm_put(Dst, 2954);
	         break;
	     case REG_L:
	dasm_put(Dst, 2958);
	         break;
	     case IMM8:
	dasm_put(Dst, 2963, inst->args[1]);
	         break;
	     case MEM_8:
	dasm_put(Dst, 2966, (0xff00+inst->args[1]));
	         break;
	     case MEM_HL:
	dasm_put(Dst, 2971);
	         break;
	     case MEM_DEC_HL:
	dasm_put(Dst, 3007);
	         break;
	     case MEM_INC_HL:
	dasm_put(Dst, 3058);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_B:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 3109);
	         break;
	     case REG_B:
	dasm_put(Dst, 3112);
	         break;
	     case REG_C:
	dasm_put(Dst, 3115);
	         break;
	     case REG_D:
	dasm_put(Dst, 3118);
	         break;
	     case REG_E:
	dasm_put(Dst, 3121);
	         break;
	     case REG_H:
	dasm_put(Dst, 3126);
	         break;
	     case REG_L:
	dasm_put(Dst, 3131);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 3136);
	         break;
	     case IMM8:
	dasm_put(Dst, 3172, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_C:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 3176);
	         break;
	     case REG_B:
	dasm_put(Dst, 3179);
	         break;
	     case REG_C:
	dasm_put(Dst, 3182);
	         break;
	     case REG_D:
	dasm_put(Dst, 3185);
	         break;
	     case REG_E:
	dasm_put(Dst, 3188);
	         break;
	     case REG_H:
	dasm_put(Dst, 3193);
	         break;
	     case REG_L:
	dasm_put(Dst, 3198);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 3203);
	         break;
	     case IMM8:
	dasm_put(Dst, 3239, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_D:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 3243);
	         break;
	     case REG_B:
	dasm_put(Dst, 3246);
	         break;
	     case REG_C:
	dasm_put(Dst, 3249);
	         break;
	     case REG_D:
	dasm_put(Dst, 3252);
	         break;
	     case REG_E:
	dasm_put(Dst, 3255);
	         break;
	     case REG_H:
	dasm_put(Dst, 3260);
	         break;
	     case REG_L:
	dasm_put(Dst, 3265);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 3270);
	         break;
	     case IMM8:
	dasm_put(Dst, 3306, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_E:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 3310);
	         break;
	     case REG_B:
	dasm_put(Dst, 3314);
	         break;
	     case REG_C:
	dasm_put(Dst, 3318);
	         break;
	     case REG_D:
	dasm_put(Dst, 3322);
	         break;
	     case REG_E:
	dasm_put(Dst, 3326);
	         break;
	     case REG_H:
	dasm_put(Dst, 3331);
	         break;
	     case REG_L:
	dasm_put(Dst, 3336);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 3341);
	         break;
	     case IMM8:
	dasm_put(Dst, 3377, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_H:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 3382);
	         break;
	     case REG_B:
	dasm_put(Dst, 3386);
	         break;
	     case REG_C:
	dasm_put(Dst, 3390);
	         break;
	     case REG_D:
	dasm_put(Dst, 3394);
	         break;
	     case REG_E:
	dasm_put(Dst, 3398);
	         break;
	     case REG_H:
	dasm_put(Dst, 3403);
	         break;
	     case REG_L:
	dasm_put(Dst, 3408);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 3413);
	         break;
	     case IMM8:
	dasm_put(Dst, 3449, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_L:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 3454);
	         break;
	     case REG_B:
	dasm_put(Dst, 3458);
	         break;
	     case REG_C:
	dasm_put(Dst, 3462);
	         break;
	     case REG_D:
	dasm_put(Dst, 3466);
	         break;
	     case REG_E:
	dasm_put(Dst, 3470);
	         break;
	     case REG_H:
	dasm_put(Dst, 3475);
	         break;
	     case REG_L:
	dasm_put(Dst, 3480);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 3485);
	         break;
	     case IMM8:
	dasm_put(Dst, 3521, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_HL:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 3526);
	         break;
	     case REG_B:
	dasm_put(Dst, 3562);
	         break;
	     case REG_C:
	dasm_put(Dst, 3598);
	         break;
	     case REG_D:
	dasm_put(Dst, 3634);
	         break;
	     case REG_E:
	dasm_put(Dst, 3670);
	         break;
	     case REG_H:
	dasm_put(Dst, 3706);
	         break;
	     case REG_L:
	dasm_put(Dst, 3742);
	         break;
	     case IMM8:
	dasm_put(Dst, 3778, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_DEC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1501, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_INC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1594, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_8:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 3815, (0xff00+inst->args[1]));
	     } else {
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_C:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 3820, 0xff00);
	     } else {
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_16:
	     if(inst->op2 == REG_A) {
	         uint16_t addr = inst->args[2]*256+inst->args[1];
	dasm_put(Dst, 1707, addr, (ptrdiff_t)(printnum), Dt1(->mem), addr, (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to or\n", inst->op2);
	         return false;
	     }
	     break;
	 default:
	     printf("Unsupported operand inst->op1=%i to or\n", inst->op1);
	     return false;
	 }
#line 286 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool inst_xor(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "XOR"
	//| inst xor, inst->op1, inst->op2
	dasm_put(Dst, 0, "XOR", (ptrdiff_t)(puts));
	 switch(inst->op1) {
	 case REG_A:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 3835);
	         break;
	     case REG_B:
	dasm_put(Dst, 3838);
	         break;
	     case REG_C:
	dasm_put(Dst, 3841);
	         break;
	     case REG_D:
	dasm_put(Dst, 3844);
	         break;
	     case REG_E:
	dasm_put(Dst, 3847);
	         break;
	     case REG_H:
	dasm_put(Dst, 3851);
	         break;
	     case REG_L:
	dasm_put(Dst, 3855);
	         break;
	     case IMM8:
	dasm_put(Dst, 3860, inst->args[1]);
	         break;
	     case MEM_8:
	dasm_put(Dst, 3863, (0xff00+inst->args[1]));
	         break;
	     case MEM_HL:
	dasm_put(Dst, 3868);
	         break;
	     case MEM_DEC_HL:
	dasm_put(Dst, 3904);
	         break;
	     case MEM_INC_HL:
	dasm_put(Dst, 3955);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_B:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 4006);
	         break;
	     case REG_B:
	dasm_put(Dst, 4009);
	         break;
	     case REG_C:
	dasm_put(Dst, 4012);
	         break;
	     case REG_D:
	dasm_put(Dst, 4015);
	         break;
	     case REG_E:
	dasm_put(Dst, 4018);
	         break;
	     case REG_H:
	dasm_put(Dst, 4023);
	         break;
	     case REG_L:
	dasm_put(Dst, 4028);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 4033);
	         break;
	     case IMM8:
	dasm_put(Dst, 4069, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_C:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 4074);
	         break;
	     case REG_B:
	dasm_put(Dst, 4077);
	         break;
	     case REG_C:
	dasm_put(Dst, 4080);
	         break;
	     case REG_D:
	dasm_put(Dst, 4083);
	         break;
	     case REG_E:
	dasm_put(Dst, 4086);
	         break;
	     case REG_H:
	dasm_put(Dst, 4091);
	         break;
	     case REG_L:
	dasm_put(Dst, 4096);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 4101);
	         break;
	     case IMM8:
	dasm_put(Dst, 4137, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_D:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 4142);
	         break;
	     case REG_B:
	dasm_put(Dst, 4145);
	         break;
	     case REG_C:
	dasm_put(Dst, 4148);
	         break;
	     case REG_D:
	dasm_put(Dst, 4151);
	         break;
	     case REG_E:
	dasm_put(Dst, 4154);
	         break;
	     case REG_H:
	dasm_put(Dst, 4159);
	         break;
	     case REG_L:
	dasm_put(Dst, 4164);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 4169);
	         break;
	     case IMM8:
	dasm_put(Dst, 4205, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_E:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 4210);
	         break;
	     case REG_B:
	dasm_put(Dst, 4214);
	         break;
	     case REG_C:
	dasm_put(Dst, 4218);
	         break;
	     case REG_D:
	dasm_put(Dst, 4222);
	         break;
	     case REG_E:
	dasm_put(Dst, 4226);
	         break;
	     case REG_H:
	dasm_put(Dst, 4231);
	         break;
	     case REG_L:
	dasm_put(Dst, 4236);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 4241);
	         break;
	     case IMM8:
	dasm_put(Dst, 4277, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_H:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 4283);
	         break;
	     case REG_B:
	dasm_put(Dst, 4287);
	         break;
	     case REG_C:
	dasm_put(Dst, 4291);
	         break;
	     case REG_D:
	dasm_put(Dst, 4295);
	         break;
	     case REG_E:
	dasm_put(Dst, 4299);
	         break;
	     case REG_H:
	dasm_put(Dst, 4304);
	         break;
	     case REG_L:
	dasm_put(Dst, 4309);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 4314);
	         break;
	     case IMM8:
	dasm_put(Dst, 4350, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_L:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 4356);
	         break;
	     case REG_B:
	dasm_put(Dst, 4360);
	         break;
	     case REG_C:
	dasm_put(Dst, 4364);
	         break;
	     case REG_D:
	dasm_put(Dst, 4368);
	         break;
	     case REG_E:
	dasm_put(Dst, 4372);
	         break;
	     case REG_H:
	dasm_put(Dst, 4377);
	         break;
	     case REG_L:
	dasm_put(Dst, 4382);
	         break;
	     case MEM_HL:
	dasm_put(Dst, 4387);
	         break;
	     case IMM8:
	dasm_put(Dst, 4423, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_HL:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 4429);
	         break;
	     case REG_B:
	dasm_put(Dst, 4465);
	         break;
	     case REG_C:
	dasm_put(Dst, 4501);
	         break;
	     case REG_D:
	dasm_put(Dst, 4537);
	         break;
	     case REG_E:
	dasm_put(Dst, 4573);
	         break;
	     case REG_H:
	dasm_put(Dst, 4609);
	         break;
	     case REG_L:
	dasm_put(Dst, 4645);
	         break;
	     case IMM8:
	dasm_put(Dst, 4681, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_DEC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1501, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_INC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 1594, Dt1(->mem), inst->args[1], (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_8:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 4718, (0xff00+inst->args[1]));
	     } else {
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_C:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 4723, 0xff00);
	     } else {
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_16:
	     if(inst->op2 == REG_A) {
	         uint16_t addr = inst->args[2]*256+inst->args[1];
	dasm_put(Dst, 1707, addr, (ptrdiff_t)(printnum), Dt1(->mem), addr, (ptrdiff_t)(gb_memory_write));
	     } else {
	         printf("Unsupported operand inst->op2=%i to xor\n", inst->op2);
	         return false;
	     }
	     break;
	 default:
	     printf("Unsupported operand inst->op1=%i to xor\n", inst->op1);
	     return false;
	 }
#line 293 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool emit(gb_block *block, gb_instruction *inst, int n) {
    dasm_State* d;
    uint32_t npc = 4;
    uint32_t nextpc = 0;
    uint64_t cycles = 0;

    //|.section code
#define DASM_SECTION_CODE	0
#define DASM_MAXSECTION		1
#line 304 "src/emit.dasc"
    dasm_init(&d, DASM_MAXSECTION);

    //|.globals lbl_
enum {
  lbl_f_start,
  lbl__MAX
};
#line 307 "src/emit.dasc"
    void* labels[lbl__MAX];
    dasm_setupglobal(&d, labels, lbl__MAX);

    //|.actionlist gb_actions
static const unsigned char gb_actions[4913] = {
  156,80,81,82,86,87,65,80,65,81,65,82,65,83,72,199,199,237,72,199,192,237,
  252,255,208,65,91,65,90,65,89,65,88,95,94,90,89,88,157,255,15,132,244,247,
  255,15,133,244,247,255,15,130,244,247,255,15,131,244,247,255,72,129,231,252,
  255,252,255,0,0,102,65,199,4,56,236,102,252,255,207,102,252,255,207,255,73,
  129,129,233,239,255,65,136,129,233,65,136,137,233,65,136,145,233,65,136,153,
  233,69,136,169,233,65,136,169,233,65,136,177,233,102,65,137,185,233,65,95,
  65,94,65,93,65,92,93,92,91,72,199,192,237,195,255,73,137,252,234,73,193,226,
  8,73,1,252,242,65,136,129,233,65,136,137,233,65,136,145,233,65,136,153,233,
  69,136,169,233,65,136,169,233,65,136,177,233,102,65,137,185,233,65,95,65,
  94,65,93,65,92,93,92,91,76,137,208,195,255,248,1,255,73,129,129,233,239,102,
  252,255,199,102,252,255,199,72,129,231,252,255,252,255,0,0,77,139,20,56,73,
  129,226,252,255,252,255,0,0,65,136,129,233,65,136,137,233,65,136,145,233,
  65,136,153,233,69,136,169,233,65,136,169,233,65,136,177,233,102,65,137,185,
  233,65,95,65,94,65,93,65,92,93,92,91,76,137,208,195,255,177,235,178,235,255,
  179,235,65,181,235,255,64,181,235,64,182,235,255,102,191,236,255,136,192,
  255,136,200,255,136,208,255,136,216,255,68,136,232,255,64,136,232,255,64,
  136,252,240,255,176,235,255,65,138,128,233,255,72,129,230,252,255,0,0,0,72,
  129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,
  67,138,4,16,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,
  252,235,73,193,227,8,73,137,252,242,77,1,218,67,138,4,16,73,252,255,202,68,
  136,214,73,193,252,234,8,68,136,213,255,72,129,230,252,255,0,0,0,72,129,229,
  252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,138,
  4,16,73,252,255,194,68,136,214,73,193,252,234,8,68,136,213,255,136,193,255,
  136,201,255,136,209,255,136,217,255,68,136,252,233,255,64,136,252,233,255,
  64,136,252,241,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,
  252,235,73,193,227,8,73,137,252,242,77,1,218,67,138,12,16,255,177,235,255,
  136,194,255,136,202,255,136,210,255,136,218,255,68,136,252,234,255,64,136,
  252,234,255,64,136,252,242,255,72,129,230,252,255,0,0,0,72,129,229,252,255,
  0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,138,20,16,255,
  136,195,255,136,203,255,136,211,255,136,219,255,68,136,252,235,255,64,136,
  252,235,255,64,136,252,243,255,72,129,230,252,255,0,0,0,72,129,229,252,255,
  0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,138,28,16,255,
  179,235,255,65,136,197,255,65,136,205,255,65,136,213,255,65,136,221,255,69,
  136,252,237,255,65,136,252,237,255,65,136,252,245,255,72,129,230,252,255,
  0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,
  77,1,218,71,138,44,16,255,64,136,197,255,64,136,205,255,64,136,213,255,64,
  136,221,255,68,136,252,237,255,64,136,252,237,255,64,136,252,245,255,72,129,
  230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,
  137,252,242,77,1,218,67,138,44,16,255,64,181,235,255,64,136,198,255,64,136,
  206,255,64,136,214,255,64,136,222,255,68,136,252,238,255,64,136,252,238,255,
  64,136,252,246,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,
  252,235,73,193,227,8,73,137,252,242,77,1,218,67,138,52,16,255,72,129,230,
  252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,
  252,242,77,1,218,156,80,81,82,86,87,65,80,65,81,65,82,65,83,73,139,185,233,
  76,137,214,72,137,194,72,199,192,237,252,255,208,65,91,65,90,65,89,65,88,
  95,94,90,89,88,157,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,
  73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,156,80,81,82,86,87,65,
  80,65,81,65,82,65,83,73,139,185,233,76,137,214,72,137,202,72,199,192,237,
  252,255,208,65,91,65,90,65,89,65,88,95,94,90,89,88,157,255,72,129,230,252,
  255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,
  242,77,1,218,156,80,81,82,86,87,65,80,65,81,65,82,65,83,73,139,185,233,76,
  137,214,72,137,210,72,199,192,237,252,255,208,65,91,65,90,65,89,65,88,95,
  94,90,89,88,157,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,
  137,252,235,73,193,227,8,73,137,252,242,77,1,218,156,80,81,82,86,87,65,80,
  65,81,65,82,65,83,73,139,185,233,76,137,214,72,137,218,72,199,192,237,252,
  255,208,65,91,65,90,65,89,65,88,95,94,90,89,88,157,255,72,129,230,252,255,
  0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,
  77,1,218,156,80,81,82,86,87,65,80,65,81,65,82,65,83,73,139,185,233,76,137,
  214,76,137,252,234,72,199,192,237,252,255,208,65,91,65,90,65,89,65,88,95,
  94,90,89,88,157,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,
  137,252,235,73,193,227,8,73,137,252,242,77,1,218,156,80,81,82,86,87,65,80,
  65,81,65,82,65,83,73,139,185,233,76,137,214,72,137,252,234,72,199,192,237,
  252,255,208,65,91,65,90,65,89,65,88,95,94,90,89,88,157,255,72,129,230,252,
  255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,
  242,77,1,218,156,80,81,82,86,87,65,80,65,81,65,82,65,83,73,139,185,233,76,
  137,214,72,137,252,242,72,199,192,237,252,255,208,65,91,65,90,65,89,65,88,
  95,94,90,89,88,157,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,
  73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,156,80,81,82,86,87,65,
  80,65,81,65,82,65,83,73,139,185,233,76,137,214,72,199,194,237,72,199,192,
  237,252,255,208,65,91,65,90,65,89,65,88,95,94,90,89,88,157,255,72,129,230,
  252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,
  252,242,77,1,218,156,80,81,82,86,87,65,80,65,81,65,82,65,83,73,139,185,233,
  76,137,214,72,199,194,237,72,199,192,237,252,255,208,65,91,65,90,65,89,65,
  88,95,94,90,89,88,157,73,252,255,202,68,136,214,73,193,252,234,8,68,136,213,
  255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,
  227,8,73,137,252,242,77,1,218,156,80,81,82,86,87,65,80,65,81,65,82,65,83,
  73,139,185,233,76,137,214,72,199,194,237,72,199,192,237,252,255,208,65,91,
  65,90,65,89,65,88,95,94,90,89,88,157,73,252,255,194,68,136,214,73,193,252,
  234,8,68,136,213,255,65,136,128,233,255,72,129,226,252,255,0,0,0,65,136,132,
  253,16,233,255,156,80,81,82,86,87,65,80,65,81,65,82,65,83,72,199,199,237,
  72,199,192,237,252,255,208,65,91,65,90,65,89,65,88,95,94,90,89,88,157,156,
  80,81,82,86,87,65,80,65,81,65,82,65,83,73,139,185,233,72,199,198,237,72,137,
  194,72,199,192,237,252,255,208,65,91,65,90,65,89,65,88,95,94,90,89,88,157,
  255,252,254,192,255,252,254,193,255,252,254,194,255,252,254,195,255,65,252,
  254,197,255,64,252,254,197,255,64,252,254,198,255,252,254,200,255,252,254,
  201,255,252,254,202,255,252,254,203,255,65,252,254,205,255,64,252,254,205,
  255,64,252,254,206,255,73,137,203,73,193,227,8,73,137,210,77,1,218,73,252,
  255,202,68,136,210,73,193,252,234,8,68,136,209,255,73,137,219,73,193,227,
  8,77,137,252,234,77,1,218,73,252,255,202,69,136,213,73,193,252,234,8,68,136,
  211,255,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,73,252,255,202,
  68,136,214,73,193,252,234,8,68,136,213,255,156,80,81,82,86,87,65,80,65,81,
  65,82,65,83,72,199,199,237,72,199,192,237,252,255,208,65,91,65,90,65,89,65,
  88,95,94,90,89,88,157,65,198,129,233,0,255,156,80,81,82,86,87,65,80,65,81,
  65,82,65,83,72,199,199,237,72,199,192,237,252,255,208,65,91,65,90,65,89,65,
  88,95,94,90,89,88,157,65,198,129,233,1,255,56,192,255,56,200,255,56,208,255,
  56,216,255,68,56,232,255,64,56,232,255,64,56,252,240,255,60,235,255,65,58,
  128,233,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,
  235,73,193,227,8,73,137,252,242,77,1,218,67,58,4,16,255,72,129,230,252,255,
  0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,
  77,1,218,67,58,4,16,73,252,255,202,68,136,214,73,193,252,234,8,68,136,213,
  255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,
  227,8,73,137,252,242,77,1,218,67,58,4,16,73,252,255,194,68,136,214,73,193,
  252,234,8,68,136,213,255,56,193,255,56,201,255,56,209,255,56,217,255,68,56,
  252,233,255,64,56,252,233,255,64,56,252,241,255,72,129,230,252,255,0,0,0,
  72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,
  218,67,58,12,16,255,128,252,249,235,255,56,194,255,56,202,255,56,210,255,
  56,218,255,68,56,252,234,255,64,56,252,234,255,64,56,252,242,255,72,129,230,
  252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,
  252,242,77,1,218,67,58,20,16,255,128,252,250,235,255,56,195,255,56,203,255,
  56,211,255,56,219,255,68,56,252,235,255,64,56,252,235,255,64,56,252,243,255,
  72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,
  8,73,137,252,242,77,1,218,67,58,28,16,255,128,252,251,235,255,65,56,197,255,
  65,56,205,255,65,56,213,255,65,56,221,255,69,56,252,237,255,65,56,252,237,
  255,65,56,252,245,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,
  137,252,235,73,193,227,8,73,137,252,242,77,1,218,71,58,44,16,255,65,128,252,
  253,235,255,64,56,197,255,64,56,205,255,64,56,213,255,64,56,221,255,68,56,
  252,237,255,64,56,252,237,255,64,56,252,245,255,72,129,230,252,255,0,0,0,
  72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,
  218,67,58,44,16,255,64,128,252,253,235,255,64,56,198,255,64,56,206,255,64,
  56,214,255,64,56,222,255,68,56,252,238,255,64,56,252,238,255,64,56,252,246,
  255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,
  227,8,73,137,252,242,77,1,218,67,58,52,16,255,64,128,252,254,235,255,72,129,
  230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,
  137,252,242,77,1,218,67,56,4,16,255,72,129,230,252,255,0,0,0,72,129,229,252,
  255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,56,12,16,
  255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,
  227,8,73,137,252,242,77,1,218,67,56,20,16,255,72,129,230,252,255,0,0,0,72,
  129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,
  67,56,28,16,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,
  252,235,73,193,227,8,73,137,252,242,77,1,218,71,56,44,16,255,72,129,230,252,
  255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,
  242,77,1,218,67,56,44,16,255,72,129,230,252,255,0,0,0,72,129,229,252,255,
  0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,56,52,16,255,
  72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,
  8,73,137,252,242,77,1,218,67,128,60,16,235,255,65,56,128,233,255,72,129,226,
  252,255,0,0,0,65,56,132,253,16,233,255,8,192,255,8,200,255,8,208,255,8,216,
  255,68,8,232,255,64,8,232,255,64,8,252,240,255,12,235,255,65,10,128,233,255,
  72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,
  8,73,137,252,242,77,1,218,67,10,4,16,255,72,129,230,252,255,0,0,0,72,129,
  229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,
  10,4,16,73,252,255,202,68,136,214,73,193,252,234,8,68,136,213,255,72,129,
  230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,
  137,252,242,77,1,218,67,10,4,16,73,252,255,194,68,136,214,73,193,252,234,
  8,68,136,213,255,8,193,255,8,201,255,8,209,255,8,217,255,68,8,252,233,255,
  64,8,252,233,255,64,8,252,241,255,72,129,230,252,255,0,0,0,72,129,229,252,
  255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,10,12,16,
  255,128,201,235,255,8,194,255,8,202,255,8,210,255,8,218,255,68,8,252,234,
  255,64,8,252,234,255,64,8,252,242,255,72,129,230,252,255,0,0,0,72,129,229,
  252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,10,20,
  16,255,128,202,235,255,8,195,255,8,203,255,8,211,255,8,219,255,68,8,252,235,
  255,64,8,252,235,255,64,8,252,243,255,72,129,230,252,255,0,0,0,72,129,229,
  252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,10,28,
  16,255,128,203,235,255,65,8,197,255,65,8,205,255,65,8,213,255,65,8,221,255,
  69,8,252,237,255,65,8,252,237,255,65,8,252,245,255,72,129,230,252,255,0,0,
  0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,
  1,218,71,10,44,16,255,65,128,205,235,255,64,8,197,255,64,8,205,255,64,8,213,
  255,64,8,221,255,68,8,252,237,255,64,8,252,237,255,64,8,252,245,255,72,129,
  230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,
  137,252,242,77,1,218,67,10,44,16,255,64,128,205,235,255,64,8,198,255,64,8,
  206,255,64,8,214,255,64,8,222,255,68,8,252,238,255,64,8,252,238,255,64,8,
  252,246,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,
  235,73,193,227,8,73,137,252,242,77,1,218,67,10,52,16,255,64,128,206,235,255,
  72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,
  8,73,137,252,242,77,1,218,67,8,4,16,255,72,129,230,252,255,0,0,0,72,129,229,
  252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,8,12,
  16,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,
  193,227,8,73,137,252,242,77,1,218,67,8,20,16,255,72,129,230,252,255,0,0,0,
  72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,
  218,67,8,28,16,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,
  252,235,73,193,227,8,73,137,252,242,77,1,218,71,8,44,16,255,72,129,230,252,
  255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,
  242,77,1,218,67,8,44,16,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,
  0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,8,52,16,255,72,
  129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,
  8,73,137,252,242,77,1,218,67,128,12,16,235,255,65,8,128,233,255,72,129,226,
  252,255,0,0,0,65,8,132,253,16,233,255,48,192,255,48,200,255,48,208,255,48,
  216,255,68,48,232,255,64,48,232,255,64,48,252,240,255,52,235,255,65,50,128,
  233,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,
  73,193,227,8,73,137,252,242,77,1,218,67,50,4,16,255,72,129,230,252,255,0,
  0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,
  1,218,67,50,4,16,73,252,255,202,68,136,214,73,193,252,234,8,68,136,213,255,
  72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,
  8,73,137,252,242,77,1,218,67,50,4,16,73,252,255,194,68,136,214,73,193,252,
  234,8,68,136,213,255,48,193,255,48,201,255,48,209,255,48,217,255,68,48,252,
  233,255,64,48,252,233,255,64,48,252,241,255,72,129,230,252,255,0,0,0,72,129,
  229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,
  50,12,16,255,128,252,241,235,255,48,194,255,48,202,255,48,210,255,48,218,
  255,68,48,252,234,255,64,48,252,234,255,64,48,252,242,255,72,129,230,252,
  255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,
  242,77,1,218,67,50,20,16,255,128,252,242,235,255,48,195,255,48,203,255,48,
  211,255,48,219,255,68,48,252,235,255,64,48,252,235,255,64,48,252,243,255,
  72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,
  8,73,137,252,242,77,1,218,67,50,28,16,255,128,252,243,235,255,65,48,197,255,
  65,48,205,255,65,48,213,255,65,48,221,255,69,48,252,237,255,65,48,252,237,
  255,65,48,252,245,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,
  137,252,235,73,193,227,8,73,137,252,242,77,1,218,71,50,44,16,255,65,128,252,
  245,235,255,64,48,197,255,64,48,205,255,64,48,213,255,64,48,221,255,68,48,
  252,237,255,64,48,252,237,255,64,48,252,245,255,72,129,230,252,255,0,0,0,
  72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,
  218,67,50,44,16,255,64,128,252,245,235,255,64,48,198,255,64,48,206,255,64,
  48,214,255,64,48,222,255,68,48,252,238,255,64,48,252,238,255,64,48,252,246,
  255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,
  227,8,73,137,252,242,77,1,218,67,50,52,16,255,64,128,252,246,235,255,72,129,
  230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,
  137,252,242,77,1,218,67,48,4,16,255,72,129,230,252,255,0,0,0,72,129,229,252,
  255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,48,12,16,
  255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,
  227,8,73,137,252,242,77,1,218,67,48,20,16,255,72,129,230,252,255,0,0,0,72,
  129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,
  67,48,28,16,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,
  252,235,73,193,227,8,73,137,252,242,77,1,218,71,48,44,16,255,72,129,230,252,
  255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,137,252,
  242,77,1,218,67,48,44,16,255,72,129,230,252,255,0,0,0,72,129,229,252,255,
  0,0,0,73,137,252,235,73,193,227,8,73,137,252,242,77,1,218,67,48,52,16,255,
  72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,
  8,73,137,252,242,77,1,218,67,128,52,16,235,255,65,48,128,233,255,72,129,226,
  252,255,0,0,0,65,48,132,253,16,233,255,254,0,248,10,83,84,85,65,84,65,85,
  65,86,65,87,73,137,252,249,72,199,192,0,0,0,0,65,138,129,233,72,199,193,0,
  0,0,0,65,138,137,233,72,199,194,0,0,0,0,65,138,145,233,72,199,195,0,0,0,0,
  65,138,153,233,73,199,197,0,0,0,0,69,138,169,233,72,199,197,0,0,0,0,65,138,
  169,233,72,199,198,0,0,0,0,65,138,177,233,72,199,199,0,0,0,0,102,65,139,185,
  233,77,139,145,233,77,139,130,233,255,73,129,129,233,239,65,136,129,233,65,
  136,137,233,65,136,145,233,65,136,153,233,69,136,169,233,65,136,169,233,65,
  136,177,233,102,65,137,185,233,65,95,65,94,65,93,65,92,93,92,91,72,199,192,
  0,0,0,0,195,255
};

#line 311 "src/emit.dasc"
    dasm_setup(&d, gb_actions);

    dasm_growpc(&d, npc);

	dasm_State** Dst = &d;
	//|.code
	dasm_put(Dst, 4738);
#line 317 "src/emit.dasc"
	//|->f_start:
    //| prologue
    dasm_put(Dst, 4740, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp), Dt1(->mem), offsetof(gb_memory, mem));
#line 319 "src/emit.dasc"

    for(int i = 0; i <= n; ++i) {
        switch(inst[i].opcode) {
        case NOP:
            if(!inst_nop(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case LD16:
            if(!inst_ld16(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case LD:
            if(!inst_ld(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case INC:
            if(!inst_inc(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case DEC16:
            if(!inst_dec16(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case DEC:
            if(!inst_dec(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case JP:
		case JR:
        case CALL:
            if(!inst_jp(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
		case DI:
			if(!inst_di(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
		case EI:
			if(!inst_ei(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
		case CP:
	        if(!inst_cp(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case OR:
            if(!inst_or(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case XOR:
            if(!inst_xor(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case RET:
            if(!inst_ret(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        default:
            printf("unrecognized opcode (%i)\n", inst[i].opcode);
            goto exit_fail;
        }
    }

    //| add qword state->inst_count, cycles
    //| return 0
    dasm_put(Dst, 4855, Dt1(->inst_count), cycles, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp));
#line 384 "src/emit.dasc"

    size_t sz;
    if(dasm_link(&d, &sz) != 0) {
        printf("dasm_link failed\n");
        goto exit_fail;
    }
        
    void *buf = mmap(0, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(!buf) {
        printf("could not allocate memory for function\n");
        goto exit_fail;
    }
    
    if(dasm_encode(&d, buf) != 0) {
        printf("dynasm_encode failed\n");
        goto exit_fail;
    }
    
    if(mprotect(buf, sz, PROT_READ | PROT_EXEC) != 0) {
        printf("could not make compiled function executable\n");
        goto exit_fail;
    }

    block->func = labels[lbl_f_start];

	dasm_free(&d);

    printf("compilation successful!\n");

    return true;
    
exit_fail:
	dasm_free(&d);
    return false;
}
