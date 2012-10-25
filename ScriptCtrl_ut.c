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

#include "trace.h"
#include "unittest.h"
#define NUM_TEST_LOOPS 255
 
int gSetColorWasCalled;
int gSetFadeWasCalled;

/**************** includes and functions for wrapping ****************/
#include "platform.h"
jmp_buf g_ResetEnvironment;

#include "ScriptCtrl.h"
struct ScriptBuf gScriptBuf;

#include "ledstrip.h"
void Ledstrip_SetColor(struct cmd_set_color *pCmd)
{
	Trace_String("Ledstrip_SetColor was called\n");
	gSetColorWasCalled = TRUE;
}

void Ledstrip_SetFade(struct cmd_set_fade *pCmd)
{
	Trace_String("Ledstrip_SetFade was called\n");
	gSetFadeWasCalled = TRUE;
}

#include "timer.h"
unsigned char date_timer_add_event(struct cmd_add_color* pCmd)
{
	return 0;
}

/******************************* test functions *******************************/
/* add simple command and read back */
int ut_ScriptCtrl_SimpleReadWrite(void)
{
	int errors = 0;
	struct led_cmd testCmd;
	testCmd.cmd = SET_COLOR;
	ScriptCtrl_Clear();

	/* read from empty script buffer */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();

	/* add simple command and read back */
	ScriptCtrl_Add(&testCmd);
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	assert(gSetColorWasCalled);

	/* buffer should be empty again */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	assert(!gSetColorWasCalled);

	return errors;
}

/* add clear command */
int ut_ScriptCtrl_Clear(void)
{
	int errors = 0;
	struct led_cmd testCmd;
	testCmd.cmd = SET_COLOR;

	/* add dummy command to buffer */
	ScriptCtrl_Add(&testCmd);
	gSetColorWasCalled = FALSE;

	/* send DELETE */
	testCmd.cmd = DELETE;
	ScriptCtrl_Add(&testCmd);

	/* buffer should be empty again */
	ScriptCtrl_Run();
	assert(!gSetColorWasCalled);

	return errors;
}

/* add simple loop */
int ut_ScriptCtrl_SimpleLoop(void)
{
	int errors = 0;
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* add loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add dummy command to buffer */
	testCmd.cmd = SET_COLOR;
	ScriptCtrl_Add(&testCmd);

	/* add loop end to buffer */
	testCmd.cmd = LOOP_OFF;
	testCmd.data.loopEnd.numLoops = NUM_TEST_LOOPS;
	ScriptCtrl_Add(&testCmd);

	/* start loop should be read */
	assert(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	assert(gScriptBuf.inLoop);

	int i;
	for(i = 0; i < NUM_TEST_LOOPS; i++)
	{
		/* dummy command should be executed */
		gSetColorWasCalled = FALSE;
		ScriptCtrl_Run();
		assert(gSetColorWasCalled);
		
		/* now next loop should be called */
		gSetColorWasCalled = FALSE;
		ScriptCtrl_Run();
		assert(!gSetColorWasCalled);
	}

	/* no more command should be available */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	assert(!gSetColorWasCalled);

	return errors;
}

int ut_ScriptCtrl_DoOuterInnerLoop(int loopCount)
{
	int errors = 0;
	int i, j;
	for(i = 0; i < loopCount; i++)
	{
		/* outer dummy command should be executed */
		gSetColorWasCalled = FALSE;
		ScriptCtrl_Run();
		assert(gSetColorWasCalled);

		/* start inner loop should be read */
		assert(gScriptBuf.inLoop)
		ScriptCtrl_Run();
		assert(gScriptBuf.inLoop);

		for(j = 0; j < loopCount; j++)
		{
			/* outer dummy command should be executed */
			gSetFadeWasCalled = FALSE;
			ScriptCtrl_Run();
			assert(gSetFadeWasCalled);

			/* now next inner loop should be called */
			gSetColorWasCalled = FALSE;
			ScriptCtrl_Run();
			assert(!gSetColorWasCalled);
		}

		/* now next outer loop should be called */
		gSetColorWasCalled = FALSE;
		ScriptCtrl_Run();
		assert(!gSetColorWasCalled);
	}
	return errors;
}


/* add inner loop */
int ut_ScriptCtrl_InnerLoop(void)
{
	int errors = 0;
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_COLOR;
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
	assert(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	assert(gScriptBuf.inLoop);

	errors+= ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);

	/* no more command should be available */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	assert(!gSetColorWasCalled);

	return errors;
}

/* add infinite loop */
int ut_ScriptCtrl_InfiniteLoop(void)
{
	int errors = 0;
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_COLOR;
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
	assert(!gScriptBuf.inLoop)
	ScriptCtrl_Run();
	assert(gScriptBuf.inLoop);

	/* multiple calls should be no problem since we are in an infinite loop */
	errors+= ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);
	errors+= ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);
	errors+= ut_ScriptCtrl_DoOuterInnerLoop(NUM_TEST_LOOPS);

	/* now terminate the loop */
	testCmd.cmd = DELETE;
	ScriptCtrl_Add(&testCmd);

	/* buffer should be empty again */
	gSetColorWasCalled = FALSE;
	ScriptCtrl_Run();
	assert(!gSetColorWasCalled);

	return errors;
}

