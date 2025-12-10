#include <stdint.h>
#include <display/display.h>

#include "system/cpu.h"
#include "system/ram.h"

/*allocate and initialize a CPU*/
int CPU_init(struct CPU* cpu, struct RAM* ram, struct Display* display){
    /*set default values*/
    /*registers*/
    for(int i=0; i<CPU_VX_NUMBER; i++) cpu->Vx[i] = 0;
    cpu->I = 0;
    cpu->PC = 0;

    /*stack*/
    for(int i=0; i<CPU_STACK_SIZE; i++) cpu->Sx[i] = 0;
    cpu->SP = 0;
    
    /*timers*/
    cpu->DT = 0;
    cpu->ST = 0;

    /*pointers*/
    cpu->ram_ptr = ram;
    cpu->display_ptr = display;

    return 0;
}

/*free the CPU*/
void CPU_destroy(struct CPU* cpu){
    free(cpu);
}

/*Fetch the next instruction*/
static uint16_t CPU_fetch(struct CPU* cpu){
    /*get the current instruction from ram*/
    uint16_t instruction = RAM_get_instruction(cpu->ram_ptr, cpu->PC);

    /*increment the program counter*/
    cpu->PC += 2;

    return instruction;
}

/*Decode an instruction*/
static enum CPU_Instruction CPU_decode(uint16_t instruction){
    /*try to match patterns to decode the instruction*/
    switch (instruction & 0xF000)
    {
    case 0x0000:
        switch (instruction)
        {
        case 0x00E0:
            return CLS;
        case 0x00EE:
            return RET;
        default:
            return SYS;
        }
    case 0x1000:
        return JP_A;
    case 0x2000:
        return CALL;
    case 0x3000:
        return SE_VB;
    case 0x4000:
        return SNE_VB;
    case 0x5000:
        return SE_VV;
    case 0x6000:
        return LD_VB;
    case 0x7000:
        return ADD_VB;
    case 0x8000:
        switch(instruction & 0x000F){
            case 0x0000:
                return LD_VV;
            case 0x0001:
                return OR_VV;
            case 0x0002:
                return AND_VV;
            case 0x0003:
                return XOR_VV;
            case 0x0004:
                return ADD_VV;
            case 0x0005:
                return SUB_VV;
            case 0x0006:
                return SHR;
            case 0x0007:
                return SUBN_VV;
            case 0x000E:
                return SHL;
            default:
                return UNKNOWN;
        }
    case 0x9000:
        return SNE_VV;
    case 0xA000:
        return LD_IA;
    case 0xB000:
        return JP_VA;
    case 0xC000:
        return RND;
    case 0xD000:
        return DRW;
    case 0xE000:
        switch (instruction & 0x00FF)
        {
        case 0x009E:
            return SKP;
        case 0x00A1:
            return SKNP;
        default:
            return UNKNOWN;
        }
    case 0xF000:
        switch (instruction & 0x00FF)
        {
        case 0x0007:
            return LD_VDT;
        case 0x000A:
            return LD_VK;
        case 0x0015:
            return LD_DTV;
        case 0x0018:
            return LD_STV;
        case 0x001E:
            return ADD_IV;
        case 0x0029:
            return LD_FV;
        case 0x0033:
            return LD_BV;
        case 0x0055:
            return LD_IV;
        case 0x0065:
            return LD_VI;
        default:
            return UNKNOWN;
        }
    default:
        return UNKNOWN;
    }
}

/*extract data from uint16_t instruction and execute it (precondition: ` decoded_instruction` and `full_instruction` must be coherent)*/
static int CPU_execute(struct CPU* cpu, enum CPU_Instruction decoded_instruction, uint16_t full_instruction){

    /*define variables that can be used to extract data from an instruction*/
    uint8_t x, y, n, byte;
    uint16_t addr;

    /*execute all instruction based on the decoded instruction*/
    switch (decoded_instruction)
    {
    case CLS:
        int errCLS = Display_CLS(cpu->display_ptr);
        if(errCLS != 0){
            fprintf(stderr, "[error] : CLS execution error. Error code : %d\n", errCLS);
            exit(1);
        }
        break;
        
    case JP_A: //à tester
        addr = (full_instruction & 0x0FFF);

        cpu->PC = addr;
        break;

    case LD_VB:
        x = (full_instruction & 0x0F00) >> 8;
        byte = full_instruction & 0x00FF;

        cpu->Vx[x] = byte;
        break;

    case ADD_VB:
        x = (full_instruction & 0x0F00) >> 8;
        byte = full_instruction & 0x00FF;

        cpu->Vx[x] += byte;
        break;

    case LD_IA:
        addr = (full_instruction & 0x0FFF);

        cpu->I = addr;
        break;

    case DRW:
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;
        n = full_instruction & 0x000F;

        struct Sprite sprite;
        Sprite_init(&sprite, n);

        for (unsigned int i = 0; i < n; i++){
            Sprite_add(&sprite, RAM_get_value(cpu->ram_ptr, cpu->I + i));
        }

        Display_DRW(cpu->display_ptr, &sprite, cpu->Vx[x], cpu->Vx[y], &(cpu->Vx[0xF]));
        Sprite_destroy(&sprite);
        break;
    default:
        return -1; // error
    }
    return 0;
}

/*Do a fetch-decode-execute cycle*/
int CPU_FDE(struct CPU* cpu){
    /*fetch*/
    uint16_t full_instruction = CPU_fetch(cpu);

    /*decode*/
    enum CPU_Instruction decoded_instruction = CPU_decode(full_instruction);
    if(decoded_instruction == UNKNOWN){
        fprintf(stderr, "[error] : CPU decoding error : unknown instruction 0x%04x\n", full_instruction);
        exit(1);
    }
    
    /*execute*/
    int error_code = CPU_execute(cpu, decoded_instruction, full_instruction);
    if(error_code != 0){
        fprintf(stderr, "[error] : CPU execution error. Error code : %d, failed instruction : 0x%04x\n", error_code, full_instruction);
        exit(1);
    }
    return 0;
}