#include <stdio.h>
#include <misc/debug.h>

#include "system/ram.h"
#include "system/rom.h"

//void go ();

int main(){
    //go();
    RAM* ram = RAM_init();
    ROM_load_to_ram("./roms/1-chip8-logo.ch8", ram, 0);
    
    return 0;
}