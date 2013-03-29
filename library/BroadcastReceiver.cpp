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

const std::string BroadcastReceiver::DEVICE_ID("Wifly_Light");
const std::string BroadcastReceiver::DEVICE_ID_OLD("WiFly");
const std::string BroadcastReceiver::STOP_MSG{"StopThread"};
const Endpoint BroadcastReceiver::EMPTY_ENDPOINT;

BroadcastReceiver::BroadcastReceiver(uint16_t port)
	: mPort(port), mIsRunning(true), mNumInstances(0)
{
}

BroadcastReceiver::~BroadcastReceiver(void)
{
	Stop();
}

void BroadcastReceiver::operator() (std::ostream& out, timeval* pTimeout)
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

const Endpoint& BroadcastReceiver::GetEndpoint(size_t index) const
{
	if(index >= mIpTable.size())
		return EMPTY_ENDPOINT;

	auto it = mIpTable.begin();
	for(; index != 0; --index, ++it);
	return *it;
}

Endpoint BroadcastReceiver::GetNextRemote(timeval* timeout) throw (FatalError)
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
		Endpoint newRemote(remoteAddr, remoteAddrLength, msg.port);
		mMutex.lock();
		bool added = mIpTable.insert(newRemote).second;
		mMutex.unlock();
		return added ? newRemote : Endpoint();
	}
	return Endpoint();
}

size_t BroadcastReceiver::NumRemotes(void) const
{
	return mIpTable.size();
}

void BroadcastReceiver::Stop(void)
{
	mIsRunning = false;
	UdpSocket sock(INADDR_LOOPBACK, mPort, false);
	sock.Send((uint8_t*)STOP_MSG.data(), STOP_MSG.size());
}

