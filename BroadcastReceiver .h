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

#ifndef _BROADCAST_RECEIVER_H_
#define _BROADCAST_RECEIVER_H_

#include <sys/socket.h>
#include <netinet/in.h>

#define IP_TABLE_ROWS 128
#define IP_TABLE_COLUMNS 16
#define BROADCAST_PORT 55555

class BroadcastReceiver
{
	private:
		const int mSock;
		struct sockaddr_in mSockAddr;
		char mIpTable[IP_TABLE_ROWS][IP_TABLE_COLUMNS];
		pthread_t mBroadcastReceiverThread;
		void* startBroadcastReceiver(void*);

	public:
		BroadcastReceiver(void);
		~BroadcastReceiver(void);

		/**
		 * Returns a table of ipAddresses as const char* string from available Wifly_Lights
		 * Return Value is the number of available Wifly_Lights
		 */
		
		size_t getIpTable(char pTable[][], size_t numRows, size_t numColumns) const;
};

#endif /* #ifndef _BROADCAST_RECEIVER_H_ */
