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

#include "BroadcastReceiver.h"
#include "BroadcastMessage.h"
#include "timeval.h"
#include "trace.h"
#include "WiflyColor.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <mutex>

namespace WyLight {

	static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING;

	const std::string BroadcastReceiver::DEVICE_ID("Wifly_Light");
	const std::string BroadcastReceiver::DEVICE_ID_OLD("WiFly");
	const std::string BroadcastReceiver::DEVICE_VERSION("WiFly Ver 2.45, 10-09-2012");
	const std::string BroadcastReceiver::DEVICE_VERSION4("wifly-EZX Ver 4.00.1, Apr 19");
	const std::string BroadcastReceiver::STOP_MSG {"StopThread"};
	Endpoint BroadcastReceiver::EMPTY_ENDPOINT {};


	BroadcastReceiver::BroadcastReceiver(uint16_t port, const std::string& recentFilename, const std::function<void(size_t index, const Endpoint& newRemote)>& onNewRemote)
		: mPort(port), mIsRunning(true), mNumInstances(0), mRecentFilename(recentFilename), mOnNewRemote(onNewRemote)
	{
		ReadRecentEndpoints(mRecentFilename);
	}

	BroadcastReceiver::~BroadcastReceiver(void)
	{
		Stop();
		WriteRecentEndpoints(mRecentFilename);
	}

	void BroadcastReceiver::operator() (timeval *pTimeout) throw (FatalError)
	{
		// only one thread allowed per instance
		if(0 == std::atomic_fetch_add(&mNumInstances, 1))
			try {
				size_t numRemotes = mIpTable.size();
				timeval endTime, now;
				gettimeofday(&endTime, NULL);
				timeval_add(&endTime, pTimeout);
				do
				{
					const Endpoint remote = GetNextRemote(pTimeout);
					if(remote.IsValid()) {
						numRemotes++;
					}
					gettimeofday(&now, NULL);
				}
				while(mIsRunning && timeval_sub(&endTime, &now, pTimeout));
			} catch(FatalError& e) {
				std::atomic_fetch_sub(&mNumInstances, 1);
				throw(e);
			}



		std::atomic_fetch_sub(&mNumInstances, 1);
	}

	Endpoint& BroadcastReceiver::GetEndpoint(size_t index)
	{
		auto it = mIpTable.find(index);
		return (mIpTable.end() == it) ? EMPTY_ENDPOINT : it->second;
	}

	Endpoint& BroadcastReceiver::GetEndpointByFingerprint(const uint64_t fingerprint)
	{
		for(auto it = mIpTable.begin(); it != mIpTable.end(); it++) {
			if(fingerprint == (*it).second.AsUint64()) {
				return (*it).second;
			}
		}
		return EMPTY_ENDPOINT;
	}

	Endpoint BroadcastReceiver::GetNextRemote(timeval *timeout) throw (FatalError)
	{
		UdpSocket udpSock(INADDR_ANY, mPort, true, 1);
		sockaddr_storage remoteAddr;
		socklen_t remoteAddrLength = sizeof(remoteAddr);

		BroadcastMessage msg;
		const size_t bytesRead = udpSock.RecvFrom((uint8_t *)&msg, sizeof(msg), timeout, (sockaddr *)&remoteAddr, &remoteAddrLength);
		TraceBuffer(ZONE_VERBOSE, msg.deviceId, sizeof(msg.deviceId), "%c", "%zu bytes broadcast message received DeviceId: \n", bytesRead);
		if(msg.IsWiflyBroadcast(bytesRead)) {
			Trace(ZONE_INFO, "Broadcast detected\n");
			Endpoint newRemote(remoteAddr, remoteAddrLength, msg.port, std::string((char *)&msg.deviceId[0]));
			return LockedInsert(newRemote) ? newRemote : Endpoint();
		}
		return Endpoint();
	}

	bool BroadcastReceiver::LockedInsert(Endpoint& newEndpoint)
	{
		std::lock_guard<std::mutex> lg(mMutex);
		auto addedElement = mIpTableShadow.insert(newEndpoint);
		if(addedElement.second) {
			if(mOnNewRemote) mOnNewRemote(mIpTable.size(), newEndpoint);
			mIpTable.insert(std::pair<size_t, Endpoint>(mIpTable.size(), newEndpoint));
		} else {
			Endpoint& ref = GetEndpointByFingerprint(newEndpoint.AsUint64());
			if(ref.IsValid()) {
				ref.SetDeviceId(newEndpoint.GetDeviceId());
			}
		}
		return true;
	}

	size_t BroadcastReceiver::NumRemotes(void) const
	{
		return mIpTable.size();
	}

	void BroadcastReceiver::ReadRecentEndpoints(const std::string& filename)
	{
		std::ifstream inFile;
		if(filename.compare("") == 0)
			inFile.open(mRecentFilename, std::ios::in);
		else
			inFile.open(filename, std::ios::in);


		if(!inFile.is_open()) {
			Trace(ZONE_ERROR, "Open file to read recent endpoints failed\n");
			return;
		}

		int score, port;
		std::string ip, deviceId;
		while(inFile >> score >> ip >> port >> deviceId) {
			Endpoint next(WiflyColor::ToARGB(ip), port, score, deviceId);
			LockedInsert(next);
		}
		inFile.close();
	}

	void BroadcastReceiver::Stop(void)
	{
		mIsRunning = false;
		UdpSocket sock(INADDR_LOOPBACK, mPort, false);
		sock.Send((uint8_t *)STOP_MSG.data(), STOP_MSG.size());
	}

	void BroadcastReceiver::WriteRecentEndpoints(const std::string& filename, uint8_t threshold) const
	{
		std::ofstream outFile;
		if(filename.compare("") == 0)
			outFile.open(mRecentFilename, std::ios::trunc);
		else
			outFile.open(filename, std::ios::trunc);

		if(!outFile.is_open()) {
			Trace(ZONE_ERROR, "Open file to write recent endpoints failed\n");
			return;
		}
		// write to file
		for(auto it = mIpTable.begin(); it != mIpTable.end(); it++) {
			if((*it).second.GetScore() >= threshold) {
				//TODO refactor this but then we have to change the CLI implementation
				outFile << (int)((*it).second.GetScore()) << ' ' << std::hex << (*it).second.GetIp() << ' ' << std::dec << (*it).second.GetPort() << ' ' << (*it).second.GetDeviceId() << '\n';
			}
		}
		outFile.close();
	}

	void BroadcastReceiver::DeleteRecentEndpointFile(const std::string& filename)
	{
		std::lock_guard<std::mutex> lock(this->mMutex);
		this->mIpTableShadow.clear();
		this->mIpTable.clear();

		int returnCode;
		if(filename.compare("") == 0)
			returnCode = remove(mRecentFilename.c_str());

		else
			returnCode = remove(filename.c_str());

		if(returnCode) {
			Trace(ZONE_ERROR, "Delete file \"%s\" recent endpoints failed\n", mRecentFilename.c_str());
			return;
		}
	}

} /* namespace WyLight */
