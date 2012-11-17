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

#include "trace.h"

#ifdef TEST

struct RingBuffer g_TraceBuf;
	
void Trace_String(const char *string)
{
 uns8 ps;
 ps = *string;
 while(ps > 0)
   {
    string++;
   	RingBuf_Put(&g_TraceBuf,ps);
    ps = *string;
   }
}
	
void Trace_Number(uns8 input)
{
   uns8 temp;
   uns8 h,z,e;
 
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
   if(h!=0) RingBuf_Put(&g_TraceBuf, h+0x30);
   RingBuf_Put(&g_TraceBuf, z+0x30);
   RingBuf_Put(&g_TraceBuf, e+0x30);
}
	
void Trace_Hex(uns8 input)
{
	uns8 temp4 = input & 0xf0;
	temp4 = temp4 >> 4;
	if(temp4 > 9)
	{
		temp4 -= 10;
		RingBuf_Put(&g_TraceBuf,temp4 + 'A');
	}
	else
	{
		RingBuf_Put(&g_TraceBuf,temp4 + '0');
	}
	temp4 = input & 0x0f;
	if(temp4 > 9)
	{
		temp4 -= 10;
		RingBuf_Put(&g_TraceBuf,temp4 + 'A');
	}
	else
	{
		RingBuf_Put(&g_TraceBuf,temp4 + '0');
	}
}
	
void Trace_Print()
{	
	UART_Send(0x0d);UART_Send(0x0a);
	UART_SendString("TRACEBUFFER:");
	
	while(RingBuf_IsEmpty(&g_TraceBuf) == 0)
	{
	    UART_Send(RingBuf_Get(&g_TraceBuf));
	}
	UART_Send(0x0d);UART_Send(0x0a);
	UART_SendString("Timevalues: ScriptBuf Waitvalue: ");
	UART_SendHex_16(gScriptBuf.waitValue);
	UART_Send(0x0d);UART_Send(0x0a);
	UART_SendString("FadeTimeValue: ");
	UART_SendHex_16(gLedBuf.fadeTmms);
	UART_Send(0x0d);UART_Send(0x0a);
}

#endif 