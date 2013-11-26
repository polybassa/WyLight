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
#include <set>
#include <string>
#include <functional>

namespace WyLight {

	class BroadcastReceiver
	{
	public:
		static const uint16_t BROADCAST_PORT = 55555;
		static const std::string DEVICE_ID;
		static const std::string DEVICE_ID_OLD;
		static const std::string DEVICE_VERSION;
		static const std::string DEVICE_VERSION4;
		static const std::string STOP_MSG;
		static Endpoint EMPTY_ENDPOINT;

		/*
		 * Construct an object for broadcast listening on the specified port
		 * @param path to the containing files used to store recent remotes
		 * @param port to listen on, deault is @see BROADCAST_PORT
		 * @param onNewEndpoint callback, which is called if a new endpoint got discovered
		 */
		BroadcastReceiver(uint16_t port = BROADCAST_PORT, const std::string& recentFilename = "", const std::function<void(size_t index, const Endpoint& newEndpoint)>& onNewEndpoint = NULL);

		/*
		 * Stop receiving loop and cleanup
		 */
		~BroadcastReceiver(void);

		/**
		 * Listen for broadcasts and print them to a stream
		 * @param timeout in seconds, until execution is terminated, to wait indefinetly use NULL (default)
		 */
		void operator() (timeval *timeout = NULL) throw (FatalError);

		/*
		 * Get a reference to the endpoint at the specified index
		 * @param index of the endpoint in the internal IpTable, should be lees than NumRemotes()
		 * @return a reference to the endpoint at the specified index or an empty object, if the index was out of bound
		 */
		Endpoint& GetEndpoint(size_t index);

		/*
		 * Get a reference to an endpoint with a matching fingerprint
		 * @param fingerprint to search for
		 * @return a reference to an endpoint with a matching fingerprint an empty object, if no matching endpoint was found
		 */
		Endpoint& GetEndpointByFingerprint(const uint64_t fingerprint);

		/*
		 * Listen for broadcasts until a new remote is discovered.
		 * @param timeout to wait until give up, use NULL to wait forever
		 * @return an empty Endpoint object in case of an error, if a new remote is discovered an Endpoint object with its address and port is returned.
		 * @throw FatalError if something failed seriously in the underlying socket
		 */
		Endpoint GetNextRemote(timeval *timeout) throw (FatalError);

		/**
		 * @return number of discovered remotes addresses
		 */
		size_t NumRemotes(void) const;

		/**
		 * Read recent endpoints from file and add them to mIpTable
		 * @param filename of the file containing the recent endpoints
		 */
		void ReadRecentEndpoints(const std::string& filename = "");

		/**
		 * Sends a stop event to terminate execution of operator()
		 */
		void Stop(void);

		/**
		 * Write recent endpoints to file
		 * @param filename of the file containing the recent endpoints
		 * @param threshold which an endpoints score has to have at least to be written to the file
		 */
		void WriteRecentEndpoints(const std::string& filename = "", uint8_t threshold = 1) const;

		/**
		 * Delete all recent endpoints in file and in the internal IpTables
		 * @param filename of the file containing the recent endpoints
		 */
		void DeleteRecentEndpointFile(const std::string& filename = "");

	private:
		const uint16_t mPort;
		std::set<Endpoint> mIpTableShadow;
		std::map<size_t, Endpoint> mIpTable;
		volatile bool mIsRunning;
		std::atomic<int32_t> mNumInstances;
		std::mutex mMutex;
		const std::string mRecentFilename;
		const std::function<void(size_t index, const Endpoint& newRemote)> mOnNewRemote;

		/**
		 * Insert threadsafe a new endpoint to the mIpTable
		 * @param endpoint a copy of this referenced object will be stored to mIpTable
		 * @return true if a new endpoint was added, false if it already existed or an error occur
		 */
		bool LockedInsert(Endpoint& endpoint);
	};
}
#endif /* #ifndef _BROADCAST_RECEIVER_H_ */

