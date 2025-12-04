#include <stdio.h>
#include <misc/debug.h>

#include "system/ram.h"
#include "system/rom.h"

int main(){
    struct RAM* ram = RAM_init();
    ROM_load_to_ram("./roms/1-chip8-logo.ch8", ram, 0x200);

    uint16_t instruction = RAM_get_instruction(ram, 0x200);
    printf("%s\n", instruction_as_str(instruction));

    instruction = RAM_get_instruction(ram, 0x202);
    printf("%s\n", instruction_as_str(instruction));
    
    return 0;
}