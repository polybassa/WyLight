/*
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


#include <stdio.h>
#include "trace.h"
#include "unittest.h"
#include "WiflyControlNoThrow.h"

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

/***** Wrappers ****/
ClientSocket::ClientSocket(uint32_t, uint16_t, int) throw (FatalError) : mSock(0) {}
ClientSocket::~ClientSocket(void) {}
TcpSocket::TcpSocket(uint32_t addr, uint16_t port) throw (ConnectionLost, FatalError) : ClientSocket(addr, port, 0) {}
size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const throw (FatalError) { return 0;}
size_t TcpSocket::Send(const uint8_t* frame, size_t length) const {return 0; }
ComProxy::ComProxy(const TcpSocket& sock) : mSock (sock) {}
TelnetProxy::TelnetProxy(const TcpSocket& sock) : mSock (sock) {}

WiflyControl::WiflyControl(uint32_t addr, uint16_t port)
: mSock(addr, port), mProxy(mSock), mTelnet(mSock)
{
	//TODO remove length
	mCmdFrame.length = (uns8)sizeof(struct cmd_set_fade) + 2;
}

void WiflyControl::BlEnableAutostart(void) const throw(ConnectionTimeout, FatalError){}

void WiflyControl::BlEraseEeprom(void) const throw(ConnectionTimeout, FatalError){}

void WiflyControl::BlEraseFlash(void) const throw(ConnectionTimeout, FatalError){}

void WiflyControl::BlProgramFlash(const std::string& filename) const throw (ConnectionTimeout, FatalError) {}

size_t WiflyControl::BlReadCrcFlash(uint8_t* pBuffer, uint32_t address, uint16_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter) { return 0; }

size_t WiflyControl::BlReadEeprom(uint8_t* pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter) {return 0; }

size_t WiflyControl::BlReadFlash(uint8_t* pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter) {return 0; }

std::string WiflyControl::BlReadFwVersion(void) const throw (ConnectionTimeout, FatalError) {return ""; }

void WiflyControl::BlReadInfo(BlInfo& info) const throw (ConnectionTimeout, FatalError){}

void WiflyControl::BlRunApp(void) const throw (ConnectionTimeout, FatalError){}

std::string WiflyControl::ConfGetSsid(void) const {return ""; }

bool WiflyControl::ConfSetDefaults(void) const {return true; }

bool WiflyControl::ConfSetWlan(const std::string& phrase, const std::string& ssid) const {return true; }

void WiflyControl::FwClearScript(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

std::string WiflyControl::FwGetCycletime(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {return ""; }

void WiflyControl::FwGetRtc(tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull){}

std::string WiflyControl::FwGetTracebuffer(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {return ""; }

std::string WiflyControl::FwGetVersion(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {return ""; }

void WiflyControl::FwLoopOff(uint8_t numLoops) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

void WiflyControl::FwLoopOn(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

void WiflyControl::FwSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

void WiflyControl::FwSetFade(const std::string& rgb, uint16_t fadeTime, const std::string& addr, bool parallelFade) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

void WiflyControl::FwSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr, bool parallelFade) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

void WiflyControl::FwSetRtc(const tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

void WiflyControl::FwSetWait(uint16_t waitTime) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

void WiflyControl::FwStartBl(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();

	UnitTestMainEnd();
}