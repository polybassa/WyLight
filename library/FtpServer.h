/**
                Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

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

#ifndef _FTPSERVER_H_
#define _FTPSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "ClientSocket.h"
#include "WiflyControlException.h"

namespace WyLight {

/**
 * Abstract base class controlling the low level socket file descriptor
 */
	class FtpServer
	{
	public:
		/**
		 * Aquire a simple ftp server on port 21
		 * @throw FatalError if the creation of the bsd sock descriptor fails
		 */
		
		//TODO: current working directory path as parameter for constructor
		FtpServer(void) throw (FatalError);
		
		~FtpServer(void);
				
	private:
		void handleFiletransfer(const TcpSocket& telnet);
		TcpServerSocket* openDataConnection(const TcpSocket& telnet) throw(FatalError);
		void transferDataPassive(std::ifstream& file, const TcpServerSocket& dataSocket) const throw(FatalError);

		size_t Send(const void *frame, const size_t length, const int& socket) const throw(FatalError);
		size_t Send(const std::string& message, const int& socket) const throw(FatalError);
		
		bool mFtpServerRunning = true;
		std::thread mFtpServerThread;
	};
}
#endif /* #ifndef _CLIENTSOCKET_H_ */

