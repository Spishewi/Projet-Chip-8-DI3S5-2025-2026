#include <stdio.h>
#include <misc/debug.h>
#include <display/display.h>

#include "system/ram.h"
#include "system/rom.h"
#include "system/cpu.h"

int main(){
    struct RAM* ram = RAM_init();
    ROM_load_to_ram("./roms/1-chip8-logo.ch8", ram, 0x200);
    RAM_print_instructions(ram, 0x200, 39);

    // TODO TEST ERRORS
    struct Display display;
    Display_init(&display, 10);
    Display_set_colors(&display, 0, 0, 0, 0, 255, 0);

    struct CPU* cpu = CPU_init(ram, &display);
    cpu->PC = 0x200;

    while (1)
    {
        CPU_FDE(cpu);
        Display_update(&display);
        SDL_Delay(1000);
    }
    
    Display_destroy(&display);
    CPU_destroy(cpu);
    RAM_Destroy(ram);
    return 0;
}