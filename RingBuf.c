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

#include "RingBuf.h"

struct RingBuffer g_RingBuf;

void RingBuf_Init(struct RingBuffer *pBuf)
{
	pBuf->read = 0;
	pBuf->write = 0;
	pBuf->error_full = 0;
}

uns8 RingBuf_Get(struct RingBuffer *pBuf)
{
	uns8 read = pBuf->read;
	uns8 result = pBuf->data[read];
	pBuf->read = RingBufInc(read);
	return result;
}

void RingBuf_Put(struct RingBuffer *pBuf, uns8 value)
{
	uns8 writeNext = RingBufInc(pBuf->write);
	if(writeNext != pBuf->read)
	{
		uns8 write = pBuf->write;
		pBuf->data[write] = value;
		pBuf->write = writeNext;
	}
	else pBuf->error_full = 1;
}

bit RingBuf_HasError(struct RingBuffer *pBuf)
{
	return pBuf->error_full;
}

void RingBuf_ClearError(struct RingBuffer *pBuf)
{
	pBuf->error_full = FALSE;
}

bit RingBuf_IsNotEmpty(struct RingBuffer *pBuf)
{
	uns8 write = pBuf->write;
	uns8 read = pBuf->read;
	return write != read;
}