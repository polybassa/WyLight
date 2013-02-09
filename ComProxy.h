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
#include <iostream>

class TelnetResponse
{
	public:
		uint8_t mBuffer[256];
		size_t mBufferLength;
	
	public:
		TelnetResponse(void) : mBufferLength(0) {};
		uint8_t* GetBuffer(void) {return mBuffer;};
		size_t GetCapacity(void) const {return sizeof(mBuffer);};
		size_t GetSize(void) const {return mBufferLength;};

		void Recv(TcpSocket const& sock, timeval* timeout) {
			mBufferLength = sock.Recv(mBuffer, sizeof(mBuffer), timeout);
		};

		bool Equals(std::string const& expected) const {
			return 0 == memcmp(expected.data(), mBuffer, expected.size());
		};

		bool IsCmdAck(void) const {
			static const uint8_t CMD_MODE_ACK[] {'C', 'M', 'D', '\r', '\n'};
			return (sizeof(CMD_MODE_ACK) == mBufferLength) && (0 == memcmp(CMD_MODE_ACK, mBuffer, sizeof(CMD_MODE_ACK)));
		};

		bool IsAok(std::string const& msg) const {
			static const uint8_t AOK[] {'\r', '\n', 'A', 'O', 'K', '\r', '\n'};
			if(0 != memcmp(msg.data(), mBuffer, msg.size()))
				return false;
			return (0 == memcmp(AOK, mBuffer + msg.size(), sizeof(AOK)));
		};

		void Print(std::ostream& out) const {
			out << mBufferLength << " >>";
			for(size_t i = 0; i < mBufferLength; i++) {
				out << mBuffer[i];
			}
			out << "<<\n";
		};
};

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
		int32_t Send(struct cmd_frame const* pFrame, uint8_t* pResponse, size_t responseSize, bool doSync) const;
		int32_t Send(uint8_t const* pRequest, const size_t requestSize, uint8_t* pResponse, size_t responseSize, bool checkCrc, bool sync, bool crcInLittleEndian = true) const;

		void TelnetClearResponse(void) const;
		bool TelnetClose(void) const;
		bool TelnetOpen(void) const;
		bool TelnetSend(std::string const& telnetMessage, std::string const& telnetResponse) const;		
		size_t UnmaskControlCharacters(uint8_t const* pInput, size_t inputLength, uint8_t* pOutput, size_t outputLength, bool checkCrc, bool crcInLittleEndian = true) const;
};

#endif /* #ifndef _COM_PROXY_H_ */
