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

#include "SimplelinkServers.h"
#include "simplelink.h"
#include "RingBuf.h"
#include "wy_firmware.h"
#include "firmware/trace.h"


#ifndef SUCCESS
#define SUCCESS 0
#endif

#define BUFFERSIZE 256

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

const uint16_t TcpServer::port = SERVER_PORT;
const uint16_t UdpServer::port = SERVER_PORT;

static xSemaphoreHandle g_StoreDataSemaphore;

OsiSyncObj_t *Server::StoreDataSemaphore = nullptr;

Server::Server(void){
	if (StoreDataSemaphore == nullptr) {
		StoreDataSemaphore = &g_StoreDataSemaphore;
		osi_SyncObjCreate(StoreDataSemaphore);
		osi_SyncObjSignal(StoreDataSemaphore);
	}
}

void Server::storeData(const uint8_t* data, const size_t length){
	osi_SyncObjWait(Server::StoreDataSemaphore, OSI_WAIT_FOREVER);
	for (size_t i = 0; i < length; i++) {
		RingBuf_Put(&g_RingBuf, *data++);
	}
	osi_SyncObjSignal(NewDataAvailableSemaphore);
	osi_SyncObjSignal(Server::StoreDataSemaphore);
}

void TcpServer::run(void) {
	Task::run();
}

void TcpServer::stop(void) {
	Task::stop();
}

void TcpServer::receive(const bool& stopFlag, const int childSock) {
	uint8_t buffer[BUFFERSIZE];
	
	while (!stopFlag) {
		int bytesToSend = 0;
		while(!RingBuf_IsEmpty(&g_RingBuf_Tx)){
			buffer[bytesToSend++] = RingBuf_Get(&g_RingBuf_Tx);
		}
		if (bytesToSend && (bytesToSend != send(childSock, buffer, bytesToSend, 0))) {
			Trace(ZONE_ERROR,"Tcp => Error during transmit\r\n");
		}
		
		int bytesReceived = recv(childSock, buffer, sizeof(buffer), 0);
		
		if (EAGAIN == bytesReceived) {
			// if we don't recveived data, we can sleep a little bit
			osi_Sleep(20);
			continue;
		}
		
		if (bytesReceived <= 0) {
			// Error or close occured on child socket
			return;
		}
		
		Trace(ZONE_VERBOSE,",");
		this->storeData(buffer, bytesReceived);
	}
}

void TcpServer::serve(const bool& stopFlag, const int serverSock) {
	sockaddr_in childAddr;
	socklen_t childAddrLen = sizeof(sockaddr_in);
	
	for (;;) {
		// non blocking accept
		int childSock = accept(serverSock, (sockaddr *) &childAddr, &childAddrLen);
		
		if (stopFlag) {
			if (childSock >= 0) {
				close(childSock);
			}
			return;
		}
		if (EAGAIN == childSock) {
			osi_Sleep(100);
			continue;
		}
		if (childSock < 0) {
			Trace(ZONE_ERROR, "Accept error: %d\n\r", childSock);
			continue;
		}
		
		int nonBlocking = 1;
		setsockopt(childSock, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
		
		Trace(ZONE_INFO,"TCP Client connected\r\n");
		this->receive(stopFlag, childSock);
		close(childSock);
		childSock = ERROR;
		Trace(ZONE_INFO,"TCP Client disconnected\r\n");
	}
}


TcpServer::TcpServer(void) : Task((const char *)"TcpServer", OSI_STACK_SIZE, 5, [&](const bool& stopFlag){
	const sockaddr_in LocalAddr( AF_INET, htons(TcpServer::port), htonl(INADDR_ANY) );
	
	int serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSock < 0) {
		Trace(ZONE_ERROR, " Socket Error: %d", serverSock);
		return;
	}
	
	if (SUCCESS != bind(serverSock, (sockaddr *) &LocalAddr, sizeof(LocalAddr))) {
		Trace(ZONE_ERROR," Bind Error\n\r");
		close(serverSock);
		return;
	}
	
	// Backlog = 1 to accept maximal 1 connection
	if (SUCCESS != listen(serverSock, 1)) {
		Trace(ZONE_ERROR," Listen Error\n\r");
		close(serverSock);
		return;
	}
	
	int nonBlocking = 1;
	setsockopt(serverSock, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
	Trace(ZONE_INFO,"TcpServer started\r\n");
	this->serve(stopFlag, serverSock);
	Trace(ZONE_INFO,"TcpServer stopped\r\n");
	close(serverSock);
}) {}

void UdpServer::run(void) {
	Task::run();
}

void UdpServer::stop(void) {
	Task::stop();
}

void UdpServer::receive(const bool& stopFlag, const int serverSock) {
	socklen_t RemoteAddrLen = sizeof(sockaddr_in);
	uint8_t buffer[BUFFERSIZE];
	sockaddr_in remoteAddr;
	
	while (!stopFlag) {
		const int bytesReceived = recvfrom(serverSock, buffer, sizeof(buffer), 0, (sockaddr *) &remoteAddr,
										   &RemoteAddrLen);
		
		if (EAGAIN == bytesReceived) {
			osi_Sleep(15);
			continue;
		}
		
		if (bytesReceived <= 0) {
			return;
		}
		
		Trace(ZONE_VERBOSE,".");
		this->storeData(buffer, bytesReceived);
	}
}

UdpServer::UdpServer(void) : Task((const char *)"UdpServer", OSI_STACK_SIZE, 6, [&](const bool& stopFlag){
	const sockaddr_in LocalAddr(AF_INET, htons(UdpServer::port), htonl(INADDR_ANY) );
	
	int serverSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverSock < 0) {
		Trace(ZONE_ERROR,"Udp Socket Error\r\n");
		osi_Sleep(100);
		return;
	}
	int nonBlocking = 1;
	setsockopt(serverSock, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
	
	if (SUCCESS != bind(serverSock, (sockaddr *) &LocalAddr, sizeof(LocalAddr))) {
		Trace(ZONE_ERROR," Bind Error\n\r");
		close(serverSock);
		return;
	}
	
	Trace(ZONE_INFO,"UDP Server started \r\n");
	this->receive(stopFlag, serverSock);
	close(serverSock);
	Trace(ZONE_INFO,"UDP Server stopped \r\n");
}) {}