#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include "core.h"
#include "debugger.h"
#include "savestate.h"

void usage(const char *exe) {
    printf("usage: %s [-b BREAKPOINT] [-O LEVEL] [-d] file.gb\n", exe);
}

//gb_vm vm;

void sig_segv_handler(int sig, siginfo_t *si, void *unused) {
    void *array[10];
    size_t size;

    size = backtrace(array, 10);

    printf("Got SIGSEGV at address: %#lx\n",(long) si->si_addr);

    backtrace_symbols_fd(array, size, STDERR_FILENO);

/*    printf("ioregs: STAT=%02x LY=%02x IE=%02x\n", vm.memory.mem[0xff41], vm.memory.mem[0xff44], vm.memory.mem[0xffff]);
    printf("register: A=%02x, BC=%02x%02x, DE=%02x%02x, HL=%02x%02x, SP=%04x\n",
           vm.state.a, vm.state.b, vm.state.c, vm.state.d, vm.state.e,
           vm.state.h, vm.state.l, vm.state._sp);
    printf("last address: %#x\n", vm.state.last_pc);
    printf("next address: %#x\n", vm.state.pc);*/

    exit(1);
}

int main(int argc, char *argv[]) {

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sig_segv_handler;
    sigaction(SIGSEGV, &sa, NULL);

    // check arguments
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }

    bool debug_mode = false;
    int opt_level = 0;
    int breakpoint = -1;
    int c;
    
    while((c = getopt(argc, argv, "b:O:d")) != -1) {
        switch(c) {
        case 'b':
            sscanf(optarg, "%x", &breakpoint);
            break;

        case 'O':
            sscanf(optarg, "%i", &opt_level);
            break;

        case 'd':
            debug_mode = true;
            break;

        case '?':
        default:
            usage(argv[0]);
            return -1;
        }
    }

    if(optind >= argc) {
        usage(argv[0]);
        return -1;
    }

    if(opt_level > 3) opt_level = 3;
    if(opt_level < 0) opt_level = 0;

    // init memory
    gb_vm *vm = malloc(sizeof(gb_vm));
    init_vm(vm, argv[optind], opt_level, true);
    
    gb_debug dbg;
    gb_debug_init(&dbg, vm);
    gb_debug_set_enabled(&dbg, debug_mode);
    
    if(breakpoint >= 0) {
        gb_debug_set_breakpoint(&dbg, breakpoint);
    }
    
#ifdef DEBUG
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_VERBOSE);
#endif

    SDL_Event evt;

    // start emulation
    while(run_vm_debug(&dbg)) {

        while(SDL_PollEvent(&evt)) {
            switch(evt.type) {
            case SDL_KEYUP:
                switch(evt.key.keysym.sym) {
                case SDLK_a:
                    vm->state.keys.state &= ~GB_KEY_A;
                    break;
                case SDLK_b:
                    vm->state.keys.state &= ~GB_KEY_B;
                    break;
                case SDLK_UP:
                    vm->state.keys.state &= ~GB_KEY_UP;
                    break;
                case SDLK_DOWN:
                    vm->state.keys.state &= ~GB_KEY_DOWN;
                    break;
                case SDLK_LEFT:
                    vm->state.keys.state &= ~GB_KEY_LEFT;
                    break;
                case SDLK_RIGHT:
                    vm->state.keys.state &= ~GB_KEY_RIGHT;
                    break;
                case SDLK_y: // start button
                    vm->state.keys.state &= ~GB_KEY_START;
                    break;
                case SDLK_x: // select button
                    vm->state.keys.state &= ~GB_KEY_SELECT;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch(evt.key.keysym.sym) {
                case SDLK_a:
                    vm->state.keys.state |= GB_KEY_A;
                    vm->memory.mem[0xff0f] |= 0x10;
                    break;
                case SDLK_b:
                    vm->state.keys.state |= GB_KEY_B;
                    vm->memory.mem[0xff0f] |= 0x10;
                    break;
                case SDLK_UP:
                    vm->state.keys.state |= GB_KEY_UP;
                    vm->memory.mem[0xff0f] |= 0x10;
                    break;
                case SDLK_DOWN:
                    vm->state.keys.state |= GB_KEY_DOWN;
                    vm->memory.mem[0xff0f] |= 0x10;
                    break;
                case SDLK_LEFT:
                    vm->state.keys.state |= GB_KEY_LEFT;
                    vm->memory.mem[0xff0f] |= 0x10;
                    break;
                case SDLK_RIGHT:
                    vm->state.keys.state |= GB_KEY_RIGHT;
                    vm->memory.mem[0xff0f] |= 0x10;
                    break;
                case SDLK_y: // start button
                    vm->state.keys.state |= GB_KEY_START;
                    vm->memory.mem[0xff0f] |= 0x10;
                    break;
                case SDLK_x: // select button
                    vm->state.keys.state |= GB_KEY_SELECT;
                    vm->memory.mem[0xff0f] |= 0x10;
                    break;
                case SDLK_d: // enter debugging
                    if(gb_debug_is_enabled(&dbg)) {
                        gb_debug_set_enabled(&dbg, false);
                    } else {
                        gb_debug_set_enabled(&dbg, true);
                    }
                    break;
                case SDLK_F2: // save state
                    if(savestate_save(vm, "quicksave.dat")) {
                        printf("quicksave.dat saved.\n");
                    } else {
                        printf("quicksave failed!\n");
                    }
                    break;
                case SDLK_F3: // load state
                    if(savestate_load(vm, "quicksave.dat")) {
                        printf("quicksave.dat loaded.\n");
                    } else {
                        printf("quickload failed!\n");
                    }
                    break;
                case SDLK_ESCAPE:
                    goto end_program;
                default:
                    break;
                }
                break;
            case SDL_QUIT:
                goto end_program;
            default:
                break;
            }
        }
    }

end_program:
    printf("exiting...\n");

    gb_debug_free(&dbg);

    // free memory
    free_vm(vm);
    free(vm);
}
