/*
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

#if defined(DEBUG) && defined(__CC8E__)
struct RingBuffer g_TraceBuf;

void Trace_Init(void)
{
	RingBuf_Init(&g_TraceBuf);
}

void PutToBuf(const uns8 Byte)
{
	if(RingBuf_HasError(&g_TraceBuf)) return;
	RingBuf_Put(&g_TraceBuf, Byte);
}

void Trace_String(const char *string)
{
	if(RingBuf_HasError(&g_TraceBuf)) return;
	uns8 ps;
	ps = *string;
	while(ps > 0)
	{
		string++;
		PutToBuf(ps);
		ps = *string;
	}
}

void Trace_Char(const uns8 input)
{
		PutToBuf(input);
}

void Trace_Number(const uns8 input)
{
	if(RingBuf_HasError(&g_TraceBuf)) return;
	uns8 temp;
	uns8 h,z,e;

	h = 0;
	z = 0;
	e = 0;

	if(input > 99) {
		h = input / 100;
		temp = 0;
		temp = 100 * h;
		input = input - temp;
	}

	if(input > 9) {
		z = input / 10;
		temp = 0;
		temp = z * 10;
		input = input - temp;
	}

	if(input <= 9) {
		e = input;
	}

	if(h != 0) PutToBuf(h + 0x30);

		PutToBuf(z + 0x30);

		PutToBuf(e + 0x30);
}

void Trace_Hex(const uns8 input)
{
	if(RingBuf_HasError(&g_TraceBuf)) return;

	uns8 temp4 = input & 0xf0;
	temp4 = temp4 >> 4;
	if(temp4 > 9) {
		temp4 -= 10;
		PutToBuf(temp4 + 'A');
	} else {
		PutToBuf(temp4 + '0');
	}

	temp4 = input & 0x0f;
	if(temp4 > 9) {
		temp4 -= 10;
		PutToBuf(temp4 + 'A');
	} else {
		PutToBuf(temp4 + '0');
	}
}

void Trace_Hex16(const uns16 input)
{
	Trace_Hex(input.high8);
	Trace_Hex(input.low8);
}

uns8 Trace_Print(uns8 *pArray, const uns16 arraySize)
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
#else
uns8 Trace_Print(uns8 *pArray, const uns16 arraySize)
{
	return 0;
}
#endif
