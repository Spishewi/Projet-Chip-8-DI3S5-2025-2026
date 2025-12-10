#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <misc/debug.h>
#include <system/ram.h>

/*allocate the RAM array*/
int RAM_init(struct RAM* ram){
    /*allocate the RAM array*/
    /*uses calloc to allocate the memory and set it to 0*/
    ram->ram_array = calloc(RAM_SIZE, sizeof(uint8_t));

    /*if the allocation havn't worked, return NULL*/
    if(!ram->ram_array){
        return 1;
    }

    return 0;
}

/*free the RAM array*/
void RAM_Destroy(struct RAM* ram){
    free(ram);
}

/*set all the RAM to 0*/
void RAM_clear(struct RAM* ram){
    for(int i=0; i<RAM_SIZE; i++){
        RAM_set_value(ram, i, 0);
    }
}

/*set a value in the RAM*/
void RAM_set_value(struct RAM* ram, uint16_t address, uint8_t value){
    if(address >= RAM_SIZE)
    {
        fprintf(stderr, "[ERROR] : trying to set value at incorrect address 0x%04x.", address);
        exit(1);
    }
    /*set the value in the ram*/
    ram->ram_array[address] = value;
}

/*get value from the RAM*/
uint8_t RAM_get_value(struct RAM* ram, uint16_t address){
    if(address >= RAM_SIZE)
    {
        fprintf(stderr, "[ERROR] : trying to get value from incorrect address 0x%04x.", address);
        exit(1);
    }
    return ram->ram_array[address];
}

/*set an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
void RAM_set_instruction(struct RAM* ram, uint16_t address, uint16_t instruction){
    /*split the instruction into two bytes to be able to set them in ram properly*/
    uint8_t first_value = (instruction >> 8) & 0xff; // masking is optionnal, but it make more readable as it helps to understand what's going on
    uint8_t second_value = (instruction) & 0xff; // masking is optionnal, but it make more readable as it helps to understand what's going on

    RAM_set_value(ram, address, first_value);
    RAM_set_value(ram, address + 1, second_value);
}

/*set a block of data in the RAM*/
void RAM_set_block(struct RAM* ram, uint16_t address, uint8_t* block, uint16_t block_size){
    if (address + block_size >= RAM_SIZE){
        fprintf(stderr, "[ERROR] : trying to set block outside the RAM.");
        exit(1);
    }
    for(unsigned int i = 0; i < block_size; i++){
        RAM_set_value(ram, address + i, block[i]);
    }
}

/*get a block of data from the RAM. /!\ the block is a pointer to the RAM, so it will be lost if the RAM is freed*/
const uint8_t* RAM_get_block(struct RAM* ram, uint16_t address, uint16_t block_size){
    if (address + block_size >= RAM_SIZE){
        fprintf(stderr, "[ERROR] : trying to get block from outside the RAM.");
        exit(1);
    }

    return &(ram->ram_array[address]);
}

/*get an instruction in the RAM (an instruction is 16 bits instead of 8 bits for a value)*/
uint16_t RAM_get_instruction(struct RAM* ram, uint16_t address){
    uint8_t first_value = RAM_get_value(ram, address);
    uint8_t second_value = RAM_get_value(ram, address + 1);
    
    // concatenate the two bytes into one instruction
    return (first_value << 8) | second_value;
}

/*print `n` instructions starting from `start_address`*/
void RAM_print_instructions(struct RAM* ram, uint16_t start_address, uint16_t n){
    if(start_address >= RAM_SIZE)
    {
        fprintf(stderr, "[ERROR] : trying to get value from incorrect address 0x%04x.", start_address);
        exit(1);
    }
    if(start_address + n*sizeof(uint16_t) >= RAM_SIZE)
    {
        fprintf(stderr, "[ERROR] : too many instructions to read 0x%04x.", (uint16_t)(start_address + n*sizeof(uint16_t)));
        exit(1);
    }
    else{
        for (unsigned int i = 0; i < n; i++){
            uint16_t instruction = RAM_get_instruction(ram, start_address + i*sizeof(uint16_t));
            printf("%s\n", instruction_as_str(instruction));
        }
    }
}