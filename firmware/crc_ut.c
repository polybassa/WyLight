/*
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.

 This file is part of Wifly_Light.

 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#include "platform.h"
#include "unittest.h"
#include "crc.h"
#include <stdbool.h>

/* test crc commands */
int ut_Crc_AddCrc(void)
{
	TestCaseBegin();
	uns8 crcHigh = 0xff;
	uns8 crcLow = 0xff;
	Crc_AddCrc('h', &crcHigh, &crcLow);
	CHECK(0x0c == crcHigh);
	CHECK(0x5e == crcLow);

	crcHigh = crcLow = 0xff;
	Crc_AddCrc('7', &crcHigh, &crcLow);
	CHECK(0xa7 == crcHigh);
	CHECK(0x44 == crcLow);
	TestCaseEnd();
}

int ut_Crc_BuildCrc(void)
{
	TestCaseBegin();
	static const char *testString1 = "Huhu unittest crc me!";
	static const char *testString2 = "abc";
	uns8 crcHigh = 0xff;
	uns8 crcLow = 0xff;

	Crc_BuildCrc((uns8 *)testString1, 21, &crcHigh, &crcLow);
	Trace_Hex(crcHigh);
	Trace_Hex(crcLow);
	CHECK(0x84 == crcHigh);
	CHECK(0xb8 == crcLow);

	crcHigh = 0xff;
	crcLow = 0xff;
	Crc_BuildCrc((uns8 *)testString2, 3, &crcHigh, &crcLow);
	Trace_Hex(crcHigh);
	Trace_Hex(crcLow);
	CHECK(0x51 == crcHigh);
	CHECK(0x4a == crcLow);
	TestCaseEnd();
}

int main(int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_Crc_AddCrc);
	RunTest(true, ut_Crc_BuildCrc);
	UnitTestMainEnd();
}

