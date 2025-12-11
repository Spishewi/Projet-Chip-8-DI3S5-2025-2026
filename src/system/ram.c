#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <misc/debug.h>
#include <system/ram.h>

/*set all the RAM to 0*/
int RAM_clear(struct RAM* ram){
    int error_code = 0;

    for(int i=0; i<RAM_SIZE; i++){
        error_code = RAM_set_value(ram, i, 0);
        if(error_code) return 1;
    }

    return 0;
}

/*set a value in the RAM*/
int RAM_set_value(struct RAM* ram, uint16_t address, uint8_t value){
    if(address >= RAM_SIZE) return 1;

    /*set the value in the ram*/
    ram->ram_array[address] = value;
    return 0;
}

/*get value from the RAM*/
int RAM_get_value(struct RAM* ram, uint16_t address, uint8_t* value){
    if(address >= RAM_SIZE) return 1;

    /*store the value in the storage variable*/
    *value = ram->ram_array[address];
    return 0;
}

/*set an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
int RAM_set_instruction(struct RAM* ram, uint16_t address, uint16_t instruction){
    int error_code = 0;

    /*split the instruction into two bytes to be able to set them in ram properly*/
    uint8_t first_value = (instruction >> 8) & 0xff; // masking is optionnal, but it make more readable as it helps to understand what's going on
    uint8_t second_value = (instruction) & 0xff; // masking is optionnal, but it make more readable as it helps to understand what's going on

    error_code = RAM_set_value(ram, address, first_value);
    if(error_code) return 1;

    error_code = RAM_set_value(ram, address + 1, second_value);
    if(error_code) return 2;

    return 0;
}

/*get an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
int RAM_get_instruction(struct RAM* ram, uint16_t address, uint16_t* instruction){
    int error_code = 0;
    uint8_t first_value;
    uint8_t second_value;

    /*get the two bytes of the instruction*/
    error_code = RAM_get_value(ram, address, &first_value);
    if(error_code) return 1;

    error_code = RAM_get_value(ram, address + 1, &second_value);
    if(error_code) return 2;
    
    // concatenate the two bytes into one instruction
    *instruction = (first_value << 8) | second_value;
    return 0;
}

/*print `n` instructions starting from `start_address`*/
int RAM_print_instructions(struct RAM* ram, uint16_t start_address, uint16_t n){
    int error_code = 0;
    uint16_t instruction;

    if(start_address >= RAM_SIZE) return 1;
    if(start_address + n*sizeof(uint16_t) >= RAM_SIZE) return 2;

    for (unsigned int i = 0; i < n; i++){
        error_code = RAM_get_instruction(ram, start_address + i*sizeof(uint16_t), &instruction);
        if(error_code) return 1;

        printf("%s\n", instruction_as_str(instruction));
    }

    return  0;
}