/*
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.

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

#ifndef _TRACE_H_
#define _TRACE_H_
#define ZONE_ERROR   0x00000001
#define ZONE_WARNING 0x00000002
#define ZONE_INFO    0x00000004
#define ZONE_VERBOSE 0x00000008

#include "platform.h"
#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#ifdef DEBUG
extern struct RingBuffer g_TraceBuf;
#ifdef __CC8E__
void Trace_Init();

void Trace_String(const char *string);

void Trace_Number(const uns8 input);

void Trace_Hex(const uns8 input);

void Trace_Hex16(const uns16 input);

void Trace_Char(const uns8 input);

uns8 Trace_Print(uns8 *pArray, const uns16 arraySize);
#elif TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    #define Trace_Init(x)
    #define Trace_String(str)
    #define Trace_Number(input)
    #define Trace_Hex(hex)
    #define Trace_Hex16(hex)
    #define Trace_Print(array, arraySize)
    #define Trace_Char(x)
    #define TraceBuffer(ZONE, BUFFER, LENGTH, BUFFER_FORMAT, ...)
	#define Trace(ZONE, ...) do { \
		if(g_DebugZones & (ZONE)) { \
			printf("%s:%u: ", __FILE__, __LINE__); \
			printf(__VA_ARGS__); \
		} \
} \
	while(0)

#else
	#include "stdio.h"
	#define Trace_Init(x)
	#define Trace_String(str) do { printf("%s", str); } \
	while(0)
	#define Trace_Number(input) do { printf("%04x", input); } \
	while(0)
	#define Trace_Hex(hex) do { printf("%02x ", hex); } \
	while(0)
	#define Trace_Hex16(hex) do { printf("%04x ", hex); } \
	while(0)
uns8 Trace_Print(uns8 *pArray, const uns16 arraySize);
	#define Trace_Char(input) do { printf("%c", input); } \
	while(0)

	#define TraceBuffer(ZONE, BUFFER, LENGTH, BUFFER_FORMAT, ...) do { \
		if(g_DebugZones & (ZONE)) { \
			Trace(ZONE, __VA_ARGS__); \
			for(size_t i = 0; i < (LENGTH); i++) { \
				printf(BUFFER_FORMAT, ((uint8_t *)(BUFFER))[i]); \
			} \
				printf("\n"); \
		} \
} \
	while(0)

	#define Trace(ZONE, ...) do { \
		if(g_DebugZones & (ZONE)) { \
				printf("%s:%u: ", __FILE__, __LINE__); \
				printf(__VA_ARGS__); \
		} \
} \
	while(0)
#endif
#else
	#define Trace_Init(x)
	#define Trace_String(str)
	#define Trace_Number(input)
	#define Trace_Hex(hex)
	#define Trace_Hex16(hex)
uns8 Trace_Print(uns8 *pArray, const uns16 arraySize);
	#define Trace_Char(x)
	#define TraceBuffer(ZONE, BUFFER, LENGTH, BUFFER_FORMAT, ...)
	#define Trace(ZONE, ...)
#endif
#endif /* #ifndef _TRACE_H_ */

