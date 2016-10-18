/**
   Copyright (C) 2016 Patrick Bruenn.

   This file is part of WyLight.

   WyLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   WyLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include <sys/stat.h>
#include <array>
#include <fstream>
#include "eeprom.h"

#define EEPROM_FILENAME "eeprom.bin"
std::fstream g_Eeprom;

void Eeprom_Init(void)
{
    static const auto flags = std::ios::in | std::ios::out | std::ios::binary;
    struct stat buffer;

    bool missing = stat(EEPROM_FILENAME, &buffer);

    if (missing || (EEPROM_SIZE != buffer.st_size)) {
        std::array<char, EEPROM_SIZE> empty_buffer {0};
        g_Eeprom.open(EEPROM_FILENAME, flags | std::ios_base::trunc);
        g_Eeprom.write(empty_buffer.data(), empty_buffer.size());
    } else {
        g_Eeprom.open(EEPROM_FILENAME, flags);
    }
    g_Eeprom.flush();
}

uns8 Eeprom_Read(const uns16 address)
{
    uns8 buffer;
    Eeprom_ReadBlock(&buffer, address, 1);
    return buffer;
}

void Eeprom_Write(const uns16 address, const uns8 data)
{
    Eeprom_WriteBlock(&data, address, 1);
}

void Eeprom_WriteBlock(const uns8* array, uns16 address, const uns8 length)
{
    if (address + length <= EEPROM_SIZE) {
        g_Eeprom.seekp(address);
        g_Eeprom.write(reinterpret_cast<const char*>(array), length);
    }
}

void Eeprom_ReadBlock(uns8* array, uns16 address, const uns8 length)
{
    if (address + length <= EEPROM_SIZE) {
        g_Eeprom.seekg(address);
        g_Eeprom.read(reinterpret_cast<char*>(array), length);
    }
}
