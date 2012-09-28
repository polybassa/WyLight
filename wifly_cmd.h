
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

#ifndef _WIFLY_CMD_H_
#define _WIFLY_CMD_H_

#include "platform.h"
#include "rtc.h"

//*********************** ENUMERATIONS *********************************************
#define STX 0xFF
#define WAIT 0xFE
#define SET_COLOR 0xFD
#define SET_FADE 0xFC
#define SET_RUN 0xFB
#define SET_ON 0xFA
#define SET_OFF 0xF9
#define DELETE 0xF8
#define LOOP_ON 0xF7
#define LOOP_OFF 0xF6
#define START_BL 0xF5
#define SET_RTC 0xF4 			/* FRAME: <STX><LEN><SET_RTC><SEC><MIN><HOUR><DAY><MONTH><YEAR><WDAY><CRC><CRC> */
#define GET_RTC 0xF3
#define DISPLAY_RTC 0xF2
#define SET_COLOR_DIRECT 0xF1
#define GET_CYCLETIME 0xF0
#define ADD_COLOR 0xEF

#define LOOP_INFINITE 0

//*********************** STRUCT DECLARATION *********************************************
struct cmd_add_color {
//TODO add this later, when we can handle longer cmd_frames
//TODO	uns8 addr[4];
	uns8 red;
	uns8 green;
	uns8 blue;
//TODO uns8 hour;
	uns8 minute;
	uns8 second;
};

struct cmd_set_color {
	uns8 addr[4];
	uns8 red;
	uns8 green;
	uns8 blue;
};

#ifdef X86
#pragma pack(1)
#endif
struct cmd_set_fade {
	uns8 addr[4];
	uns8 red;
	uns8 green;
	uns8 blue;
	uns8 parallelFade;
	uns16 fadeTmms; //fadetime in ms
};

struct cmd_loop_end {
	uns8 startIndex; /* pointer to the corresponding cmd_loop_start */
	uns8 counter; /* current loop counter, used due processing */
	uns8 numLoops; /* number of programmed loops f.e. LOOP_INFINITE */
	uns8 depth; /* number of recursions */
};

struct cmd_wait {
	uns16 waitTmms;
};

struct cmd_set_run {
	uns8 direction;
	uns16 durationTmms;
	uns16 fadeTmms;
};

struct cmd_set_color_direct {
	uns8 ptr_led_array;
};

struct led_cmd {
	uns8 cmd;
	union {
		struct cmd_add_color add_color;
		struct cmd_set_color set_color;
		struct cmd_set_fade set_fade;
		struct cmd_set_run set_run;
		struct cmd_wait wait;
		struct cmd_loop_end loopEnd;
		struct rtc_time set_rtc;
		struct cmd_set_color_direct set_color_direct;
	}data;
};

struct cmd_frame {
	uns8 stx;
	uns8 length;
	struct led_cmd led;
	uns8 crcHigh;
	uns8 crcLow;
};
#define FRAMELENGTH (sizeof(struct cmd_frame) + 1)			// *** max length of one commandframe
//TODO remove this line #define FRAMELENGTH (NUM_OF_LED * 3 + 8)
#endif /* #ifndef _WIFLY_CMD_H_ */
