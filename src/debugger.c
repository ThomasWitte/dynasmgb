#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "debugger.h"

void gb_stack_push(gb_stack *stack, gb_stack_frame *frame) {
    if(stack->size == stack->max_size) {
        stack->max_size += 10;
        stack->data = realloc(stack->data, stack->max_size * sizeof(gb_stack_frame));
    }
    
    stack->data[stack->size++] = *frame;
}

void gb_stack_pop(gb_stack *stack) {
    if(stack->size != 0)
        stack->size--;
//    else
//        printf("Callstack is empty!\n");
}

bool cmd_help(gb_debug* dbg, bool* quit) {
    // check for arguments
    char* tok = strtok(NULL, " ;");

    if(tok) {
        if(strcmp(tok, "help") == 0) {
            goto print_help;
        }
        printf("No help available for %s.\n", tok);
    } else {
print_help:
        printf("List of commands: \n\n"
               "\e[1mbacktrace\e[0m  -- show stack backtrace\n"
               "\e[1mbreak\e[0m      -- set breakpoint\n"
               "\e[1mcall\e[0m       -- call function at address\n"
               "\e[1mcontinue\e[0m   -- continue to next breakpoint/watchpoint\n"
               "\e[1mexec\e[0m       -- exec byte sequence\n"
               "\e[1mfind\e[0m       -- find a sequence of bytes in memory\n"
               "\e[1mhelp\e[0m       -- print help for a command\n"
               "\e[1mmeminspect\e[0m -- start/stop meminspector\n"
               "\e[1mprint\e[0m      -- print value of memory address\n"
               "\e[1mquit\e[0m       -- exit dynasmgb\n"
               "\e[1mregisters\e[0m  -- print register contents\n"
               "\e[1mset\e[0m        -- set register or memory contents\n"
               "\e[1mstep\e[0m       -- execute next block\n"
               "\e[1mwatch\e[0m      -- set watchpoint\n\n"
               "help [CMD] prints more details\n");
    }
    
    *quit = false;
    return true;
}

bool cmd_quit(gb_debug* dbg, bool* quit) {
    *quit = true;
    return true;
}

bool cmd_call(gb_debug* dbg, bool* quit) {
    *quit = false;
    
    char* arg = strtok(NULL, " ;");
    
    if(!arg) return false;

    int addr = 0;
    sscanf(arg, "%x", &addr);
    
    // save PC to stack
    dbg->vm->state._sp -= 2;
    *(uint16_t*)(&dbg->vm->state.mem->mem[dbg->vm->state._sp]) = dbg->vm->state.pc;
    // jump to address
    dbg->vm->state.pc = addr;
    
    return true;
}

bool cmd_continue(gb_debug* dbg, bool* quit) {
    gb_debug_set_enabled(dbg, false);
    *quit = false;
    return true;
}

bool cmd_break(gb_debug* dbg, bool* quit) {
    *quit = false;
    char* arg = strtok(NULL, " ;");
    
    if(!arg) return false;

    int addr = -1;
    sscanf(arg, "%x", &addr);

    gb_debug_set_breakpoint(dbg, addr);
    printf("breakpoint set @%#x.\n", addr);
    return true;
}

bool cmd_watch(gb_debug* dbg, bool* quit) {
    *quit = false;
    char* arg = strtok(NULL, " ;");
    
    if(!arg) return false;

    int addr = -1;
    sscanf(arg, "%x", &addr);

    gb_debug_set_watchpoint(dbg, addr);
    printf("watchpoint set @%#x.\n", addr);
    return true;
}

bool cmd_print(gb_debug* dbg, bool* quit) {
    *quit = false;
    char* arg = strtok(NULL, " ;");
    
    if(!arg) return false;

    int addr = -1;
    sscanf(arg, "%x", &addr);

    addr &= 0xffff;

    printf("[0x%.4x] = 0x%.2x\n", addr, dbg->vm->memory.mem[addr]);
    return true;
}

bool cmd_registers(gb_debug* dbg, bool* quit) {
    uint8_t f = (dbg->vm->state.flags & 0x1) << 4 |
                (dbg->vm->state.flags & 0x50) << 1 |
                (dbg->vm->state.f_subtract & 0x1) << 6;

    printf("AF=%02x%02x, BC=%02x%02x, DE=%02x%02x, HL=%02x%02x, SP=%04x, PC=%04x\n",
           dbg->vm->state.a, f, dbg->vm->state.b, dbg->vm->state.c,
           dbg->vm->state.d, dbg->vm->state.e, dbg->vm->state.h,
           dbg->vm->state.l, dbg->vm->state._sp, dbg->vm->state.pc);

    *quit = false;
    return true;
}

