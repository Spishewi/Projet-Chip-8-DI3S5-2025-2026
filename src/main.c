/*-------------------------------------------------------------------------*
 | Copyright (C) 2026 Aurèle AUMONT--VESNIER and Ethan NOMBELLA.           |
 |                                                                         |
 | This file is part of chip-8-s5, yet another CHIP 8 emulator.            |
 |                                                                         |
 | chip-8-s5 is free software; you can redistribute it and/or modify       |
 | it under the terms of the GNU General Public License as published by    |
 | the Free Software Foundation; either version 3 of the License,          |
 | or (at your option) any later version.                                  |
 |                                                                         |
 | chip-8-s5 is distributed in the hope that it will be useful,            |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
 | GNU General Public License for more details.                            |
 |                                                                         |
 | You should have received a copy of the GNU General Public License       |
 | along with this program. If not, see <http://www.gnu.org/licenses/>.    |
 *-------------------------------------------------------------------------*/

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
    if(RAM_init_char_sprites(&ram, 0x0)){
        fprintf(stderr, "[ERROR] : cannot set the characters sprites in the RAM.\n");
        return 1;
    }

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
    int error_code = 0;
    while (running)
    {   
        // used to do nothing when we are in pause
        // we still do keyboard get to prevent having errors when we are not checking events for too long
        if(pause){
            error_code = Keyboard_get(&keyboard, 0, NULL);
            if(error_code == QUIT){
                running = false;
            }
            else if(error_code){
                fprintf(stderr, "[ERROR] : Keyboard get error.\n");
                running = false;
            }
            
            continue;
        }
        
        /*debug to print the instruction that will be executed*/
        /*
        if(RAM_print_instructions(&ram, cpu.PC, 1)){
            fprintf(stderr, "[ERROR] : RAM print instructions error.\n");
            pause = true;
        }*/

        /*run one FDE cycle*/
        error_code = CPU_FDE(&cpu, SDL_GetTicks64());
        if(error_code == -1){
            running = false;
        }else if(error_code){
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