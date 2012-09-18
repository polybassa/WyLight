/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
 
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

#ifndef _BL_REQUEST_H_
#define _BL_REQUEST_H_

#include "ClientSocket.h"

#define WORD(HIGH, LOW) (unsigned short)(((((unsigned short)(HIGH))<< 8) | (((unsigned short)(LOW)) & 0x00ff)))
#define DWORD(HIGH, LOW) (unsigned int)(((((unsigned int)(HIGH))<< 16) | (((unsigned int)(LOW)) & 0x0000ffff)))


#define BL_STX 0x0f
#define BL_ETX 0x04
#define BL_DLE 0x05
#define BL_CRTL_CHAR_NUM 3
#define IsCtrlChar(X) (((X)==BL_STX) || ((X)==BL_ETX) || ((X)==BL_DLE))

static const unsigned int BL_MAX_RETRIES = 5;
static const size_t BL_MAX_MESSAGE_LENGTH = 512;
static const unsigned long BL_RESPONSE_TIMEOUT_TMMS = 1000;
static const unsigned char BL_SYNC[] = {BL_STX, BL_STX};

struct BlRequest {
	virtual const unsigned char* GetData() const = 0;
	virtual size_t GetSize() const = 0;
};

class BlProxy {
	private:
		const ClientSocket* const mSock;

	public:
		BlProxy(const ClientSocket* const pSock);
		size_t MaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength) const;
		int Send(BlRequest& req, unsigned char* pResponse, size_t responseSize) const;
		int Send(const unsigned char* pRequest, const size_t requestSize, unsigned char* pResponse, size_t responseSize) const;
		size_t UnmaskControlCharacters(unsigned char* const pInput, size_t inputLength) const;
};

struct BlInfo  {
	unsigned char sizeLow;
	unsigned char sizeHigh;
	unsigned char versionMajor;
	unsigned char versionMinor;
	unsigned char cmdmaskHigh;
	unsigned char familyId : 4;
	unsigned char cmdmaskLow :4;
	unsigned char startLow;
	unsigned char startHigh;
	unsigned char startU;
	unsigned char zero;
#ifdef PIC16
	unsigned char deviceIdLow;
	unsigned char deviceIdHigh;
#endif
	unsigned char crcLow;
	unsigned char crcHigh;
};

struct BlReadFlashRequest : public BlRequest {
		BlReadFlashRequest(unsigned int address, unsigned short bytes)
		: one(0x01), zero(0x00), etx(BL_ETX)
		{
			addressLow = static_cast<unsigned char>(address & 0x000000FF);
			addressHigh = static_cast<unsigned char>((address & 0x0000FF00) >> 8);
			addressU = static_cast<unsigned char>((address & 0x00FF0000) >> 16);
			bytesLow = static_cast<unsigned char>(address & 0x00FF);
			bytesHigh = static_cast<unsigned char>((address & 0xFF00) >> 8);
		};

		const unsigned char* GetData(void) const { return &one; };
		size_t GetSize(void) const {return sizeof(BlReadFlashRequest)-sizeof(BlRequest);};

		const unsigned char one;
		unsigned char addressLow;
		unsigned char addressHigh;
		unsigned char addressU;
		const unsigned char zero;
		unsigned char bytesLow;
		unsigned char bytesHigh;
		unsigned char crcLow;
		unsigned char crcHigh;
		const unsigned char etx;
};

struct BlReadInfoRequest : public BlRequest {
	BlReadInfoRequest() : zero(0), crcLow(0), crcHigh(0) {};
	const unsigned char zero;
	const unsigned char crcLow;
	const unsigned char crcHigh;

	const unsigned char* GetData(void) const { return &zero; };
	size_t GetSize(void) const { return 3;};//sizeof(BlReadInfoRequest) - sizeof(BlRequest);};
};
#endif /* #ifndef _BL_REQUEST_H_ */
