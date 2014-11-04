#include <stdio.h>
#include "core.h"

void usage(const char *exe) {
    printf("usage: %s file.gb\n", exe);
}

int main(int argc, char *argv[]) {
    // check arguments
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }

    // init memory
    gb_vm vm;
    init_vm(&vm, argv[1]);
    
    // start emulation
    while(run_vm(&vm))
        ;
    
    // free memory
    free_vm(&vm);
}
