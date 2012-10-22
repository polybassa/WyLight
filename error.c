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

#include "commandstorage.h"
#include "error.h"
#include "RingBuf.h"
#include "usart.h"

struct ErrorBits g_ErrorBits;

void Error_Throw()
{
	if(RingBuf_HasError) 
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		Commandstorage_Clear();
		UART_SendString("E:03; ERROR: Receivebuffer full");
		// *** Re-init the Ringbuffer to get a consistent commandstring and reset error
		RingBuf_Init();
	}
	if(g_ErrorBits.CrcFailure)
	{
		UART_SendString("E:02; ERROR:Crc-Check failed");
		g_ErrorBits.CrcFailure = 0;
	}
	if(g_ErrorBits.EepromFailure)
	{
		UART_SendString("E:01; ERROR: EEPROM is full");
		g_ErrorBits.EepromFailure = 0;
	}
}
