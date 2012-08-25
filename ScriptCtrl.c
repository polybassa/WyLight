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

struct ScriptBuf gScriptBuf;

void ScriptCtrl_Add(struct led_cmd* pCmd)
{
	if(LOOP_ON == pCmd->cmd)
	{
		pCmd->data.loop_start.depth = gScriptBuf.loopDepth;
		gScriptBuf.loopStart[gScriptBuf.loopDepth] = gScriptBuf.write;
		gScriptBuf.loopDepth++;		
	}
	else if (LOOP_OFF == pCmd->cmd)
	{
		uns8 loopStart = gScriptBuf.loopStart[gScriptBuf.loopDepth];
		pCmd->data.loop_stop.startIndex = loopStart;
		pCmd->data.loop_stop.depth = gScriptBuf.loopDepth;
		gScriptBuf.loopDepth--;
	}
	ScriptCtrl_Write(pCmd);
}

void ScriptCtrl_Run(void)
{
	/* cmd available? */
	if(gScriptBuf.execute == gScriptBuf.write) return;

	/* read next cmd from buffer */
	struct led_cmd* pCmd = &gScriptBuf.cmd[gScriptBuf.execute];
	
	/* increment execute pointer */
	gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);

	switch(pCmd->cmd)
	{
		case LOOP_ON:
			if(LOOP_INFINITE == pCmd->data.loop_start.counter)
			{
				/* loop forever */;
			}
			else if(0 == pCmd->data.loop_start.counter)
			{
				/* loop reached end, is it a top loop? */
				if(1 == pCmd->data.loop_start.depth)
				{
					/* top loop reached end -> delete commands in loop body */
					do
					{
						gScriptBuf.read = ScriptBufInc(gScriptBuf.read);
					} while (gScriptBuf.read != gScriptBuf.execute);

					/* set pointers to next valid command */
					gScriptBuf.read = ScriptBufInc(gScriptBuf.read);
					gScriptBuf.execute = gScriptBuf.read;
				}
				else
				{
					/* end of no top loop reached -> reinit counter for next iteration */
					pCmd->data.loop_start.counter = pCmd->data.loop_start.numLoops,
					gScriptBuf.loopSkip = TRUE;
				}
			}
			else
			{
				/* normal loop iteration -> update counter */
				pCmd->data.loop_start.counter--;
			}
			break;
		case LOOP_OFF:
			if(!gScriptBuf.loopSkip)
			{
				gScriptBuf.execute = pCmd->data.loop_stop.startIndex;
			}
			break;
		case SET_COLOR:
			Ledstrip_SetColor(&pCmd->data.set_color);
			break;
		case SET_FADE:
			Ledstrip_SetFade(&pCmd->data.set_fade);
			break;
	}	
}

void ScriptCtrl_Write(struct led_cmd* pCmd)
{
	uns8 writeNext = ScriptBufInc(gScriptBuf.write);
	if(writeNext != gScriptBuf.read)
	{
		memcpy(&gScriptBuf.cmd[gScriptBuf.write], pCmd, sizeof(struct led_cmd));
		gScriptBuf.write = writeNext;
	}	
}

