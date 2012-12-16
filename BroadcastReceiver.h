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
#include <boost/asio.hpp>
using boost::asio::ip::udp;
#include <boost/thread.hpp>

class BroadcastReceiver
{
	private:
		static const unsigned short BROADCAST_PORT = 55555;
		static const char BROADCAST_MSG[];
		static const size_t BROADCAST_MSG_LENGTH;
		static const char STOP_MSG[];
		static const size_t STOP_MSG_LENGTH;
		const unsigned short mPort;

		std::map<unsigned long, std::string> mIpTable;
		boost::thread mThread;
		boost::mutex mMutex;

	public:
		BroadcastReceiver(unsigned short port);
		~BroadcastReceiver(void);

		/**
		 * Main loop wait for wifly broadcast messages and save them to the known IP list
		 */
		void operator()(void);

		/**
		 * Returns a list of IP address strings from available Wifly_Lights
		 * @param outputVector a vector to add the strings to
		 * @return number of IP address strings added to <outputVector>
		 */
		size_t GetIpTable(std::vector<std::string>& outputVector);

		/**
		 * Sends a stop event to terminate execution of operator()
		 */
		void Stop(void);
};

#endif /* #ifndef _BROADCAST_RECEIVER_H_ */
