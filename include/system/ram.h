#include <stdint.h>

#ifndef SYSTEM_RAM_H
#define SYSTEM_RAM_H

#define RAM_SIZE 4096


struct RAM {
    uint8_t ram_array[RAM_SIZE];
};

/*
const uint8_t hexadecimal_char_sprites[16][5] = {
    [0] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
};
*/
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
int RAM_get_hex_char(struct RAM* ram, uint16_t start_address, uint8_t character);

#endif