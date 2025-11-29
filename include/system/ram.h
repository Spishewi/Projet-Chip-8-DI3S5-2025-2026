#include <stdint.h>

#ifndef SYSTEM_RAM_H
#define SYSTEM_RAM_H

#define RAM_SIZE 4096


typedef uint8_t RAM[RAM_SIZE];

/*allocate the RAM array*/
RAM* RAM_init();

/*free the RAM array*/
void RAM_Destroy(RAM* ram);

/*set a value in the RAM*/
void RAM_set_value(RAM* ram, uint16_t address, uint8_t value);

/*set an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
void RAM_set_instruction(RAM* ram, uint16_t address, uint16_t instruction);

/*set all the RAM to 0*/
void RAM_clear(RAM* ram);

/*get value from the RAM*/
uint8_t RAM_get_value(RAM* ram, uint16_t address);

#endif