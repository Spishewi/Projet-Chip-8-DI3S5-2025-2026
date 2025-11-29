#include <stdint.h>
#include <stdio.h>

#include <misc/debug.h>

#include "system/ram.h"
#include "system/rom.h"

/*load a rom into ram*/
int ROM_load_to_ram(const char* restrict file_path, RAM* ram, uint16_t start_address){
        uint16_t instruction_buffer = 0;
        FILE* file_ptr = NULL;

        uint16_t current_address = start_address;

        /*read the rom in binary mode*/
        file_ptr = fopen(file_path, "rb");
        if(!file_ptr){
            fprintf(stderr, "[ERROR] : cannot read ROM at \"%s\".", file_path);
            return 1;
        }

        /*read the file instruction by instruction*/
        while(fread(&instruction_buffer, sizeof(instruction_buffer), 1, file_ptr)){
            //printf("Setting value %04x at address %04x\n", instruction_buffer, current_address);
            printf("%s\n", instruction_as_str(instruction_buffer));
            RAM_set_instruction(ram, current_address, instruction_buffer);
            current_address += sizeof(instruction_buffer);
        }

        /*don't forget to close file*/
        fclose(file_ptr);

        return 0;
}