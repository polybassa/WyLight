#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_
#include <sys/socket.h>
#include <netinet/in.h>

class ClientSocket
{
	private:
		const int mSock;
		struct sockaddr_in mSockAddr;

	public:
		ClientSocket(long addr, short port);
		~ClientSocket();
		int Send(char* frame, size_t length) const;
};
#endif /* #ifndef _CLIENTSOCKET_H_ */

