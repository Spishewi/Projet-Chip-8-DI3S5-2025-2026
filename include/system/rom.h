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
#include "system/ram.h"

#ifndef SYSTEM_ROM_H
#define SYSTEM_ROM_H

/*load a rom into ram*/
int ROM_load_to_ram(const char* restrict file_path, struct RAM* ram, uint16_t start_address);

#endif