/*
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

#ifndef _BL_REQUEST_H_
#define _BL_REQUEST_H_

#include "wifly_cmd.h"

#include <cassert>
#include <cstring>
#include <stdio.h>
#include <stdint.h>

namespace WyLight {

#define BL_WORD(HIGH, LOW) (uint16_t)(((((uint16_t)(HIGH)) << 8) | (((uint16_t)(LOW)) & 0x00ff)))
#define BL_DWORD(HIGH, LOW) (uint32_t)(((((uint32_t)(HIGH)) << 16) | (((uint32_t)(LOW)) & 0x0000ffff)))

#define FLASH_ERASE_BLOCKSIZE 64
#define FLASH_ERASE_BLOCKS 128          /* Blocks erased by one command */
#define FLASH_READ_BLOCKSIZE 256
#define FLASH_WRITE_BLOCKSIZE 64
#define FLASH_SIZE 0x10000
#define FLASH_CRC_BLOCKSIZE 252
#define EEPROM_READ_BLOCKSIZE 256
#define EEPROM_WRITE_BLOCKSIZE 256
#define EEPROM_SIZE 1024
#define BL_AUTOSTART_ADDRESS 0x3ff
	static const uint8_t BL_STX = STX;
	static const uint8_t BL_ETX = ETX;
	static const uint8_t BL_DLE = DLE;
#define BL_CRTL_CHAR_NUM 3
	inline bool IsCtrlChar(uint8_t X) {
		return (((X) == BL_STX) || ((X) == BL_ETX) || ((X) == BL_DLE));
	}

	static const unsigned int BL_MAX_RETRIES = 5;
	static const size_t BL_MAX_MESSAGE_LENGTH = 512;
	static const unsigned char BL_SYNC[] = {BL_STX, BL_STX};

	struct BlRequest
	{
		BlRequest(size_t size, unsigned char cmd) : mSize(1 + size), mCmd(cmd) {};
		const size_t mSize;
		const unsigned char mCmd;
		const unsigned char *GetData() const { return &mCmd; };
		size_t GetSize() const { return mSize; };
		virtual bool CheckCrc() const { return true; };
	};

	struct BlAddressRequest : public BlRequest
	{
		BlAddressRequest(size_t size, unsigned char cmd) : BlRequest(size + 4, cmd), zero(0x00) {};
		virtual void SetAddress(unsigned int address)
		{
			addressLow = static_cast<unsigned char>(address & 0x000000FF);
			addressHigh = static_cast<unsigned char>((address & 0x0000FF00) >> 8);
			addressU = static_cast<unsigned char>((address & 0x00FF0000) >> 16);
		};

		unsigned char addressLow;
		unsigned char addressHigh;
		unsigned char addressU;
		const unsigned char zero;
	};

	struct BlReadRequest : public BlAddressRequest
	{
		BlReadRequest(size_t size, unsigned char cmd) : BlAddressRequest(size + 2, cmd) {};
		void SetAddressNumBytes(unsigned int address, unsigned short numBytes)
		{
			SetAddress(address);
			numBytesLow = static_cast<unsigned char>(numBytes & 0x00FF);
			numBytesHigh = static_cast<unsigned char>((numBytes & 0xFF00) >> 8);
		};

		unsigned char numBytesLow;
		unsigned char numBytesHigh;
	};

	struct BlInfo
	{
		unsigned char sizeLow;
		unsigned char sizeHigh;
		unsigned char versionMajor;
		unsigned char versionMinor;
		unsigned char cmdmaskHigh;
		unsigned char familyId : 4;
		unsigned char cmdmaskLow : 4;
		unsigned char startLow;
		unsigned char startHigh;
		unsigned char startU;
		unsigned char zero;
#ifdef PIC16
		unsigned char deviceIdLow;
		unsigned char deviceIdHigh;
#endif

		uint32_t GetAddress(void) const
		{
			return BL_DWORD(BL_WORD(zero, startU), BL_WORD(startHigh, startLow));
		}

		void Print(void) const
		{
			switch(familyId)
			{
			case 0x02:
				printf("PIC16");
	#ifdef PIC16
				printf("F%d", BL_WORD(deviceIdHigh, deviceIdLow));
	#endif
				break;
			case 0x04:
				printf("PIC18");
				break;
			default:
				printf("unknown(0x%1x)",                    familyId);
				break;
			}
				printf(" bootloader V%d.%d\n",              versionMajor, versionMinor);
				printf("Size: %d\n",                        BL_WORD(sizeHigh, sizeLow));
				printf("Startaddress: 0x%x\n",              GetAddress());
				printf("erase flash command %ssupported\n", ((0x02 == familyId) && (0x01 != cmdmaskHigh)) ? "not " : "");
		};
	};

	struct BlEepromReadRequest : public BlReadRequest
	{
		BlEepromReadRequest() : BlReadRequest(0, 0x05) {};
	};

	struct BlEepromWriteRequest : public BlAddressRequest
	{
		BlEepromWriteRequest() : BlAddressRequest(sizeof(payload) + 2, 0x06) {};

		void SetData(unsigned int address, const uint8_t *pData, size_t numBytes)
		{
			assert(numBytes <= sizeof(payload));
			SetAddress(address);
			for(unsigned int i = 0; i < sizeof(payload); i++) {
				payload[i] = 0xff;
			}
			memcpy(payload, pData, numBytes);
			numBytesLow = static_cast<unsigned char>(numBytes & 0x00FF);
			numBytesHigh = static_cast<unsigned char>((numBytes & 0xFF00) >> 8);
		};

		unsigned char numBytesLow;
		unsigned char numBytesHigh;
		unsigned char payload[EEPROM_WRITE_BLOCKSIZE];
	};

	struct BlFlashCrc16Request : public BlAddressRequest
	{
		BlFlashCrc16Request(uint32_t address, uint16_t numBlocks)
			: BlAddressRequest(2, 0x02)
		{
			SetAddress(address);
			numBlocksLow = static_cast<uint8_t>(numBlocks & 0x00FF);
			numBlocksHigh = static_cast<uint8_t>((numBlocks & 0xFF00) >> 8);
		};

		uint8_t numBlocksLow;
		uint8_t numBlocksHigh;

		// this is a special command where no crc is generated for the response
		virtual bool CheckCrc() const { return false; };
	};

	struct BlFlashEraseRequest : public BlAddressRequest
	{
		BlFlashEraseRequest(uint32_t address, uint8_t numFlashPages)
			: BlAddressRequest(1, 0x03), numPages(numFlashPages)
		{
			SetAddress(address);
		};

		const unsigned char numPages;
	};

	struct BlFlashReadRequest : public BlReadRequest
	{
		BlFlashReadRequest() : BlReadRequest(0, 0x01) {};
	};

	struct BlFlashWriteRequest : public BlAddressRequest
	{
		BlFlashWriteRequest() : BlAddressRequest(sizeof(payload) + 1, 0x04), numBlocksLow(0x01) {};

		void SetData(unsigned int address, unsigned char *pData, size_t numBytes)
		{
			assert(numBytes <= sizeof(payload));
			SetAddress(address);

			for(unsigned int i = 0; i < sizeof(payload); i++) {
				payload[i] = 0xff;
			}

			memcpy(payload, pData, numBytes);
		};

		unsigned char numBlocksLow;
		unsigned char payload[FLASH_WRITE_BLOCKSIZE];
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

} /* namespace WyLight */
#endif /* #ifndef _BL_REQUEST_H_ */
