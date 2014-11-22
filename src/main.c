#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include "core.h"

void usage(const char *exe) {
    printf("usage: %s file.gb\n", exe);
}

void sig_segv_handler(int sig, siginfo_t *si, void *unused) {
    void *array[10];
    size_t size;

    size = backtrace(array, 10);

    printf("Got SIGSEGV at address: %#lx\n",(long) si->si_addr);

    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main(int argc, char *argv[]) {

//    struct sigaction sa;
//    sa.sa_flags = SA_SIGINFO;
//    sigemptyset(&sa.sa_mask);
//    sa.sa_sigaction = sig_segv_handler;
//    sigaction(SIGSEGV, &sa, NULL);

    // check arguments
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }

    // init memory
    gb_vm vm;
    init_vm(&vm, argv[1]);

    SDL_Event evt;
    bool debug_mode = false;
    memory_inspector_t inspector;

    // start emulation
    while(debug_mode || run_vm(&vm)) {
        if(vm.state.pc == 0x1a07) {
            if(!debug_mode) {
                debug_mode = true;
                SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                                   SDL_LOG_PRIORITY_VERBOSE);

                memory_inspector_init(&inspector, &vm.memory);
            }
            goto test;
        }

        while(SDL_PollEvent(&evt)) {
            switch(evt.type) {
            case SDL_KEYDOWN:
                switch(evt.key.keysym.sym) {
                case SDLK_d: // enter debugging
                    if(debug_mode) {
                        debug_mode = false;
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
test:
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
