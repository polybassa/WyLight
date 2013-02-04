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

#include "CommandIO.h"
#include "error.h"
#include "RingBuf.h"
#include "usart.h"
#include "trace.h"

struct ErrorBits g_ErrorBits;

uns8 Error_GetState()
{
	if(RingBuf_HasError(&g_TraceBuf)) return ErrorTraceBufFull;
	else if(g_ErrorBits.CmdBufOverflow) return ErrorCmdBufFull;
	else if(RingBuf_HasError(&g_RingBuf)) return ErrorRecvBufFull;
	else if(g_ErrorBits.CrcFailure) return ErrorCrcCheckFail;
	else if(g_ErrorBits.EepromFailure) return ErrorEepromFull;
	else return NoError;
}

void Error_Throw()
{
#ifdef TEST
	if(RingBuf_HasError(&g_TraceBuf)) 
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		Trace_String("E:05; ERROR: Tracebuffer full");
		// *** Re-init the Ringbuffer to get a consistent commandstring and reset error
		RingBuf_Init(&g_TraceBuf);
	}
#endif
	
	if(g_ErrorBits.CmdBufOverflow)
	{
		Trace_String("E:04; ERROR: Commandbuffer full");
		CommandIO_Init();
		g_ErrorBits.CmdBufOverflow = 0;
	}
	
	if(RingBuf_HasError(&g_RingBuf)) 
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		CommandIO_Init();
		Trace_String("E:03; ERROR: Receivebuffer full");
		// *** Re-init the Ringbuffer to get a consistent commandstring and reset error
		RingBuf_Init(&g_RingBuf);
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
