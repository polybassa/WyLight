/*
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.
 
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

#include "ClientSocket.h"
#include "Endpoint.h"
#include <atomic>
#include <cstring>
#include <stdint.h>
#include <string>
#include <map>
#include <mutex>
#include <ostream>
#include <string>

class BroadcastReceiver
{
	public:
		static const uint16_t BROADCAST_PORT = 55555;
		static const std::string DEVICE_ID;
		static const std::string DEVICE_ID_OLD;
		static const std::string DEVICE_VERSION;
		static const std::string STOP_MSG;
		static const Endpoint EMPTY_ENDPOINT;

		/*
		 * Construct an object for broadcast listening on the specified port
		 * @param path to the containing files used to store recent remotes
		 * @param port to listen on, deault is @see BROADCAST_PORT
		 */
		BroadcastReceiver(uint16_t port = BROADCAST_PORT);

		/*
		 * Stop receiving loop and cleanup
		 */
		~BroadcastReceiver(void);

		/**
		 * Listen for broadcasts and print them to a stream
		 * @param out stream to print collected remotes on
		 * @param timeout in seconds, until execution is terminated, to wait indefinetly use NULL (default)
		 */
		void operator() (std::ostream& out, timeval* timeout = NULL);

		/*
		 * Get a reference to the endpoint at the specified index
		 * @param index of the endpoint in the internal IpTable, should be lees than NumRemotes()
		 * @return a reference to the endpoint at the specified index or an empty object (@see EMPTY_ENDPOINT), when the index was out of bound
		 */
		Endpoint& GetEndpoint(size_t index);

		/*
		 * Listen for broadcasts until a new remote is discovered.
		 * @param timeout to wait until give up, use NULL to wait forever
		 * @return an empty Endpoint object in case of an error, if a new remote is discovered an Endpoint object with its address and port is returned.
		 * @throw FatalError if something failed seriously in the underlying socket
		 */
		Endpoint GetNextRemote(timeval* timeout) throw (FatalError);

		/**
		 * @return number of discovered remotes addresses
		 */
		size_t NumRemotes(void) const;

		void PrintAllEndpoints(std::ostream& out);

		/**
		 * Read recent endpoints from file
		 * @param filename of the file containing the recent endpoints
		 */
		void ReadRecentEndpoints(const std::string& filename);

		/**
		 * Sends a stop event to terminate execution of operator()
		 */
		void Stop(void);

		/**
		 * Write recent endpoints to file
		 * @param filename of the file containing the recent endpoints
		 */
		void WriteRecentEndpoints(const std::string& filename, uint8_t threshold = 1) const;
	

	private:
		const uint16_t mPort;
		std::map<size_t, Endpoint> mIpTable;
		volatile bool mIsRunning;
		std::atomic<int32_t> mNumInstances;
		std::mutex mMutex;
		std::string mFavourites;
};
#endif /* #ifndef _BROADCAST_RECEIVER_H_ */

