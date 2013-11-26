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

#include "CommandIO.h"
#include "unittest.h"
#include "crc.c"
#include "crc.h"
#include "RingBuf.c"
#include "RingBuf.h"
#include "error.h"
#include "wifly_cmd.h"
#include "VersionFile.h"
#include "Version.c"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define NUM_TEST_LOOPS 255

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO;

struct RingBuffer g_RingBufResponse;
uns8 g_RandomDataPool[500];

const uns8 dummyFrame_noSTX[] = { 0xff, DLE, ETX, 0x00, 0x14, 0xAF, ETX, 0x05, 0x04, 0x00 };
const uns8 dummyFrame_completeFrame[] = { DLE, ETX, STX, 0x01, 0x02, 0x03, DLE, 0x04, DLE, 0x05, ETX };
const uns8 dummyFrame_completeFramePure[] = { 0x01, 0x02, 0x03, 0x04,0x05 };
const uns8 dummyFrame_twoFrames_1[] = { STX, STX, STX, DLE, DLE, DLE, ETX, DLE, STX, DLE, ETX, ETX, DLE, STX, DLE, DLE, DLE, STX, ETX };
const uns8 dummyFrame_twoFrames_2[] = { STX, STX, STX, DLE, DLE, DLE, ETX, DLE, STX, DLE, ETX, ETX, ETX, STX, STX, DLE, DLE, DLE, STX, ETX };
const uns8 dummyFrame_twoFramesPure_1[] = { DLE, ETX, STX, ETX };
const uns8 dummyFrame_twoFramesPure_2[] = { DLE, STX };

/* X86 function wrappers */
void UART_Init() {}
void UART_Send(const uns8 ch)
{
	//printf("%x", ch);
	RingBuf_Put(&g_RingBufResponse, ch);
}
void UART_SendString(const uns8 *string)
{
	printf("%s", string);
}
uns8 ScriptCtrl_Add(struct led_cmd *pCmd)
{
	return 1;
}

void Rtc_Ctl(enum RTC_request req,struct rtc_time *pRtcTime)
{
	int i;
	uns8 *pData = (uns8 *)pRtcTime;
	for(i = 0; i < sizeof(struct rtc_time); i++) {
		*pData++ = g_RandomDataPool[i];
	}
}

uns8 Timer_PrintCycletime(uns16 *pArray, uns16 arraySize)
{
	int i;
	uns8 *pData = (uns8 *)pArray;
	for(i = 0; i < arraySize; i++) {
		*pData++ = g_RandomDataPool[i];
	}
	return i;

}

uns8 Trace_Print(uns8 *pArray, uns16 arraySize)
{
	int i;
	uns8 *pData = (uns8 *)pArray;
	for(i = 0; i < arraySize; i++) {
		*pData++ = g_RandomDataPool[i];
	}
	return i;
}

/*--------------- TEST FUNCTIONS----------------*/


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

int ut_CommandIO_FW_IDENT(void)
{
	TestCaseBegin();

	RingBuf_Init(&g_RingBufResponse);
	RingBuf_Init(&g_RingBuf);
	CommandIO_Init();

	CHECK(0 == g_CmdBuf.counter);
	CHECK(CS_WaitForSTX == g_CmdBuf.state);
	CHECK(0 == g_CmdBuf.CrcL);
	CHECK(0 == g_CmdBuf.CrcH);

	int i;
	for(i = 0; i < sizeof(dummyFrame_twoFrames_2); i++) {
		CommandIO_GetCommands();
		RingBuf_Put(&g_RingBuf, dummyFrame_twoFrames_2[i]);
		CommandIO_GetCommands();
	}

	uint8_t identCount = 0;
	while(!RingBuf_IsEmpty(&g_RingBufResponse)) {
		if(RingBuf_Get(&g_RingBufResponse) == FW_IDENT) {
			identCount++;
		}
	}

	CHECK(identCount == 5);

	TestCaseEnd();
}


