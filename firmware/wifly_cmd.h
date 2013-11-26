
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

#ifndef _WIFLY_CMD_H_
#define _WIFLY_CMD_H_

#include "platform.h"
#include "rtc.h"
#include "RingBuf.h"
#include "timer.h"
#include "error.h"
#include "VersionFile.h"

#ifdef __cplusplus
#include <algorithm>
#include <ostream>
#include <stdint.h>
#include <iomanip>
#endif

//*********************** ENUMERATIONS *********************************************
#define STX 0x0F
#define DLE 0x05
#define ETX 0x04

#define FW_IDENT 0x0D
#define BL_IDENT 0x0F

#define WAIT 0xFE
#define SET_FADE 0xFC
#define SET_GRADIENT 0xF9
#define CLEAR_SCRIPT 0xF8
#define LOOP_ON 0xF7
#define LOOP_OFF 0xF6
#define START_BL 0xF5
#define SET_RTC 0xF4
#define GET_RTC 0xF3
#define SET_COLOR_DIRECT 0xF1
#define GET_CYCLETIME 0xF0
#define GET_TRACE 0xEE
#define GET_FW_VERSION 0xED
#define FW_STARTED 0xEC

#define LOOP_INFINITE 0

#define FW_MAX_MESSAGE_LENGTH 128

//*********************** STRUCT DECLARATION *********************************************
struct __attribute__((__packed__)) cmd_set_fade {
	uns8 addr[4];
	uns8 red;
	uns8 green;
	uns8 blue;
	uns8 parallelFade;
	uns16 fadeTmms; //fadetime in ten ms
#ifdef __cplusplus
	void Set(uint32_t __addr, uint32_t argb, uint8_t parallel, uint16_t fadeTime) {
		addr[3] = (uint8_t)(__addr >> 24);
		addr[2] = (uint8_t)(__addr >> 16);
		addr[1] = (uint8_t)(__addr >> 8);
		addr[0] = (uint8_t)(__addr);
		red = (uint8_t)(argb >> 16);
		green = (uint8_t)(argb >> 8);
		blue = (uint8_t)(argb);
		parallelFade = parallel;
		fadeTmms = htons(std::max((uint16_t)1, fadeTime));
	};

	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		const uint32_t addrVal = addr[3] << 24 | addr[2] << 16 | addr[1] << 8 | addr[0];
		const uint32_t argbVal = red << 16 | green << 8 | blue;
		return out << "0x" << std::hex << addrVal << " 0x" << std::hex << argbVal << ' ' << std::dec << ntohs(fadeTmms) << ' ' << (int)parallelFade;
	};
#endif /* #ifdef __cplusplus */
};

struct __attribute__((__packed__)) cmd_set_gradient {
	uns8 red_1;
	uns8 green_1;
	uns8 blue_1;
	uns8 red_2;
	uns8 green_2;
	uns8 blue_2;
	uns8 parallelAndOffset; //most significant bit is the parallel bit, the 7 lower bit's hold the number of the offset
	uns8 numberOfLeds;
	uns16 fadeTmms; //fadetime in ten ms

#ifdef __cplusplus
	void Set(uint32_t argb_1, uint32_t argb_2, uint8_t parallel, uint8_t offset, uint8_t length, uint16_t fadeTime) {
//		if (offset > 0x7f) throw FatalError("Invalid Parameter, offset is greater than 127");
		red_1 = (uint8_t)(argb_1 >> 16);
		green_1 = (uint8_t)(argb_1 >> 8);
		blue_1 = (uint8_t)(argb_1);
		red_2 = (uint8_t)(argb_2 >> 16);
		green_2 = (uint8_t)(argb_2 >> 8);
		blue_2 = (uint8_t)(argb_2);
		parallelAndOffset = (parallel ? 0x80 : 0x00) | (offset & 0x7F);
		numberOfLeds = length;
		fadeTmms = htons(std::max((uint16_t)1, fadeTime));
	};

	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		const uint32_t argbVal_1 = red_1 << 16 | green_1 << 8 | blue_1;
		const uint32_t argbVal_2 = red_2 << 16 | green_2 << 8 | blue_2;
		return out << "0x" << std::hex << argbVal_1 << " 0x" << std::hex << argbVal_2 << ' ' << std::dec << ntohs(fadeTmms) << ' ' << (int)(parallelAndOffset & 0x7F) << ' ' << (int)numberOfLeds << ' ' << (int)(parallelAndOffset & 0x80);
	};
#endif
};

struct __attribute__((__packed__)) cmd_loop_end {
	uns8 startIndex; /* pointer to the corresponding cmd_loop_start */
	uns8 counter; /* current loop counter, used due processing */
	uns8 numLoops; /* number of programmed loops f.e. LOOP_INFINITE */
	uns8 depth; /* number of recursions */
};

struct __attribute__((__packed__)) cmd_wait {
	uns16 waitTmms;
};

struct __attribute__((__packed__)) cmd_set_color_direct {
#ifdef __CC8E__
	uns8 ptr_led_array;
#else
	uns8 ptr_led_array[NUM_OF_LED * 3];
#ifdef __cplusplus
	void Set(const uint8_t red, const uint8_t green, const uint8_t blue, const uint32_t addr)
	{
		memset(ptr_led_array, 0, sizeof(ptr_led_array));
		uint8_t *pCur = ptr_led_array;
		for(uint32_t mask = 0x1; mask > 0; mask = mask << 1) {
			static_assert(sizeof(mask) * 8 * 3 == sizeof(ptr_led_array),
				      "This trick only works if the mask field overflows to zero exactly with the last led");
			if(addr & mask) {
				*pCur = red;
				*(++pCur) = green;
				*(++pCur) = blue;
				++pCur;
			} else {
				pCur += 3;
			}
		}
	};

	void Set(const uint8_t *pBuffer, size_t bufferLength)
	{
		bufferLength = std::min(bufferLength, sizeof(ptr_led_array));
		memcpy(ptr_led_array, pBuffer, bufferLength);
		memset(ptr_led_array + bufferLength, 0, sizeof(ptr_led_array) - bufferLength);
	};
#endif
#endif
};

struct __attribute__((__packed__)) response_frame {
	uns16 length;           /* only for Firmware, do not use in Client */
	uns8 cmd;
	ErrorCode state;
	union __attribute__((__packed__)) {
		struct rtc_time time;
		uns16 versionData;
		uns8 trace_string[RingBufferSize];
		uns16 max_cycle_times[CYCLETIME_METHODE_ENUM_SIZE];
	}
	data;
};

struct __attribute__((__packed__)) led_cmd {
	uns8 cmd;
	union {
		struct cmd_set_fade set_fade;
		struct cmd_wait wait;
		struct cmd_loop_end loopEnd;
		struct rtc_time set_rtc;
		struct cmd_set_color_direct set_color_direct;
		struct cmd_set_gradient set_gradient;
	}
	data;
};

#endif /* #ifndef _WIFLY_CMD_H_ */
