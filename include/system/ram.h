#include <stdint.h>

#ifndef SYSTEM_RAM_H
#define SYSTEM_RAM_H

#define RAM_SIZE 4096


struct RAM {
    uint8_t* ram_array;
};

/*allocate the RAM*/
int RAM_init(struct RAM* ram);

/*free the RAM*/
void RAM_Destroy(struct RAM* ram);

/*set all the RAM to 0*/
void RAM_clear(struct RAM* ram);

/*set a value in the RAM*/
void RAM_set_value(struct RAM* ram, uint16_t address, uint8_t value);

/*get value from the RAM*/
uint8_t RAM_get_value(struct RAM* ram, uint16_t address);

/*set an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
void RAM_set_instruction(struct RAM* ram, uint16_t address, uint16_t instruction);

/*get an instruction from the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
uint16_t RAM_get_instruction(struct RAM* ram, uint16_t address);

/*set a block of data in the RAM*/
void RAM_set_block(struct RAM* ram, uint16_t address, uint8_t* block, uint16_t block_size);

/*get a block of data from the RAM. /!\ the block is a pointer to the RAM, so it will be lost if the RAM is freed*/
const uint8_t* RAM_get_block(struct RAM* ram, uint16_t address, uint16_t block_size);

/*print `n` instructions starting from `start_address`*/
void RAM_print_instructions(struct RAM* ram, uint16_t start_address, uint16_t n);

#endif