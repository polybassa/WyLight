/**
                Copyright (C) 2013 - 2015 Nils Weiss, Patrick Bruenn.

    This file is part of WyLight.

    WyLight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    WyLight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include "Endpoint.h"

namespace WyLight
{
Endpoint::Endpoint(const BroadcastMessage& msg, sockaddr_in* addr)
    : mIp(ntohl(addr->sin_addr.s_addr)),
    mPort(ntohs(msg.port)),
    mScore(1),
    mDeviceId(&msg.deviceId[0],
              strnlen(&msg.deviceId[0],
                      sizeof(msg.deviceId))),
    mType(msg.IsRN171Broadcast() ? RN171 : CC3200)
{}

Endpoint::Endpoint(uint32_t            ip,
                   uint16_t            port,
                   uint8_t             score,
                   std::string         devId,
                   enum Endpoint::TYPE type)
    : mIp(ip),
    mPort(port),
    mScore(score),
    mDeviceId(devId),
    mType(type)
{}

bool Endpoint::operator<(const Endpoint& ref) const
{
    return (mIp < ref.GetIp())
           || ((mIp == ref.GetIp()) && (mPort < ref.GetPort()));
}

void Endpoint::WriteTo(std::ostream& out) const
{
    out << (int)mScore << ' ' <<
        std::hex << GetIp() << ' ' <<
        std::dec << GetPort() << ' ' <<
        GetDeviceId() << '\n';
}

std::ostream& operator<<(std::ostream& out, const Endpoint& ref)
{
    return out << (int)ref.mScore << ' ' <<
           ((ref.mIp & 0xff000000) >> 24) << '.' <<
           ((ref.mIp & 0x00ff0000) >> 16) << '.' <<
           ((ref.mIp & 0x0000ff00) >> 8) << '.' <<
           (ref.mIp & 0x000000ff) <<
           ':' << ref.mPort <<
           "  :  " << ref.mDeviceId;
}

std::istream& operator>>(std::istream& in, Endpoint& ref)
{
    std::string ip;
    in >> ref.mScore >> ip >> ref.mPort >> ref.mDeviceId;
    ref.mIp = WiflyColor::ToARGB(ip);
    return in;
}

bool operator==(const Endpoint& lhs, const Endpoint& rhs)
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

bool operator!=(const Endpoint& lhs, const Endpoint& rhs)
{
    return !(lhs == rhs);
}

uint64_t Endpoint::AsUint64(void) const
{
    return ((uint64_t)mIp << 32) | mPort;
}

std::string Endpoint::GetDeviceId(void) const
{
    return mDeviceId;
}

uint32_t Endpoint::GetIp(void) const
{
    return mIp;
}

uint16_t Endpoint::GetPort(void) const
{
    return mPort;
}

uint8_t Endpoint::GetScore(void) const
{
    return mScore;
}

enum Endpoint::TYPE Endpoint::GetType(void) const
{
    return mType;
}

void Endpoint::SetDeviceId(const std::string& deviceId)
{
    mDeviceId = deviceId;
}

void Endpoint::SetScore(const uint8_t score)
{
    mScore = score;
}

Endpoint& Endpoint::operator++(void)
{
    if (mScore < 255) ++mScore;
    return *this;
}

bool Endpoint::IsValid(void) const
{
    return (0 != mIp) && (0 != mPort);
}
}
