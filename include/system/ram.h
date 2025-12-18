#include <stdint.h>

#ifndef SYSTEM_RAM_H
#define SYSTEM_RAM_H

#define RAM_SIZE 4096


struct RAM {
    uint8_t ram_array[RAM_SIZE];
};

uint8_t hexadecimal_char_sprites[16][5] = {
    [0x0] ={0b11110000,
            0b10010000,
            0b10010000,
            0b10010000,
            0b11110000},

    [0x1] ={0b00100000,
            0b01100000,
            0b00100000,
            0b00100000,
            0b01110000},

    [0x2] ={0b11110000,
            0b00010000,
            0b11110000,
            0b10000000,
            0b11110000},

    [0x3] ={0b11110000,
            0b00010000,
            0b11110000,
            0b00010000,
            0b11110000},

    [0x4] ={0b10010000,
            0b10010000,
            0b11110000,
            0b00010000,
            0b00010000},

    [0x5] ={0b11110000,
            0b10000000,
            0b11110000,
            0b00010000,
            0b11110000},

    [0x6] ={0b11110000,
            0b10000000,
            0b11110000,
            0b10010000,
            0b11110000},

    [0x7] ={0b11110000,
            0b00010000,
            0b00100000,
            0b01000000,
            0b01000000},

    [0x8] ={0b11110000,
            0b10010000,
            0b11110000,
            0b10010000,
            0b11110000},

    [0x9] ={0b11110000,
            0b10010000,
            0b11110000,
            0b00010000,
            0b11110000},

    [0xA] ={0b11110000,
            0b10010000,
            0b11110000,
            0b10010000,
            0b10010000},

    [0xB] ={0b11100000,
            0b10010000,
            0b11100000,
            0b10010000,
            0b11100000},

    [0xC] ={0b11110000,
            0b10000000,
            0b10000000,
            0b10000000,
            0b11110000},

    [0xD] ={0b11100000,
            0b10010000,
            0b10010000,
            0b10010000,
            0b11100000},

    [0xE] ={0b11110000,
            0b10000000,
            0b11110000,
            0b10000000,
            0b11110000},

    [0xF] ={0b11110000,
            0b10000000,
            0b11110000,
            0b10000000,
            0b10000000}
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
int RAM_get_hex_char(struct RAM* ram, uint16_t start_address, uint8_t character);

#endif