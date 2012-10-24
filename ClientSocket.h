/**
		Copyright (C) 2012 Nils Weiss, Patrick Bruenn.

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
	protected:
		const int mSock;
		struct sockaddr_in mSockAddr;

	public:
		ClientSocket(const char* pAddr, short port, int style);
		virtual ~ClientSocket();
		virtual size_t Recv(unsigned char* pBuffer, size_t length, unsigned long timeoutTmms = 0) const = 0;
		virtual int Send(const unsigned char* frame, size_t length) const = 0;
};

class TcpSocket : public ClientSocket
{
	public:
		TcpSocket(const char* pAddr, short port);
		virtual size_t Recv(unsigned char* pBuffer, size_t length, unsigned long timeoutTmms = 0) const;
		virtual int Send(const unsigned char* frame, size_t length) const;
};

class UdpSocket : public ClientSocket
{
	public:
		UdpSocket(const char* pAddr, short port);
		virtual size_t Recv(unsigned char* pBuffer, size_t length, unsigned long timeoutTmms = 0) const;
		virtual int Send(const unsigned char* frame, size_t length) const;
};

#ifdef UNIT_TEST
class TestSocket : public ClientSocket
{
	public:
		TestSocket(const char* pAddr, short port);
		virtual size_t Recv(unsigned char* pBuffer, size_t length, unsigned long timeoutTmms = 0) const;
		virtual int Send(const unsigned char* frame, size_t length) const;
};
#endif /* #ifndef UNIT_TEST */
#endif /* #ifndef _CLIENTSOCKET_H_ */

