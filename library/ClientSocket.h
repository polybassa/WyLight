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

#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_

#include "WiflyControlException.h"

#include <cassert>
#include <ostream>
#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>

namespace WyLight {

/**
 * Helper class to simplify handling of sockaddr_in
 */
	struct Ipv4Addr : public sockaddr_in {
		Ipv4Addr(uint32_t addr, uint16_t port) {
			memset(this, 0, sizeof(*this));
			sin_family = AF_INET;
			sin_port = htons(port);
			sin_addr.s_addr = htonl(addr);
		};
	};

/**
 * Abstract base class controlling the low level socket file descriptor
 */
	class ClientSocket
	{
	public:
		/**
		 * Aquire the low level socket file descriptor
		 * @param addr IPv4 address in host byte order
		 * @param port IPv4 port number in host byte order
		 * @param style either SOCK_DGRAM (udp) or SOCK_STREAM for tcp socket
		 * @throw FatalError if the creation of the bsd sock descriptor fails
		 */
		ClientSocket(uint32_t addr, uint16_t port, int style) throw (FatalError);

		/**
		 * Destructor releasing the socket file descriptor
		 */
		virtual ~ClientSocket();

		/**
		 * wait for data on the low level socket
		 * @param timeout to wait for data, to block indefinitly use NULL, which is default
		 * @return true if select() timed out, false if data is ready
		 * @throw FatalError if something very unexpected happens
		 */
		bool Select(timeval *timeout) const throw (FatalError);

		/**
		 * Interface to send a data frame with a given length, you have to implement
		 * this function in child classes
		 */
		virtual size_t Send(const uint8_t *frame, size_t length) const = 0;

	protected:
		/**
		 * low level socket file descriptor
		 */
		const int mSock;

		/**
		 * IPv4 address of listening or target port
		 */
		Ipv4Addr mSockAddr;
	};

/**
 * Wrapper to make tcp socket handling more easy
 */
	class TcpSocket : public ClientSocket
	{
	public:
		/**
		 * @param Addr IPv4 address in host byte order
		 * @param port IPv4 port number in host byte order
		 * @throw FatalError if the base class constructor fails @see ClientSocket#ClientSocket
		 * @throw ConnectionLost if connect() fails on the internal socket
		 */
		TcpSocket(uint32_t Addr, uint16_t port) throw (ConnectionLost, FatalError);

		/**
		 * Receive data from the remote socket.
		 * @param pBuffer to store the read data
		 * @param length size of the pBuffer
		 * @param timeout to wait for data, to block indefinitly use NULL, which is default
		 * @return number of bytes read into \<pBuffer\>
		 * @throw FatalError if something very unexpected happens
		 */
		size_t Recv(uint8_t *pBuffer, size_t length, timeval *timeout = NULL) const throw (FatalError);

		/**
		 * @see ClientSocket#Send
		 */
		virtual size_t Send(const uint8_t *frame, size_t length) const;
	};

/**
 * Wrapper to make udp socket handling more easy
 */
	class UdpSocket : public ClientSocket
	{
	public:
		/**
		 * @param addr IPv4 address in host byte order
		 * @param port IPv4 port number in host byte order
		 * @param doBind set to true to
		 * @param enableBroadcast use 1 to enable broadcast else set 0 (default)
		 * @throw FatalError if the base class constructor fails
		 */
		UdpSocket(uint32_t addr, uint16_t port, bool doBind = true, int enableBroadcast = 0) throw (FatalError);

		/**
		 * Receive data from the remote socket.
		 * @param pBuffer to store the read data
		 * @param length size of the pBuffer
		 * @param timeout to wait for data, to block indefinitly use NULL, which is default
		 * @param remoteAddr pointer to a struct where the senders address should be stored, this param is optional use NULL to ignore it
		 * @param remoteAddrLength size of the struct remoteAddr is pointing to, after a successfull call with no NULL pointers in remoteAddrLength and remoteAddr it will point to the size of the written remoteAddr struct
		 * @return number of bytes read into \<pBuffer\>, 0 in case of a timeout
		 * @throw FatalError if something very unexpected happens
		 */
		size_t RecvFrom(uint8_t *pBuffer, size_t length, timeval *timeout = NULL, struct sockaddr *remoteAddr = NULL, socklen_t *remoteAddrLength = NULL) const throw (FatalError);

		virtual size_t Send(const uint8_t *frame, size_t length) const;
	};
}
#endif /* #ifndef _CLIENTSOCKET_H_ */

