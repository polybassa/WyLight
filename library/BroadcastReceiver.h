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
#include <mutex>
#include <ostream>
#include <set>
#include <string>
#include <functional>

namespace WyLight{

class BroadcastReceiver
{
	public:
		static const uint16_t BROADCAST_PORT = 55555;
		static const std::string DEVICE_ID;
		static const std::string DEVICE_ID_OLD;
		static const std::string DEVICE_VERSION;
		static const std::string STOP_MSG;
		static const Endpoint EMPTY_ENDPOINT;
		const uint16_t mPort;

		/*
		 * Construct an object for broadcast listening on the specified port
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
		const Endpoint& GetEndpoint(size_t index) const;

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

		/**
		 * Sends a stop event to terminate execution of operator()
		 */
		void Stop(void);
	
		void PrintAllEndpoints(std::ostream& out);
	
		/**
		 * Callback methode to notify that a new Enpoint was add to the IpTable
		 */
		void SetCallbackAddedNewRemote(const std::function<void(const Endpoint& newEndpoint)>& functionObj);

	private:
		std::set<Endpoint> mIpTable;
		volatile bool mIsRunning;
		std::atomic<int32_t> mNumInstances;
		std::mutex mMutex;
		std::function<void(const Endpoint& newEndpoint)> mAddedNewRemoteCallback;
};
}
#endif /* #ifndef _BROADCAST_RECEIVER_H_ */

