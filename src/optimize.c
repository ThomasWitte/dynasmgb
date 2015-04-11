#include "optimize.h"

// Instruktion hat deterministisch ein immer gleiches Ergebnis, unabhängig von
// Speicher, speziellen Registern, etc.
bool is_const(gb_instruction *inst) {
	switch(inst->opcode) {
	case NOP:
		return true;
	case LD:
		if(inst->op1 == MEM_HL || inst->op2 == MEM_HL ||
		   inst->op1 == MEM_INC_HL || inst->op2 == MEM_INC_HL ||
		   inst->op1 == MEM_DEC_HL || inst->op2 == MEM_DEC_HL ||
		   inst->op1 == MEM_C || inst->op2 == MEM_C)
			return false;

		if((inst->op1 == MEM_8 || inst->op2 == MEM_8) &&
		   (inst->args[1] < 0x80 || inst->args[1] == 0xff))
			return false;
		
		return true;
	case AND:
		return inst->op2 == MEM_HL ? false : true;
	default:
		return false;
	}
}

// Instruktion ohne Speicherzugriff (lesen/schreiben), oder anderen Seiteneffekten,
// die durch einen Interrupt beeinflusst werden können
bool is_no_mem_access(gb_instruction *inst) {
	switch(inst->opcode) {
	case NOP:
	case ADD16:
		return true;
	case LD:
		if(/*inst->op1 == MEM_HL ||*/ inst->op2 == MEM_HL ||
		   /*inst->op1 == MEM_INC_HL || inst->op2 == MEM_INC_HL || */
		   /*inst->op1 == MEM_DEC_HL || inst->op2 == MEM_DEC_HL || */
		   /*inst->op1 == MEM_C ||*/ inst->op2 == MEM_C ||
		   /*inst->op1 == MEM_8 ||*/ inst->op2 == MEM_8)
			return false;
		
		return true;
	case AND:
		return inst->op2 == MEM_HL ? false : true;

	case INC:
	case DEC:
		return inst->op1 == MEM_HL ? false : true;

	default:
		return false;
	}
}

bool is_jump_to_start(gb_instruction *inst, int byte_offset) {
	if(inst->opcode == JR && inst->op2 == IMM8 && (int8_t)inst->args[1] + byte_offset == 0) {
		return true;
	}
	return false;
}

bool optimize_block(GList** instructions) {
	for(GList *inst = *instructions; inst != NULL; inst = inst->next) {
		if(DATA(inst)->address == 0x27a4)
			printf("27a4: %#x\n", *(uint32_t*) DATA(inst)->args);

		uint32_t *a = (uint32_t*) DATA(inst)->args;
		if((*a & 0xffffff) == 0x13122a) {
			printf("optimizing block @%#x (3)\n", DATA(*instructions)->address);
			// pattern is LD A,HL+; LD (DE),A; INC DE -> LD DE+, HL+
			DATA(inst)->op1 = MEM_INC_DE;
			DATA(inst)->cycles = 6;
			g_free(inst->next->data);
			*instructions = g_list_delete_link(*instructions, inst->next);
			g_free(inst->next->data);
			*instructions = g_list_delete_link(*instructions, inst->next);
		}
	}

	int byte_offset = 0;
	for(GList *inst = *instructions; inst != NULL; inst = inst->next) {
		byte_offset += DATA(inst)->bytes;
		if(is_jump_to_start(DATA(inst), byte_offset)) {
			bool can_optimize1 = true;
			bool can_optimize2 = true;
			for(GList *inst2 = *instructions; inst2 != inst; inst2 = inst2->next) {
				if(!is_const(DATA(inst2)))
					can_optimize1 = false;

				if(!is_no_mem_access(DATA(inst2)))
					can_optimize2 = false;
			}

			if(can_optimize1) {
				printf("optimizing block @%#x (1)\n", DATA(*instructions)->address);
				// optimize  2: ... JP <2
				// to        JP >1 2: HALT 1: ... JP <2

				// insert jump target at the position of the old start instruction
				gb_instruction *jp_target = g_new(gb_instruction, 1);
				*jp_target = (gb_instruction){JP_TARGET, TARGET_1, NONE, 0, DATA(*instructions)->address, 0, 0, 0, 0};
				*instructions = g_list_prepend(*instructions, jp_target);
				// prepend halt, as the loop cannot change the break condition
				gb_instruction *halt_inst = g_new(gb_instruction, 1);
				*halt_inst = (gb_instruction){HALT, NONE, NONE, 0, DATA(*instructions)->address, 1, 1, 1, INST_FLAG_ENDS_BLOCK};
				*instructions = g_list_prepend(*instructions, halt_inst);
				// prepend jump target before halt instruction
				gb_instruction *jp_target2 = g_new(gb_instruction, 1);
				*jp_target2 = (gb_instruction){JP_TARGET, TARGET_2, NONE, 0, DATA(*instructions)->address, 0, 0, 0, 0};
				*instructions = g_list_prepend(*instructions, jp_target2);
				// prepend jp, to jump to the old start instruction
				gb_instruction *jp_inst = g_new(gb_instruction, 1);
				*jp_inst = (gb_instruction){JP_FWD, NONE, TARGET_1, 0, DATA(*instructions)->address, 0, 0, 0, 0};
				*instructions = g_list_prepend(*instructions, jp_inst);
				// modify jump to point to the halt instruction
				DATA(inst)->opcode = JP_BWD;
				DATA(inst)->op2 = TARGET_2;
				DATA(inst)->flags &= ~INST_FLAG_ENDS_BLOCK;
				break;
			} else if(can_optimize2) {
				if(DATA(*instructions)->address < 0xff00)
					printf("optimizing block @%#x (2)\n", DATA(*instructions)->address);

				// insert jump target at the position of the old start instruction
				gb_instruction *jp_target = g_new(gb_instruction, 1);
				*jp_target = (gb_instruction){JP_TARGET, TARGET_1, NONE, 0, DATA(*instructions)->address, 0, 0, 0, 0};
				*instructions = g_list_prepend(*instructions, jp_target);

				DATA(inst)->opcode = JP_BWD;
				DATA(inst)->op2 = TARGET_1;							
			}
		}
    }

	return true;
}
