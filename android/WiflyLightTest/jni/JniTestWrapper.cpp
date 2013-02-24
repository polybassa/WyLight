/**
		Copyright (C) 2013 Nils Weiss, Patrick Bruenn.

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
#include "WiflyControl.h"

BroadcastReceiver::BroadcastReceiver(uint16_t port) : mPort(port), mIsRunning(true), mNumInstances(0) {};
BroadcastReceiver::~BroadcastReceiver() {};
Endpoint BroadcastReceiver::GetNextRemote(timeval* timeout) { return {0, 0}; };

ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) : mSock(NULL) {};
ClientSocket::~ClientSocket() {};

ComProxy::ComProxy(const TcpSocket& sock) : mSock(sock) {};

TcpSocket::TcpSocket(uint32_t addr, uint16_t port) : ClientSocket(addr, port, SOCK_STREAM) {};
size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const { return 0; };
size_t TcpSocket::Send(const uint8_t* frame, size_t length) const { return 0; };

TelnetProxy::TelnetProxy(const TcpSocket& sock) : mSock(sock) {};

static std::string g_WiflyControlSsid;
WiflyControl::WiflyControl(uint32_t addr, uint16_t port)
: mSock(addr, port), mProxy(mSock), mTelnet(mSock)
{};

std::string WiflyControl::ConfGetSsid(void) const
{
	return g_WiflyControlSsid;
};

bool WiflyControl::ConfSetWlan(const std::string& phrase, const std::string& ssid) const
{
	g_WiflyControlSsid.assign(ssid);
	return true;
};

ERROR_CODE WiflyControl::FwSetColor(unsigned long addr, unsigned long rgba) { return NoResponse; };
