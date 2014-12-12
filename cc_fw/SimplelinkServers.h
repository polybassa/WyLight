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

#include "CPPTask.h"
#include "SimpleLinkCustomer.h"
#include <stdint.h>
#include "osi.h"

#define SERVER_PORT				2000

class Server {
protected:
	Server(void);
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
	Server(Server&&) = delete;
	
	static OsiSyncObj_t * StoreDataSemaphore;
	void storeData(const uint8_t* data, const size_t length);
};

class TcpServer : public Task, SimplelinkCustomer, Server {
	void receive(const tBoolean& stopFlag, const int childSock);
	void serve(const tBoolean& stopFlag, const int serverSock);
	
public:
	TcpServer(void);
	TcpServer(const TcpServer&) = delete;
	TcpServer& operator=(const TcpServer&) = delete;
	TcpServer(TcpServer&&) = delete;
	
	static const uint16_t port;
	
	virtual void run(void);
	virtual void stop(void);
};

class UdpServer : public Task, SimplelinkCustomer, Server {
	void receive(const tBoolean& stopFlag, const int serverSock);
	
public:
	UdpServer(void);
	UdpServer(const UdpServer&) = delete;
	UdpServer& operator=(const UdpServer&) = delete;
	UdpServer(UdpServer&&) = delete;
	
	static const uint16_t port;
	
	virtual void run(void);
	virtual void stop(void);
};


#endif /* defined(__cc_firmware__SimplelinkServers__) */
