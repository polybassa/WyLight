/*
   Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

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

#ifndef __cc_firmware__SimplelinkServers__
#define __cc_firmware__SimplelinkServers__

#include <stdint.h>
#include "wy_Task.h"
#include "SimplelinkCustomer.h"

class Server {
protected:
    Server(void);
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;

    static xSemaphoreHandle* StoreDataSemaphore;
    void storeData(const uint8_t* data, const size_t length);
};

class TcpServer : public Task, SimplelinkCustomer, Server {
    void receive(const bool& stopFlag, const int childSock);
    void serve(const bool& stopFlag, const int serverSock);

public:
    TcpServer(void);
    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    TcpServer(TcpServer&&) = delete;

    static const uint16_t port = 2000;
    static const uint16_t rxBufferSize = 256;
    static const size_t STACK_SIZE = 2048;
    static const size_t TASK_PRIORITY = 5;

    virtual void run(void);
    virtual void stop(void);
};

class UdpServer : public Task, SimplelinkCustomer, Server {
    void receive(const bool& stopFlag, const int serverSock);

public:
    UdpServer(void);
    UdpServer(const UdpServer&) = delete;
    UdpServer& operator=(const UdpServer&) = delete;
    UdpServer(UdpServer&&) = delete;

    static const uint16_t port = 2000;
    static const uint16_t rxBufferSize = 256;
    static const size_t STACK_SIZE = 2048;
    static const size_t TASK_PRIORITY = 6;

    virtual void run(void);
    virtual void stop(void);
};

#endif /* defined(__cc_firmware__SimplelinkServers__) */
