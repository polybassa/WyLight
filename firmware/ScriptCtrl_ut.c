/*
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.

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

#include "trace.h"
#include "unittest.h"
#include <stdbool.h>

#define NUM_TEST_LOOPS 255

int gSetColorDirectWasCalled;
int gSetFadeWasCalled;
int gSetGradientWasCalled;

/**************** includes and functions for wrapping ****************/
#include "ScriptCtrl.h"
jmp_buf g_ResetEnvironment;
struct ScriptBuf gScriptBuf;
struct response_frame g_ResponseBuf;

void CommandIO_CreateResponse(struct response_frame *mFrame, uns8 cmd)
{}

void CommandIO_SendResponse(struct response_frame *mFrame)
{}

void Ledstrip_SetColorDirect(struct cmd_set_fade *pCmd)
{
	Trace_String("Ledstrip_SetColorDirect was called\n");
	gSetColorDirectWasCalled = TRUE;
}

void Ledstrip_SetFade(struct cmd_set_fade *pCmd)
{
	Trace_String("Ledstrip_SetFade was called\n");
	gSetFadeWasCalled = TRUE;
}

void Ledstrip_SetGradient(struct cmd_set_fade *pCmd)
{
	Trace_String("Ledstrip_SetGradient was called\n");
	gSetGradientWasCalled = TRUE;
}

/******************************* test functions *******************************/
/* add simple command and read back */
int ut_ScriptCtrl_SimpleReadWrite(void)
{
	TestCaseBegin();
	struct led_cmd testCmd;
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Clear();

	/* read from empty script buffer */
	gSetFadeWasCalled = FALSE;
	ScriptCtrl_Run();

	/* add simple command and read back */
	ScriptCtrl_Add(&testCmd);
	gSetFadeWasCalled = FALSE;
	ScriptCtrl_Run();
	CHECK(gSetFadeWasCalled);

	/* buffer should be empty again */
	gSetFadeWasCalled = FALSE;
	ScriptCtrl_Run();
	CHECK(!gSetFadeWasCalled);
	TestCaseEnd();
}

/* add clear command */
int ut_ScriptCtrl_Clear(void)
{
	TestCaseBegin();
	struct led_cmd testCmd;
	testCmd.cmd = SET_FADE;

	/* add dummy command to buffer */
	ScriptCtrl_Add(&testCmd);
	gSetFadeWasCalled = FALSE;

	/* send CLEAR_SCRIPT */
	testCmd.cmd = CLEAR_SCRIPT;
	ScriptCtrl_Add(&testCmd);

	/* buffer should be empty again */
	ScriptCtrl_Run();
	CHECK(!gSetFadeWasCalled);
	TestCaseEnd();
}

