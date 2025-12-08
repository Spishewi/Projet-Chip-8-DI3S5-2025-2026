#include <stdint.h>
#include <display/display.h>

#include "system/ram.h"


#ifndef SYSTEM_CPU_H
#define SYSTEM_CPU_H

#define CPU_VX_NUMBER 16

struct CPU {
    /*registers*/
    uint8_t Vx[CPU_VX_NUMBER];
    uint16_t I;
    uint8_t PC;

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