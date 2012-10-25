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
#include "ScriptCtrl.h"
#include "ledstrip.h"
#include "eeprom.h"
#include "trace.h"

/**************** private functions/ macros *****************/
/**
 * Helper to calculate an eeprom address from a command pointer
 */
#define ScriptBufAddr(x) ((uns16)EEPROM_SCRIPTBUF_BASE + ((uns16)(x)*(uns16)sizeof(struct led_cmd)))

/**
 * Helper to increment a ScriptBuf pointer
 */
#define ScriptBufInc(x) ((x + 1) & SCRIPTCTRL_NUM_CMD_MAX)

/**
 * Setter for ScriptBuf.inLoop
 */
#define ScriptBufSetInLoop(x) { \
	Eeprom_Write(EEPROM_SCRIPTBUF_INLOOP, x); \
	gScriptBuf.inLoop = x; \
}

/**
 * Setter for ScriptBuf.read
 */
#define ScriptBufSetRead(x) { \
	Eeprom_Write(EEPROM_SCRIPTBUF_READ, x); \
	gScriptBuf.read = x; \
}

/**
 * Setter for ScriptBuf.write
 */
#define ScriptBufSetWrite(x) { \
	Eeprom_Write(EEPROM_SCRIPTBUF_WRITE, x); \
	gScriptBuf.write = x; \
}

/**
 * save command to eeprom
 */
uns8 ScriptCtrl_Write(struct led_cmd* pCmd);

/* private globals */
struct ScriptBuf gScriptBuf;
struct led_cmd nextCmd;

uns8 ScriptCtrl_Add(struct led_cmd* pCmd)
{
	/* We have to reject all commands until buffer was cleared completely */
	if(gScriptBuf.isClearing)
	{
		return FALSE;
	}

	switch(pCmd->cmd)
	{
		case DELETE:
			gScriptBuf.isClearing = TRUE;
			return TRUE;
		case LOOP_ON:
			gScriptBuf.loopStart[gScriptBuf.loopDepth] = gScriptBuf.write;
			gScriptBuf.loopDepth++;
			return ScriptCtrl_Write(pCmd);
		case LOOP_OFF:
		{
			gScriptBuf.loopDepth--;
			uns8 loopStart = gScriptBuf.loopStart[gScriptBuf.loopDepth];
			pCmd->data.loopEnd.startIndex = ScriptBufInc(loopStart);
			pCmd->data.loopEnd.depth = gScriptBuf.loopDepth;
			uns8 numLoops = pCmd->data.loopEnd.numLoops;
			pCmd->data.loopEnd.counter = numLoops;
			Trace_String("Add LOOP_OFF: ");
			Trace_Hex(gScriptBuf.write);
			Trace_Hex(pCmd->data.loopEnd.startIndex);
			Trace_Hex(pCmd->data.loopEnd.depth);
			Trace_Hex(pCmd->data.loopEnd.counter);
			Trace_String("\n");
			return ScriptCtrl_Write(pCmd);
		}
		case WAIT:
		{
			return ScriptCtrl_Write(pCmd);
		}
		case START_BL:
#ifdef __CC8E__
			UART_SendString("Leaving Application --> Starting Bootloader");
#endif
			Eeprom_Write(0x3ff, 0xff);
			softReset();
			/* never reach this */
			return FALSE;
#ifndef X86
/* TODO multiple things!
 - DISPLAY_RTC is only a debug command, isn't it? -> remove or replace UART_Send with Trace_*/
		case DISPLAY_RTC:
		{
			Rtc_Ctl(RTC_RD_TIME, &g_RtcTime);
			UART_SendNumber(g_RtcTime.tm_year,'Y');
			UART_SendNumber(g_RtcTime.tm_mon,'M');
			UART_SendNumber(g_RtcTime.tm_mday,'D');
			UART_SendNumber(g_RtcTime.tm_wday,'W');
			UART_SendNumber(g_RtcTime.tm_hour,'h');
			UART_SendNumber(g_RtcTime.tm_min,'m');
			UART_SendNumber(g_RtcTime.tm_sec,'s');
			UART_Send(0x0d);
			UART_Send(0x0a);
			return TRUE;
		}
		case GET_RTC:
		{
			Rtc_Ctl(RTC_RD_TIME, &g_RtcTime);
			UART_Send(g_RtcTime.tm_sec);
			UART_Send(g_RtcTime.tm_min);
			UART_Send(g_RtcTime.tm_hour);
			UART_Send(g_RtcTime.tm_mday);
			UART_Send(g_RtcTime.tm_mon);
			UART_Send(g_RtcTime.tm_year);
			UART_Send(g_RtcTime.tm_wday);
			return TRUE;
		}
		case SET_RTC:
		{
			g_RtcTime.tm_year = pCmd->data.set_rtc.tm_year;
			g_RtcTime.tm_mon = pCmd->data.set_rtc.tm_mon;
			g_RtcTime.tm_mday = pCmd->data.set_rtc.tm_mday;
			g_RtcTime.tm_wday = pCmd->data.set_rtc.tm_wday;
			g_RtcTime.tm_hour = pCmd->data.set_rtc.tm_hour;
			g_RtcTime.tm_min = pCmd->data.set_rtc.tm_min;
			g_RtcTime.tm_sec = pCmd->data.set_rtc.tm_sec;
			Rtc_Ctl(RTC_SET_TIME, &g_RtcTime);
			return TRUE;
		}
		case SET_COLOR_DIRECT:
		{
			Ledstrip_SetColorDirect(&pCmd->data.set_color_direct.ptr_led_array);
			Trace_String("GV");
			return TRUE;
		}	
		case GET_CYCLETIME:
		{
			Timer_PrintCycletime();
			return TRUE;
		}
#endif /* #ifndef X86 */
		case ADD_COLOR:
		{
			date_timer_add_event(&pCmd->data.add_color);
			return TRUE;
		}
	}
	return ScriptCtrl_Write(pCmd);
}

