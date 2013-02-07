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

#ifdef DEBUG 
#ifdef __CC8E__

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

void Trace_Char(uns8 input)
{
    RingBuf_Put(&g_TraceBuf, input);
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

void Trace_Hex16(uns16 input)
{
	Trace_Hex(input.high8);
    Trace_Hex(input.low8);
}
	
uns8 Trace_Print(uns8 *pArray, uns16 arraySize)
{
	uns16 i = 0;
	uns8 tempByte;
	while(RingBuf_IsEmpty(&g_TraceBuf) == 0 && i < arraySize)
	{
	    tempByte = RingBuf_Get(&g_TraceBuf);
		*pArray = tempByte;
		pArray++;
		i++;
	}
	return i;
}
#endif
#else /* DEBUG && __CC8E__ */
uns8 Trace_Print(uns8 *pArray, uns16 arraySize)
{
	return 0;
}
#endif