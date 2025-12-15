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
    cpu->PC = 0x200;

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

/*Fetch the next instruction*/
static int CPU_fetch(struct CPU* cpu, uint16_t* instruction){
    /*get the current instruction from ram*/
    int error_code = 0;

    error_code = RAM_get_instruction(cpu->ram_ptr, cpu->PC, instruction);
    if(error_code) return 1; // RAM get instruction error

    /*increment the program counter*/
    cpu->PC += 2;

    return 0;
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
    int error_code = 0;
    uint8_t x, y, n, byte, ram_value;
    uint16_t addr;

    struct Sprite sprite;
    uint8_t tmp;

    /*execute all instruction based on the decoded instruction*/
    switch (decoded_instruction)
    {
    case SYS: // 0nnn
        addr = full_instruction & 0x0FFF;
        cpu->PC = addr;
        break;

    case CLS: // 00E0
        error_code = Display_CLS(cpu->display_ptr);
        if(error_code){
            fprintf(stderr, "[ERROR] : Display CLS error.\n");
            return 2; // CLS error
        }
        break;
        
    case RET: // 00EE
        if(cpu->SP <= 0){
            fprintf(stderr, "[ERROR] : RET error, the stack pointer is already at the bottom.\n");
            return 2; // RET error
        }

        cpu->PC = cpu->Sx[cpu->SP];
        cpu->SP -= 1;
        break;
    
    case JP_A: // 1nnn
        addr = (full_instruction & 0x0FFF);

        cpu->PC = addr;
        break;

    case CALL: // 2nnn
        addr = (full_instruction & 0x0FFF);

        if(cpu->SP >= CPU_STACK_SIZE){
            fprintf(stderr, "[ERROR] : CALL error, stack overflow.\n");
            return 2; // CALL error
        }

        cpu->SP += 1;
        cpu->Sx[cpu->SP] = cpu->PC;
        cpu->PC = addr;
        break;
        
    case SE_VB: // 3xkk
        x = (full_instruction & 0x0F00) >> 8;
        byte = full_instruction & 0x00FF;

        if (cpu->Vx[x] == byte){
            cpu->PC += 2;
        }
        break;
    
    case SNE_VB: // 4xkk
        x = (full_instruction & 0x0F00) >> 8;
        byte = full_instruction & 0x00FF;

        if (cpu->Vx[x] != byte){
            cpu->PC += 2;
        }
        break;
    
    case SE_VV: // 5xy0
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;

        if (cpu->Vx[x] == cpu->Vx[y]){
            cpu->PC += 2;
        }
        break;
    
    case LD_VB: // 6xkk
        x = (full_instruction & 0x0F00) >> 8;
        byte = full_instruction & 0x00FF;

        cpu->Vx[x] = byte;
        break;

    case ADD_VB: // 7xkk
        x = (full_instruction & 0x0F00) >> 8;
        byte = full_instruction & 0x00FF;

        cpu->Vx[x] += byte;
        break;

    case LD_VV: // 8xy0
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;

        cpu->Vx[x] = cpu->Vx[y];
        break;

    case OR_VV: // 8xy1
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;

        cpu->Vx[x] = cpu->Vx[x] | cpu->Vx[y];
        break;

    case AND_VV: // 8xy2
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;

        cpu->Vx[x] = cpu->Vx[x] & cpu->Vx[y];
        break;

    case XOR_VV: // 8xy3
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;

        cpu->Vx[x] = cpu->Vx[x] ^ cpu->Vx[y];
        break;

    case ADD_VV: // 8xy4
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;

        /*we calculate if there is a carry*/
        /*use uint16_t to see if it's bigger than an uint8_t to set the carry*/
        tmp = ((uint16_t) cpu->Vx[x] + (uint16_t) cpu->Vx[y]) > 0xFF;

        cpu->Vx[x] = cpu->Vx[x] + cpu->Vx[y];
        cpu->Vx[0xF] = tmp;
        break;

    case SUB_VV: // 8xy5
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;
        
        /*we calculate if there is a carry*/
        tmp = cpu->Vx[x] >= cpu->Vx[y];

        /*do the calculation*/
        cpu->Vx[x] = cpu->Vx[x] - cpu->Vx[y];

        /*we set the carry*/
        cpu->Vx[0xF] = tmp;
        break;

    case SHR: // 8xy6
        x = (full_instruction & 0x0F00) >> 8;

        /*we calculate if there is a carry*/
        tmp = cpu->Vx[x] & 1;

        /*we divide by 2*/
        cpu->Vx[x] = cpu->Vx[x] >> 1;

        /*we set the carry*/
        cpu->Vx[0xF] = tmp;
        break;

    case SUBN_VV: // 8xy7
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;
        
        /*we calculate if there's a carry*/
        tmp = cpu->Vx[y] >= cpu->Vx[x];

        /*we set the carry*/
        cpu->Vx[x] = cpu->Vx[y] - cpu->Vx[x];
        cpu->Vx[0xF] = tmp;
        break;

    case SHL: // 8xyE
        x = (full_instruction & 0x0F00) >> 8;
        
        /*we calculate if there is a carry*/
        tmp = (cpu->Vx[x] & 0b10000000) >> 7;

        /*we multiply by 2*/
        cpu->Vx[x] = cpu->Vx[x] << 1;

        /*we set the carry*/
        cpu->Vx[0xF] = tmp;
        break;

    case SNE_VV: // 9xy0
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;

        if (cpu->Vx[x] != cpu->Vx[y]){
            cpu->PC += 2;
        }
        break;

    case LD_IA: // Annn
        addr = (full_instruction & 0x0FFF);

        cpu->I = addr;
        break;

    case DRW: // Dxyn
        x = (full_instruction & 0x0F00) >> 8;
        y = (full_instruction & 0x00F0) >> 4;
        n = full_instruction & 0x000F;

        error_code = Sprite_init(&sprite, n);
        if(error_code) return 2; // Sprite init error

        for (unsigned int i = 0; i < n; i++){
            error_code = RAM_get_value(cpu->ram_ptr, cpu->I + i, &ram_value);
            if(error_code) return 3; // RAM get value error

            error_code = Sprite_add(&sprite, ram_value);
            if(error_code){
                fprintf(stderr, "[ERROR] : Sprite add error.\n");
                return 4; // Sprite add error
            }
        }

        error_code = Display_DRW(cpu->display_ptr, &sprite, cpu->Vx[x], cpu->Vx[y], &(cpu->Vx[0xF]));
        if(error_code){
            fprintf(stderr, "[ERROR] : Display DRW error.\n");
            return 5; // Display DRW error
        }

        Sprite_destroy(&sprite);
        break;

    case LD_VDT: //Fx07
        x = (full_instruction & 0x0F00) >> 8;

        /*we set Vx at the delay timer*/
        cpu->Vx[x] = cpu->DT;
        break;

    case LD_DTV: //Fx15
        x = (full_instruction & 0x0F00) >> 8;

        /*we set the delay timer*/
        cpu->DT = cpu->Vx[x];
        break;
    
    case ADD_IV: // Fx1E
        x = (full_instruction & 0x0F00) >> 8;

        cpu->I = cpu->I + cpu->Vx[x];
        break;

    case LD_BV: // Fx33
        x = (full_instruction & 0x0F00) >> 8;

        error_code = RAM_set_value(cpu->ram_ptr, cpu->I, cpu->Vx[x] / 100);
        if(error_code) return 2;

        error_code = RAM_set_value(cpu->ram_ptr, cpu->I + 1, (cpu->Vx[x] / 10) % 10);
        if(error_code) return 3;

        error_code = RAM_set_value(cpu->ram_ptr, cpu->I + 2, cpu->Vx[x] % 10);
        if(error_code) return 4;
        break;

    case LD_IV: // Fx55
        x = (full_instruction & 0x0F00) >> 8;

        for(unsigned int i = 0; i <= x; i++){
            error_code = RAM_set_value(cpu->ram_ptr, cpu->I + i, cpu->Vx[i]);
            if(error_code) return 2;
        }
        break;

    case LD_VI: // Fx65
        x = (full_instruction & 0x0F00) >> 8;

        for(unsigned int i = 0; i <= x; i++){
            error_code = RAM_get_value(cpu->ram_ptr, cpu->I + i, &ram_value);
            if(error_code) return 2;

            cpu->Vx[i] = ram_value;
        }
        break;

    default:
        return 1;
    }
    return 0;
}

/*Do a fetch-decode-execute cycle*/
int CPU_FDE(struct CPU* cpu){
    /*fetch*/
    int error_code = 0;
    uint16_t full_instruction;

    error_code = CPU_fetch(cpu, &full_instruction);
    if(error_code){
        fprintf(stderr, "[ERROR] : CPU fetch error.\n");
        return 1; // fetch error
    }

    /*decode*/
    enum CPU_Instruction decoded_instruction = CPU_decode(full_instruction);
    if(decoded_instruction == UNKNOWN){
        fprintf(stderr, "[ERROR] : CPU decode error.\n");
        return 2; // decode error
    }
    
    
    /*execute*/
    error_code = CPU_execute(cpu, decoded_instruction, full_instruction);
    if(error_code){
        fprintf(stderr, "[ERROR] : CPU execute error.\n");
        return 3; // execute error
    }

    return 0;
}