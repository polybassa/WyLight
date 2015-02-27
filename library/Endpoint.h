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
#include "BroadcastMessage.h"
#include "WiflyColor.h"

namespace WyLight
{
class Endpoint {
public:
    enum TYPE {
        RN171,
        CC3200
    };

    Endpoint(const BroadcastMessage& msg, sockaddr_in* addr)
        : mIp(ntohl(addr->sin_addr.s_addr)),
        mPort(ntohs(msg.port)),
        mScore(1),
        mDeviceId(&msg.deviceId[0], strnlen(&msg.deviceId[0], sizeof(msg.deviceId))),
        mType(msg.IsRN171Broadcast() ? RN171 : CC3200)
    {}

    Endpoint(uint32_t            ip = 0,
             uint16_t            port = 0,
             uint8_t             score = 0,
             std::string         devId = "",
             enum Endpoint::TYPE type = RN171)
        : mIp(ip), mPort(port), mScore(score), mDeviceId(devId), mType(type)
    {}

    bool operator<(const Endpoint& ref) const
    {
        return (mIp < ref.GetIp())
               || ((mIp == ref.GetIp()) && (mPort < ref.GetPort()));
    }

    void WriteTo(std::ostream& out) const
    {
        out << (int)mScore << ' ' <<
            std::hex << GetIp() << ' ' <<
            std::dec << GetPort() << ' ' <<
            GetDeviceId() << '\n';
    }

    friend std::ostream& operator<<(std::ostream& out, const Endpoint& ref)
    {
        return out << (int)ref.mScore << ' ' <<
               ((ref.mIp & 0xff000000) >> 24) << '.' <<
               ((ref.mIp & 0x00ff0000) >> 16) << '.' <<
               ((ref.mIp & 0x0000ff00) >> 8) << '.' <<
               (ref.mIp & 0x000000ff) <<
               ':' << ref.mPort <<
               "  :  " << ref.mDeviceId;
    }

    friend std::istream& operator>>(std::istream& in, Endpoint& ref)
    {
        std::string ip;
        in >> ref.mScore >> ip >> ref.mPort >> ref.mDeviceId;
        ref.mIp = WiflyColor::ToARGB(ip);
        return in;
    }

    friend bool operator==(const Endpoint& lhs, const Endpoint& rhs)
    {
        if ((lhs.mDeviceId == rhs.mDeviceId) &&
            (lhs.mIp == rhs.mIp) &&
            (lhs.mPort == rhs.mPort) &&
            (lhs.mScore == rhs.mScore)
            )
            return true;
        else
            return false;
    }

    friend bool operator!=(const Endpoint& lhs, const Endpoint& rhs)
    {
        return !(lhs == rhs);
    }

    /*
     * @return ipv4 address(A) and port(P) as a combined 64 bit value 0xAAAAAAAA0000PPPP
     */
    uint64_t AsUint64(void) const
    {
        return ((uint64_t)mIp << 32) | mPort;
    }

    std::string GetDeviceId(void) const
    {
        return mDeviceId;
    }

    uint32_t GetIp(void) const
    {
        return mIp;
    }

    uint16_t GetPort(void) const
    {
        return mPort;
    }

    uint8_t GetScore(void) const
    {
        return mScore;
    }

    enum TYPE GetType(void) const
    {
        return mType;
    }

    void SetDeviceId(const std::string& deviceId)
    {
        mDeviceId = deviceId;
    }

    void SetScore(const uint8_t score)
    {
        mScore = score;
    }

    /*
     * Increment score
     * @return reference to itself
     */
    Endpoint& operator++(void)
    {
        if (mScore < 255) ++mScore;
        return *this;
    }

    bool IsValid(void) const
    {
        return (0 != mIp) && (0 != mPort);
    }

private:
    uint32_t mIp;
    uint16_t mPort;
    uint8_t mScore;
    std::string mDeviceId;
    enum TYPE mType;
};
}
#endif /* #ifndef _ENDPOINT_H_ */
