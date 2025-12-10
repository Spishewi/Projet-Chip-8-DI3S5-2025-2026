#include <stdio.h>
#include <stdbool.h>

#include <misc/debug.h>
#include <display/display.h>

#include "system/ram.h"
#include "system/rom.h"
#include "system/cpu.h"

int main(int argc, char** argv){
    struct RAM* ram = RAM_init();
    ROM_load_to_ram("./roms/1-chip8-logo.ch8", ram, 0x200);
    //RAM_print_instructions(ram, 0x200, 39);

    // TODO TEST ERRORS

    struct Display display;

    if (Display_init(&display, 10))
    {
        fprintf(stderr, "[Error] : Display initialisation error\n");
        return 1;
    }
    printf("Display initialized.\n");

    if (Display_set_colors(&display, 0, 0, 0, 0, 0, 255))
    {
        fprintf(stderr, "[Error] : Display was not able to set its color\n");
        return 1;
    }

    printf("Display color set.\n"); 

    struct CPU* cpu = CPU_init(ram, &display);
    cpu->PC = 0x200;

    bool running = true;
    while (running)
    {
        /*process events to have a reponding window and a working close button*/
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        RAM_print_instructions(ram, cpu->PC, 1);
        CPU_FDE(cpu);
        Display_update(&display);
        SDL_Delay(25);
    }
    
    Display_destroy(&display);
    CPU_destroy(cpu);
    RAM_Destroy(ram);

    return 0;
}