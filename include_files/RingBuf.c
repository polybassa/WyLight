
//#include "unused_files/RingBuf.h"
//#include "RingBuf.h"

#message Global variables are defined in RingBuf.c

struct RingBuffer gRingBuf;

void RingBufInit(void)
{
	gRingBuf.read = 0;
	gRingBuf.write = 0;
	gRingBuf.error_full = 0;
}

char RingBufGet(void)
{
	char result = gRingBuf.data[gRingBuf.read];
	gRingBuf.read = RingBufInc(gRingBuf.read);
	return result;
}

void RingBufPut(char value)
{
	char writeNext = RingBufInc(gRingBuf.write);
	if(writeNext != gRingBuf.read)
	{
		gRingBuf.data[gRingBuf.write] = value;
		gRingBuf.write = writeNext;
	}
	else gRingBuf.error_full = 1;
}

