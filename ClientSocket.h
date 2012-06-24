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
		int Send(struct cmd_frame* frame, size_t length) const;
};

class WiflyControl
{
	public:
		const ClientSocket mSock;
	private:
		struct cmd_frame mCmdFrame;//[sizeof(led_cmd) + 4];
	public:
		WiflyControl();
		void SetColor(char r, char g, char b);
};
#endif /* #ifndef _CLIENTSOCKET_H_ */