int ut_CommandIO_WaitForSTX(void)
{
	TestCaseBegin();

	CommandIO_Init();

	int i;

	for(i = 0; i < sizeof(dummyFrame_noSTX); i++) {
		RingBuf_Put(&g_RingBuf, dummyFrame_noSTX[i]);
	}

	for(i = 0; i < sizeof(dummyFrame_noSTX) + 1; i++) {
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

	for(i = 0; i < sizeof(dummyFrame_completeFrame); i++) {
		RingBuf_Put(&g_RingBuf, dummyFrame_completeFrame[i]);
	}

	for(i = 0; i < sizeof(dummyFrame_completeFrame) + 1; i++) {
		CommandIO_GetCommands();
	}
	CHECK(0 == memcmp(dummyFrame_completeFramePure, g_CmdBuf.buffer, g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);

	TestCaseEnd();
}

int ut_CommandIO_ReadTwoFrames_1(void)
{
	TestCaseBegin();

	CommandIO_Init();

	int i;

	for(i = 0; i < sizeof(dummyFrame_twoFrames_1); i++) {
		RingBuf_Put(&g_RingBuf, dummyFrame_twoFrames_1[i]);
	}

	for(i = 0; i < sizeof(dummyFrame_twoFrames_1) - 1; i++) {
		CommandIO_GetCommands();
	}
		CommandIO_GetCommands();
		CommandIO_GetCommands();
		CommandIO_GetCommands();

	CHECK(0 == memcmp(dummyFrame_twoFramesPure_2, g_CmdBuf.buffer, g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);

	TestCaseEnd();
}

int ut_CommandIO_ReadTwoFrames_2(void)
{
	TestCaseBegin();

	CommandIO_Init();

	int i;

	for(i = 0; i < sizeof(dummyFrame_twoFrames_2); i++) {
		RingBuf_Put(&g_RingBuf, dummyFrame_twoFrames_2[i]);
	}

	for(i = 0; i < sizeof(dummyFrame_twoFrames_2) - 1; i++) {
		CommandIO_GetCommands();

	}
		CommandIO_GetCommands();
		CommandIO_GetCommands();
		CommandIO_GetCommands();

	CHECK(0 == memcmp(dummyFrame_twoFramesPure_2, g_CmdBuf.buffer, g_CmdBuf.counter));
	CHECK(CS_WaitForSTX == g_CmdBuf.state);

	TestCaseEnd();
}

int ut_CommandIO_SendResponse(void)
{
	TestCaseBegin();

	/* init random seed */
	srand(time(NULL));

	/* generate a default frame with maximal length of commandbuffer */
	struct response_frame mFrame;
	if(CMDFRAMELENGTH >= 250) {
		mFrame.length = 250;
	} else mFrame.length = CMDFRAMELENGTH - 2;
	/* get a pointer to start of payload */
	uns8 *pPayload;
	pPayload = (uns8 *)&(mFrame.cmd);

	/* fill payload with random data */
	int i;
	for(i = 0; i < mFrame.length - 2; i++, pPayload++) {
		*pPayload = (uns8) rand() % 255;
	}

#ifdef DEBUG
	/* to find out which randomData kills the test */
	const struct response_frame randomData = mFrame;
#endif

	/* init everything, g_RingBufResponse is used to save all send data from UART_send */
	RingBuf_Init(&g_RingBufResponse);
	CommandIO_Init();
	RingBuf_Init(&g_RingBuf);

	/* send data */
	CommandIO_SendResponse(&mFrame);

	/* swap data from one ringbuf to the other, to scan data with CommandIO_GetCommands */
	while(RingBuf_IsEmpty(&g_RingBufResponse) == 0)
	{
		RingBuf_Put(&g_RingBuf, RingBuf_Get(&g_RingBufResponse));
	}

	for(i = 0; i < mFrame.length * 2; i++) {
		CommandIO_GetCommands();
	}
#ifdef DEBUG
	uns8 *pFrame = (uns8 *)&mFrame;
	uns8 *pCmd = (uns8 *)&g_CmdBuf.buffer;
	for(i = 0; i < mFrame.length; i++) {
		printf(" %#x == %#x \n", *pFrame++, *pCmd++);
	}
#endif
	/* in the inputbuffer of g_CmdBuf must be the same data as in mFrame now */
	const bool success = (0 == memcmp((void *)&mFrame, (void *)&g_CmdBuf.buffer, mFrame.length));
	if(!success) TraceBuffer(ZONE_INFO, (uint8_t *)&randomData, sizeof(randomData), "%02x ", "this random data killed the test:\n");
	CHECK(success);

	TestCaseEnd();
}

int ut_CommandIO_CreateResponse_RTC(void)
{
	TestCaseBegin();
	/* init random seed */
	srand(time(NULL));

	/* fill gloabal random data pool */
	int i;
	for(i = 0; i < sizeof(g_RandomDataPool); i++) g_RandomDataPool[i] = (uns8) rand() % 255;

	struct response_frame mFrame;

	CommandIO_CreateResponse(&mFrame, GET_RTC, OK);
	CHECK(0 == memcmp((void *)&(mFrame.data), (void *)&g_RandomDataPool[0], sizeof(struct rtc_time)));
	CHECK(mFrame.cmd == GET_RTC);
	CHECK(mFrame.length == sizeof(struct rtc_time) + sizeof(uns8) * 2 + sizeof(uns16));
	CHECK(mFrame.state == OK);

	TestCaseEnd();
}

int ut_CommandIO_CreateResponse_CYCLETIME(void)
{
	TestCaseBegin();
	/* init random seed */
	srand(time(NULL));

	/* fill gloabal random data pool */
	int i;
	for(i = 0; i < sizeof(g_RandomDataPool); i++) g_RandomDataPool[i] = (uns8) rand() % 255;

	struct response_frame mFrame;

	CommandIO_CreateResponse(&mFrame, GET_CYCLETIME, OK);
	CHECK(0 == memcmp((void *)&(mFrame.data), (void *)&g_RandomDataPool[0], sizeof(struct response_frame) - 4));
	CHECK(mFrame.cmd == GET_CYCLETIME);

	CHECK(mFrame.length == sizeof(struct response_frame));
	CHECK(mFrame.state == OK);

	TestCaseEnd();
}

int ut_CommandIO_CreateResponse_TRACE(void)
{
	TestCaseBegin();
	/* init random seed */
	srand(time(NULL));

	/* fill gloabal random data pool */
	int i;
	for(i = 0; i < sizeof(g_RandomDataPool); i++) g_RandomDataPool[i] = (uns8) rand() % 255;

	struct response_frame mFrame;

	CommandIO_CreateResponse(&mFrame, GET_TRACE, OK);
	CHECK(0 == memcmp((void *)&(mFrame.data), (void *)&g_RandomDataPool[0], sizeof(struct response_frame) - 4));
	CHECK(mFrame.cmd == GET_TRACE);

	CHECK(mFrame.length == sizeof(struct response_frame));
	CHECK(mFrame.state == OK);

	TestCaseEnd();
}

int ut_CommandIO_CreateResponse_FW_VERSION(void)
{
	TestCaseBegin();
	struct response_frame mFrame;

	const uint16_t g_Version = VERSION;
	const uint16_t g_RightVersion = ntohs(g_Version);

	CommandIO_CreateResponse(&mFrame, GET_FW_VERSION, OK);
	CHECK(0 == memcmp((void *)&(mFrame.data), (void *)&g_RightVersion, sizeof(uns16)));
	CHECK(mFrame.cmd == GET_FW_VERSION);
	CHECK(mFrame.length == 4 + sizeof(uns16));
	CHECK(mFrame.state == OK);

	TestCaseEnd();
}

int ut_CommandIO_CreateResponse_SET_FADE(void)
{
	TestCaseBegin();
	struct response_frame mFrame;

	CommandIO_CreateResponse(&mFrame, SET_FADE, OK);
	CHECK(mFrame.cmd == SET_FADE);
	CHECK(mFrame.length == 4);
	CHECK(mFrame.state == OK);

	TestCaseEnd();
}

int ut_CommandIO_Create_n_Send(void)
{
	TestCaseBegin();
	/* init random seed */
	srand(time(NULL));

	/* fill gloabal random data pool */
	int i;
	for(i = 0; i < sizeof(g_RandomDataPool); i++) g_RandomDataPool[i] = (uns8) rand() % 255;

	struct response_frame mFrame;

	CommandIO_CreateResponse(&mFrame, GET_RTC, OK);

	/* init everything, g_RingBufResponse is used to save all send data from UART_send */
	RingBuf_Init(&g_RingBufResponse);
	CommandIO_Init();
	RingBuf_Init(&g_RingBuf);

	/* send data */
	CommandIO_SendResponse(&mFrame);

	/* swap data from one ringbuf to the other, to scan data with CommandIO_GetCommands */
	while(RingBuf_IsEmpty(&g_RingBufResponse) == 0)
	{
		RingBuf_Put(&g_RingBuf, RingBuf_Get(&g_RingBufResponse));
	}

	for(i = 0; i < 100; i++) {
		CommandIO_GetCommands();
	}

	struct response_frame *rFrame;

	rFrame = (void *)&g_CmdBuf.buffer;

	CHECK(0 == memcmp((void *)&(rFrame->data), (void *)&g_RandomDataPool[0], sizeof(struct rtc_time)));
	CHECK(rFrame->cmd == GET_RTC);
	CHECK(rFrame->length == sizeof(struct rtc_time) + sizeof(uns8) * 2 + sizeof(uns16));
	CHECK(rFrame->state == OK);


	TestCaseEnd();
}


int main(int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_CommandIO_Init);
	RunTest(true, ut_CommandIO_FW_IDENT);
	RunTest(true, ut_CommandIO_WaitForSTX);
	RunTest(true, ut_CommandIO_ReadCompleteFrame);
	RunTest(true, ut_CommandIO_ReadTwoFrames_1);
	RunTest(true, ut_CommandIO_ReadTwoFrames_2);
	RunTest(true, ut_CommandIO_SendResponse);
	RunTest(true, ut_CommandIO_CreateResponse_RTC);
	RunTest(true, ut_CommandIO_CreateResponse_CYCLETIME);
	RunTest(true, ut_CommandIO_CreateResponse_TRACE);
	RunTest(true, ut_CommandIO_CreateResponse_FW_VERSION);
	RunTest(true, ut_CommandIO_CreateResponse_SET_FADE);
	RunTest(true, ut_CommandIO_Create_n_Send);
	UnitTestMainEnd();
}

