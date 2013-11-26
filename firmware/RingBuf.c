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

#include "RingBuf.h"

bank7 struct RingBuffer g_RingBuf;

void RingBuf_Init(struct RingBuffer *pBuf)
{
	pBuf->read = 0;
	pBuf->write = 0;
	pBuf->error_full = 0;
}

uns8 RingBuf_Get(struct RingBuffer *pBuf)
{
	//Platform_DisableAllInterrupts();
	uns8 read = pBuf->read;
	uns8 result = pBuf->data[read];
	pBuf->read = RingBufInc(read);

	//TODO make this thread safe or remove flag!
	pBuf->error_full = FALSE;
	//Platform_EnableAllInterrupts();
	return result;
}

void RingBuf_Put(struct RingBuffer *pBuf,const uns8 value)
{
	//Platform_DisableAllInterrupts();
	uns8 writeNext = RingBufInc(pBuf->write);
	if(writeNext != pBuf->read) {
		uns8 write = pBuf->write;
		pBuf->data[write] = value;
		pBuf->write = writeNext;
	} else pBuf->error_full = 1;
	//Platform_EnableAllInterrupts();
}

bit RingBuf_HasError(struct RingBuffer *pBuf)
{
	return pBuf->error_full;
}

bit RingBuf_IsEmpty(const struct RingBuffer *pBuf)
{
	//Platform_DisableAllInterrupts();
	uns8 write = pBuf->write;
	uns8 read = pBuf->read;
	//Platform_EnableAllInterrupts();
	return write == read;
}
