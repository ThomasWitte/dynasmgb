#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include "core.h"
#include "savestate.h"

void usage(const char *exe) {
    printf("usage: %s [-b BREAKPOINT] file.gb\n", exe);
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

    int breakpoint = -1;
    if(strcmp(argv[1], "-b") == 0) {
        // check arguments
        if(argc < 4) {
            usage(argv[0]);
            return -1;
        }

        sscanf(argv[2], "%x", &breakpoint);
        argv += 2;
        argc -= 2;
    }

    // init memory
    gb_vm vm;
    init_vm(&vm, argv[1]);

#ifdef DEBUG
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                       SDL_LOG_PRIORITY_VERBOSE);
#endif

    SDL_Event evt;
    bool debug_mode = false;
    memory_inspector_t inspector;

    // start emulation
    while(debug_mode || run_vm(&vm)) {
        if(vm.state.pc == breakpoint) {
            if(!debug_mode) {
                debug_mode = true;
                SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                                   SDL_LOG_PRIORITY_VERBOSE);

                memory_inspector_init(&inspector, &vm.memory);
                memory_inspector_update(&inspector);
                printf("debugging enabled\n");
            }
        }

        while(SDL_PollEvent(&evt)) {
            switch(evt.type) {
            case SDL_KEYUP:
                switch(evt.key.keysym.sym) {
                case SDLK_a:
                    vm.state.keys.state &= ~GB_KEY_A;
                    break;
                case SDLK_b:
                    vm.state.keys.state &= ~GB_KEY_B;
                    break;
                case SDLK_UP:
                    vm.state.keys.state &= ~GB_KEY_UP;
                    break;
                case SDLK_DOWN:
                    vm.state.keys.state &= ~GB_KEY_DOWN;
                    break;
                case SDLK_LEFT:
                    vm.state.keys.state &= ~GB_KEY_LEFT;
                    break;
                case SDLK_RIGHT:
                    vm.state.keys.state &= ~GB_KEY_RIGHT;
                    break;
                case SDLK_y: // start button
                    vm.state.keys.state &= ~GB_KEY_START;
                    break;
                case SDLK_x: // select button
                    vm.state.keys.state &= ~GB_KEY_SELECT;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch(evt.key.keysym.sym) {
                case SDLK_a:
                    vm.state.keys.state |= GB_KEY_A;
                    break;
                case SDLK_b:
                    vm.state.keys.state |= GB_KEY_B;
                    break;
                case SDLK_UP:
                    vm.state.keys.state |= GB_KEY_UP;
                    break;
                case SDLK_DOWN:
                    vm.state.keys.state |= GB_KEY_DOWN;
                    break;
                case SDLK_LEFT:
                    vm.state.keys.state |= GB_KEY_LEFT;
                    break;
                case SDLK_RIGHT:
                    vm.state.keys.state |= GB_KEY_RIGHT;
                    break;
                case SDLK_y: // start button
                    vm.state.keys.state |= GB_KEY_START;
                    break;
                case SDLK_x: // select button
                    vm.state.keys.state |= GB_KEY_SELECT;
                    break;
                case SDLK_d: // enter debugging
                    if(debug_mode) {
                        debug_mode = false;
                        vm.next_frame_time = SDL_GetTicks() + 17;
                        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                                           SDL_LOG_PRIORITY_ERROR);

                        memory_inspector_free(&inspector);
                        printf("debugging disabled\n");
                    } else {
                        debug_mode = true;
                        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                                           SDL_LOG_PRIORITY_VERBOSE);

                        memory_inspector_init(&inspector, &vm.memory);
                        memory_inspector_update(&inspector);
                        printf("debugging enabled\n");
                    }
                    break;
                case SDLK_s: // step block
                    if(debug_mode) {
                        if(run_vm(&vm) == false)
                            goto end_program;
                        memory_inspector_update(&inspector);
                    }
                    break;
                case SDLK_n: // step frame
                    if(debug_mode) {
                        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                                           SDL_LOG_PRIORITY_ERROR);

                        do {
                            if(run_vm(&vm) == false)
                                goto end_program;
                        } while(vm.state.pc != 0x40);

                        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                                           SDL_LOG_PRIORITY_VERBOSE);

                        memory_inspector_update(&inspector);
                    }
                    break;
                case SDLK_F2: // save state
                    if(savestate_save(&vm, "quicksave.dat")) {
                        printf("quicksave.dat saved.\n");
                    } else {
                        printf("quicksave failed!\n");
                    }
                    break;
                case SDLK_F3: // load state
                    if(savestate_load(&vm, "quicksave.dat")) {
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

        if(debug_mode) {
            SDL_Delay(0);
        }
    }

end_program:
    printf("exiting...\n");

    // free memory
    free_vm(&vm);
}
