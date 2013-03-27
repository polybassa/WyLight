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

#ifndef _COM_PROXY_H_
#define _COM_PROXY_H_

#include "BlRequest.h"
#include "ClientSocket.h"
#include "trace.h"
#include "wifly_cmd.h"

class ComProxy
{
	private:
		const TcpSocket& mSock;

		/*
		 * @throw FatalError if responsed timed out
		 */
		size_t Recv(uint8_t* pBuffer, size_t length, timeval* pTimeout = NULL, bool checkCrc = true, bool crcInLittleEndian = true) const throw(ConnectionTimeout);
		
		void SyncWithBootloader(void) const throw (FatalError);

	public:
		ComProxy(const TcpSocket& sock);
		size_t Send(BlRequest& req, uint8_t* pResponse, size_t responseSize, bool doSync = true) const;
		size_t Send(struct cmd_frame const* pFrame, response_frame* pResponse, size_t responseSize, bool doSync) const;
		size_t Send(uint8_t const* pRequest, const size_t requestSize, uint8_t* pResponse, size_t responseSize, bool checkCrc, bool sync, bool crcInLittleEndian = true) const;
};

#endif /* #ifndef _COM_PROXY_H_ */
