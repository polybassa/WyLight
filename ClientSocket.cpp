#include "ClientSocket.h"

#include <cstring>
#include <iostream>

ClientSocket::ClientSocket(long addr, short port)
	: mSock(socket(AF_INET, SOCK_DGRAM, 0))
{
	memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_port = htons(port);
	mSockAddr.sin_addr.s_addr = htonl(addr);
}

ClientSocket::~ClientSocket()
{
	close(mSock);
}

int ClientSocket::Send(char* buf, size_t length)
{
	return sendto(mSock, buf, length, 0, (struct sockaddr*)&mSockAddr, sizeof(mSockAddr));
}

void colorLoop(ClientSocket& socket)
{
	static const size_t NUM_COLORS = 3;
	char* buffer[NUM_COLORS] = {cmd_set_color_red, cmd_set_color_green, cmd_set_color_blue};
	for(int i = 0;;i++)
	{
		int bytesWritten = socket.Send(buffer[i % NUM_COLORS], sizeof(cmd_set_color_red));
		std::cout << "Send " << bytesWritten << " bytes" << std::endl;

		//wait a second
		sleep(1);
	}
}

int main(int argc, const char* argv[])
{
	ClientSocket socket(INADDR_ANY, 12345);
	colorLoop(socket);
	return 0;
}
