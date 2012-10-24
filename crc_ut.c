/**
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
#include "trace.h"
#include "unittest.h"

#include "crc.h"

#include "stdio.h"

/* test crc commands */
int ut_Crc_AddCrc(void)
{
	int errors = 0;
	unsigned char crcHigh = 0xff;
	unsigned char crcLow = 0xff;
	Crc_AddCrc('h', &crcHigh, &crcLow);
	assert(0x0c == crcHigh);
	assert(0x5e == crcLow);

	crcHigh = crcLow = 0xff;
	Crc_AddCrc('7', &crcHigh, &crcLow);
	assert(0xa7 == crcHigh);
	assert(0x44 == crcLow);
	return errors;
}

int ut_Crc_BuildCrc(void)
{
	static const char* testString1 = "Huhu unittest crc me!";
	static const char* testString2 = "abc";
	int errors = 0;
	unsigned char crcHigh = 0xff;
	unsigned char crcLow = 0xff;

	Crc_BuildCrc((unsigned char*)testString1, 21,  &crcHigh, &crcLow);
	Trace_Hex(crcHigh);
	Trace_Hex(crcLow);
	assert(0x84 == crcHigh);
	assert(0xb8 == crcLow);

	crcHigh =  0xff;
	crcLow = 0xff;
	Crc_BuildCrc((unsigned char*)testString2, 3, &crcHigh, &crcLow);
	Trace_Hex(crcHigh);
	Trace_Hex(crcLow);
	assert(0x51 == crcHigh);
	assert(0x4a == crcLow);
	
	return errors;
}

int main(int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_Crc_AddCrc);
	RunTest(true, ut_Crc_BuildCrc);
	UnitTestMainEnd();
}

