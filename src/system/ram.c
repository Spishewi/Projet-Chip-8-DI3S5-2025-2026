#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <system/ram.h>

/*allocate the RAM array*/
RAM* RAM_init(){
    /*allocate the RAM array*/
    /*uses calloc to allocate the memory and set it to 0*/
    RAM* ram = calloc(1, sizeof(RAM));

    /*if the allocation havn't worked, return NULL*/
    if(!ram){
        fprintf(stderr, "[ERROR] : RAM cannot be allocated.");
        exit(1);
    }

    return ram;
}

/*free the RAM array*/
void RAM_Destroy(RAM* ram){
    free(ram);
}

/*set a value in the RAM*/
void RAM_set_value(RAM* ram, uint16_t address, uint8_t value){
    if(address >= RAM_SIZE)
    {
        fprintf(stderr, "[ERROR] : trying to set value at incorrect address 0x%08x.", address);
        exit(1);
    }
    /*set the value in the ram*/
    (*ram)[address] = value;
}

/*set an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
void RAM_set_instruction(RAM* ram, uint16_t address, uint16_t instruction){
    /*split the instruction into two bytes to be able to set them in ram properly*/

    uint8_t first_value = (instruction >> 8) & 0xff; // masking is optionnal, but it make more readable as it helps to understand what's going on
    uint8_t second_value = (instruction) & 0xff; // masking is optionnal, but it make more readable as it helps to understand what's going on

    //printf("0x%02x and 0x%02x is 0x%04x\n", first_value, second_value, instruction);

    RAM_set_value(ram, address, first_value);
    RAM_set_value(ram, address + 1, second_value);
}

/*set all the RAM to 0*/
void RAM_clear(RAM* ram){
    for(int i=0; i<RAM_SIZE; i++){
        RAM_set_value(ram, i, 0);
    }
}

/*get value from the RAM*/
uint8_t RAM_get_value(RAM* ram, uint16_t address){
    if(address >= RAM_SIZE)
    {
        fprintf(stderr, "[ERROR] : trying to get value from incorrect address 0x%08x.", address);
        exit(1);
    }
    return (*ram)[address];
}

/*get an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
uint16_t RAM_get_instruction(RAM* ram, uint16_t address){
    uint8_t first_value = RAM_get_value(ram, address);
    uint8_t second_value = RAM_get_value(ram, address + 1);
    
    // concatenate the two bytes into one instruction
    return (first_value << 8) | second_value;
}