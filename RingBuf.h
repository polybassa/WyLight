#ifndef _RINGBUF_H_
#define _RINGBUF_H_

#ifdef X86
typedef char bit;
#endif /* #ifdef X86 */

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
#define gRingBufSize 15

struct RingBuffer{
	char data[gRingBufSize + 1];
	char read;
	char write;
	bit error_full;
};
extern struct RingBuffer gRingBuf;

/**
 * Some macros 
**/

#define RingBufInc(x) ((x + 1) & gRingBufSize)
#define RingBufClearError gRingBuf.error_full = FALSE
#define RingBufHasError (gRingBuf.error_full)
#define RingBufIsNotEmpty (gRingBuf.write != gRingBuf.read)


/**
 * Initialize the ring buffer and all associated variables
**/
void RingBufInit(void);

/**
 * This function will increment the read pointer of the
 * ring buffer and return the byte from the previous position.
 * WARNING: never call this function on an empty buffer!
 * Test with <RingBufIsNotEmpty> for data first!
 */
char RingBufGet(void);

/**
 * If the buffer is not full, value is added to the ring
 * at the current position of <gRingBufWrite> and <gRingBufWrite>
 * is incremented by one position.
 * 
 * If the buffer is already full, <g_error_ringbuff> is set.
 */
void RingBufPut(char value);
#endif /* #ifndef _RINGBUF_H_ */
