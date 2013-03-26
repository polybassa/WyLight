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

#ifndef _MASK_BUFFER_H_
#define _MASK_BUFFER_H_

#include "BlRequest.h"
#include "crc.h"
#include "WiflyControlException.h"

class MaskBuffer
{
	public:
		MaskBuffer(size_t capacity) : mCapacity(capacity), mLength(0), mCrc(0)
		{
			mData = new uint8_t[capacity];
			AddPure(BL_STX);
		};

		~MaskBuffer(void)
		{
			delete mData;
		};

		void AddWithCrc(uint8_t newByte)
		{
			Add(newByte);
			Crc_AddCrc16(newByte, &mCrc);
		};

		void AppendCrc(bool crcInLittleEndian)
		{
			if(crcInLittleEndian)
			{
				Add((uint8_t)(mCrc & 0xff));
				Add((uint8_t)(mCrc >> 8));
			}
			else
			{
				Add((uint8_t)(mCrc >> 8));
				Add((uint8_t)(mCrc & 0xff));
			}
		};

		void CompleteWithETX(void)
		{
			AddPure(BL_ETX);
		};

		uint16_t Crc16(void) const
		{
			return mCrc;
		};

		const uint8_t* Data(void) const
		{
			return mData;
		};

		void Mask(const uint8_t* pInput, const uint8_t* const pInputEnd, const bool crcInLittleEndian = true)
		{
			while(pInput < pInputEnd)
			{
				AddWithCrc(*pInput);
				pInput++;
			}
			AppendCrc(crcInLittleEndian);
		};

		size_t Size(void) const
		{
			return mLength;
		};

	private:
		const size_t mCapacity;
		uint8_t* mData;
		size_t mLength;
		uint16_t mCrc;

		void Add(uint8_t newByte)
		{
			if(IsCtrlChar(newByte))
			{
				AddPure(BL_DLE);
			}
			AddPure(newByte);
		};

		void AddPure(uint8_t newByte)
		{
			if(mLength >= mCapacity) {
				throw FatalError("MaskBuffer overflow");
			}
			mData[mLength] = newByte;
			mLength++;
		};
};
#endif /* #ifndef _MASK_BUFFER_H_ */

