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

#ifndef _RINGBUF_H_
#define _RINGBUF_H_

#include "platform.h"

/**
 * We will use a 31+1 byte ringbuffer. We need one additional byte to distinguish
 * between an empty and a full buffer.
 * Empty: gRingBufRead == gRingBufWrite
 * Full:  gRingBufRead == gRingBufWrite + 1
 * 
 * size of buffer has to be a power of 2 - 1 to do this nasty index calulation trick:
 * newIndex = (index + 1) & size
 * f.e.: size = 31 -> realsize = 32 -> we want an index of 31++ to overflow to 0:
 * binary: (00011111 + 00000001) & 00011111 = 00100000 & 00011111 = 00000000
 */
#define gRingBufSize 127

struct RingBuffer{
	uns8 data[gRingBufSize + 1];
	uns8 read;
	uns8 write;
	bit error_full;
};
extern struct RingBuffer g_RingBuf;

/**
 * Some macros 
 */
#define RingBufInc(x) ((x + 1) & gRingBufSize)
#define RingBufClearError g_RingBuf.error_full = FALSE
#define RingBuf_HasError (g_RingBuf.error_full)
#define RingBufIsNotEmpty (g_RingBuf.write != g_RingBuf.read)

/**
 * Initialize the ring buffer and all associated variables
 */
void RingBuf_Init(void);

/**
 * This function will increment the read pointer of the
 * ring buffer and return the byte from the previous position.
 * WARNING: never call this function on an empty buffer!
 * Test with <RingBufIsNotEmpty> for data first!
 */
char RingBuf_Get(void);

/**
 * If the buffer is not full, value is added to the ring
 * at the current position of <gRingBufWrite> and <gRingBufWrite>
 * is incremented by one position.
 * 
 * If the buffer is already full, <g_error_ringbuff> is set.
 */
void RingBuf_Put(char value);
#endif /* #ifndef _RINGBUF_H_ */