bool cmd_set(gb_debug* dbg, bool* quit) {
    *quit = false;
    char* arg1 = strtok(NULL, " ;");
    char* arg2 = strtok(NULL, " ;");
    
    if(!arg1 || !arg2) return false;

    uint64_t value = 0;
    sscanf(arg2, "%lx", &value);

    if(strcmp(arg1, "A") == 0) {
        dbg->vm->state.a = value;
        printf("A = 0x%.2x\n", dbg->vm->state.a);
    } else if(strcmp(arg1, "F") == 0) {
        dbg->vm->state.flags &= ~0x51;
        dbg->vm->state.flags |= (value & 0x10) >> 4 | (value & 0xa0) >> 1;
        dbg->vm->state.f_subtract = (value & 0x40) >> 6;
        
        uint8_t f = (dbg->vm->state.flags & 0x1) << 4 |
                    (dbg->vm->state.flags & 0x50) << 1 |
                    (dbg->vm->state.f_subtract & 0x1) << 6;
        
        printf("F = 0x%.2x\n", f);
    } else if(strcmp(arg1, "B") == 0) {
        dbg->vm->state.b = value;
        printf("B = 0x%.2x\n", dbg->vm->state.b);
    } else if(strcmp(arg1, "C") == 0) {
        dbg->vm->state.c = value;
        printf("C = 0x%.2x\n", dbg->vm->state.c);
    } else if(strcmp(arg1, "D") == 0) {
        dbg->vm->state.d = value;
        printf("D = 0x%.2x\n", dbg->vm->state.d);
    } else if(strcmp(arg1, "E") == 0) {
        dbg->vm->state.e = value;
        printf("E = 0x%.2x\n", dbg->vm->state.e);
    } else if(strcmp(arg1, "H") == 0) {
        dbg->vm->state.h = value;
        printf("H = 0x%.2x\n", dbg->vm->state.h);
    } else if(strcmp(arg1, "L") == 0) {
        dbg->vm->state.l = value;
        printf("L = 0x%.2x\n", dbg->vm->state.l);
    } else if(strcmp(arg1, "SP") == 0) {
        dbg->vm->state._sp = value;
        printf("SP = 0x%.2x\n", dbg->vm->state._sp);
    } else if(strcmp(arg1, "PC") == 0) {
        dbg->vm->state.pc = value;
        printf("PC = 0x%.2x\n", dbg->vm->state.pc);
    } else {
        uint64_t addr = 0;
        sscanf(arg1, "%lx", &addr);

        gb_memory_write(&dbg->vm->state, addr, value);

        printf("[0x%.4lx] = 0x%.2x\n", addr, dbg->vm->memory.mem[addr]);
    }
    return true;
}

bool cmd_find(gb_debug* dbg, bool* quit) {
    *quit = false;
    
    uint8_t pattern[20]; // search for max. 20 bytes
    int pattern_len = 0;
    
    char* arg;
    while((arg = strtok(NULL, " ;,[]"))) {
        sscanf(arg, "%hhx", pattern + pattern_len);
        pattern_len++;
        if(pattern_len == 20)
            break;
    }
    
    if(pattern_len == 0) return false;

    for(int addr = 0; addr <= 0xffff - pattern_len; ++addr) {
        int l = 0;
        for(; l < pattern_len; ++l) {
            if(dbg->vm->memory.mem[addr + l] != pattern[l])
                break;
        }
        if(l == pattern_len) {
            printf("pattern found @0x%.4x: ", addr);
            if(addr >= 5) {
                for(int i = addr-5; i < addr; i++) {
                    printf("%.2x ", dbg->vm->memory.mem[i]);
                }
            }
            
            for(int i = addr; i < addr+pattern_len; i++) {
                printf("\e[1m%.2x\e[0m ", dbg->vm->memory.mem[i]);
            }
            
            if(addr+pattern_len+5 <= 0xffff) {
                for(int i = addr+pattern_len; i < addr+pattern_len+5; i++) {
                    printf("%.2x ", dbg->vm->memory.mem[i]);
                }
            }
            printf("\n");
        }
    }

    return true;
}

