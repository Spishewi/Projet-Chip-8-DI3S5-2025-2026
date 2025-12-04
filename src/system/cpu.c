#include <stdint.h>
#include <display/display.h>

#include "system/ram.h"

/*allocate and initialize a CPU*/
struct CPU* CPU_init(struct RAM* ram, struct Display* display){}

/*free the CPU*/
void CPU_destroy(struct CPU* cpu){}

/*Do a fetch-decode-execute cycle*/
int CPU_FDE(struct CPU* cpu){}