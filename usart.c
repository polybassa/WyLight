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

#ifndef X86
//*******  Initialisierungs-Funktion  *************************************************
void UART_Init()
{
	//USART TX Pin als Ausgang
	TRISC.6 = 0;
	BRGH1=1;					// High Baudrate activated
	BRG16=1;
	SPBRG1=34;				// 19200 Bps @ 8 MHz Clock
	SPBRGH1=0;
    SPEN1 = 1;               // Set_Serial_Pins;
    SYNC1 = 0;               // Set_Async_Mode;
    TX9_1 = 0;                // Set_8bit_Tx;
    RX9_1 = 0;                // Set_8bit_Rx;
    CREN1 = 1;               // Enable_Rx;
    TXEN1 = 1;               // Enable_Tx;
    RC1IE=1;                 // Rx Interrupt aus
	ADDEN1=0;				// Disable Adressdetection
}

//*******  Sende-char-Funktion  *************************************************
void UART_Send(unsigned char ch)
{
	while(!TX1IF);
	TXREG1=ch;
}
#endif /* #ifndef X86 */

//*******  Sende-String-Funktion  *************************************************
void UART_SendString(const char *string)
{
 char ps;
 ps = *string;
 while(ps > 0)
   {
    string++;
   	UART_Send(ps);
    ps = *string;
   }
}

//*******  Sende-Array-Funktion  *************************************************
void UART_SendArray(char *array, char length)
{
	if(array == 0) return;
	char i;
	for(i=0;i<length;i++)
	{
		UART_Send(*array);
		array++;
	}
}

//*******  Sende-Zahl-als-String-Funktion  *************************************************
#ifndef TEST
void UART_SendNumber(char input, char sign) {}
#else
void UART_SendNumber(char input, char sign)
{
   char temp;
   char h,z,e;
 
   h=0;
   z=0;
   e=0; 
   
   if (input>99)
   {
      h=input/100;
      temp=0;
      temp=100*h;
      input=input-temp; 
   }

   if (input>9)
   {
      z=input/10;
      temp=0;
      temp=z*10;
      input=input-temp;      
   }

   if (input<=9)
   {
      e=input;
   }
   if(h!=0)UART_Send(h+0x30);
   UART_Send(z+0x30);
   UART_Send(e+0x30);
   UART_Send(sign); 	//Zeichen senden
}
#endif
/*
//SENDE BCD-Zahl als String
void UART_SendTime(unsigned char input,unsigned char sign)
{
	char 	temp;
	char	z,e;
	
	e=input&0x0f;
	input=swap(input);
	z=input&0x0f;
	if(e>9)
	{
		e-=10;
		z++;
	}
	
	if(z>5)z=0;
	
	UART_Send(z+0x30);
	UART_Send(e+0x30);
	UART_Send(sign);
}*/
	

