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
#include <map>
#include <string>
#include <vector>

#define BROADCAST_PORT 55555

class BroadcastReceiver
{
	private:
		const int mSock;
		struct sockaddr_in mSockAddr;
		std::map<unsigned long, std::string> mIpTable;
		pthread_t mBroadcastReceiverThread;

	public:
		BroadcastReceiver(void);
		~BroadcastReceiver(void);
		void Run(void);

		/**
		 * Returns a list of IP address strings from available Wifly_Lights
		 * @param outputVector a vector to add the strings to
		 * @return number of IP address strings added to <outputVector>
		 */
		size_t GetIpTable(std::vector<std::string>& outputVector) const;
};

#endif /* #ifndef _BROADCAST_RECEIVER_H_ */
