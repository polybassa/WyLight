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

#include <array>
#include "unittest.h"
#include "eeprom.h"

static const std::array<char, EEPROM_SIZE> empty {};
static const uns8 block_size = 255;

int ut_Eeprom_Init(void)
{
    std::array<uns8, EEPROM_SIZE> buffer {};
    std::fill(buffer.begin(), buffer.end(), 0xAA);
    uns8* pPos = buffer.data();
    std::remove("eeprom.bin");
    TestCaseBegin();
    Eeprom_Init();
    while (pPos + block_size < buffer.data() + buffer.size()) {
        Eeprom_ReadBlock(pPos, pPos - buffer.data(), block_size);
        CHECK_MEMCMP(pPos, empty.data(), block_size);
    }
    const auto remaining = buffer.data() + buffer.size() - pPos;
    Eeprom_ReadBlock(pPos, 0, remaining);
    CHECK_MEMCMP(pPos, empty.data(), remaining);
    TestCaseEnd();
}

int ut_Eeprom_ReadBlock(void)
{
    std::array<uns8, EEPROM_SIZE> buffer {0xAA};
    uns8* pPos = buffer.data();
    TestCaseBegin();
    Eeprom_ReadBlock(pPos, EEPROM_SIZE, 0);
    CHECK(*pPos == 0xAA);
    Eeprom_ReadBlock(pPos, EEPROM_SIZE, 1);
    CHECK(*pPos == 0xAA);
    Eeprom_ReadBlock(pPos, EEPROM_SIZE, block_size);
    CHECK(*pPos == 0xAA);
    Eeprom_ReadBlock(pPos, EEPROM_SIZE - 1, 1);
    CHECK(*pPos == 0);
    TestCaseEnd();
}

int ut_Eeprom_ReadWrite(void)
{
    TestCaseBegin();

    Eeprom_Write(EEPROM_SIZE + 1, 0xAA);
    CHECK(!Eeprom_Read(EEPROM_SIZE + 1));
    Eeprom_Write(EEPROM_SIZE + 1, 0x55);
    CHECK(!Eeprom_Read(EEPROM_SIZE + 1));

    Eeprom_Write(EEPROM_SIZE, 0xAA);
    CHECK(!Eeprom_Read(EEPROM_SIZE));
    Eeprom_Write(EEPROM_SIZE, 0x55);
    CHECK(!Eeprom_Read(EEPROM_SIZE));

    Eeprom_Write(EEPROM_SIZE - 1, 0xAA);
    CHECK(0xAA == Eeprom_Read(EEPROM_SIZE - 1));
    Eeprom_Write(EEPROM_SIZE - 1, 0x55);
    CHECK(0x55 == Eeprom_Read(EEPROM_SIZE - 1));
    Eeprom_Write(EEPROM_SIZE - 1, 0x00);
    CHECK(0x00 == Eeprom_Read(EEPROM_SIZE - 1));
    TestCaseEnd();
}

int ut_Eeprom_WriteBlock(void)
{
    std::array<uns8, EEPROM_SIZE> read_buffer {};
    std::array<uns8, EEPROM_SIZE> buffer {};
    std::fill(buffer.begin(), buffer.end(), 0xAA);
    uns8* pPos = buffer.data();

    TestCaseBegin();
    Eeprom_WriteBlock(buffer.data(), EEPROM_SIZE + 1 - block_size, block_size);
    CHECK(0x00 == Eeprom_Read(EEPROM_SIZE - 1));

    while (pPos + block_size < buffer.data() + buffer.size()) {
        Eeprom_WriteBlock(pPos, pPos - buffer.data(), block_size);
        Eeprom_ReadBlock(read_buffer.data(), pPos - buffer.data(), block_size);
        CHECK_MEMCMP(pPos, read_buffer.data(), block_size);
    }
    const auto remaining = buffer.data() + buffer.size() - pPos;
    Eeprom_WriteBlock(pPos, pPos - buffer.data(), remaining);
    Eeprom_ReadBlock(read_buffer.data(), pPos - buffer.data(), remaining);
    CHECK_MEMCMP(pPos, read_buffer.data(), remaining);
    TestCaseEnd();
}

int main(int argc, const char* argv[])
{
    UnitTestMainBegin();
    RunTest(true, ut_Eeprom_Init);
    RunTest(true, ut_Eeprom_ReadBlock);
    RunTest(true, ut_Eeprom_ReadWrite);
    RunTest(true, ut_Eeprom_WriteBlock);
    UnitTestMainEnd();
}
