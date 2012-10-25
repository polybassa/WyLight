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

#ifndef _TRACE_H_
#define _TRACE_H_
#ifdef TEST
	#include "usart.h"
	#define Trace_String(str) do { UART_SendString(str); } while (0)
	#define Trace_Number(input, sign) do { UART_SendNumber(input, sign); } while (0)
	#define Trace_Hex(hex) do { UART_Send(hex); } while(0)
#elif DEBUG
	#include "stdio.h"
	#define Trace_String(str) do { printf("%s", str); } while (0)
	#define Trace_Number(input, sign) do { printf("%04x%c", input, sign); } while (0)
	#define Trace_Hex(hex) do { printf("%02x ", hex); } while(0)
#else
	#define Trace_String(str)
	#define Trace_Number(input, sign)
	#define Trace_Hex(hex)
#endif
#endif /* #ifndef _TRACE_H_ */

