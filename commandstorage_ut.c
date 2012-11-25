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


#include "commandstorage.h"

#ifndef _old_commandstorage_

#include "unittest.h"
#include "platform.h"

#include "crc.c"
#include "RingBuf.c"

#define NUM_TEST_LOOPS 255

struct ErrorBits g_ErrorBits;

const unsigned char dummyFrame_noSTX[] = { 0xff, DLE, ETX, 0x00, 0x14, 0xAF, ETX, 0x05, 0x04, 0x00 };
const unsigned char dummyFrame_completeFrame[] = { DLE, ETX, STX, 0x01, 0x02, 0x03, DLE, 0x04, DLE, 0x05, ETX };
const unsigned char dummyFrame_completeFramePure[] = { 0x01, 0x02, 0x03, 0x04,0x05 };
const unsigned char dummyFrame_twoFrames_1[] = { STX, STX, STX, DLE, DLE, DLE, ETX, DLE, STX, DLE, ETX, ETX, DLE, STX, DLE, DLE, DLE, STX, ETX };
const unsigned char dummyFrame_twoFrames_2[] = { STX, STX, STX, DLE, DLE, DLE, ETX, DLE, STX, DLE, ETX, ETX, STX, DLE, DLE, DLE, STX, ETX };
const unsigned char dummyFrame_twoFramesPure_1[] = { DLE, ETX, STX, ETX };
const unsigned char dummyFrame_twoFramesPure_2[] = { DLE, STX };

int ut_Commandstorage_Init(void)
{
	TestCaseBegin();
	
	Commandstorage_Init();
	
	CHECK(0 == g_CmdBuf.counter);
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	CHECK(0xff == g_CmdBuf.CrcL);
	CHECK(0xff == g_CmdBuf.CrcH);
	
	TestCaseEnd();
}

int ut_Commandstorage_WaitForSTX(void)
{
	TestCaseBegin();
	
	Commandstorage_Init();
	
	int i;
	
	for(i = 0; i < sizeof(dummyFrame_noSTX); i++)
	{
	  RingBuf_Put(&g_RingBuf, dummyFrame_noSTX[i]);
	}
	
	for(i = 0; i < sizeof(dummyFrame_noSTX) + 1; i++)
	{
	  Commandstorage_GetCommands();
	  CHECK(0 == g_CmdBuf.counter);
	  CHECK(CS_WaitForSTX == g_CmdBuf.state);
	  CHECK(0xff == g_CmdBuf.CrcL);
	  CHECK(0xff == g_CmdBuf.CrcH);
	}
	TestCaseEnd();
}

int ut_Commandstorage_ReadCompleteFrame(void)
{
	TestCaseBegin();
	
	Commandstorage_Init();
	
	int i;
	
	for(i = 0; i < sizeof(dummyFrame_completeFrame); i++)
	{
	  RingBuf_Put(&g_RingBuf, dummyFrame_completeFrame[i]);
	}
	
	for(i = 0; i < sizeof(dummyFrame_completeFrame) + 1; i++)
	{
	  Commandstorage_GetCommands();
	}
	CHECK(0 == memcmp(dummyFrame_completeFramePure, g_CmdBuf.buffer , g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	
	TestCaseEnd();
}

int ut_Commandstorage_ReadTwoFrames_1(void)
{
	TestCaseBegin();
	
	Commandstorage_Init();
	
	int i;
	
	for(i = 0; i < sizeof(dummyFrame_twoFrames_1); i++)
	{
	  RingBuf_Put(&g_RingBuf, dummyFrame_twoFrames_1[i]);
	}
	
	for(i = 0; i < sizeof(dummyFrame_twoFrames_1) - 1; i++)
	{
	  Commandstorage_GetCommands();
	  
	  if(g_ErrorBits.CrcFailure == 1)
	  {
	      g_ErrorBits.CrcFailure = 0;
	      CHECK(0 == memcmp(dummyFrame_twoFramesPure_1, g_CmdBuf.buffer , g_CmdBuf.counter));
	      CHECK(CS_WaitForSTX == g_CmdBuf.state);
	  }
	}
	Commandstorage_GetCommands();
	Commandstorage_GetCommands();
	Commandstorage_GetCommands();
	
	CHECK(0 == memcmp(dummyFrame_twoFramesPure_2, g_CmdBuf.buffer , g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	
	TestCaseEnd();
}

int ut_Commandstorage_ReadTwoFrames_2(void)
{
	TestCaseBegin();
	
	Commandstorage_Init();
	
	int i;
	
	for(i = 0; i < sizeof(dummyFrame_twoFrames_2); i++)
	{
	  RingBuf_Put(&g_RingBuf, dummyFrame_twoFrames_2[i]);
	}
	
	for(i = 0; i < sizeof(dummyFrame_twoFrames_2) - 1; i++)
	{
	  Commandstorage_GetCommands();
	  
	  if(g_ErrorBits.CrcFailure == 1)
	  {
	      g_ErrorBits.CrcFailure = 0;
	      CHECK(0 == memcmp(dummyFrame_twoFramesPure_1, g_CmdBuf.buffer , g_CmdBuf.counter));
	      CHECK(CS_WaitForSTX == g_CmdBuf.state);
	  }
	}
	Commandstorage_GetCommands();
	Commandstorage_GetCommands();
	Commandstorage_GetCommands();
	
	CHECK(0 == memcmp(dummyFrame_twoFramesPure_2, g_CmdBuf.buffer , g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	
	TestCaseEnd();
}

int main(int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_Commandstorage_Init);
	RunTest(true, ut_Commandstorage_WaitForSTX);
	RunTest(true, ut_Commandstorage_ReadCompleteFrame);
	RunTest(true, ut_Commandstorage_ReadTwoFrames_1);
	RunTest(true, ut_Commandstorage_ReadTwoFrames_2);
	UnitTestMainEnd();
}

#endif /* _old_commandstorage_ */
