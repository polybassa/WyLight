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
#ifndef ANDROID
	close(mSock);
#endif
}

int ClientSocket::Send(char* frame, size_t length) const
{
	return sendto(mSock, frame, length, 0, (struct sockaddr*)&mSockAddr, sizeof(mSockAddr));
}

