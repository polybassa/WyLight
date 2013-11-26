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

#include "ledstrip.h"
#include "unittest.h"
#include <stdbool.h>

#define NUM_TEST_LOOPS 255


/** Dummies and wrapper functions */
#include "ScriptCtrl.h"
void SPI_Init(void) {};
void SPI_SendLedBuffer(void) {};
struct ScriptBuf gScriptBuf;

int ut_Ledstrip_Init(void)
{
	TestCaseBegin();
	const char *pCur = (const char *)(&gLedBuf);
	const char *const pEnd = pCur + sizeof(gLedBuf);
	Ledstrip_Init();
	while(pCur < pEnd)
	{
		CHECK(0 == *pCur);
		pCur++;
	}
	TestCaseEnd();
}
/* TODO Refactor UNIT_TEST to SetFade *
int ut_Ledstrip_SetColor(void)
{
        TestCaseBegin();
        size_t i;
        const uns8 red = 0xA5;
        const uns8 green = 0x5A;
        const uns8 blue = 3;
        struct cmd_set_color testCmd = {{0xff, 0xff, 0xff, 0xff}, red, green, blue};
        Ledstrip_SetColor(&testCmd);
        for(i = 0; i < NUM_OF_LED;)
        {
                // colors are in reverse BGR because we shift the last byte as first to the led strip.
                CHECK(blue == (gLedBuf.led_array[i++]));
                CHECK(green == (gLedBuf.led_array[i++]));
                CHECK(red == (gLedBuf.led_array[i++]));
        }
        TestCaseEnd();
}
*/
int ut_Ledstrip_SetColorDirect(void)
{
	TestCaseBegin();
	size_t i;
	uns8 testCmd[NUM_OF_LED * 3];
	for(i = 0; i < sizeof(testCmd); i++) {
		testCmd[i] = i;
	}

	Ledstrip_SetColorDirect(testCmd);
	for(i = 0; i < NUM_OF_LED * 3; i += 3) {
		// led_array is ordered brg not rgb!
		CHECK(i == (gLedBuf.led_array[i + 2]));
		CHECK(i + 1 == (gLedBuf.led_array[i + 1]));
		CHECK(i + 2 == (gLedBuf.led_array[i]));
	}
	TestCaseEnd();
}

int main(int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_Ledstrip_Init);
	//RunTest(true, ut_Ledstrip_SetColor);
	RunTest(true, ut_Ledstrip_SetColorDirect);
	UnitTestMainEnd();
}

