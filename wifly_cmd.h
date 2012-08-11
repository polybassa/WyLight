/**
 Copyright (C) 2012 Nils Weiss, Patrick Brünn.
 
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

//*********************** ENUMERATIONS *********************************************
#define STX 0xFF
#define SET_COLOR 0xFD
#define SET_FADE 0xFC
#define SET_RUN 0xFB
#define WAIT 0xFE
#define SET_ON 0xFA
#define SET_OFF 0xF9
#define DELETE 0xF8
#define LOOP_ON 0xF7
#define LOOP_OFF 0xF6
#define START_BL 0xF5
#define SET_RTC 0xF4 			/* FRAME: <STX><LEN><SET_RTC><YEAR><MONTH><DAY><WDAY><HOUR><MIN><SEC><CRC><CRC> */
#define GET_RTC 0xF3
#define DISPLAY_RTC 0xF2

//*********************** STRUCT DECLARATION *********************************************
struct cmd_set_color {
	uns8 addr[4];
	uns8 red;
	uns8 green;
	uns8 blue;
	uns8 reserved[2];
};
#ifdef X86
#pragma pack(1)
#endif
struct cmd_set_fade {
	uns8 addr[4];
	uns8 red;
	uns8 green;
	uns8 blue;
	uns16 fadeTmms; //fadetime in ms
};

struct cmd_wait {
	uns8 valueH;
	uns8 valueL;
	uns8 reserved[7];
};

struct cmd_set_run {
	uns8 dummy;
};

struct led_cmd {
	uns8 cmd;
	union {
		struct cmd_set_color set_color;
		struct cmd_set_fade set_fade;
		struct cmd_set_run set_run;
		struct cmd_wait wait;
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
#endif /* #ifndef _WIFLY_CMD_H_ */
