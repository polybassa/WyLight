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
#include <cassert>
#include <ostream>
#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>

class Endpoint
{
	public:
		Endpoint(sockaddr_storage& addr, const size_t size, uint16_t port) {
			assert(sizeof(sockaddr_in) == size);
			m_Addr = ntohl(((sockaddr_in&)addr).sin_addr.s_addr);
			m_Port = ntohs(port);
		};

		Endpoint(uint32_t addr = 0, uint16_t port = 0) : m_Addr(addr), m_Port(port) {
		};

		friend std::ostream& operator << (std::ostream& out, const Endpoint& ref) {
			return out << std::hex << ref.m_Addr << ':' << std::dec << ref.m_Port;
		};

		/* 
		 * @return ipv4 address(A) and port(P) as a combined 64 bit value 0xAAAAAAAA0000PPPP
		 */
		uint64_t AsUint64(void) const {
			return ((uint64_t)m_Addr << 32) | m_Port; 
		};

		bool IsValid(void) const {
			return (0 != m_Addr) && (0 != m_Port);
		};

		uint32_t m_Addr;
		uint16_t m_Port;
};

class ClientSocket
{
	protected:
		const int mSock;
		struct sockaddr_in mSockAddr;

	public:
		ClientSocket(uint32_t addr, uint16_t port, int style);
		virtual ~ClientSocket();
		virtual size_t Recv(uint8_t* pBuffer, size_t length, timeval* timeout = NULL) const = 0;
		virtual size_t Send(const uint8_t* frame, size_t length) const = 0;
};

class TcpSocket : public ClientSocket
{
	public:
		TcpSocket(uint32_t Addr, uint16_t port);
		virtual size_t Recv(uint8_t* pBuffer, size_t length, timeval* timeout = NULL) const;
		virtual size_t Send(const uint8_t* frame, size_t length) const;
};

class UdpSocket : public ClientSocket
{
	public:
		/**
		 * @param enableBroadcast use 1 to enable broadcast else set 0
		 */
		UdpSocket(uint32_t addr, uint16_t port, bool doBind = true, int enableBroadcast = 0);
		virtual size_t Recv(uint8_t* pBuffer, size_t length, timeval* timeout = NULL) const;
		virtual size_t RecvFrom(uint8_t* pBuffer, size_t length, timeval* timeout = NULL, struct sockaddr* remoteAddr = NULL, socklen_t* remoteAddrLength = NULL) const;
		virtual size_t Send(const uint8_t* frame, size_t length) const;
};
#endif /* #ifndef _CLIENTSOCKET_H_ */

