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
#include <iostream>
#include <stdio.h>

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

const std::string WyLight::BroadcastReceiver::DEVICE_ID("Wifly_Light");
const std::string WyLight::BroadcastReceiver::DEVICE_ID_OLD("WiFly");
const std::string WyLight::BroadcastReceiver::DEVICE_VERSION("WiFly Ver 2.45, 10-09-2012");
const std::string WyLight::BroadcastReceiver::STOP_MSG{"StopThread"};
const WyLight::Endpoint WyLight::BroadcastReceiver::EMPTY_ENDPOINT;

WyLight::BroadcastReceiver::BroadcastReceiver(uint16_t port)
	: mPort(port), mIsRunning(true), mNumInstances(0), mAddedNewRemoteCallback(nullptr)
{
}

WyLight::BroadcastReceiver::~BroadcastReceiver(void)
{
	Stop();
}

void WyLight::BroadcastReceiver::operator() (std::ostream& out, timeval* pTimeout)
{
	// only one thread allowed per instance
	if(0 == std::atomic_fetch_add(&mNumInstances, 1))
	try
	{
		size_t numRemotes = 0;
		timeval endTime, now;
		gettimeofday(&endTime, NULL);
		timeval_add(&endTime, pTimeout);
		do
		{
			const Endpoint remote = GetNextRemote(pTimeout);
			if(remote.IsValid()) {
				out << numRemotes << ':' << remote << '\n';
				numRemotes++;
				if(mAddedNewRemoteCallback) mAddedNewRemoteCallback(remote);
			}
			gettimeofday(&now, NULL);
		} while(mIsRunning && timeval_sub(&endTime, &now, pTimeout));
	}
	catch (FatalError& e)
	{
		out << "EXCEPTION in " << __FILE__ << ':' << __LINE__ << ' ' << e << '\n';
	}
	std::atomic_fetch_sub(&mNumInstances, 1);
}

void WyLight::BroadcastReceiver::PrintAllEndpoints(std::ostream& out)
{
	int index = 0;
	//TODO wait for full c++11 features in android ndk. by the way we should move this printing functions out of BroadcastReceiver into cli or whoever wants to "print" something out
	//for(auto x : mIpTable)
	for(auto it = mIpTable.begin(); it != mIpTable.end(); it++)
	{
		//std::cout << index << ':' << *it << '\n';
		out << index++ << ':' << *it << '\n';
	}	
}

const WyLight::Endpoint& WyLight::BroadcastReceiver::GetEndpoint(size_t index) const
{
	if(index >= mIpTable.size())
		return EMPTY_ENDPOINT;

	auto it = mIpTable.begin();
	for(; index != 0; --index, ++it);
	return *it;
}

WyLight::Endpoint WyLight::BroadcastReceiver::GetNextRemote(timeval* timeout) throw (FatalError)
{
	UdpSocket udpSock(INADDR_ANY, mPort, true, 1);
	sockaddr_storage remoteAddr;
	socklen_t remoteAddrLength = sizeof(remoteAddr);

	BroadcastMessage msg;
	const size_t bytesRead = udpSock.RecvFrom((uint8_t*)&msg, sizeof(msg), timeout, (sockaddr*)&remoteAddr, &remoteAddrLength);
	TraceBuffer(ZONE_VERBOSE, msg.deviceId, sizeof(msg.deviceId), "%c", "%zu bytes broadcast message received DeviceId: \n", bytesRead);
	if(msg.IsWiflyBroadcast(bytesRead))
	{
		Trace(ZONE_INFO, "Broadcast detected\n");
		Endpoint newRemote(remoteAddr, remoteAddrLength, msg.port, std::string((char*)&msg.deviceId[0]));
		mMutex.lock();
		bool added = mIpTable.insert(newRemote).second;
		mMutex.unlock();
		return added ? newRemote : Endpoint();
	}
	return Endpoint();
}

size_t WyLight::BroadcastReceiver::NumRemotes(void) const
{
	return mIpTable.size();
}

void WyLight::BroadcastReceiver::Stop(void)
{
	mIsRunning = false;
	UdpSocket sock(INADDR_LOOPBACK, mPort, false);
	sock.Send((uint8_t*)STOP_MSG.data(), STOP_MSG.size());
}

void WyLight::BroadcastReceiver::SetCallbackAddedNewRemote(const std::function<void(const Endpoint& newEndpoint)>& functionObj)
{
	mAddedNewRemoteCallback = functionObj;
}

