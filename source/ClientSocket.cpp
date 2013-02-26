/**
		Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.

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
#include "trace.h"

#include <arpa/inet.h>
#include <sys/select.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <stdio.h>

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING;

ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style)
	: mSock(socket(AF_INET, style, 0))
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

TcpSocket::TcpSocket(uint32_t addr, uint16_t port)
	: ClientSocket(addr, port, SOCK_STREAM)
{
	if(connect(mSock, reinterpret_cast<sockaddr*>(&mSockAddr), sizeof(mSockAddr)) < 0)
	{
		Trace(ZONE_ERROR, "Connection to 0x%08x:%05u failed\n", addr, port);
	}
}

size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const
{
	/* prepare socket set for select() */
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(mSock, &readSockets);
	
	/* wait for receive data and check if socket was correct */
	if((1 == select(mSock + 1, &readSockets, NULL, NULL, timeout))
	&& (FD_ISSET(mSock, &readSockets)))
	{
		size_t bytesRead = recv(mSock, pBuffer, length, 0);
		if(bytesRead > 0)
		{
			Trace(ZONE_INFO, "Receiving %u bytes\n", bytesRead);
			return static_cast<size_t>(bytesRead);
		}
	}
	
	/* some error occur */
	return 0;
}

size_t TcpSocket::Send(const uint8_t* frame, size_t length) const
{
	TraceBuffer(ZONE_INFO, frame, length, "%02x ", "Sending %u bytes: ", length);
	return send(mSock, frame, length, 0);
}

UdpSocket::UdpSocket(uint32_t addr, uint16_t port, bool doBind, int enableBroadcast)
	: ClientSocket(addr, port, SOCK_DGRAM)
{
	setsockopt(mSock, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast));
	if(doBind && 0 != bind(mSock, reinterpret_cast<struct sockaddr *>(&mSockAddr), sizeof(struct sockaddr)))
	{
				Trace(ZONE_ERROR, "Bind failure!\n");
	      pthread_exit(NULL);
	      return;
	}
}

size_t UdpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const
{
	Trace(ZONE_ERROR, "Not implemented\n");
	return 0;
}

size_t UdpSocket::RecvFrom(uint8_t* pBuffer, size_t length, timeval* timeout, struct sockaddr* remoteAddr, socklen_t* remoteAddrLength) const
{
	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(mSock, &readSet);
	if(0 < select(mSock+1, &readSet, NULL, NULL, timeout))
	{
		return recvfrom(mSock, pBuffer, length, 0, remoteAddr, remoteAddrLength);
	}
	return 0;
}

size_t UdpSocket::Send(const uint8_t* frame, size_t length) const
{
	TraceBuffer(ZONE_INFO, frame, length, "%02x ", "Sending %u bytes: ", length);
	return sendto(mSock, frame, length, 0, (struct sockaddr*)&mSockAddr, sizeof(mSockAddr));
}

