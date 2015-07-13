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
#include <cstring>
#include <ostream>
#include <stddef.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "BroadcastMessage.h"
#include "WiflyColor.h"

namespace WyLight
{
struct Endpoint {
    enum TYPE {
        RN171 = 0,
        CC3200
    };

    Endpoint(const BroadcastMessage& msg, sockaddr_in* addr);
    Endpoint(uint32_t            ip = 0,
             uint16_t            port = 0,
             uint8_t             score = 0,
             std::string         devId = "",
             enum Endpoint::TYPE type = RN171);

    static uint32_t IPv4FromString(const char* const ipv4)
    {
        in_addr_t addr;
        inet_pton(AF_INET, ipv4, &addr);
        return ntohl(addr);
    }
    Endpoint(const char* const ipv4, uint16_t port)
        : Endpoint(IPv4FromString(ipv4), port) {}

    bool operator<(const Endpoint& ref) const;
    void WriteTo(std::ostream& out) const;

    friend std::ostream& operator<<(std::ostream& out, const Endpoint& ref);
    friend std::istream& operator>>(std::istream& in, Endpoint& ref);
    friend bool operator==(const Endpoint& lhs, const Endpoint& rhs);
    friend bool operator!=(const Endpoint& lhs, const Endpoint& rhs);

    /*
     * @return ipv4 address(A) and port(P) as a combined 64 bit value 0xAAAAAAAA0000PPPP
     */
    uint64_t AsUint64(void) const;
    std::string GetDeviceId(void) const;
    uint32_t GetIp(void) const;
    uint16_t GetPort(void) const;
    uint8_t GetScore(void) const;
    enum TYPE GetType(void) const;
    void SetDeviceId(const std::string& deviceId);
    void SetScore(const uint8_t score);
    /*
     * Increment score
     * @return reference to itself
     */
    Endpoint& operator++(void);
    bool IsValid(void) const;

private:
    uint32_t mIp;
    uint16_t mPort;
    uint8_t mScore;
    std::string mDeviceId;
    enum TYPE mType;
};
}
#endif /* #ifndef _ENDPOINT_H_ */
