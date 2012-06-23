#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_
#include <sys/socket.h>
#include <netinet/in.h>

#include "wifly_cmd.h"
class ClientSocket
{
	private:
		const int mSock;
		struct sockaddr_in mSockAddr;

	public:
		ClientSocket(long addr, short port);
		~ClientSocket();
		int Send(char* buf, size_t length);
};

static char cmd_set_color_red[] = {
		STX, (uns8)sizeof(struct cmd_set_color) + 2,
		SET_COLOR,
		0xff, 0xff, 0xff, 0xff,
		128, 0, 0,
		0, 0,
		0xDE, 0xAD};

static char cmd_set_color_green[] = {
		STX, (uns8)sizeof(struct cmd_set_color) + 2,
		SET_COLOR,
		0xff, 0xff, 0xff, 0xff,
		0, 128, 0,
		0, 0,
		0xDE, 0xAD};

static char cmd_set_color_blue[] = {
		STX, (uns8)sizeof(struct cmd_set_color) + 2,
		SET_COLOR,
		0xff, 0xff, 0xff, 0xff,
		0, 0, 128,
		0, 0,
		0xDE, 0xAD};

static char cmd_set_fade_blue[] = {
		STX, (uns8)sizeof(struct cmd_set_color) + 2,
		SET_FADE,
		0xff, 0xff, 0xff, 0xff,
		0, 0, 128,
		10, 0,
		0xDE, 0xAD};
#endif /* #ifndef _CLIENTSOCKET_H_ */

