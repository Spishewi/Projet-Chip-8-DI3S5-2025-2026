#include <stdio.h>
#include <stdbool.h>

#include <misc/debug.h>
#include <display/display.h>

#include "system/ram.h"
#include "system/rom.h"
#include "system/cpu.h"

int main(int argc, char** argv){
    /*handling the arguments*/
    if(argc != 2){
        fprintf(stderr, "[Error] : Missing ROM file in the arguments.\n");
        return 1;
    }

    /*get the rom file path from the arguments*/
    char* rom_file_path = argv[1];
    printf("Running ROM : \"%s\".", rom_file_path);

    /*create the RAM*/
    struct RAM ram;

    /*load a ROM into the RAM*/
    if(ROM_load_to_ram(rom_file_path, &ram, 0x200)){
        fprintf(stderr, "[ERROR] : cannot read ROM at \"%s\".\n", rom_file_path);
        return 1;
    }

    /*initialize the display*/
    struct Display display;
    if (Display_init(&display, 10))
    {
        fprintf(stderr, "[Error] : Display initialisation error.\n");
        return 1;
    }
    printf("Display initialized.\n");

    /*set the colors of the display*/
    if (Display_set_colors(&display, 0, 0, 0, 0, 0, 255))
    {
        fprintf(stderr, "[Error] : Display was not able to set its color\n");
        return 1;
    }
    printf("Display color set.\n"); 

    /*initialize the CPU*/
    struct CPU cpu;
    if(CPU_init(&cpu, &ram, &display)){
        fprintf(stderr, "[ERROR] : RAM initialization error.\n");
        return 1;
    }

    /*main loop (run the fetch-decode-execute cycle)*/
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

        /*debug*/
        RAM_print_instructions(&ram, cpu.PC, 1);

        /*run one FDE cycle*/
        CPU_FDE(&cpu);

        /*update the screen*/
        Display_update(&display);

        /*wait reduce the speed of the emulator and have a playable game*/
        SDL_Delay(25);
    }
    
    /*free all the memory*/
    Display_destroy(&display);
    CPU_destroy(&cpu);

    return 0;
}