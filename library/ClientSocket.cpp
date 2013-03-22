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

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;


ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) throw (FatalError) 
	: mSock(socket(AF_INET, style, 0)) 
{
	if( -1 == mSock) throw FatalError("Create socket failed");

	memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_port = htons(port);
	mSockAddr.sin_addr.s_addr = htonl(addr);
}

ClientSocket::~ClientSocket()
{
	close(mSock);
}

bool ClientSocket::Select(timeval* timeout) const throw (FatalError)
{
	/* prepare socket set for select() */
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(mSock, &readSockets);
	
	/* wait for receive data */
	const int selectState = select(mSock + 1, &readSockets, NULL, NULL, timeout);
	if(0 == selectState)
	{
		Trace(ZONE_INFO, "Select timed out\n");
		return true;
	}

	/* and check if socket was correct */
	if((1 != selectState) || (!FD_ISSET(mSock, &readSockets)))
	{
		throw FatalError("something strange happen in select() called by TcpSocket::Recv()");
	}
	return false;
}

TcpSocket::TcpSocket(uint32_t addr, uint16_t port) throw (ConnectionLost, FatalError)
	: ClientSocket(addr, port, SOCK_STREAM)
{
	if(0 != connect(mSock, reinterpret_cast<sockaddr*>(&mSockAddr), sizeof(mSockAddr)))
	{
		throw ConnectionLost("connect() failed", addr, port);
	}
}

size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const throw(FatalError)
{
	return Select(timeout) ? 0 : recv(mSock, pBuffer, length, 0);
}

size_t TcpSocket::Send(const uint8_t* frame, size_t length) const
{
	TraceBuffer(ZONE_INFO, frame, length, "%02x ", "Sending on socket 0x%04x, %zu bytes: ", mSock, length);
	return send(mSock, frame, length, 0);
}

UdpSocket::UdpSocket(uint32_t addr, uint16_t port, bool doBind, int enableBroadcast) throw (FatalError)
	: ClientSocket(addr, port, SOCK_DGRAM)
{
	if(0 != setsockopt(mSock, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)))
	{
		throw FatalError("setsockopt() failed");
	}

	if(doBind && 0 != bind(mSock, reinterpret_cast<struct sockaddr *>(&mSockAddr), sizeof(struct sockaddr)))
	{
		throw FatalError("Bind UDP socket failed");
	}
}

size_t UdpSocket::RecvFrom(uint8_t* pBuffer, size_t length, timeval* timeout, struct sockaddr* remoteAddr, socklen_t* remoteAddrLength) const throw (FatalError)
{
	return Select(timeout) ? 0 : recvfrom(mSock, pBuffer, length, 0, remoteAddr, remoteAddrLength);
}

size_t UdpSocket::Send(const uint8_t* frame, size_t length) const
{
	TraceBuffer(ZONE_INFO, frame, length, "%02x ", "Sending %zu bytes: ", length);
	return sendto(mSock, frame, length, 0, (struct sockaddr*)&mSockAddr, sizeof(mSockAddr));
}

