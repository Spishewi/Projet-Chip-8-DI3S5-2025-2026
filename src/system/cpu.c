#include <stdint.h>
#include <display/display.h>

#include "system/cpu.h"
#include "system/ram.h"

/*allocate and initialize a CPU*/
struct CPU* CPU_init(struct RAM* ram, struct Display* display){
    /*allocate the CPU array*/
    /*uses malloc to allocate the memory*/
    struct CPU* cpu = malloc(sizeof(struct CPU));

    /*if the allocation havn't worked, return NULL*/
    if(!cpu){
        fprintf(stderr, "[ERROR] : CPU cannot be allocated.");
        exit(1);
    }

    return cpu;
}

/*free the CPU*/
void CPU_destroy(struct CPU* cpu){
    free(cpu);
}

/*Do a fetch-decode-execute cycle*/
int CPU_FDE(struct CPU* cpu){}