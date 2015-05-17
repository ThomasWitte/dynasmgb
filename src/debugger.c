#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "debugger.h"

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
               "\e[1mbreak\e[0m      -- set breakpoint\n"
               "\e[1mcontinue\e[0m   -- continue to next breakpoint/watchpoint\n"
               "\e[1mhelp\e[0m       -- print help for a command\n"
               "\e[1mprint\e[0m      -- print value of memory address\n"
               "\e[1mquit\e[0m       -- exit dynasmgb\n"
               "\e[1mregisters\e[0m  -- print register contents\n"
               "\e[1mset\e[0m        -- set register or memory contents\n\n"
               "help [CMD] prints more details\n");
    }
    
    *quit = false;
    return true;
}

bool cmd_quit(gb_debug* dbg, bool* quit) {
    *quit = true;
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

    uint64_t addr = 0;
    sscanf(arg1, "%lx", &addr);

    uint64_t value = 0;
    sscanf(arg2, "%lx", &value);

    gb_memory_write(&dbg->vm->state, addr, value);

    printf("[0x%.4lx] = 0x%.2x\n", addr, dbg->vm->memory.mem[addr]);
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
    
    return false;
}

void gb_debug_init(gb_debug* dbg, gb_vm* vm) {
    dbg->vm = vm;
    dbg->enabled = false;
    dbg->breakpoint = 0;
}

void gb_debug_free(gb_debug* dbg) {
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
    }
    
    free(cmdline);
    return !quit;
}

void gb_debug_set_breakpoint(gb_debug *dbg, int breakpoint) {
    dbg->breakpoint = breakpoint;
}

bool run_vm_debug(gb_debug *dbg) {
    if(dbg->vm->state.pc == dbg->breakpoint) {
        printf("reached breakpoint @%#x\n", dbg->breakpoint);
        gb_debug_set_breakpoint(dbg, -1); // remove breakpoint
        gb_debug_set_enabled(dbg, true);
    }

    if(dbg->enabled) {
        SDL_Delay(0);
        return gb_debug_prompt(dbg);
    }

    return run_vm(dbg->vm);
}
