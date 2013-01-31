/**
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

#include "unittest.h"
#include "WiflyControl.h"

// empty wrappers to satisfy the linker
ClientSocket::ClientSocket(uint32_t, uint16_t, int) : mSock(0) {}
ClientSocket::~ClientSocket(void) {}
TcpSocket::TcpSocket(uint32_t addr, uint16_t port) : ClientSocket(addr, port, 0) {}
size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const { return 0;}
size_t TcpSocket::Send(const uint8_t* frame, size_t length) const {return 0; }

// wrapper to test WiflyControl
ComProxy::ComProxy(uint32_t addr, uint16_t port) : mSock (addr, port) {}

int32_t ComProxy::Send(BlRequest& req, uint8_t* pResponse, size_t responseSize, bool doSync) const
{
	return -1;
}

int32_t ComProxy::Send(cmd_frame const* pFrame, uint8_t* pResponse, size_t responseSize, bool doSync) const
{
	return -1;
}

size_t ut_WiflyControl_WlanSetJoin(void)
{
	NOT_IMPLEMENTED();
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_WiflyControl_WlanSetJoin);
	UnitTestMainEnd();
}