void ScriptCtrl_Clear(void)
{
	ScriptBufSetInLoop(FALSE);
	ScriptBufSetRead(EEPROM_SCRIPTBUF_BASE);
	ScriptBufSetWrite(EEPROM_SCRIPTBUF_BASE);
	gScriptBuf.execute = gScriptBuf.read;
	gScriptBuf.waitValue = 0;
	gScriptBuf.isClearing = FALSE;
}

void ScriptCtrl_Init(void)
{
	gScriptBuf.inLoop = Eeprom_Read(EEPROM_SCRIPTBUF_INLOOP);
	gScriptBuf.read = Eeprom_Read(EEPROM_SCRIPTBUF_READ);
	gScriptBuf.write = Eeprom_Read(EEPROM_SCRIPTBUF_WRITE);
	gScriptBuf.execute = gScriptBuf.read;
}

void ScriptCtrl_Run(void)
{
	/* delete command was triggered? */
	if(gScriptBuf.isClearing)
	{
		ScriptCtrl_Clear();
	}
#ifndef X86	
	if(gScriptBuf.waitValue > 0)
	{
		return;
	}
#endif /* #ifndef X86 */	
	/* cmd available? */
	if(gScriptBuf.execute == gScriptBuf.write)
	{
		return;
	}

	/* read next cmd from buffer */
	uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
	Eeprom_ReadBlock((uns8*)&nextCmd, tempAddress, sizeof(nextCmd));

	switch(nextCmd.cmd)
	{
		case LOOP_ON:
		{
			Trace_String("LOOP_ON\n");
			/* move execute pointer to the next command */
			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
			ScriptBufSetInLoop(TRUE);
			break;
		}
		case LOOP_OFF:
		{
			if(LOOP_INFINITE == nextCmd.data.loopEnd.counter)
			{
				Trace_String("End of infinite loop reached\n");
				/* move execute pointer to the top of this loop */
				gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
			}
			else if(nextCmd.data.loopEnd.counter > 1)
			{
				Trace_String("normal loop iteration");
				Trace_Hex(nextCmd.data.loopEnd.counter);
				Trace_Hex(nextCmd.data.loopEnd.depth);
				Trace_String("\n");
				/* update counter and set execute pointer to start of the loop */
				nextCmd.data.loopEnd.counter--;
				Eeprom_WriteBlock((uns8*)&nextCmd, tempAddress, sizeof(struct led_cmd));

				/* move execute pointer to the top of this loop */
				gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
			}
			else
			{
				if(0 == nextCmd.data.loopEnd.depth)
				{
					Trace_String("End of top loop reached\n");
					/* move execute pointer to the next command */
					gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);

					/* delete loop body from buffer */
					ScriptBufSetRead(gScriptBuf.execute);
					ScriptBufSetInLoop(FALSE);
				}
				else
				{
					Trace_String("End of inner loop reached\n");
					/* reinit counter for next iteration */
					nextCmd.data.loopEnd.counter = nextCmd.data.loopEnd.numLoops;
					uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
					Eeprom_WriteBlock((uns8*)&nextCmd, tempAddress, sizeof(struct led_cmd));

					/* move execute pointer to the next command */
					gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
				}
			}
			break;
		}
		case SET_COLOR:
		{
			Ledstrip_SetColor(&nextCmd.data.set_color);
			/* move execute pointer to the next command */
			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
			if(!gScriptBuf.inLoop)
			{
				ScriptBufSetRead(gScriptBuf.execute);
			}
			break;
		}
		case SET_FADE:
		{
			Ledstrip_SetFade(&nextCmd.data.set_fade);
			if(nextCmd.data.set_fade.parallelFade == 0)
			{
				gScriptBuf.waitValue = nextCmd.data.set_fade.fadeTmms;
			}
			/* move execute pointer to the next command */
			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
			if(!gScriptBuf.inLoop)
			{
				ScriptBufSetRead(gScriptBuf.execute);
			}
			break;
		}
#ifndef X86
		/* TODO implement this for X86 */
		case SET_RUN: 
		{
			Timer_StartStopwatch(eSET_RUN);
			gScriptBuf.waitValue = nextCmd.data.set_run.durationTmms;
			Ledstrip_SetRun(&nextCmd.data.set_run);
			Timer_StopStopwatch(eSET_RUN);
			/* move execute pointer to the next command */
			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
			if(!gScriptBuf.inLoop)
			{
				ScriptBufSetRead(gScriptBuf.execute);
			}
			break;
		}
#endif /* #ifndef X86 */
		case WAIT:
		{
			/* TODO we should disable interrupts while changing waitValue */
			gScriptBuf.waitValue = nextCmd.data.wait.waitTmms;
			/* move execute pointer to the next command */
			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
			if(!gScriptBuf.inLoop)
			{
				ScriptBufSetRead(gScriptBuf.execute);
			}
			break;
		}
	}	
}

uns8 ScriptCtrl_Write(struct led_cmd* pCmd)
{
	uns8 writeNext = ScriptBufInc(gScriptBuf.write);
	if(writeNext != gScriptBuf.read)
	{
		uns16 tempAddress = ScriptBufAddr(gScriptBuf.write);
		Eeprom_WriteBlock((uns8*)pCmd, tempAddress, sizeof(struct led_cmd));
		ScriptBufSetWrite(writeNext);
		return TRUE;
	}
	return FALSE;
}

