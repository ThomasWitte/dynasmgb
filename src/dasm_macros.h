|.macro print, text
    | pushfq
	| push r0
	| push r1
	| push r2
	| push r6
	| push r7
	| push r8
	| push r9
	| push r10
	| push r11
	| mov rArg1, text
	| mov rax, &puts
	| call rax
	| .nop 1
	| pop r11
	| pop r10
	| pop r9
	| pop r8
	| pop r7
	| pop r6
	| pop r2
	| pop r1
	| pop r0
	| popfq
|.endmacro

|.macro prologue
    | push rbx
    | push rsp
    | push rbp
    | push r12
    | push r13
    | push r14
    | push r15
    | mov aState, rArg1
    | mov xA, 0
	| mov A, state->a
    | mov xB, 0
    | mov B, state->b
    | mov xC, 0
    | mov C, state->c
    | mov xD, 0
    | mov D, state->d
    | mov xE, 0
    | mov E, state->e
    | mov xH, 0
    | mov H, state->h
    | mov xL, 0
    | mov L, state->l
    | mov xSP, 0
    | mov SP, state->_sp
// HACK: mem.mem funktioniert nicht, deshalb nur mem in der Hoffnung, dass es
//       das erste Element der Struktur ist.
	| mov aMem, state->mem
|.endmacro
	
|.macro return, addr
    | mov state->a, A
    | mov state->b, B
    | mov state->c, C
    | mov state->d, D
    | mov state->e, E
    | mov state->h, H
    | mov state->l, L
    | mov state->_sp, SP
	| pop r15
    | pop r14
    | pop r13
    | pop r12
    | pop rbp
    | pop rsp
    | pop rbx
    | mov rRet, addr
	| ret
|.endmacro

