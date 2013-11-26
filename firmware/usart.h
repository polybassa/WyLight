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

#ifndef _USART_H_
#define _USART_H_
#include "platform.h"

void UART_Init();
void UART_Send(const uns8 ch);

/* UNUSED FUNCTIONS: REMOVE IF WE DON'T NEED THEM ANYMORE */
/*void UART_SendString(const char *string);
void UART_SendArray(const uns8 *array, const uns8 length);
void UART_SendNumber(uns8 input, const uns8 sign);
void UART_SendHex_8(const uns8 input);
void UART_SendHex_16(const uns16 input);
*/

#endif
