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

