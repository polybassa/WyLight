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

#ifndef _TELNETRESPONSE_H_
#define _TELNETRESPONSE_H_

#include "ClientSocket.h"

class TelnetResponse
{
	public:
		uint8_t mBuffer[256];
		size_t mBufferLength;
	
	public:
		TelnetResponse(void);
		bool Equals(std::string const& expected) const;
		uint8_t* GetBuffer(void);
		size_t GetCapacity(void) const;
		size_t GetSize(void) const;
		bool IsCmdAck(void) const;
		void Recv(TcpSocket const& sock, timeval* timeout);
};

#endif /* #ifndef _TELNETRESPONSE_H_ */

