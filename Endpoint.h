/**
		Copyright (C) 2013 Nils Weiss, Patrick Bruenn.

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

#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_
#include <cassert>
#include <ostream>
#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>

class Endpoint
{
	public:
		Endpoint(sockaddr_storage& addr, const size_t size, uint16_t port)
		{
			assert(sizeof(sockaddr_in) == size);
			mAddr = ntohl(((sockaddr_in&)addr).sin_addr.s_addr);
			mPort = ntohs(port);
		};

		Endpoint(uint32_t addr = 0, uint16_t port = 0)
			: mAddr(addr), mPort(port)
		{
		};

		friend std::ostream& operator << (std::ostream& out, const Endpoint& ref)
		{
			return out << ((ref.mAddr & 0xff000000 ) >> 24) << '.'
								 << ((ref.mAddr & 0x00ff0000 ) >> 16) << '.'
								 << ((ref.mAddr & 0x0000ff00 ) >> 8) << '.'
								 << (ref.mAddr & 0x000000ff )
								 << ':' << ref.mPort;
		};

		/* 
		 * @return ipv4 address(A) and port(P) as a combined 64 bit value 0xAAAAAAAA0000PPPP
		 */
		uint64_t AsUint64(void) const
		{
			return ((uint64_t)mAddr << 32) | mPort; 
		};

		uint32_t GetIp(void) const
		{
			return mAddr;
		};

		uint16_t GetPort(void) const
		{
			return mPort;
		};

		bool IsValid(void) const
		{
			return (0 != mAddr) && (0 != mPort);
		};

	private:
		uint32_t mAddr;
		uint16_t mPort;
};
#endif /* #ifndef _ENDPOINT_H_ */

