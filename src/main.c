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
        fprintf(stderr, "[ERROR] : Missing ROM file in the arguments.\n");
        return 1;
    }

    /*get the rom file path from the arguments*/
    char* rom_file_path = argv[1];
    printf("Running ROM : \"%s\".", rom_file_path);

    /*create the RAM*/
    struct RAM ram;

    /*set the characters sprites in the RAM*/
    RAM_init_char_sprites(&ram, 0x000);

    /*load a ROM into the RAM*/
    if(ROM_load_to_ram(rom_file_path, &ram, 0x200)){
        fprintf(stderr, "[ERROR] : cannot read ROM at \"%s\".\n", rom_file_path);
        return 1;
    }

    /*initialize the display*/
    struct Display display;
    if (Display_init(&display, 10))
    {
        fprintf(stderr, "[ERROR] : Display initialisation error.\n");
        return 1;
    }
    printf("Display initialized.\n");

    /*set the colors of the display*/
    if (Display_set_colors(&display, 0, 0, 0, 0, 0, 255))
    {
        fprintf(stderr, "[ERROR] : Display was not able to set its color\n");
        return 1;
    }
    printf("Display color set.\n"); 

    /*initialize the keyboard*/
    struct Keyboard keyboard;
    if (Keyboard_init(&keyboard))
    {
        fprintf(stderr, "[ERROR] : Keyboard initialisation error.\n");
        return 1;
    }
    printf("Keyboard initialised.\n");

    /*initialize the speaker*/
    struct Speaker speaker;
    if (Speaker_init(&speaker))
    {
        fprintf(stderr, "[ERROR] : Speaker initialisation error.\n");
        return 1;
    }
    printf("Speaker initialised.\n");

    /*initialize the CPU*/
    struct CPU cpu;
    if(CPU_init(&cpu, &ram, &display, &keyboard, &speaker, SDL_GetTicks64())){
        fprintf(stderr, "[ERROR] : RAM initialisation error.\n");
        return 1;
    }

    /*main loop (run the fetch-decode-execute cycle)*/
    bool running = true;
    bool pause = false;
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

        if(pause) continue;

        /*debug*/
        /*
        if(RAM_print_instructions(&ram, cpu.PC, 1)){
            fprintf(stderr, "[ERROR] : RAM print instructions error.\n");
            pause = true;
        }*/

        /*run one FDE cycle*/
        if(CPU_FDE(&cpu, SDL_GetTicks64())){
            fprintf(stderr, "[ERROR] : CPU FDE error.\n");
            pause = true;
        }

        /*update the screen*/
        if(Display_update(&display)){
            fprintf(stderr, "[ERROR] : Display update error.\n");
            pause = true;
        }

        /*wait reduce the speed of the emulator and have a playable game*/
        SDL_Delay(1);
    }

    /*free all the memory*/
    Speaker_destroy(&speaker);
    Keyboard_destroy(&keyboard);
    Display_destroy(&display);
    return 0;
}