#ifndef _COMMANDSTORAGE_H_
#define _COMMANDSTORAGE_H_
/** Changelog
 * 2012-05-08 pb:
 * - refactor functions to access and manage a buffer for led commands, which is stored in the eeprom
**/

#define CmdPointerAddr 0xff		// *** Address at EERPOM. Commandpointer indicates the nummer of commands
#define CmdLoopPointerAddr 0xfd // *** Address at EEPROM. CommandLoopPointer indicates the next command. Used in Loop-Mode

struct cmd_set_color {
	char addr[4];
	char red;
	char green;
	char blue;
	char reserved0;
	char reserved1;
};

struct cmd_set_fade {
	char dummy;
};

struct cmd_set_run {
	char dummy;
};

struct led_cmd {
	unsigned cmd;
	union {
		struct cmd_set_color set_color;
		struct cmd_set_fade set_fade;
		struct cmd_set_run set_run;
	}data;
};
#define CmdWidth sizeof(struct led_cmd)	// *** Number of Bytes for one command

/**
 * pDest: output buffer
 * movePtr: if set, the command pointer will be moved to the next command, after command was read
 * return: - 0, if no command available in buffer
 *         - pDest, if available command was written to pDest
**/
struct led_cmd* commandstorage_read(struct led_cmd *pDest, bit movePtr);

/**
 * pSrc: command which should be written to eeprom
 * length: of the payload data in pSrc
 * return: - TRUE, if command was successfully written to buffer
 *         - FALSE, if not
**/
bit commandstorage_write(char *pSrc, char length);

#endif /* #ifndef _COMMANDSTORAGE_H_ */

