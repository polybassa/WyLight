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
#include <cstring>
#include <iostream>

#include <stdio.h>

BroadcastReceiver::BroadcastReceiver(void) : mSock(socket(AF_INET, SOCK_DGRAM, 0))
{
	memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_port = htons(BROADCAST_PORT);
	mSockAddr.sin_addr.s_addr = INADDR_ANY;	
	pthread_create(&mBroadcastReceiverThread, NULL, startBroadcastReceiver, NULL);
}

BroadcastReceiver::~BroadcastReceiver()
{
    
	pthread_cancel(mBroadcastReceiverThread);
	pthread_join(mBroadcastReceiverThread, NULL);
	close(mSock);
}

void* BroadcastReceiver::startBroadcastReceiver(void*)
{	
	if( bind(mSock, (struct sockaddr *)&mSockAddr, sizeof(struct sockaddr)) == -1)
	{
	      std::cout << __FILE__ << ":" << __FUNCTION__ << ": Bind failure! ";
	      pthread_exit(NULL);
	      return;
	}
	char buffer[512];
	struct sockaddr_in connectorAddr;
	int numBytes;
	socklen_t addrLength = sizeof(struct sockaddr);

	while(true)
	{
	      numBytes = recvfrom(mSock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&connectorAddr, &addrLength);
	      
	      if(numBytes > 100 && 0 == memcmp(&buffer, "WiFly_Light", numBytes))
	      {
		    strcpy(&mIpTable[0][0],inet_ntoa(connectorAddr.sin_addr));
	      }
	}
}

size_t BroadcastReceiver::getIpTable(char pTable[][], size_t numRows, size_t numColumns) const
{
	if(numRows >= IP_TABLE_ROWS) numRows = IP_TABLE_ROWS;
	if(numColumns >= IP_TABLE_COLUMNS) numColumns = IP_TABLE_COLUMNS;
	for(int i = 0; i < (int) numRows; i++)
	{
	    for(int j = 0; j < (int) numColumns; j++)
	    {
		pTable[i][j] = mIpTable[i][j];
	    }
	}
	return 1;
}