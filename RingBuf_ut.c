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

#include "RingBuf.h"
#include "unittest.h"


/* test WAIT command */
int ut_RingBuf_Init(void)
{
	int errors = 0;
	struct RingBuffer testBuffer;
	
	RingBuf_Init(&testBuffer);
	
	assert(testBuffer.read == 0);
	assert(testBuffer.write == 0);
	assert(testBuffer.error_full == 0);
	return errors;
}

int main(int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_RingBuf_Init);
	UnitTestMainEnd();
}

