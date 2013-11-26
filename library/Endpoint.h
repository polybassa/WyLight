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
#include <atomic>
#include <cassert>
#include <ostream>
#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>

namespace WyLight {

	class Endpoint
	{
	public:
		Endpoint(sockaddr_storage& addr, const size_t size, uint16_t port, std::string devId = "")
			: mScore(0)
		{
			assert(sizeof(sockaddr_in) == size);
			mIp = ntohl(((sockaddr_in&)addr).sin_addr.s_addr);
			mPort = ntohs(port);
			mDeviceId = devId;
		};

		Endpoint(uint32_t ip = 0, uint16_t port = 0, uint8_t score = 0, std::string devId = "")
			: mIp(ip), mPort(port), mScore(score), mDeviceId(devId)
		{};

		bool operator<(const Endpoint& ref) const {
			return (mIp < ref.GetIp())
			       || ((mIp == ref.GetIp()) && (mPort < ref.GetPort()));
		};

		friend std::ostream& operator << (std::ostream& out, const Endpoint& ref)
		{
			return out << (int)ref.mScore << ' '
				   << ((ref.mIp & 0xff000000) >> 24) << '.'
				   << ((ref.mIp & 0x00ff0000) >> 16) << '.'
				   << ((ref.mIp & 0x0000ff00) >> 8) << '.'
				   << (ref.mIp & 0x000000ff)
				   << ':' << ref.mPort
				   << "  :  " << ref.mDeviceId;
		};

		friend bool operator== (const Endpoint& lhs, const Endpoint& rhs)
		{
			if((lhs.mDeviceId == rhs.mDeviceId) &&
			   (lhs.mIp == rhs.mIp) &&
			   (lhs.mPort == rhs.mPort) &&
			   (lhs.mScore == rhs.mScore)
			   ) {
				return true;
			} else {
				return false;
			}
		}

		friend bool operator!= (const Endpoint& lhs, const Endpoint& rhs)
		{
			return !(lhs == rhs);
		}


		/*
		 * @return ipv4 address(A) and port(P) as a combined 64 bit value 0xAAAAAAAA0000PPPP
		 */
		uint64_t AsUint64(void) const {
			return ((uint64_t)mIp << 32) | mPort;
		};

		std::string GetDeviceId(void) const {
			return mDeviceId;
		};

		uint32_t GetIp(void) const {
			return mIp;
		};

		uint16_t GetPort(void) const {
			return mPort;
		};

		uint8_t GetScore(void) const {
			return mScore;
		};

		void SetDeviceId(const std::string& deviceId) {
			mDeviceId = deviceId;
		};

		void SetScore(const uint8_t& score) {
			mScore = score;
		};

		/*
		 * Increment score
		 * @return reference to itself
		 */
		Endpoint& operator ++(void) {
			if(mScore < 255) ++mScore;
			return *this;
		}

		bool IsValid(void) const {
			return (0 != mIp) && (0 != mPort);
		};

	private:
		uint32_t mIp;
		uint16_t mPort;
		uint8_t mScore;
		std::string mDeviceId;
	};
}
#endif /* #ifndef _ENDPOINT_H_ */

