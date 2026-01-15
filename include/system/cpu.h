/*-------------------------------------------------------------------------*
 | Copyright (C) 2026 Aurèle AUMONT--VESNIER and Ethan NOMBELLA.           |
 |                                                                         |
 | This file is part of chip-8-s5, yet another CHIP 8 emulator.            |
 |                                                                         |
 | chip-8-s5 is free software; you can redistribute it and/or modify       |
 | it under the terms of the GNU General Public License as published by    |
 | the Free Software Foundation; either version 3 of the License,          |
 | or (at your option) any later version.                                  |
 |                                                                         |
 | chip-8-s5 is distributed in the hope that it will be useful,            |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
 | GNU General Public License for more details.                            |
 |                                                                         |
 | You should have received a copy of the GNU General Public License       |
 | along with this program. If not, see <http://www.gnu.org/licenses/>.    |
 *-------------------------------------------------------------------------*/

#include <stdint.h>
#include <display/display.h>
#include <keyboard/keyboard.h>
#include <speaker/speaker.h>

#include "system/ram.h"


#ifndef SYSTEM_CPU_H
#define SYSTEM_CPU_H

#define CPU_VX_NUMBER 16
#define CPU_STACK_SIZE 16
#define CPU_TIMER_HZ 60

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

    uint64_t last_state_update; // used to update timers

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
int CPU_init(struct CPU* cpu, struct RAM* ram, struct Display* display, struct Keyboard* keyboard, struct Speaker* speaker, uint64_t current_timestamp);

/*Do a fetch-decode-execute cycle*/
int CPU_FDE(struct CPU* cpu, uint64_t current_timestamp);

/*Fetch the next instruction*/
int CPU_fetch(struct CPU* cpu, uint16_t* instruction);

/*Decode an instruction*/
enum CPU_Instruction CPU_decode(uint16_t instruction);

/*extract data from uint16_t instruction and execute it (precondition: ` decoded_instruction` and `full_instruction` must be coherent)*/
int CPU_execute(struct CPU* cpu, enum CPU_Instruction decoded_instruction, uint16_t full_instruction);

/*update timers and sound state*/
void CPU_update_state(struct CPU* cpu, uint64_t current_timestamp);
#endif