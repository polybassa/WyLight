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

#include "usart.h"

#ifdef __CC8E__
//*******  Initialisierungs-Funktion  *************************************************
void UART_Init()
{
    //USART TX Pin als Ausgang
    TRISC .6 = 0;
    BRGH1 = 1;                                        // High Baudrate activated
    BRG16 = 1;
    SPBRG1 = 34;                              // 115200 Bps @ 64 MHz Clock
    SPBRGH1 = 0;
    SPEN1 = 1;               // Set_Serial_Pins;
    SYNC1 = 0;               // Set_Async_Mode;
    TX9_1 = 0;                // Set_8bit_Tx;
    RX9_1 = 0;                // Set_8bit_Rx;
    CREN1 = 1;               // Enable_Rx;
    TXEN1 = 1;               // Enable_Tx;
    RC1IE = 1;                 // Rx Interrupt aus
    ADDEN1 = 0;                               // Disable Adressdetection
}

//*******  Sende-char-Funktion  *************************************************
void UART_Send(const uns8 ch)
{
    while (!TX1IF) {}
    TXREG1 = ch;
}

#endif /* #ifdef CC8E */
