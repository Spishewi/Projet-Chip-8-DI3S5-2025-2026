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

#include <stdint.h>
#include <stdio.h>

#include <misc/debug.h>

#include "system/ram.h"
#include "system/rom.h"

/*load a rom into ram*/
int ROM_load_to_ram(const char* restrict file_path, struct RAM* ram, uint16_t start_address){
    int error_code = 0;
    uint8_t read_buffer = 0;
    FILE* file_ptr = NULL;

    uint16_t current_address = start_address;

    /*open the rom file in binary mode*/
    file_ptr = fopen(file_path, "rb");
    if(!file_ptr){
        return 1;
    }

    /*read the file instruction by instruction*/
    while(fread(&read_buffer, sizeof(read_buffer), 1, file_ptr)){
        /*set the value in the ram*/
        error_code = RAM_set_value(ram, current_address, read_buffer);
        if(error_code) return 2;
        
        /*increment the address*/
        current_address += sizeof(read_buffer);
    }

    /*don't forget to close file*/
    fclose(file_ptr);

    return 0;
}