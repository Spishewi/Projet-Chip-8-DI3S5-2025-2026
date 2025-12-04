#include <stdio.h>
#include <misc/debug.h>

#include "system/ram.h"
#include "system/rom.h"

int main(){
    struct RAM* ram = RAM_init();
    ROM_load_to_ram("./roms/1-chip8-logo.ch8", ram, 0x200);

    RAM_print_instructions(ram, 4093, 1);
    
    return 0;
}