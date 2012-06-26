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

//*********************** STRUCT DECLARATION *********************************************
struct cmd_set_color {
	char addr[4];
	uns8 red;
	uns8 green;
	uns8 blue;
	char reserved[2];
};

struct cmd_set_fade {
	char addr[4];
	char red;
	char green;
	char blue;
	char timevalue;
	char reserved0;
};

struct cmd_wait {
	char valueH;
	char valueL;
	char reserved[7];
};

struct cmd_set_run {
	char dummy;
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
#define FRAMELENGTH (sizeof(struct led_cmd) + 5)			// *** max length of one commandframe
#endif /* #ifndef _WIFLY_CMD_H_ */
