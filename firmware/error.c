/**
 Copyright (C) 2012 Nils Weiss, Patrick Brünn.
 
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

#include "error.h"
#include "CommandIO.h"
#include "RingBuf.h"
#include "trace.h"
#include "ledstrip.h"

struct ErrorBits g_ErrorBits;

ErrorCode Error_GetState()
{
	if(g_ErrorBits.EepromFailure) return SCRIPTBUFFER_FULL;
	else if(g_ErrorBits.CrcFailure) return CRC_CHECK_FAILED;
	else return OK;
}

void Error_Throw()
{
#ifdef DEBUG
	if(RingBuf_HasError(&g_TraceBuf)) 
	{
		// *** Re-init the Ringbuffer
		RingBuf_Init(&g_TraceBuf);
		Trace_String("E:05; ERROR: Tracebuffer full");
	}
#endif
	
	if(g_ErrorBits.CmdBufOverflow)
	{
		Trace_String("E:04; ERROR: Commandbuffer full");
		Error_FatalError();
	}
	
	if(RingBuf_HasError(&g_RingBuf)) 
	{
		Trace_String("E:03; ERROR: Receivebuffer full");
		// *** Re-init the Ringbuffer to get a consistent commandstring and reset error
		RingBuf_Init(&g_RingBuf);
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		CommandIO_Init();
	}
	if(g_ErrorBits.CrcFailure)
	{
		Trace_String("E:02; ERROR: Crc-Check failed");
		g_ErrorBits.CrcFailure = 0;
	}
	if(g_ErrorBits.EepromFailure)
	{
		Trace_String("E:01; ERROR: EEPROM is full");
		g_ErrorBits.EepromFailure = 0;
	}
}

void Error_FatalError()
{
	uns8 i = 0;
	for(;i < NUM_OF_LED * 3; i++)
	{
		gLedBuf.led_array[i] = 0x00;
		i++;
		gLedBuf.led_array[i] = 0x00;
		i++;
		gLedBuf.led_array[i] = 0xff;
	}
	
	Ledstrip_UpdateLed();
#ifdef __CC8E__
	while(PORTB.5 != 0);
	while(PORTB.5 == 0);
#endif
	CommandIO_Init();
	RingBuf_Init(&g_RingBuf);
}
