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

#include "usart.h"
#include "rn_171.h"

#define Send_CRLF(x) UART_Send(0x0d); UART_Send(0x0a);

void Rn171FactoryRestore()
{
	UART_Send('$');
	UART_Send('$');
	UART_Send('$');
	Send_CRLF();
	
	UART_SendString("load config_nils");
	Send_CRLF();
	UART_SendString("save");
	Send_CRLF();
	UART_SendString("reboot");
	Send_CRLF();
}