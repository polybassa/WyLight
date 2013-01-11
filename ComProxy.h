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

class ComProxy
{
	private:
		const ClientSocket& mSock;
		size_t Recv(unsigned char* pBuffer, size_t length, timeval* pTimeout = NULL, bool checkCrc = true, bool crcInLittleEndian = true) const;

	public:
		ComProxy(const ClientSocket& sock);

		/**
		 * Mask bytes of input buffer and add CRC16-CITT checksum to the end
		 * @param pInput input buffer
		 * @param inputLength number of bytes in input buffer
		 * @param pOutput output buffer
		 * @param outputLength size of the output buffer
		 */
		size_t MaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength, bool crcInLittleEndian = true) const;
		int Send(BlRequest& req, unsigned char* pResponse, size_t responseSize, bool doSync = true) const;
		int Send(const struct cmd_frame* pFrame, unsigned char* pResponse, size_t responseSize, bool doSync) const;
		int Send(const unsigned char* pRequest, const size_t requestSize, unsigned char* pResponse, size_t responseSize, bool checkCrc, bool sync, bool crcInLittleEndian = true) const;
		size_t UnmaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength, bool checkCrc, bool crcInLittleEndian = true) const;
};

#endif /* #ifndef _COM_PROXY_H_ */
