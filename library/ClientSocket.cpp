/**
                Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

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

#include <algorithm>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>

#include "__stl_patches.h"

#ifdef SO_NOSIGPIPE
const int TCP_SEND_FLAGS = 0;
#else
const int TCP_SEND_FLAGS = MSG_NOSIGNAL;
#endif

namespace WyLight
{
static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO;    // | ZONE_VERBOSE;

#define ESTABLISH_CONNECTION_TIMEOUT 5

ClientSocket::ClientSocket() : mSock(-1), mSockAddr(0, 0)
{}

ClientSocket::ClientSocket(uint32_t addr, uint16_t port,
                           int style) : mSock(socket(AF_INET, style, 0)), mSockAddr(addr, port)
{
    if (-1 == mSock) throw FatalError("Create socket failed");
}

ClientSocket::~ClientSocket()
{
    close(mSock);
}

std::string ClientSocket::GetAddrCommaSeparated() const
{
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (-1 == getsockname(mSock, (struct sockaddr*)&sin, &len))
        return "0,0,0,0";

    std::string ip(inet_ntoa(sin.sin_addr));
    std::replace(ip.begin(), ip.end(), '.', ',');
    const uint16_t p = ntohs(sin.sin_port);
    const std::string port = std::to_string(p / 256) + ',' + std::to_string(p % 256);
    return ip + ',' + port;
}

bool ClientSocket::Select(timeval* timeout) const
{
    /* prepare socket set for select() */
    fd_set readSockets;
    FD_ZERO(&readSockets);
    FD_SET(mSock, &readSockets);

    /* wait for receive data */
    const int selectState = select(mSock + 1, &readSockets, NULL, NULL, timeout);
    if (0 == selectState) {
        Trace(ZONE_INFO, "Select timed out\n");
        return true;
    }

    /* and check if socket was correct */
    if ((1 != selectState) || (!FD_ISSET(mSock, &readSockets)))
        throw FatalError("something strange happen in select() called by TcpSocket::Recv()");
    return false;
}

TcpServerSocket::TcpServerSocket(uint32_t addr, uint16_t port) : ClientSocket(addr,
                                                                              port,
                                                                              SOCK_STREAM)
{
    //optional, steal port if necessary
    const int yes = 1;
    if (0 != setsockopt(mSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
        throw FatalError("TcpServerSocket: setsockopt() failed");

    if (0 != bind(mSock, reinterpret_cast<struct sockaddr*>(&mSockAddr), sizeof(mSockAddr)))
        throw FatalError("TcpServerSocket: bind() failed with errno: " + std::to_string(errno));

    if (0 != listen(mSock, 0))
        throw FatalError("TcpServerSocket: listen failed with errno: " + std::to_string(errno));
}

TcpSocket::TcpSocket(int listenSocket, const struct timespec* timeout)
{
    // wait for remote socket to connect
    if (timeout) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(listenSocket, &readfds);
        if (pselect(listenSocket + 1, &readfds, NULL, NULL, timeout, NULL) <= 0)
            throw FatalError("accept() timed out");
    }

    socklen_t sockAddrLen = sizeof(mSockAddr);
    mSock = accept(listenSocket, reinterpret_cast<struct sockaddr*>(&mSockAddr), &sockAddrLen);
    if (-1 == mSock)
        throw FatalError("Accept socket failed with errno: " + std::to_string(errno));

#if DEBUG
    uint16_t port = ntohs(mSockAddr.sin_port);
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &mSockAddr.sin_addr.s_addr, ip, sizeof(ip));
    Trace(ZONE_INFO, "Client connected: %s::%u\n", ip, port);
#endif
}

TcpSocket::TcpSocket(uint32_t addr, uint16_t port) : ClientSocket(addr,
                                                                  port,
                                                                  SOCK_STREAM)
{
    const int yes = 1;
    //optional, steal port if necessary
    if (0 != setsockopt(mSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
        throw FatalError("setsockopt() failed");

    //disable nagle algorithm
    const int flag = 1;
    const int result_disableNagle = setsockopt(mSock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    if (result_disableNagle < 0)
        throw ConnectionLost("connect() failed can't disable nagle algorithm", addr, port);

    //set socket options to non-blocking
    const int socketArgs = fcntl(mSock, F_GETFL, NULL) | O_NONBLOCK;
    fcntl(mSock, F_SETFL, socketArgs);

    const int result = connect(mSock, reinterpret_cast<sockaddr*>(&mSockAddr), sizeof(mSockAddr));
    if (result != 0) {
        if (errno != EINPROGRESS)
            throw ConnectionLost("connect() failed", addr, port);

        const struct timespec timeout {ESTABLISH_CONNECTION_TIMEOUT, 0};
        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(mSock, &writefds);

        // wait for socket to connect
        if (pselect(mSock + 1, NULL, &writefds, NULL, &timeout, NULL) <= 0)
            throw ConnectionLost("connect() failed with timeout", addr, port);

        // check if error pending on socket
        int errorStatus;
        socklen_t option_len = sizeof(errorStatus);
        if (0 != getsockopt(mSock, SOL_SOCKET, SO_ERROR, &errorStatus, &option_len))
            throw ConnectionLost("connect() failed with mysterious error", addr, port);
        if (0 != errorStatus)
            throw ConnectionLost("connect() failed with error", addr, port);
    }
    const int restSockArgs = fcntl(mSock, F_GETFL, NULL) & (~O_NONBLOCK);
    fcntl(mSock, F_SETFL, restSockArgs);

#ifdef SO_NOSIGPIPE
    int set = 1;
    setsockopt(mSock, SOL_SOCKET, SO_NOSIGPIPE, (void*)&set, sizeof(int));
#endif
}

size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const
{
    return Select(timeout) ? 0 : recv(mSock, pBuffer, length, 0);
}

size_t TcpSocket::Send(const uint8_t* frame, size_t length) const
{
    TraceBuffer(ZONE_VERBOSE, frame, length, "%02x ", "Sending on socket 0x%04x, %zu bytes: ", mSock, length);
    const ssize_t result = send(mSock, frame, length, TCP_SEND_FLAGS);
    if (result == -1)
        throw FatalError("send failed with returnvalue -1 and errno:" + std::to_string(errno));
    return result;
}

UdpSocket::UdpSocket(uint32_t addr, uint16_t port, bool doBind, int enableBroadcast) : ClientSocket(
        addr,
        port,
        SOCK_DGRAM)
{
    if (0 != setsockopt(mSock, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)))
        throw FatalError("setsockopt() failed");

    const int yes = 1;
    //optional, steal port if necessary
    if (0 != setsockopt(mSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
        throw FatalError("setsockopt() failed");

    if (doBind && (0 != bind(mSock, reinterpret_cast<struct sockaddr*>(&mSockAddr), sizeof(struct sockaddr))))
        throw FatalError("Bind UDP socket failed");
}

size_t UdpSocket::RecvFrom(uint8_t*         pBuffer,
                           size_t           length,
                           timeval*         timeout,
                           struct sockaddr* remoteAddr,
                           socklen_t*       remoteAddrLength) const
{
    return Select(timeout) ? 0 : recvfrom(mSock, pBuffer, length, 0, remoteAddr, remoteAddrLength);
}

size_t UdpSocket::Send(const uint8_t* frame, size_t length) const
{
    TraceBuffer(ZONE_INFO, frame, length, "%02x ", "Sending %zu bytes: ", length);
    return sendto(mSock, frame, length, 0, reinterpret_cast<const struct sockaddr*>(&mSockAddr), sizeof(mSockAddr));
}
} /* namespace WyLight */
