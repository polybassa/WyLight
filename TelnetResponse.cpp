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

#include "TelnetResponse.h"
#include "trace.h"

#include <cstring>

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO;

TelnetResponse::TelnetResponse(void) : mBufferLength(0) {}

bool TelnetResponse::Equals(std::string const& expected) const {
	return 0 == memcmp(expected.data(), mBuffer, expected.size());
}

uint8_t* TelnetResponse::GetBuffer(void) {return mBuffer;}
size_t TelnetResponse::GetCapacity(void) const {return sizeof(mBuffer);}
size_t TelnetResponse::GetSize(void) const {return mBufferLength;}

bool TelnetResponse::IsCmdAck(void) const {
	static const uint8_t CMD_MODE_ACK[] {'C', 'M', 'D', '\r', '\n'};
	return (sizeof(CMD_MODE_ACK) == mBufferLength) && (0 == memcmp(CMD_MODE_ACK, mBuffer, sizeof(CMD_MODE_ACK)));
}

void TelnetResponse::Recv(TcpSocket const& sock, timeval* timeout) {
	mBufferLength = sock.Recv(mBuffer, sizeof(mBuffer), timeout);
	TraceBuffer(ZONE_INFO, mBuffer, mBufferLength, "%c", "%u bytes received: ", mBufferLength);				
}

