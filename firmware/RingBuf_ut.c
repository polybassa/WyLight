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

#include "RingBuf.h"
#include "unittest.h"
#include <stdbool.h>

#define NUM_TEST_LOOPS 255

int ut_RingBuf_Init(void)
{
	TestCaseBegin();
	struct RingBuffer testBuffer;

	RingBuf_Init(&testBuffer);
	CHECK(testBuffer.read == 0);
	CHECK(testBuffer.write == 0);
	CHECK(testBuffer.error_full == 0);
	CHECK(RingBuf_IsEmpty(&testBuffer));
	CHECK(!RingBuf_HasError(&testBuffer));

	TestCaseEnd();
}

int ut_RingBuf_PutGet(void)
{
	TestCaseBegin();
	struct RingBuffer testBuffer;
	uns8 data = 0;

	RingBuf_Init(&testBuffer);

	uns8 i;
	for(i = 0; i < NUM_TEST_LOOPS; i++) {
		for(data = 0; data < RingBufferSize; data++) {
			RingBuf_Put(&testBuffer, data);
			CHECK(!RingBuf_IsEmpty(&testBuffer));
			CHECK(!RingBuf_HasError(&testBuffer));
		}

		/* buffer should be full now */
		RingBuf_Put(&testBuffer, data);
			CHECK(!RingBuf_IsEmpty(&testBuffer));
			CHECK(RingBuf_HasError(&testBuffer));

		/* read all data back */
		for(data = 0; data < RingBufferSize - 1; data++) {
			CHECK(data == RingBuf_Get(&testBuffer));
			CHECK(!RingBuf_IsEmpty(&testBuffer));
			CHECK(!RingBuf_HasError(&testBuffer));
		}

		/* read last byte from buffer, should be empty then */
			CHECK(RingBufferSize - 1 == RingBuf_Get(&testBuffer));
			CHECK(RingBuf_IsEmpty(&testBuffer));
			CHECK(!RingBuf_HasError(&testBuffer));
	}
	TestCaseEnd();
}

int main(int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_RingBuf_Init);
	RunTest(true, ut_RingBuf_PutGet);
	UnitTestMainEnd();
}

