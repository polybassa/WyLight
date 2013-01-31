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
		const TcpSocket mSock;
		size_t Recv(uint8_t* pBuffer, size_t length, timeval* pTimeout = NULL, bool checkCrc = true, bool crcInLittleEndian = true) const;

	public:
		ComProxy(uint32_t addr, uint16_t port);

		/**
		 * Mask bytes of input buffer and add CRC16-CITT checksum to the end
		 * @param pInput input buffer
		 * @param inputLength number of bytes in input buffer
		 * @param pOutput output buffer
		 * @param outputLength size of the output buffer
		 */
		size_t MaskControlCharacters(const uint8_t* pInput, size_t inputLength, uint8_t* pOutput, size_t outputLength, bool crcInLittleEndian = true) const;
		int32_t Send(BlRequest& req, uint8_t* pResponse, size_t responseSize, bool doSync = true) const;
		int32_t Send(const struct cmd_frame* pFrame, uint8_t* pResponse, size_t responseSize, bool doSync) const;
		int32_t Send(const uint8_t* pRequest, const size_t requestSize, uint8_t* pResponse, size_t responseSize, bool checkCrc, bool sync, bool crcInLittleEndian = true) const;
		size_t UnmaskControlCharacters(const uint8_t* pInput, size_t inputLength, uint8_t* pOutput, size_t outputLength, bool checkCrc, bool crcInLittleEndian = true) const;
};

#endif /* #ifndef _COM_PROXY_H_ */