/* add simple loop */
int ut_ScriptCtrl_SimpleLoop(void)
{
	TestCaseBegin();
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* add loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add dummy command to buffer */
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Add(&testCmd);

	/* add loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* start loop should be read */
	CHECK(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	CHECK(gScriptBuf.inLoop);

	int i;
	for(i = 0; i < NUM_TEST_LOOPS; i++) {
		/* dummy command should be executed */
		gSetFadeWasCalled = FALSE;
		ScriptCtrl_Run();
		CHECK(gSetFadeWasCalled);

		/* now next loop should be called */
		gSetFadeWasCalled = FALSE;
		ScriptCtrl_Run();
		CHECK(!gSetFadeWasCalled);
	}

	/* no more command should be available */
	gSetFadeWasCalled = FALSE;
	ScriptCtrl_Run();
	CHECK(!gSetFadeWasCalled);
	TestCaseEnd();
}

int ut_ScriptCtrl_DoOuterInnerLoop(int loopCount)
{
	TestCaseBegin();
	int i, j;
	for(i = 0; i < loopCount; i++) {
		/* outer dummy command should be executed */
		gSetFadeWasCalled = FALSE;
		ScriptCtrl_Run();
		CHECK(gSetFadeWasCalled);

		/* start inner loop should be read */
		CHECK(gScriptBuf.inLoop)
		ScriptCtrl_Run();
		CHECK(gScriptBuf.inLoop);

		for(j = 0; j < loopCount; j++) {
			/* outer dummy command should be executed */
			gSetFadeWasCalled = FALSE;
			ScriptCtrl_Run();
			CHECK(gSetFadeWasCalled);

			/* now next inner loop should be called */
			gSetFadeWasCalled = FALSE;
			ScriptCtrl_Run();
			CHECK(!gSetFadeWasCalled);
		}

		/* now next outer loop should be called */
		gSetFadeWasCalled = FALSE;
		ScriptCtrl_Run();
		CHECK(!gSetFadeWasCalled);
	}
	TestCaseEnd();
}


/* add inner loop */
int ut_ScriptCtrl_InnerLoop(void)
{
	TestCaseBegin();
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add inner dummy command to buffer */
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* add outer loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* start outer loop should be read */
	CHECK(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	CHECK(gScriptBuf.inLoop);

	errors += ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);

	/* no more command should be available */
	gSetFadeWasCalled = FALSE;
	ScriptCtrl_Run();
	CHECK(!gSetFadeWasCalled);
	TestCaseEnd();
}

/* add infinite loop */
int ut_ScriptCtrl_InfiniteLoop(void)
{
	TestCaseBegin();
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add inner dummy command to buffer */
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* add outer loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = LOOP_INFINITE;
	ScriptCtrl_Add(&testCmd);

	/* start outer loop should be read */
	CHECK(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	CHECK(gScriptBuf.inLoop);

	/* multiple calls should be no problem since we are in an infinite loop */
	errors += ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);
	errors += ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);
	errors += ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);

	/* now terminate the loop */
	testCmd.cmd = CLEAR_SCRIPT;
	ScriptCtrl_Add(&testCmd);

	/* buffer should be empty again */
	gSetFadeWasCalled = FALSE;
	ScriptCtrl_Run();
	CHECK(!gSetFadeWasCalled);
	TestCaseEnd();
}

/* write to full script buffer */
/* write incomplete loop to full buffer */
int ut_ScriptCtrl_FullBuffer(void)
{
	TestCaseBegin();
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_FADE;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	int i;
	for(i = 0; i < SCRIPTCTRL_NUM_CMD_MAX - 3; i++) {
		/* add inner dummy command to buffer */
		testCmd.cmd = SET_FADE;
		CHECK(ScriptCtrl_Add(&testCmd));
	}

	/* Buffer full */
		CHECK(!ScriptCtrl_Add(&testCmd));
	TestCaseEnd();
}

/* add clear command */
int ut_ScriptCtrl_StartBootloader(void)
{
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	if(0 == softResetJumpDestination()) {
		/* send START_BL */
		testCmd.cmd = START_BL;
		ScriptCtrl_Add(&testCmd);

		/* shouldn't reach this -> return error */
		return 1;
	}

	/* we just jumped to softResetJumpDestination() -> it worked */
	return 0;
}

/* test WAIT command */
int ut_ScriptCtrl_Wait(void)
{
	TestCaseBegin();
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* send wait command */
	testCmd.cmd = WAIT;
	testCmd.data.wait.waitTmms = htons(0xAFFE);
	ScriptCtrl_Add(&testCmd);

	/* WAIT command should set the internal wait buffer */
	ScriptCtrl_Run();
	CHECK(0xAFFE == gScriptBuf.waitValue);
	TestCaseEnd();
}

/* test ADD_COLOR command */
int ut_ScriptCtrl_AddColor(void)
{
	NOT_IMPLEMENTED();
}

/* test RTC commands */
int ut_ScriptCtrl_RtcCommands(void)
{
	NOT_IMPLEMENTED();
}

int main(int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true,  ut_ScriptCtrl_SimpleReadWrite);
	RunTest(true,  ut_ScriptCtrl_Clear);
	RunTest(true,  ut_ScriptCtrl_SimpleLoop);
	RunTest(true,  ut_ScriptCtrl_InnerLoop);
	RunTest(true,  ut_ScriptCtrl_InfiniteLoop);
	RunTest(false, ut_ScriptCtrl_FullBuffer);
	RunTest(true,  ut_ScriptCtrl_StartBootloader);
	RunTest(true,  ut_ScriptCtrl_Wait);
	RunTest(false, ut_ScriptCtrl_AddColor);
	RunTest(false, ut_ScriptCtrl_RtcCommands);
	UnitTestMainEnd();
}

