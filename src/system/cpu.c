#include <stdint.h>
#include <display/display.h>

#include "system/cpu.h"
#include "system/ram.h"

/*allocate and initialize a CPU*/
struct CPU* CPU_init(struct RAM* ram, struct Display* display){
    /*allocate the CPU*/
    /*uses malloc to allocate the memory*/
    struct CPU* cpu = malloc(sizeof(struct CPU));

    /*if the allocation haven't worked, exit with an error*/
    if(!cpu){
        fprintf(stderr, "[ERROR] : CPU cannot be allocated.");
        exit(1);
    }

    /*set default values*/
    /*registers*/
    for(int i=0; i<CPU_VX_NUMBER; i++) cpu->Vx[i] = 0;
    cpu->I = 0;
    cpu->PC = 0;

    /*stack*/
    for(int i=0; i<CPU_STACK_SIZE; i++) cpu->Sx[i] = 0;
    cpu->SP = 0;
    
    /*timers*/
    cpu->DT = 0;
    cpu->ST = 0;

    /*pointers*/
    cpu->ram_ptr = ram;
    cpu->display_ptr = display;

    return cpu;
}

/*free the CPU*/
void CPU_destroy(struct CPU* cpu){
    free(cpu);
}

/*Do a fetch-decode-execute cycle*/
int CPU_FDE(struct CPU* cpu){}

/*Fetch the next instruction*/
static uint16_t CPU_Fetch(struct CPU* cpu){
    uint16_t instruction = RAM_get_instruction(cpu->ram_ptr, cpu->PC);
    return instruction;
}