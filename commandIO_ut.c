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

#include "commandIO.h"
#include "unittest.h"
#include "crc.c"
#include "crc.h"
#include "RingBuf.c"
#include "RingBuf.h"
#include "error.h"
#include "wifly_cmd.h"

#define NUM_TEST_LOOPS 255

struct ErrorBits g_ErrorBits;

const uns8 dummyFrame_noSTX[] = { 0xff, DLE, ETX, 0x00, 0x14, 0xAF, ETX, 0x05, 0x04, 0x00 };
const uns8 dummyFrame_completeFrame[] = { DLE, ETX, STX, 0x01, 0x02, 0x03, DLE, 0x04, DLE, 0x05, ETX };
const uns8 dummyFrame_completeFramePure[] = { 0x01, 0x02, 0x03, 0x04,0x05 };
const uns8 dummyFrame_twoFrames_1[] = { STX, STX, STX, DLE, DLE, DLE, ETX, DLE, STX, DLE, ETX, ETX, DLE, STX, DLE, DLE, DLE, STX, ETX };
const uns8 dummyFrame_twoFrames_2[] = { STX, STX, STX, DLE, DLE, DLE, ETX, DLE, STX, DLE, ETX, ETX, STX, DLE, DLE, DLE, STX, ETX };
const uns8 dummyFrame_twoFramesPure_1[] = { DLE, ETX, STX, ETX };
const uns8 dummyFrame_twoFramesPure_2[] = { DLE, STX };

/* X86 function wrappers */
void UART_Init() {}
void UART_Send(const uns8 ch)
{
	printf("%c", ch);
}
void UART_SendString(const uns8 *string)
{
	printf("%s", string);
}
uns8 ScriptCtrl_Add(struct led_cmd* pCmd)
{
	return 1;
}


int ut_CommandIO_Init(void)
{
	TestCaseBegin();
	
	CommandIO_Init();
	
	CHECK(0 == g_CmdBuf.counter);
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	CHECK(0 == g_CmdBuf.CrcL);
	CHECK(0 == g_CmdBuf.CrcH);
	
	TestCaseEnd();
}

int ut_CommandIO_WaitForSTX(void)
{
	TestCaseBegin();
	
	CommandIO_Init();
	
	int i;
	
	for(i = 0; i < sizeof(dummyFrame_noSTX); i++)
	{
	  RingBuf_Put(&g_RingBuf, dummyFrame_noSTX[i]);
	}
	
	for(i = 0; i < sizeof(dummyFrame_noSTX) + 1; i++)
	{
	  CommandIO_GetCommands();
	  CHECK(0 == g_CmdBuf.counter);
	  CHECK(CS_WaitForSTX == g_CmdBuf.state);
	  CHECK(0 == g_CmdBuf.CrcL);
	  CHECK(0 == g_CmdBuf.CrcH);
	}
	TestCaseEnd();
}

int ut_CommandIO_ReadCompleteFrame(void)
{
	TestCaseBegin();
	
	CommandIO_Init();
	
	int i;
	
	for(i = 0; i < sizeof(dummyFrame_completeFrame); i++)
	{
	  RingBuf_Put(&g_RingBuf, dummyFrame_completeFrame[i]);
	}
	
	for(i = 0; i < sizeof(dummyFrame_completeFrame) + 1; i++)
	{
	  CommandIO_GetCommands();
	}
	CHECK(0 == memcmp(dummyFrame_completeFramePure, g_CmdBuf.buffer , g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	
	TestCaseEnd();
}

int ut_CommandIO_ReadTwoFrames_1(void)
{
	TestCaseBegin();
	
	CommandIO_Init();
	
	int i;
	
	for(i = 0; i < sizeof(dummyFrame_twoFrames_1); i++)
	{
	  RingBuf_Put(&g_RingBuf, dummyFrame_twoFrames_1[i]);
	}
	
	for(i = 0; i < sizeof(dummyFrame_twoFrames_1) - 1; i++)
	{
	  CommandIO_GetCommands();
	  
	  if(g_ErrorBits.CrcFailure == 1)
	  {
	      g_ErrorBits.CrcFailure = 0;
	      CHECK(0 == memcmp(dummyFrame_twoFramesPure_1, g_CmdBuf.buffer , g_CmdBuf.counter));
	      CHECK(CS_WaitForSTX == g_CmdBuf.state);
	  }
	}
	CommandIO_GetCommands();
	CommandIO_GetCommands();
	CommandIO_GetCommands();
	
	CHECK(0 == memcmp(dummyFrame_twoFramesPure_2, g_CmdBuf.buffer , g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	
	TestCaseEnd();
}

int ut_CommandIO_ReadTwoFrames_2(void)
{
	TestCaseBegin();
	
	CommandIO_Init();
	
	int i;
	
	for(i = 0; i < sizeof(dummyFrame_twoFrames_2); i++)
	{
	  RingBuf_Put(&g_RingBuf, dummyFrame_twoFrames_2[i]);
	}
	
	for(i = 0; i < sizeof(dummyFrame_twoFrames_2) - 1; i++)
	{
	  CommandIO_GetCommands();
	  
	  if(g_ErrorBits.CrcFailure == 1)
	  {
	      g_ErrorBits.CrcFailure = 0;
	      CHECK(0 == memcmp(dummyFrame_twoFramesPure_1, g_CmdBuf.buffer , g_CmdBuf.counter));
	      CHECK(CS_WaitForSTX == g_CmdBuf.state);
	  }
	}
	CommandIO_GetCommands();
	CommandIO_GetCommands();
	CommandIO_GetCommands();
	
	CHECK(0 == memcmp(dummyFrame_twoFramesPure_2, g_CmdBuf.buffer , g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	
	TestCaseEnd();
}

int main(int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_CommandIO_Init);
	RunTest(true, ut_CommandIO_WaitForSTX);
	RunTest(true, ut_CommandIO_ReadCompleteFrame);
	RunTest(true, ut_CommandIO_ReadTwoFrames_1);
	RunTest(true, ut_CommandIO_ReadTwoFrames_2);
	UnitTestMainEnd();
}

