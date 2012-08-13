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

#define gScriptBufSize 15
#define SCRIPT_LOOP_DEPTH_MAX 4
struct ScriptBuf {
	struct led_cmd cmd[gScriptBufSize];
	uns8 loopStart[SCRIPT_LOOP_DEPTH_MAX];
	uns8 loopDepth;
	uns8 execute;
	uns8 read;
	uns8 write;
	bit override;
	bit loopSkip;
};
struct ScriptBuf gScriptBuf;

#define ScriptBufInc(x) ((x + 1) & gScriptBufSize)

void ScriptCtrl_Add(struct led_cmd* pCmd)
{
	if(LOOP_START == pCmd->cmd)
	{
		pCmd->loop_start.depth = gScript.loopDepth;
		gScript.loopStart[gScript.loopDepth] = gScriptBuf.write;
		gScript.loopDepth++;		
	}
	else if (LOOP_END)
	{
		uns8 loopStart = gScript.loopStart[gScript.loopDepth]
		pCmd->loop_stop.startIndex = loopStart;
		pCmd->loop_stop.depth = gScript.loopDepth;
		gScript.loopDepth--;
	}
	ScriptCtrl_WriteCmd(pCmd);
}

void ScriptCtrl_Run()
{
	// cmd available?
	if(gScriptBuf.execute == gScriptBuf.write) return;
	
	struct led_cmd* pCmd = &gScript.cmd[gScript.execute];
	ScriptBufInc(gScriptBuf.execute);

	switch(pCmd->cmd)
	{
		case LOOP_START:
			if(1 == pCmd->data.loop_start.counter)
			{
				gScriptOverride = (1 == pCmd->data.loop_start.depth);
				pCmd->data.loop_start.counter = pCmd->data.loop_start.numLoops,
				gScript.loopSkip = TRUE;
			}
			break;
		case LOOP_STOP:
			if(!gScript.loopSkip)
			{
				gScriptBuf.execute = pCmd->data.loop_stop.startIndex;
			}
			break;
		case SET_COLOR:
			ledstrip_set_color(&pCmd->data.set_color);
			break;
		case SET_FADE:
			ledstrip_set_fade(&pCmd->data.set_fade);
			break;
	}

	// delete command from buffer?	
	if(gScriptBuf.override)
	{
		ScriptBufInc(gScriptBuf.read);
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

