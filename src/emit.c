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
    //|.define    rRet,   rax

    //|.type state, gb_state, aState
#define Dt1(_V) (int)(ptrdiff_t)&(((gb_state *)0)_V)
#line 42 "src/emit.dasc"

    //|.include dasm_macros.h
#line 1 "src/dasm_macros.h"
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
// HACK: mem.mem funktioniert nicht, deshalb nur mem in der Hoffnung, dass es
//       das erste Element der Struktur ist.
	//| mov aMem, state->mem
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
    //||     case MEM_DEC_HL:
// TODO: load through mbc
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
    //||     default:
    //||         printf("Unsupported operand op2=%i\n", op2);
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
    //||     case IMM8:
    //|          opcode B, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i\n", op2);
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
    //||     case IMM8:
    //|          opcode C, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i\n", op2);
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
    //||     case IMM8:
    //|          opcode D, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i\n", op2);
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
    //||     case IMM8:
    //|          opcode E, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i\n", op2);
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
    //||     case IMM8:
    //|          opcode H, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i\n", op2);
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
    //||     case IMM8:
    //|          opcode L, inst->args[1]
    //||         break;
    //||     default:
    //||         printf("Unsupported operand op2=%i\n", op2);
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
    //|          opcode A, [aMem+tmp1]
    //|          dec tmp1
    //|          mov L, tmp1b
    //|          shr tmp1, 8
    //|          mov H, tmp1b
    //||     } else {
    //||         printf("Unsupported operand op2=%i\n", op2);
    //||         return false;
    //||     }
    //||     break;
    //|| default:
    //||     printf("Unsupported operand op1=%i\n", op1);
    //||     return false;
    //|| }
//|.endmacro
#line 44 "src/emit.dasc"

bool inst_nop(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "NOP"
    dasm_put(Dst, 0, "NOP", (ptrdiff_t)(puts));
#line 47 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool inst_jp(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "JP"
    dasm_put(Dst, 0, "JP", (ptrdiff_t)(puts));
#line 53 "src/emit.dasc"
    
    switch(inst->op1) {
    case NONE:
        break;
    case CC_NZ:
        //| jz >1
        dasm_put(Dst, 40);
#line 59 "src/emit.dasc"
        break;
    case CC_Z:
        //| jnz >1
        dasm_put(Dst, 45);
#line 62 "src/emit.dasc"
        break;
    case CC_NC:
        //| jc >1
        dasm_put(Dst, 50);
#line 65 "src/emit.dasc"
        break;
    case CC_C:
        //| jnc >1
        dasm_put(Dst, 55);
#line 68 "src/emit.dasc"
        break;
    default:
        printf("Invalid 1st operand to JP\n");
        return false;
    }

    //| mov qword state->inst_count, *cycles + inst->cycles;
    dasm_put(Dst, 60, Dt1(->inst_count), *cycles + inst->cycles);
#line 75 "src/emit.dasc"

    switch(inst->op2) {
    case IMM16:    
        //| return inst->args[2]*256 + inst->args[1]
        dasm_put(Dst, 66, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp), inst->args[2]*256 + inst->args[1]);
#line 79 "src/emit.dasc"
        break;
    case MEM_HL:
        //| mov tmp1, xH
        //| shl tmp1, 8
        //| add tmp1, xL
        //| return tmp1
        dasm_put(Dst, 116, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp));
#line 85 "src/emit.dasc"
        break;
    default:
        printf("Invalid 2nd operand to JP\n");
        return false;
    }

    if(inst->op1 != NONE) {
        //| 1:
        dasm_put(Dst, 177);
#line 93 "src/emit.dasc"
    }
    
    *cycles += inst->alt_cycles;
    
    return true;
}

