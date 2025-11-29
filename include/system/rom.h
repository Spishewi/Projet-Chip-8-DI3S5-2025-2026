#include <stdint.h>
#include "system/ram.h"

#ifndef SYSTEM_ROM_H
#define SYSTEM_ROM_H

/*load a rom into ram*/
int ROM_load_to_ram(const char* restrict file_path, RAM* ram, uint16_t start_address);

#endif