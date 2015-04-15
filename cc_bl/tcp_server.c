/*
   Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

   This file is part of WyLight.

   WyLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   WyLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICUL<AR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include "bootloader.h"
#include "firmware_loader.h"
#include "trace.h"
#include "wlan.h"
#include "wy_bl_network_if.h"
#include "tcp_server.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

#define BUFFERSIZE 1024
#define SERVER_PORT htons(2000)

/**
 * @return 0 on success, if new firmware was saved and validated
 */
static int ReceiveFw(int SocketTcpChild)
{
    uint8_t* pFirmware = FIRMWARE_ORIGIN;

    for ( ; ; ) {
        const int bytesReceived = recv(SocketTcpChild, pFirmware, BUFFERSIZE, 0);

        if (bytesReceived >= 0) {
            pFirmware += bytesReceived;

            if (bytesReceived < BUFFERSIZE) {
                const size_t length = (size_t)(pFirmware - FIRMWARE_ORIGIN);
                return SaveSRAMContent(FIRMWARE_ORIGIN, length);
            }
        } else if (EAGAIN == bytesReceived) {
            _SlNonOsMainLoopTask();
        } else {
            return bytesReceived;
        }
    }
}

/**
 * Create a new tcp server socket and start listening on it
 * @return positiv socket descriptor, or negative value on error
 */
static int TcpServer_Listen()
{
    const struct sockaddr_in localAddr = {
        .sin_family = AF_INET,
        .sin_port = SERVER_PORT,
        .sin_addr = {0}
    };

    int status;

    const int listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket >= 0) {
#ifdef SO_NONBLOCKING
        const int dontBlock = 1;
        status = setsockopt(listenSocket, SOL_SOCKET, SO_NONBLOCKING, &dontBlock, sizeof(dontBlock));
        if (status) {
            Trace(ZONE_ERROR, "Setsockopt ERROR \r\n");
            goto on_error_close;
        }
#endif
        status = bind(listenSocket, (struct sockaddr*)&localAddr, sizeof(localAddr));

        if (status) {
            Trace(ZONE_ERROR, "Bind Error\n\r");
            goto on_error_close;
        }

        const int maxConnections = 1;
        status = listen(listenSocket, maxConnections);
        if (status) {
            Trace(ZONE_ERROR, "Listen Error\n\r");
            goto on_error_close;
        }
    }
    return listenSocket;

on_error_close:
    close(listenSocket);
    return status;
}

/**
 * Wait for client to connect
 * NOTE: this function blocks until a good socket is accepted, it retries forever!
 * @return accepted socket descriptor
 */
int TcpServer_Accept(const int listenSocket)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    for ( ; ; ) {
        const int childSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

        if (childSocket >= 0)
            return childSocket;

        if (EAGAIN == childSocket)
            _SlNonOsMainLoopTask();
        else
            Trace(ZONE_ERROR, "Error: %d occured on accept\r\n", childSocket);
    }
}

/**
 * @return if new firmware was received and validated
 */
extern void TcpServer(void)
{
    const uint32_t BL_VERSION = htonl(BOOTLOADER_VERSION);
    char welcome[] = WELCOME_RESPONSE;

    memcpy(welcome, &BL_VERSION, sizeof(uint32_t));

    const int listenSocket = TcpServer_Listen();
    if (listenSocket < 0) {
        Trace(ZONE_ERROR, "TcpServer: Socket Error: %d \r\n", listenSocket);
        return;
    }

    int fwStatus = 0xDEAD;
    while (fwStatus) {
        const int clientSock = TcpServer_Accept(listenSocket);

        if (sizeof(welcome) == send(clientSock, welcome, sizeof(welcome), 0)) {
            fwStatus = ReceiveFw(clientSock);
            if ((fwStatus == EAGAIN) || (fwStatus == SUCCESS)) {
                const char DONE = DONE_RESPONSE;
                send(clientSock, &DONE, sizeof(DONE), 0);
            } else {
                const char FAILURE = FAILURE_RESPONSE;
                send(clientSock, &FAILURE, sizeof(FAILURE), 0);
            }
            // send EOF to quit netcat client
            const char QUIT = QUIT_RESPONSE;
            send(clientSock, &QUIT, sizeof(QUIT), 0);
        }
        close(clientSock);
    }
}
