 // Include-Datei zur Verwendung eines Ringspeichers
 //
 //
 // Nils Weiﬂ
 // 16.04.2012
 // Compiler CC5x

#pragma codepage 1
#define BUFFERSIZE 20

#message ATTENTION: Global Variables are defined in ringbuffer.h

static bit g_RingBuf_readable;
static bit g_RingBuf_writeable;

unsigned char *p_read;
unsigned char *p_write;

unsigned char g_ringbuffer[BUFFERSIZE];

void RingBufInit(void);
void RingBufPut(char data);
char RingBufGet(void);
char* ringbuff_inc_p(char* pointer);

#include "unused_files\ringbuffer.c"