/* write to full script buffer */
/* write incomplete loop to full buffer */
int ut_ScriptCtrl_FullBuffer(void)
{
	int errors = 0;
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* add outer loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	/* add outer dummy command to buffer */
	testCmd.cmd = SET_COLOR;
	ScriptCtrl_Add(&testCmd);

	/* add inner loop begin to buffer */
	testCmd.cmd = LOOP_ON;
	ScriptCtrl_Add(&testCmd);

	int i;
	for(i = 0; i < SCRIPTCTRL_NUM_CMD_MAX-3; i++)
	{
		/* add inner dummy command to buffer */
		testCmd.cmd = SET_FADE;
		assert(ScriptCtrl_Add(&testCmd));
	}

	/* Buffer full */
	assert(!ScriptCtrl_Add(&testCmd));
	return errors;
}

/* add clear command */
int ut_ScriptCtrl_StartBootloader(void)
{
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	if(0 == softResetJumpDestination())
	{
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
	int errors = 0;
	struct led_cmd testCmd;
	ScriptCtrl_Clear();

	/* send wait command */
	testCmd.cmd = WAIT;
	testCmd.data.wait.waitTmms = 0xAFFE;
	ScriptCtrl_Add(&testCmd);

	/* WAIT command should set the internal wait buffer */
	ScriptCtrl_Run();
	assert(0xAFFE == gScriptBuf.waitValue);
	return errors;
}

/* test ADD_COLOR command */
int ut_ScriptCtrl_AddColor(void)
{
	printf("Error: %s not implemented yet\n", __FUNCTION__);
	return 1;
}

/* test RTC commands */
int ut_ScriptCtrl_RtcCommands(void)
{
	printf("Error: %s not implemented yet\n", __FUNCTION__);
	return 1;
}

int main(int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_ScriptCtrl_SimpleReadWrite);
	RunTest(true, ut_ScriptCtrl_Clear);
	RunTest(true, ut_ScriptCtrl_SimpleLoop);
	RunTest(true, ut_ScriptCtrl_InnerLoop);
	RunTest(true, ut_ScriptCtrl_InfiniteLoop);
	RunTest(true, ut_ScriptCtrl_FullBuffer);
	RunTest(true, ut_ScriptCtrl_StartBootloader);
	RunTest(true, ut_ScriptCtrl_Wait);
	RunTest(false, ut_ScriptCtrl_AddColor);
	RunTest(false, ut_ScriptCtrl_RtcCommands);
	UnitTestMainEnd();
}