bool inst_ld16(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "LD16"
    dasm_put(Dst, 0, "LD16", (ptrdiff_t)(puts));
#line 102 "src/emit.dasc"
    switch(inst->op1) {
    case REG_BC:
        if(inst->op2 == IMM16) {
            //| mov B, inst->args[2];
            //| mov C, inst->args[1];
            dasm_put(Dst, 180, inst->args[2], inst->args[1]);
#line 107 "src/emit.dasc"
        } else {
            printf("Invalid 2nd operand to LD16\n");
            return false;
        }
        break;
    case REG_DE:
        if(inst->op2 == IMM16) {
            //| mov D, inst->args[2];
            //| mov E, inst->args[1];
            dasm_put(Dst, 185, inst->args[2], inst->args[1]);
#line 116 "src/emit.dasc"
        } else {
            printf("Invalid 2nd operand to LD16\n");
            return false;
        }
        break;
    case REG_HL:
        if(inst->op2 == IMM16) {
            //| mov H, inst->args[2];
            //| mov L, inst->args[1];
            dasm_put(Dst, 191, inst->args[2], inst->args[1]);
#line 125 "src/emit.dasc"
        } else {
            printf("Invalid 2nd operand to LD16\n");
            return false;
        }
        break;
    case REG_SP:
        if(inst->op2 == IMM16) {
            //| mov SP, inst->args[1];
            dasm_put(Dst, 198, inst->args[1]);
#line 133 "src/emit.dasc"
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
	dasm_put(Dst, 202);
	         break;
	     case REG_B:
	dasm_put(Dst, 205);
	         break;
	     case REG_C:
	dasm_put(Dst, 208);
	         break;
	     case REG_D:
	dasm_put(Dst, 211);
	         break;
	     case REG_E:
	dasm_put(Dst, 214);
	         break;
	     case REG_H:
	dasm_put(Dst, 218);
	         break;
	     case REG_L:
	dasm_put(Dst, 222);
	         break;
	     case IMM8:
	dasm_put(Dst, 227, inst->args[1]);
	         break;
	     case MEM_DEC_HL:
	dasm_put(Dst, 230);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_B:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 281);
	         break;
	     case REG_B:
	dasm_put(Dst, 284);
	         break;
	     case REG_C:
	dasm_put(Dst, 287);
	         break;
	     case REG_D:
	dasm_put(Dst, 290);
	         break;
	     case REG_E:
	dasm_put(Dst, 293);
	         break;
	     case REG_H:
	dasm_put(Dst, 298);
	         break;
	     case REG_L:
	dasm_put(Dst, 303);
	         break;
	     case IMM8:
	dasm_put(Dst, 308, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_C:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 311);
	         break;
	     case REG_B:
	dasm_put(Dst, 314);
	         break;
	     case REG_C:
	dasm_put(Dst, 317);
	         break;
	     case REG_D:
	dasm_put(Dst, 320);
	         break;
	     case REG_E:
	dasm_put(Dst, 323);
	         break;
	     case REG_H:
	dasm_put(Dst, 328);
	         break;
	     case REG_L:
	dasm_put(Dst, 333);
	         break;
	     case IMM8:
	dasm_put(Dst, 182, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_D:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 338);
	         break;
	     case REG_B:
	dasm_put(Dst, 341);
	         break;
	     case REG_C:
	dasm_put(Dst, 344);
	         break;
	     case REG_D:
	dasm_put(Dst, 347);
	         break;
	     case REG_E:
	dasm_put(Dst, 350);
	         break;
	     case REG_H:
	dasm_put(Dst, 355);
	         break;
	     case REG_L:
	dasm_put(Dst, 360);
	         break;
	     case IMM8:
	dasm_put(Dst, 365, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_E:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 368);
	         break;
	     case REG_B:
	dasm_put(Dst, 372);
	         break;
	     case REG_C:
	dasm_put(Dst, 376);
	         break;
	     case REG_D:
	dasm_put(Dst, 380);
	         break;
	     case REG_E:
	dasm_put(Dst, 384);
	         break;
	     case REG_H:
	dasm_put(Dst, 389);
	         break;
	     case REG_L:
	dasm_put(Dst, 394);
	         break;
	     case IMM8:
	dasm_put(Dst, 187, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_H:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 399);
	         break;
	     case REG_B:
	dasm_put(Dst, 403);
	         break;
	     case REG_C:
	dasm_put(Dst, 407);
	         break;
	     case REG_D:
	dasm_put(Dst, 411);
	         break;
	     case REG_E:
	dasm_put(Dst, 415);
	         break;
	     case REG_H:
	dasm_put(Dst, 420);
	         break;
	     case REG_L:
	dasm_put(Dst, 425);
	         break;
	     case IMM8:
	dasm_put(Dst, 430, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_L:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 434);
	         break;
	     case REG_B:
	dasm_put(Dst, 438);
	         break;
	     case REG_C:
	dasm_put(Dst, 442);
	         break;
	     case REG_D:
	dasm_put(Dst, 446);
	         break;
	     case REG_E:
	dasm_put(Dst, 450);
	         break;
	     case REG_H:
	dasm_put(Dst, 455);
	         break;
	     case REG_L:
	dasm_put(Dst, 460);
	         break;
	     case IMM8:
	dasm_put(Dst, 194, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_DEC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 230);
	     } else {
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 default:
	     printf("Unsupported operand inst->op1=%i\n", inst->op1);
	     return false;
	 }
#line 149 "src/emit.dasc"
    *cycles += inst->cycles;
    return true;
}