bool cmd_backtrace(gb_debug* dbg, bool* quit) {
    *quit = false;

    for(unsigned i = 0; i < dbg->stack.size; ++i) {
        if(dbg->stack.data[dbg->stack.size - 1 - i].trap_reason == REASON_INT) {
            printf("\e[0;31m");
        }
        
        if(dbg->stack.data[dbg->stack.size - 1 - i].trap_reason == REASON_RST) {
            printf("\e[0;32m");
        }
        
        printf("%i: 0x%.4x\e[0m\n", i, dbg->stack.data[dbg->stack.size - 1 - i].pc);
    }

    return true;
}

bool cmd_step(gb_debug* dbg, bool* quit) {
    *quit = false;

    return run_vm(dbg->vm);
}

bool cmd_exec(gb_debug* dbg, bool* quit) {
    *quit = false;
    
    uint8_t bytes[103];
    for(int i = 0; i < 100; ++i)
        bytes[i] = 0; //NOP
    
    bytes[100] = 0xc3; //jp
    bytes[101] = dbg->vm->state.pc & 0x00ff;
    bytes[102] = (dbg->vm->state.pc & 0xff00) >> 8;
    
    int len = 0;
    char* arg;
    while((arg = strtok(NULL, " ;,[]"))) {
        sscanf(arg, "%hhx", bytes + len);
        len++;
        if(len == 100) {
            printf("maximum size of block reached.");
            break;
        }
    }
    
    gb_block temp = {0};
    gb_memory temp_mem;
    temp_mem.mem = bytes;

    if(!compile(&temp, &temp_mem, 0, 0)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "an error occurred while compiling the function @%#x.\n", dbg->vm->state.pc);
        return false;
    }
    int next_pc = temp.func(&dbg->vm->state);
    if(next_pc != -1)
        dbg->vm->state.pc = next_pc;

    free_block(&temp);
    return true;
}

bool cmd_meminspect(gb_debug *dbg, bool* quit) {
    *quit = false;
    
    if(!dbg->meminspector) {
        dbg->meminspector = malloc(sizeof(memory_inspector_t));
        memory_inspector_init(dbg->meminspector, &dbg->vm->memory);
        memory_inspector_update(dbg->meminspector);
    } else {
        memory_inspector_free(dbg->meminspector);
        free(dbg->meminspector);
    }
    
    return true;
}

bool gb_debug_execute_cmd(gb_debug* dbg, char* cmdline, bool *quit) {
    char* tok = strtok(cmdline, " ;");

    if(strcmp(tok, "help") == 0) {
        return cmd_help(dbg, quit);
    }
    
    if(strcmp(tok, "quit") == 0) {
        return cmd_quit(dbg, quit);
    }

    if(strcmp(tok, "continue") == 0) {
        return cmd_continue(dbg, quit);
    }
    
    if(strcmp(tok, "break") == 0) {
        return cmd_break(dbg, quit);
    }
    
    if(strcmp(tok, "print") == 0) {
        return cmd_print(dbg, quit);
    }
    
    if(strcmp(tok, "registers") == 0) {
        return cmd_registers(dbg, quit);
    }

    if(strcmp(tok, "set") == 0) {
        return cmd_set(dbg, quit);
    }
    
    if(strcmp(tok, "watch") == 0) {
        return cmd_watch(dbg, quit);
    }
    
    if(strcmp(tok, "find") == 0) {
        return cmd_find(dbg, quit);
    }
    
    if(strcmp(tok, "backtrace") == 0) {
        return cmd_backtrace(dbg, quit);
    }
    
    if(strcmp(tok, "step") == 0) {
        return cmd_step(dbg, quit);
    }
    
    if(strcmp(tok, "meminspect") == 0) {
        return cmd_meminspect(dbg, quit);
    }
    
    if(strcmp(tok, "call") == 0) {
        return cmd_call(dbg, quit);
    }
    
    if(strcmp(tok, "exec") == 0) {
        return cmd_exec(dbg, quit);
    }
    
    return false;
}

void gb_debug_init(gb_debug* dbg, gb_vm* vm) {
    dbg->vm = vm;
    dbg->enabled = false;
    dbg->breakpoint = -1;
    dbg->watchpoint = -1;
    dbg->watchpoint_value = 0;
    
    dbg->stack.data = malloc(10 * sizeof(gb_stack_frame));
    dbg->stack.size = 0;
    dbg->stack.max_size = 10;
    
    gb_stack_frame frame = {dbg->vm->state._sp, dbg->vm->state.pc, REASON_CALL};
    gb_stack_push(&dbg->stack, &frame);
    
    dbg->meminspector = 0;
}

