#include <stdint.h>
#include <stdio.h>

#include <misc/debug.h>

#include "system/ram.h"
#include "system/rom.h"

/*load a rom into ram*/
int ROM_load_to_ram(const char* restrict file_path, struct RAM* ram, uint16_t start_address){
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
            RAM_set_value(ram, current_address, read_buffer);
            /*increment the address*/
            current_address += sizeof(read_buffer);
        }

        /*don't forget to close file*/
        fclose(file_ptr);

        return 0;
}