|.macro inst, opcode, op1, op2
    || switch(op1) {
    || case REG_A:
    ||     switch(op2) {
    ||     case REG_A:
    |          opcode A, A
    ||         break;
    ||     case REG_B:
    |          opcode A, B
    ||         break;
    ||     case REG_C:
    |          opcode A, C
    ||         break;
    ||     case REG_D:
    |          opcode A, D
    ||         break;
    ||     case REG_E:
    |          opcode A, E
    ||         break;
    ||     case REG_H:
    |          opcode A, H
    ||         break;
    ||     case REG_L:
    |          opcode A, L
    ||         break;
    ||     case IMM8:
    |          opcode A, inst->args[1]
    ||         break;
    ||     case MEM_DEC_HL:
// TODO: load through mbc
    |          and xL, 0xff
    |          and xH, 0xff
    |          mov tmp2, xH
    |          shl tmp2, 8
    |          mov tmp1, xL
    |          add tmp1, tmp2
    |          opcode A, [aMem+tmp1]
    |          dec tmp1
    |          mov L, tmp1b
    |          shr tmp1, 8
    |          mov H, tmp1b
    ||         break;
    ||     default:
    ||         printf("Unsupported operand op2=%i\n", op2);
    ||         return false;
    ||     }
    ||     break;
    || case REG_B:
    ||     switch(op2) {
    ||     case REG_A:
    |          opcode B, A
    ||         break;
    ||     case REG_B:
    |          opcode B, B
    ||         break;
    ||     case REG_C:
    |          opcode B, C
    ||         break;
    ||     case REG_D:
    |          opcode B, D
    ||         break;
    ||     case REG_E:
    |          opcode B, E
    ||         break;
    ||     case REG_H:
    |          opcode B, H
    ||         break;
    ||     case REG_L:
    |          opcode B, L
    ||         break;
    ||     case IMM8:
    |          opcode B, inst->args[1]
    ||         break;
    ||     default:
    ||         printf("Unsupported operand op2=%i\n", op2);
    ||         return false;
    ||     }
    ||     break;
    || case REG_C:
    ||     switch(op2) {
    ||     case REG_A:
    |          opcode C, A
    ||         break;
    ||     case REG_B:
    |          opcode C, B
    ||         break;
    ||     case REG_C:
    |          opcode C, C
    ||         break;
    ||     case REG_D:
    |          opcode C, D
    ||         break;
    ||     case REG_E:
    |          opcode C, E
    ||         break;
    ||     case REG_H:
    |          opcode C, H
    ||         break;
    ||     case REG_L:
    |          opcode C, L
    ||         break;
    ||     case IMM8:
    |          opcode C, inst->args[1]
    ||         break;
    ||     default:
    ||         printf("Unsupported operand op2=%i\n", op2);
    ||         return false;
    ||     }
    ||     break;
    || case REG_D:
    ||     switch(op2) {
    ||     case REG_A:
    |          opcode D, A
    ||         break;
    ||     case REG_B:
    |          opcode D, B
    ||         break;
    ||     case REG_C:
    |          opcode D, C
    ||         break;
    ||     case REG_D:
    |          opcode D, D
    ||         break;
    ||     case REG_E:
    |          opcode D, E
    ||         break;
    ||     case REG_H:
    |          opcode D, H
    ||         break;
    ||     case REG_L:
    |          opcode D, L
    ||         break;
    ||     case IMM8:
    |          opcode D, inst->args[1]
    ||         break;
    ||     default:
    ||         printf("Unsupported operand op2=%i\n", op2);
    ||         return false;
    ||     }
    ||     break;
    || case REG_E:
    ||     switch(op2) {
    ||     case REG_A:
    |          opcode E, A
    ||         break;
    ||     case REG_B:
    |          opcode E, B
    ||         break;
    ||     case REG_C:
    |          opcode E, C
    ||         break;
    ||     case REG_D:
    |          opcode E, D
    ||         break;
    ||     case REG_E:
    |          opcode E, E
    ||         break;
    ||     case REG_H:
    |          opcode E, H
    ||         break;
    ||     case REG_L:
    |          opcode E, L
    ||         break;
    ||     case IMM8:
    |          opcode E, inst->args[1]
    ||         break;
    ||     default:
    ||         printf("Unsupported operand op2=%i\n", op2);
    ||         return false;
    ||     }
    ||     break;
    || case REG_H:
    ||     switch(op2) {
    ||     case REG_A:
    |          opcode H, A
    ||         break;
    ||     case REG_B:
    |          opcode H, B
    ||         break;
    ||     case REG_C:
    |          opcode H, C
    ||         break;
    ||     case REG_D:
    |          opcode H, D
    ||         break;
    ||     case REG_E:
    |          opcode H, E
    ||         break;
    ||     case REG_H:
    |          opcode H, H
    ||         break;
    ||     case REG_L:
    |          opcode H, L
    ||         break;
    ||     case IMM8:
    |          opcode H, inst->args[1]
    ||         break;
    ||     default:
    ||         printf("Unsupported operand op2=%i\n", op2);
    ||         return false;
    ||     }
    ||     break;
    || case REG_L:
    ||     switch(op2) {
    ||     case REG_A:
    |          opcode L, A
    ||         break;
    ||     case REG_B:
    |          opcode L, B
    ||         break;
    ||     case REG_C:
    |          opcode L, C
    ||         break;
    ||     case REG_D:
    |          opcode L, D
    ||         break;
    ||     case REG_E:
    |          opcode L, E
    ||         break;
    ||     case REG_H:
    |          opcode L, H
    ||         break;
    ||     case REG_L:
    |          opcode L, L
    ||         break;
    ||     case IMM8:
    |          opcode L, inst->args[1]
    ||         break;
    ||     default:
    ||         printf("Unsupported operand op2=%i\n", op2);
    ||         return false;
    ||     }
    ||     break;
    || case MEM_DEC_HL:
// TODO: load through mbc
    ||     if(op2 == REG_A) {
    |          and xL, 0xff
    |          and xH, 0xff
    |          mov tmp2, xH
    |          shl tmp2, 8
    |          mov tmp1, xL
    |          add tmp1, tmp2
    |          opcode A, [aMem+tmp1]
    |          dec tmp1
    |          mov L, tmp1b
    |          shr tmp1, 8
    |          mov H, tmp1b
    ||     } else {
    ||         printf("Unsupported operand op2=%i\n", op2);
    ||         return false;
    ||     }
    ||     break;
    || default:
    ||     printf("Unsupported operand op1=%i\n", op1);
    ||     return false;
    || }
|.endmacro