void gb_debug_free(gb_debug* dbg) {
    free(dbg->stack.data);
    
    if(dbg->meminspector) {
        memory_inspector_free(dbg->meminspector);
        free(dbg->meminspector);
    }
}

void gb_debug_set_enabled(gb_debug* dbg, bool mode) {
    dbg->vm->next_frame_time = SDL_GetTicks() + 17;
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                       mode ? SDL_LOG_PRIORITY_VERBOSE : SDL_LOG_PRIORITY_ERROR);

    dbg->enabled = mode;
}

bool gb_debug_is_enabled(gb_debug* dbg) {
    return dbg->enabled;
}

bool gb_debug_prompt(gb_debug *dbg) {
    char* cmdline = readline("\e[1m(dynasmgb)\e[0m ");
    char* cmd = cmdline;
    
    if(!cmdline) { //EOF received
        printf("continue\n");
        
        bool quit = false;
        cmd_continue(dbg, &quit);
    
        return !quit;
    }

    // strip spaces from the beginning
    while(*cmd == ' ') ++cmd;
    
    bool quit = false;
    if(*cmd) { //dont save blank lines
        add_history(cmd);
        if(!gb_debug_execute_cmd(dbg, cmd, &quit)) {
            printf("Unsupported command, \e[1mhelp\e[0m helps.\n");
        }
    } else {
        cmd_step(dbg, &quit);
    }
    
    free(cmdline);
    return !quit;
}

void gb_debug_set_breakpoint(gb_debug *dbg, int breakpoint) {
    dbg->breakpoint = breakpoint;
}

void gb_debug_set_watchpoint(gb_debug *dbg, int watchpoint) {
    dbg->watchpoint_value = dbg->vm->memory.mem[watchpoint & 0xffff];
    dbg->watchpoint = watchpoint;
}

bool run_vm_debug(gb_debug *dbg) {
    if(dbg->vm->state.trap_reason != 0) {
        if(dbg->vm->state.trap_reason & REASON_CALL) {
            gb_stack_frame frame = {dbg->vm->state._sp, dbg->vm->state.pc, REASON_CALL};
            gb_stack_push(&dbg->stack, &frame);
        }
        
        if(dbg->vm->state.trap_reason & REASON_RST) {
            gb_stack_frame frame = {dbg->vm->state._sp, dbg->vm->state.pc, REASON_RST};
            gb_stack_push(&dbg->stack, &frame);
        }
        
        if(dbg->vm->state.trap_reason & REASON_RET) {
            // most games dont return from rst
            if(dbg->stack.size)
                while(dbg->stack.size && dbg->stack.data[dbg->stack.size - 1].trap_reason == REASON_RST)
                    gb_stack_pop(&dbg->stack);

            gb_stack_pop(&dbg->stack);
        }
        
        if(dbg->vm->state.trap_reason & REASON_INT) {
            gb_stack_frame frame = {dbg->vm->state._sp, dbg->vm->state.pc, REASON_INT};
            gb_stack_push(&dbg->stack, &frame);
        }
        dbg->vm->state.trap_reason = 0;
    }

    if(dbg->watchpoint != -1 &&
       dbg->vm->memory.mem[dbg->watchpoint] != dbg->watchpoint_value)
    {
        printf("value of watchpoint changed while executing block @%#x\n",
               dbg->vm->state.last_pc);
        printf("[0x%.4x] : 0x%.2x -> 0x%.2x\n", dbg->watchpoint & 0xffff,
               dbg->watchpoint_value, dbg->vm->memory.mem[dbg->watchpoint]);
        gb_debug_set_watchpoint(dbg, -1); // remove watchpoint
        gb_debug_set_enabled(dbg, true);
    }

    if(dbg->vm->state.pc == dbg->breakpoint) {
        printf("reached breakpoint @%#x\n", dbg->breakpoint);
        gb_debug_set_breakpoint(dbg, -1); // remove breakpoint
        gb_debug_set_enabled(dbg, true);
    }

    if(dbg->enabled) {
        if(dbg->meminspector) {
            memory_inspector_update(dbg->meminspector);
        }
        
        SDL_Delay(0);
        return gb_debug_prompt(dbg);
    }

    return run_vm(dbg->vm);
}
