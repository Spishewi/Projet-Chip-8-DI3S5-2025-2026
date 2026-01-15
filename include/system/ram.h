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

#ifndef SYSTEM_RAM_H
#define SYSTEM_RAM_H

#define RAM_SIZE 4096


struct RAM {
    uint8_t ram_array[RAM_SIZE];
};

/*set all the RAM to 0*/
int RAM_clear(struct RAM* ram);

/*set a value in the RAM*/
int RAM_set_value(struct RAM* ram, uint16_t address, uint8_t value);

/*get value from the RAM*/
int RAM_get_value(struct RAM* ram, uint16_t address, uint8_t* value);

/*set an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
int RAM_set_instruction(struct RAM* ram, uint16_t address, uint16_t instruction);

/*get an instruction from the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
int RAM_get_instruction(struct RAM* ram, uint16_t address, uint16_t* instruction);

/*print `n` instructions starting from `start_address`*/
int RAM_print_instructions(struct RAM* ram, uint16_t start_address, uint16_t n);

/*initialize the char sprites in the RAM*/
int RAM_init_char_sprites(struct RAM* ram, uint16_t start_address);

/*get the hex char from the RAM*/
int RAM_get_hex_char_address(uint16_t start_address, uint8_t character, uint16_t* return_address);

#endif