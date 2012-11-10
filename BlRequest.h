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
#include <stdio.h>

#define WORD(HIGH, LOW) (unsigned short)(((((unsigned short)(HIGH))<< 8) | (((unsigned short)(LOW)) & 0x00ff)))
#define DWORD(HIGH, LOW) (unsigned int)(((((unsigned int)(HIGH))<< 16) | (((unsigned int)(LOW)) & 0x0000ffff)))

#define FLASH_ERASE_BLOCKSIZE 64
#define FLASH_READ_BLOCKSIZE 64
#define FLASH_WRITE_BLOCKSIZE 64
#define FLASH_SIZE 32768 * 2
#define EEPROM_SIZE 1024
#define BL_STX 0x0f
#define BL_ETX 0x04
#define BL_DLE 0x05
#define BL_CRTL_CHAR_NUM 3
#define IsCtrlChar(X) (((X)==BL_STX) || ((X)==BL_ETX) || ((X)==BL_DLE))

static const unsigned int BL_MAX_RETRIES = 5;
static const size_t BL_MAX_MESSAGE_LENGTH = 512;
static const unsigned long BL_RESPONSE_TIMEOUT_TMMS = 1000;
static const unsigned char BL_SYNC[] = {BL_STX, BL_STX};

struct BlRequest
{
	BlRequest(size_t size, unsigned char cmd) : mSize(1 + size), mCmd(cmd) {};
	const size_t mSize;
	const unsigned char mCmd;
	const unsigned char* GetData() const { return &mCmd; };
	size_t GetSize() const { return mSize; };
	virtual bool CheckCrc() const { return true; };
};

struct BlReadRequest : public BlRequest
{
		BlReadRequest(size_t size, unsigned char cmd) : BlRequest(size + 6, cmd), zero(0x00) {};
		void SetAddressNumBytes(unsigned int address, unsigned short numBytes)
		{
			addressLow = static_cast<unsigned char>(address & 0x000000FF);
			addressHigh = static_cast<unsigned char>((address & 0x0000FF00) >> 8);
			addressU = static_cast<unsigned char>((address & 0x00FF0000) >> 16);
			numBytesLow = static_cast<unsigned char>(numBytes & 0x00FF);
			numBytesHigh = static_cast<unsigned char>((numBytes & 0xFF00) >> 8);
		};

		unsigned char addressLow;
		unsigned char addressHigh;
		unsigned char addressU;
		const unsigned char zero;
		unsigned char numBytesLow;
		unsigned char numBytesHigh;
};

/*struct BlWriteRequest : public BlRequest
{
		BlWriteRequest(unsigned char cmd, unsigned int address, unsigned char *pData) : BlRequest(FLASH_WRITE_BLOCKSIZE + 5, cmd), zero(0x00), numBlocks(0x01)
		{
			addressLow = static_cast<unsigned char>(address & 0x000000FF);
			addressHigh = static_cast<unsigned char>((address & 0x0000FF00) >> 8);
			addressU = static_cast<unsigned char>((address & 0x00FF0000) >> 16);
			for(int i = 0; i < FLASH_WRITE_BLOCKSIZE; i++, pData++)
			   data[i] = *pData;
		};

		unsigned char addressLow;
		unsigned char addressHigh;
		unsigned char addressU;
		const unsigned char zero;
		unsigned char numBlocks;
		unsigned char data[FLASH_WRITE_BLOCKSIZE];
};*/

class BlProxy
{
	private:
		const ClientSocket* const mSock;

	public:
		BlProxy(const ClientSocket* const pSock);

		/**
		 * Mask bytes of input buffer and add CRC16-CITT checksum to the end
		 * @param pInput input buffer
		 * @param inputLength number of bytes in input buffer
		 * @param pOutput output buffer
		 * @param outputLength size of the output buffer
		 */
		size_t MaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength) const;
		int Send(BlRequest& req, unsigned char* pResponse, size_t responseSize) const;
		int Send(const unsigned char* pRequest, const size_t requestSize, unsigned char* pResponse, size_t responseSize, bool checkCrc) const;
		size_t UnmaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength, bool checkCrc) const;
};

struct BlInfo
{
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

	unsigned int GetAddress(void) const
	{
		return DWORD(WORD(zero, startU), WORD(startHigh, startLow));
	}

	void Print(void) const
	{
		switch(familyId)
		{
			case 0x02:
				printf("PIC16");
	#ifdef PIC16
				printf("F%d", WORD(deviceIdHigh, deviceIdLow));
	#endif
				break;
			case 0x04:
				printf("PIC18");
				break;
			default:
				printf("unknown(0x%1x)", familyId);
				break;
		}
		printf(" bootloader V%d.%d\n", versionMajor, versionMinor);
		printf("Size: %d\n", WORD(sizeHigh, sizeLow));
		printf("Startaddress: 0x%x\n", GetAddress());
		printf("erase flash command %ssupported\n", ((0x02 == familyId) && (0x01 != cmdmaskHigh)) ? "not " : "");
	};
};

struct BlEepromReadRequest : public BlReadRequest
{
		BlEepromReadRequest() : BlReadRequest(0, 0x05) {};
};

struct BlFlashCrc16Request : public BlRequest
{
		BlFlashCrc16Request(unsigned int address, unsigned short numBlocks)
		: BlRequest(6, 0x02), zero(0x00)
		{
			addressLow = static_cast<unsigned char>(address & 0x000000FF);
			addressHigh = static_cast<unsigned char>((address & 0x0000FF00) >> 8);
			addressU = static_cast<unsigned char>((address & 0x00FF0000) >> 16);
			numBlocks = static_cast<unsigned char>(address & 0x00FF);
			numBlocks = static_cast<unsigned char>((address & 0xFF00) >> 8);
		};

		unsigned char addressLow;
		unsigned char addressHigh;
		unsigned char addressU;
		const unsigned char zero;
		unsigned char numBlockLow;
		unsigned char numBlocksHigh;

		// this is a special command where no crc is generated for the response
		virtual bool CheckCrc() const { return false; };
};

struct BlFlashEraseRequest : public BlRequest
{
		BlFlashEraseRequest(unsigned int endAddress, unsigned char numFlashPages)
		: BlRequest(5, 0x03), zero(0x00), numPages(numFlashPages)
		{
			endAddressLow = static_cast<unsigned char>(endAddress & 0x000000FF);
			endAddressHigh = static_cast<unsigned char>((endAddress & 0x0000FF00) >> 8);
			endAddressU = static_cast<unsigned char>((endAddress & 0x00FF0000) >> 16);
		};

		unsigned char endAddressLow;
		unsigned char endAddressHigh;
		unsigned char endAddressU;
		const unsigned char zero;
		unsigned char numPages;
};

struct BlFlashReadRequest : public BlReadRequest
{
		BlFlashReadRequest() : BlReadRequest(0, 0x01) {};
};

struct BlInfoRequest : public BlRequest
{
	BlInfoRequest() : BlRequest(0, 0x00) {};
};

struct BlRunAppRequest : public BlRequest
{
	BlRunAppRequest() : BlRequest(0, 0x08) {};
	virtual bool CheckCrc() const { return false; };
};
#endif /* #ifndef _BL_REQUEST_H_ */
