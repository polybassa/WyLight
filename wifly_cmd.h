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
	char red;
	char green;
	char blue;
	char reserved0;
	char reserved1;
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
#define FRAMELENGTH (sizeof(struct led_cmd) + 5)			// *** max length of one commandframe
#endif /* #ifndef _WIFLY_CMD_H_ */
