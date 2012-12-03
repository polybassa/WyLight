/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
 
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

#include "BroadcastReceiver.h"

#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include <stdio.h>

static const char* BROADCAST_MSG = "WiFly_Light";
static const int BROADCAST_MSG_LENGTH = sizeof(BROADCAST_MSG);

void* Run(void* ptr)
{
	if(NULL != ptr)
	{
		BroadcastReceiver* pObj = reinterpret_cast<BroadcastReceiver*>(ptr);
		pObj->Run();
	}
	return NULL;
}

BroadcastReceiver::BroadcastReceiver(void) : mSock(socket(AF_INET, SOCK_DGRAM, 0))
{
	memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_port = htons(BROADCAST_PORT);
	mSockAddr.sin_addr.s_addr = INADDR_ANY;	
	pthread_create(&mBroadcastReceiverThread, NULL, ::Run, NULL);
}

BroadcastReceiver::~BroadcastReceiver(void)
{
    
	pthread_cancel(mBroadcastReceiverThread);
	pthread_join(mBroadcastReceiverThread, NULL);
	close(mSock);
}

void BroadcastReceiver::Run(void)
{
	char buffer[512];
	struct sockaddr_in remoteAddr;
	socklen_t remoteAddrLength = sizeof(remoteAddr);
	int numBytes;

	while(true)
	{
	      numBytes = recvfrom(mSock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&remoteAddr, &remoteAddrLength);
	      
	      if(numBytes > BROADCAST_MSG_LENGTH && 0 == memcmp(&buffer, BROADCAST_MSG, BROADCAST_MSG_LENGTH))
	      {
					mIpTable.insert(std::pair<unsigned long, std::string>(remoteAddr.sin_addr.s_addr, std::string(inet_ntoa(remoteAddr.sin_addr))));
	      }
	}
}

size_t BroadcastReceiver::GetIpTable(std::vector<std::string>& outputVector) const
{
	size_t numElements = 0;
	std::map<unsigned long, std::string>::const_iterator it;

	for(it = mIpTable.begin(); it != mIpTable.end(); it++)
	{
		outputVector.push_back((*it).second);
		numElements++;
	}
	return numElements;
}

