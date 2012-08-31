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

/* private functions/ macros */
#define ScriptBufAddr(x) (EEPROM_SCRIPTBUF_BASE + ((x)*sizeof(struct led_cmd)))
#define ScriptBufInc(x) ((x + 1) & SCRIPTCTRL_NUM_CMD_MAX)
#define ScriptBufSetInLoop(x) { \
	Eeprom_Write(EEPROM_SCRIPTBUF_INLOOP, x); \
	gScriptBuf.inLoop = x; \
}
#define ScriptBufSetRead(x) { \
	Eeprom_WriteBlock((uns8*)&x, EEPROM_SCRIPTBUF_READ, sizeof(x)); \
	gScriptBuf.read = x; \
}
#define ScriptBufSetWrite(x) { \
	Eeprom_WriteBlock((uns8*)&x, EEPROM_SCRIPTBUF_WRITE, sizeof(x)); \
	gScriptBuf.write = x; \
}

/* private globals */
struct ScriptBuf gScriptBuf;
struct led_cmd nextCmd;

void ScriptCtrl_Add(struct led_cmd* pCmd)
{
	if(LOOP_ON == pCmd->cmd)
	{
		gScriptBuf.loopStart[gScriptBuf.loopDepth] = gScriptBuf.write;
		gScriptBuf.loopDepth++;
	}
	else if (LOOP_OFF == pCmd->cmd)
	{
		uns8 loopStart = gScriptBuf.loopStart[gScriptBuf.loopDepth];
		pCmd->data.loopEnd.startIndex = loopStart;
		pCmd->data.loopEnd.depth = gScriptBuf.loopDepth;
		gScriptBuf.loopDepth--;
	}
	ScriptCtrl_Write(pCmd);
}

void ScriptCtrl_Init(void)
{
	gScriptBuf.inLoop = Eeprom_Read(EEPROM_SCRIPTBUF_INLOOP);
	Eeprom_ReadBlock((uns8*)&gScriptBuf.read, EEPROM_SCRIPTBUF_READ, sizeof(gScriptBuf.read));
	Eeprom_ReadBlock((uns8*)&gScriptBuf.write, EEPROM_SCRIPTBUF_WRITE, sizeof(gScriptBuf.write));
	gScriptBuf.execute = gScriptBuf.read;
}

void ScriptCtrl_Run(void)
{
	/* cmd available? */
	if(gScriptBuf.execute == gScriptBuf.write) return;

	/* read next cmd from buffer */
	uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
	Eeprom_ReadBlock((uns8*)&nextCmd, tempAddress, sizeof(nextCmd));

	switch(nextCmd.cmd)
	{
		case LOOP_ON:
			/* move execute pointer to the next command */
			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
			ScriptBufSetInLoop(TRUE);
			break;
		case LOOP_OFF:
			if(LOOP_INFINITE == nextCmd.data.loopEnd.counter)
			{
				/* move execute pointer to the top of this loop */
				gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
			}
			else if(1 == nextCmd.data.loopEnd.counter)
			{
				/* loop reached end, is it the top loop? */
				if(1 == nextCmd.data.loopEnd.depth)
				{
					/* move execute pointer to the next command */
					gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);

					/* delete loop body from buffer */
					ScriptBufSetRead(gScriptBuf.execute);
					ScriptBufSetInLoop(FALSE);
				}
				else
				{
					/* end of no top loop reached -> reinit counter for next iteration */
					nextCmd.data.loopEnd.counter = nextCmd.data.loopEnd.numLoops;
					uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
					Eeprom_WriteBlock((uns8*)&nextCmd, tempAddress, sizeof(struct led_cmd));

					/* move execute pointer to the next command */
					gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
				}
			}
			else
			{
				/* normal loop iteration -> update counter and set execute pointer to start of the loop */
				nextCmd.data.loopEnd.counter--;

				/* move execute pointer to the top of this loop */
				gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
			}
			break;
		case SET_COLOR:
			Ledstrip_SetColor(&nextCmd.data.set_color);
			/* move execute pointer to the next command */
			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
			if(!gScriptBuf.inLoop)
			{
				ScriptBufSetRead(gScriptBuf.execute);
			}
			break;
		case SET_FADE:
			Ledstrip_SetFade(&nextCmd.data.set_fade);
			/* move execute pointer to the next command */
			gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
			if(!gScriptBuf.inLoop)
			{
				ScriptBufSetRead(gScriptBuf.execute);
			}
			break;
	}	
}

void ScriptCtrl_Write(struct led_cmd* pCmd)
{
	uns8 writeNext = ScriptBufInc(gScriptBuf.write);
	if(writeNext != gScriptBuf.read)
	{
		uns16 tempAddress = ScriptBufAddr(gScriptBuf.write);
		Eeprom_WriteBlock((uns8*)pCmd, tempAddress, sizeof(struct led_cmd));
		ScriptBufSetWrite(writeNext);
	}	
}

