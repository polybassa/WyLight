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

#ifndef _TELNET_PROXY_H_
#define _TELNET_PROXY_H_

#include "ClientSocket.h"
#include "trace.h"
#include <string>


#define AOK "\r\nAOK\r\n<2.31> "

class TelnetProxy
{
	private:
		const TcpSocket& mSock;

	public:
		TelnetProxy(const TcpSocket& sock);

		void ClearResponse(void) const;
		bool Close(bool doSave) const;
		void GetString(const std::string& getCmd, const std::string& searchKey, std::string& result) const;
		bool Open(void) const;
		bool Recv(const std::string& expectedResponse) const;
		bool RecvString(const std::string& searchKey, std::string& result) const;
		bool Send(const std::string& telnetMessage, const std::string& expectedResponse = AOK) const;
		bool SendString(const std::string& command, std::string value) const;
		bool SetReplaceChar(const char replace = '$') const;
};

#endif /* #ifndef _TELNET_PROXY_H_ */
