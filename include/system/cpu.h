#include <stdint.h>
#include <display/display.h>

#include "system/ram.h"


#ifndef SYSTEM_CPU_H
#define SYSTEM_CPU_H

#define CPU_VX_NUMBER 16
#define CPU_STACK_SIZE 16

struct CPU {
    /*registers*/
    uint8_t Vx[CPU_VX_NUMBER]; // general purpose registers
    uint16_t I; // general purpose address register
    uint16_t PC; // program counter
    
    /*stack*/
    uint16_t Sx[CPU_STACK_SIZE]; // the stack
    uint8_t SP; // stack pointer

    /*timers*/
    uint8_t DT; // delay timer
    uint8_t ST; // sound timer

    /*useful pointers*/
    struct RAM* ram_ptr;
    struct Display* display_ptr;
};

/*allocate and initialize a CPU*/
struct CPU* CPU_init(struct RAM* ram, struct Display* display);

/*free the CPU*/
void CPU_destroy(struct CPU* cpu);

/*Do a fetch-decode-execute cycle*/
int CPU_FDE(struct CPU* cpu);

#endif