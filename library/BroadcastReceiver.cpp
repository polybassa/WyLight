/*
   Copyright (C) 2012 - 2015 Nils Weiss, Patrick Bruenn.

   This file is part of WyLight.

   WyLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   WyLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include "BroadcastReceiver.h"
#include "BroadcastMessage.h"
#include "timeval.h"
#include "trace.h"
#include "WiflyColor.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <mutex>

using namespace WyLight;

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO;

const std::string BroadcastReceiver::STOP_MSG {"StopThread"};
Endpoint BroadcastReceiver::EMPTY_ENDPOINT {};

BroadcastReceiver::BroadcastReceiver(uint16_t port, const std::string& recentFilename,
                                     const std::function<void(size_t index,
                                                              const Endpoint& newRemote)> onNewRemote) : mPort(port),
    mIsRunning(true),
    mNumInstances(0),
    mRecentFilename(
        recentFilename),
    mOnNewRemote(
        onNewRemote)
{
    ReadRecentEndpoints(mRecentFilename);
}

BroadcastReceiver::~BroadcastReceiver(void)
{
    Stop();
    WriteRecentEndpoints(mRecentFilename);
}

void BroadcastReceiver::operator()(timeval* pTimeout)
{
    // only one thread allowed per instance
    if (0 == std::atomic_fetch_add(&mNumInstances, 1))
        try {
            timeval endTime, now;
            gettimeofday(&endTime, NULL);
            timeval_add(&endTime, pTimeout);
            do {
                GetNextRemote(pTimeout);
                gettimeofday(&now, NULL);
            } while (mIsRunning && timeval_sub(&endTime, &now, pTimeout));
        } catch (FatalError& e) {
            std::atomic_fetch_sub(&mNumInstances, 1);
            throw(e);
        }

    std::atomic_fetch_sub(&mNumInstances, 1);
}

Endpoint& BroadcastReceiver::GetEndpoint(size_t index)
{
    return (index < mIpTable.size()) ? mIpTable[index] : EMPTY_ENDPOINT;
}

Endpoint& BroadcastReceiver::GetEndpointByFingerprint(const uint64_t fingerprint)
{
    for (auto& endpoint : mIpTable) {
        if (fingerprint == endpoint.AsUint64())
            return endpoint;
    }
    return EMPTY_ENDPOINT;
}

Endpoint BroadcastReceiver::GetNextRemote(timeval* timeout)
{
    UdpSocket udpSock(INADDR_ANY, mPort, true, 1);
    sockaddr_storage remoteAddr;
    socklen_t remoteAddrLength = sizeof(remoteAddr);

    BroadcastMessage msg;
    const size_t bytesRead = udpSock.RecvFrom((uint8_t*)&msg,
                                              sizeof(msg),
                                              timeout,
                                              (sockaddr*)&remoteAddr,
                                              &remoteAddrLength);
    TraceBuffer(ZONE_VERBOSE,
                msg.deviceId,
                sizeof(msg.deviceId),
                "%c",
                "%zu bytes broadcast message received DeviceId: \n",
                bytesRead);
    if (bytesRead < sizeof(msg)) {
        Trace(ZONE_VERBOSE, "Message to short to be a WyLight broadcast\n");
        return Endpoint {};
    }
    return LockedInsert(Endpoint(msg, (sockaddr_in*)&remoteAddr));
}

Endpoint BroadcastReceiver::LockedInsert(Endpoint newEndpoint)
{
    std::lock_guard<std::mutex> lock(mMutex);
    size_t i = 0;
    for (const auto& e : mIpTable) {
        if (e == newEndpoint)
            break;
        ++i;
    }

    if (i == mIpTable.size())
        mIpTable.push_back(newEndpoint);
    mOnNewRemote(i, mIpTable[i]);
    return newEndpoint;
}

size_t BroadcastReceiver::NumRemotes(void) const
{
    return mIpTable.size();
}

void BroadcastReceiver::ReadRecentEndpoints(void)
{
    this->ReadRecentEndpoints(this->mRecentFilename);
}

void BroadcastReceiver::ReadRecentEndpoints(const std::string& filename)
{
    std::ifstream inFile;
    inFile.open(filename, std::ios::in);

    if (!inFile.is_open()) {
        Trace(ZONE_ERROR, "Open file to read recent endpoints failed\n");
        return;
    }

    Endpoint remote;
    while (inFile >> remote) {
        LockedInsert(remote);
    }
    inFile.close();
}

void BroadcastReceiver::Stop(void)
{
    mIsRunning = false;
    UdpSocket sock(INADDR_LOOPBACK, mPort, false);
    sock.Send((uint8_t*)STOP_MSG.data(), STOP_MSG.size());
}

void BroadcastReceiver::WriteRecentEndpoints(void) const
{
    this->WriteRecentEndpoints(this->mRecentFilename);
}

void BroadcastReceiver::WriteRecentEndpoints(const std::string& filename, uint8_t threshold) const
{
    std::ofstream outFile;
    outFile.open(filename, std::ios::trunc);

    if (!outFile.is_open()) {
        Trace(ZONE_ERROR, "Open file to write recent endpoints failed\n");
        return;
    }
    // write to file
    for (const auto& endpoint : mIpTable) {
        if (endpoint.GetScore() >= threshold)
            endpoint.WriteTo(outFile);
    }
    outFile.close();
}

void BroadcastReceiver::DeleteRecentEndpointFile()
{
    std::lock_guard<std::mutex> lock(this->mMutex);
    mIpTable.clear();

    const int returnCode = remove(mRecentFilename.c_str());
    if (returnCode)
        Trace(ZONE_ERROR, "Delete file \"%s\" recent endpoints failed\n", mRecentFilename.c_str());
}