bool inst_dec(dasm_State **Dst, gb_instruction *inst, uint64_t* cycles) {
    //| print "DEC"
    dasm_put(Dst, 0, "DEC", (ptrdiff_t)(puts));
#line 155 "src/emit.dasc"
    switch(inst->op1) {
    case REG_A:
        //| dec A
        dasm_put(Dst, 465);
#line 158 "src/emit.dasc"
        break;
    case REG_B:
        //| dec B
        dasm_put(Dst, 469);
#line 161 "src/emit.dasc"
        break;
    case REG_C:
        //| dec C
        dasm_put(Dst, 473);
#line 164 "src/emit.dasc"
        break;
    case REG_D:
        //| dec D
        dasm_put(Dst, 477);
#line 167 "src/emit.dasc"
        break;
    case REG_E:
        //| dec E
        dasm_put(Dst, 481);
#line 170 "src/emit.dasc"
        break;
    case REG_H:
        //| dec H
        dasm_put(Dst, 486);
#line 173 "src/emit.dasc"
        break;
    case REG_L:
        //| dec L
        dasm_put(Dst, 491);
#line 176 "src/emit.dasc"
        break;
    default:
        printf("Invalid operand to DEC\n");
        return false;
    }
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
	dasm_put(Dst, 496);
	         break;
	     case REG_B:
	dasm_put(Dst, 499);
	         break;
	     case REG_C:
	dasm_put(Dst, 502);
	         break;
	     case REG_D:
	dasm_put(Dst, 505);
	         break;
	     case REG_E:
	dasm_put(Dst, 508);
	         break;
	     case REG_H:
	dasm_put(Dst, 512);
	         break;
	     case REG_L:
	dasm_put(Dst, 516);
	         break;
	     case IMM8:
	dasm_put(Dst, 521, inst->args[1]);
	         break;
	     case MEM_DEC_HL:
	dasm_put(Dst, 524);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_B:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 575);
	         break;
	     case REG_B:
	dasm_put(Dst, 578);
	         break;
	     case REG_C:
	dasm_put(Dst, 581);
	         break;
	     case REG_D:
	dasm_put(Dst, 584);
	         break;
	     case REG_E:
	dasm_put(Dst, 587);
	         break;
	     case REG_H:
	dasm_put(Dst, 592);
	         break;
	     case REG_L:
	dasm_put(Dst, 597);
	         break;
	     case IMM8:
	dasm_put(Dst, 602, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_C:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 607);
	         break;
	     case REG_B:
	dasm_put(Dst, 610);
	         break;
	     case REG_C:
	dasm_put(Dst, 613);
	         break;
	     case REG_D:
	dasm_put(Dst, 616);
	         break;
	     case REG_E:
	dasm_put(Dst, 619);
	         break;
	     case REG_H:
	dasm_put(Dst, 624);
	         break;
	     case REG_L:
	dasm_put(Dst, 629);
	         break;
	     case IMM8:
	dasm_put(Dst, 634, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_D:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 639);
	         break;
	     case REG_B:
	dasm_put(Dst, 642);
	         break;
	     case REG_C:
	dasm_put(Dst, 645);
	         break;
	     case REG_D:
	dasm_put(Dst, 648);
	         break;
	     case REG_E:
	dasm_put(Dst, 651);
	         break;
	     case REG_H:
	dasm_put(Dst, 656);
	         break;
	     case REG_L:
	dasm_put(Dst, 661);
	         break;
	     case IMM8:
	dasm_put(Dst, 666, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_E:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 671);
	         break;
	     case REG_B:
	dasm_put(Dst, 675);
	         break;
	     case REG_C:
	dasm_put(Dst, 679);
	         break;
	     case REG_D:
	dasm_put(Dst, 683);
	         break;
	     case REG_E:
	dasm_put(Dst, 687);
	         break;
	     case REG_H:
	dasm_put(Dst, 692);
	         break;
	     case REG_L:
	dasm_put(Dst, 697);
	         break;
	     case IMM8:
	dasm_put(Dst, 702, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_H:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 708);
	         break;
	     case REG_B:
	dasm_put(Dst, 712);
	         break;
	     case REG_C:
	dasm_put(Dst, 716);
	         break;
	     case REG_D:
	dasm_put(Dst, 720);
	         break;
	     case REG_E:
	dasm_put(Dst, 724);
	         break;
	     case REG_H:
	dasm_put(Dst, 729);
	         break;
	     case REG_L:
	dasm_put(Dst, 734);
	         break;
	     case IMM8:
	dasm_put(Dst, 739, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case REG_L:
	     switch(inst->op2) {
	     case REG_A:
	dasm_put(Dst, 745);
	         break;
	     case REG_B:
	dasm_put(Dst, 749);
	         break;
	     case REG_C:
	dasm_put(Dst, 753);
	         break;
	     case REG_D:
	dasm_put(Dst, 757);
	         break;
	     case REG_E:
	dasm_put(Dst, 761);
	         break;
	     case REG_H:
	dasm_put(Dst, 766);
	         break;
	     case REG_L:
	dasm_put(Dst, 771);
	         break;
	     case IMM8:
	dasm_put(Dst, 776, inst->args[1]);
	         break;
	     default:
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 case MEM_DEC_HL:
	     if(inst->op2 == REG_A) {
	dasm_put(Dst, 524);
	     } else {
	         printf("Unsupported operand inst->op2=%i\n", inst->op2);
	         return false;
	     }
	     break;
	 default:
	     printf("Unsupported operand inst->op1=%i\n", inst->op1);
	     return false;
	 }
#line 188 "src/emit.dasc"
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
#line 199 "src/emit.dasc"
    dasm_init(&d, DASM_MAXSECTION);

    //|.globals lbl_
enum {
  lbl_f_start,
  lbl__MAX
};
#line 202 "src/emit.dasc"
    void* labels[lbl__MAX];
    dasm_setupglobal(&d, labels, lbl__MAX);

    //|.actionlist gb_actions
static const unsigned char gb_actions[953] = {
  156,80,81,82,86,87,65,80,65,81,65,82,65,83,72,199,199,237,72,199,192,237,
  252,255,208,65,91,65,90,65,89,65,88,95,94,90,89,88,157,255,15,132,244,247,
  255,15,133,244,247,255,15,130,244,247,255,15,131,244,247,255,73,199,129,233,
  237,255,65,136,129,233,65,136,137,233,65,136,145,233,65,136,153,233,69,136,
  169,233,65,136,169,233,65,136,177,233,102,65,137,185,233,65,95,65,94,65,93,
  65,92,93,92,91,72,199,192,237,195,255,73,137,252,234,73,193,226,8,73,1,252,
  242,65,136,129,233,65,136,137,233,65,136,145,233,65,136,153,233,69,136,169,
  233,65,136,169,233,65,136,177,233,102,65,137,185,233,65,95,65,94,65,93,65,
  92,93,92,91,76,137,208,195,255,248,1,255,177,235,178,235,255,179,235,65,181,
  235,255,64,181,235,64,182,235,255,102,191,236,255,136,192,255,136,200,255,
  136,208,255,136,216,255,68,136,232,255,64,136,232,255,64,136,252,240,255,
  176,235,255,72,129,230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,
  235,73,193,227,8,73,137,252,242,77,1,218,67,138,4,16,73,252,255,202,68,136,
  214,73,193,252,234,8,68,136,213,255,136,193,255,136,201,255,136,209,255,136,
  217,255,68,136,252,233,255,64,136,252,233,255,64,136,252,241,255,177,235,
  255,136,194,255,136,202,255,136,210,255,136,218,255,68,136,252,234,255,64,
  136,252,234,255,64,136,252,242,255,136,195,255,136,203,255,136,211,255,136,
  219,255,68,136,252,235,255,64,136,252,235,255,64,136,252,243,255,179,235,
  255,65,136,197,255,65,136,205,255,65,136,213,255,65,136,221,255,69,136,252,
  237,255,65,136,252,237,255,65,136,252,245,255,64,136,197,255,64,136,205,255,
  64,136,213,255,64,136,221,255,68,136,252,237,255,64,136,252,237,255,64,136,
  252,245,255,64,181,235,255,64,136,198,255,64,136,206,255,64,136,214,255,64,
  136,222,255,68,136,252,238,255,64,136,252,238,255,64,136,252,246,255,252,
  254,200,255,252,254,201,255,252,254,202,255,252,254,203,255,65,252,254,205,
  255,64,252,254,205,255,64,252,254,206,255,48,192,255,48,200,255,48,208,255,
  48,216,255,68,48,232,255,64,48,232,255,64,48,252,240,255,52,235,255,72,129,
  230,252,255,0,0,0,72,129,229,252,255,0,0,0,73,137,252,235,73,193,227,8,73,
  137,252,242,77,1,218,67,50,4,16,73,252,255,202,68,136,214,73,193,252,234,
  8,68,136,213,255,48,193,255,48,201,255,48,209,255,48,217,255,68,48,252,233,
  255,64,48,252,233,255,64,48,252,241,255,128,252,241,235,255,48,194,255,48,
  202,255,48,210,255,48,218,255,68,48,252,234,255,64,48,252,234,255,64,48,252,
  242,255,128,252,242,235,255,48,195,255,48,203,255,48,211,255,48,219,255,68,
  48,252,235,255,64,48,252,235,255,64,48,252,243,255,128,252,243,235,255,65,
  48,197,255,65,48,205,255,65,48,213,255,65,48,221,255,69,48,252,237,255,65,
  48,252,237,255,65,48,252,245,255,65,128,252,245,235,255,64,48,197,255,64,
  48,205,255,64,48,213,255,64,48,221,255,68,48,252,237,255,64,48,252,237,255,
  64,48,252,245,255,64,128,252,245,235,255,64,48,198,255,64,48,206,255,64,48,
  214,255,64,48,222,255,68,48,252,238,255,64,48,252,238,255,64,48,252,246,255,
  64,128,252,246,235,255,254,0,248,10,83,84,85,65,84,65,85,65,86,65,87,73,137,
  252,249,72,199,192,0,0,0,0,65,138,129,233,72,199,193,0,0,0,0,65,138,137,233,
  72,199,194,0,0,0,0,65,138,145,233,72,199,195,0,0,0,0,65,138,153,233,73,199,
  197,0,0,0,0,69,138,169,233,72,199,197,0,0,0,0,65,138,169,233,72,199,198,0,
  0,0,0,65,138,177,233,72,199,199,0,0,0,0,102,65,139,185,233,77,139,129,233,
  255,73,199,129,233,237,65,136,129,233,65,136,137,233,65,136,145,233,65,136,
  153,233,69,136,169,233,65,136,169,233,65,136,177,233,102,65,137,185,233,65,
  95,65,94,65,93,65,92,93,92,91,72,199,192,0,0,0,0,195,255
};

#line 206 "src/emit.dasc"
    dasm_setup(&d, gb_actions);

    dasm_growpc(&d, npc);

	dasm_State** Dst = &d;
	//|.code
	dasm_put(Dst, 782);
#line 212 "src/emit.dasc"
	//|->f_start:
    //| prologue
    dasm_put(Dst, 784, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp), Dt1(->mem));
#line 214 "src/emit.dasc"

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
        case DEC:
            if(!inst_dec(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case JP:
            if(!inst_jp(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        case XOR:
            if(!inst_xor(Dst, &inst[i], &cycles))
                goto exit_fail;
            break;
        default:
            printf("unrecognized opcode (%i)\n", inst[i].opcode);
            goto exit_fail;
        }
    }

    //| mov qword state->inst_count, cycles
    //| return 0
    dasm_put(Dst, 895, Dt1(->inst_count), cycles, Dt1(->a), Dt1(->b), Dt1(->c), Dt1(->d), Dt1(->e), Dt1(->h), Dt1(->l), Dt1(->_sp));
#line 249 "src/emit.dasc"

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
