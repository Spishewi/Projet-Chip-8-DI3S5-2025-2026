#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <system/ram.h>

/*allocate the RAM array*/
RAM* RAM_init(){
    /*allocate the RAM array*/
    RAM* ram = malloc(sizeof(RAM));

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
void inline RAM_set_value(RAM* ram, uint16_t address, uint8_t value){
    /*set the value in the ram*/
    *ram[address] = value;
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
        fprintf(stderr, "[ERROR] : trying to get value from incorrect address %#08x.", address);
        exit(1);
    }
    return *ram[address];
}