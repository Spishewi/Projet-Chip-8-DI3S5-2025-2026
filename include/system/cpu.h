#include <stdint.h>
#include <display/display.h>
#include <keyboard/keyboard.h>
#include <speaker/speaker.h>

#include "system/ram.h"


#ifndef SYSTEM_CPU_H
#define SYSTEM_CPU_H

#define CPU_VX_NUMBER 16
#define CPU_STACK_SIZE 16

/*structure representing a CPU. Needs to be initialized with the`CPU_init` function, and destroyed with the `CPU_destroy` function*/
struct CPU {
    /*registers*/
    uint8_t Vx[CPU_VX_NUMBER]; // general purpose registers
    uint16_t I; // general purpose address register
    uint16_t PC; // program counter
    
    /*stack*/
    uint16_t Sx[CPU_STACK_SIZE]; // the stack
    uint8_t SP; // stack pointer

    /*timers*/
    uint8_t DT; // delay timer
    uint8_t ST; // sound timer

    /*useful pointers*/
    struct RAM* ram_ptr;
    struct Display* display_ptr;
    struct Keyboard* keyboard_ptr;
    struct Speaker* speaker_ptr;
};

/*enumeration representing all the instructions supported by a chip-8 CPU*/
enum CPU_Instruction {
    SYS, // 0nnn
    CLS, // 00E0
    RET, // 00EE
    JP_A, // 1nnn
    CALL, // 2nnn
    SE_VB, // 3xkk
    SNE_VB, // 4xkk
    SE_VV, // 5xy0
    LD_VB, // 6xkk
    ADD_VB, // 7xkk
    LD_VV, // 8xy0
    OR_VV, // 8xy1
    AND_VV, // 8xy2
    XOR_VV, // 8xy3
    ADD_VV, // 8xy4
    SUB_VV, // 8xy5
    SHR, // 8xy6
    SUBN_VV, // 8xy7
    SHL, // 8xyE
    SNE_VV, // 9xy0
    LD_IA, // Annn
    JP_VA, // Bnnn
    RND, // Cxkk
    DRW, // Dxyn
    SKP, // Ex9E
    SKNP, // ExA1
    LD_VDT, // Fx07
    LD_VK, // Fx0A
    LD_DTV, // Fx15
    LD_STV, // Fx18
    ADD_IV, // Fx1E
    LD_FV, // Fx29
    LD_BV, // Fx33
    LD_IV, // Fx55
    LD_VI, // Fx65
    UNKNOWN
};

/*initialize a CPU*/
int CPU_init(struct CPU* cpu, struct RAM* ram, struct Display* display);

/*Do a fetch-decode-execute cycle*/
int CPU_FDE(struct CPU* cpu);

#endif