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

class BaseBuffer
{
	public:
		BaseBuffer(size_t capacity) : mCapacity(capacity)
		{
			mData = new uint8_t[capacity];
			Clear();
		};

		virtual ~BaseBuffer(void)
		{
			delete mData;
		};

		virtual void Clear(void)
		{
			mLength = 0;
			mCrc = 0;
		};

		const uint8_t* Data(void) const
		{
			return mData;
		};

		size_t Size(void) const
		{
			return mLength;
		};

	protected:	
		const size_t mCapacity;
		uint8_t* mData;
		size_t mLength;
		uint16_t mCrc;

		void AddPure(uint8_t newByte)
		{
			if(mLength >= mCapacity) throw FatalError("BaseBuffer overflow");
			mData[mLength] = newByte;
			mLength++;
		};
};

class MaskBuffer : public BaseBuffer
{
	public:
		MaskBuffer(size_t capacity) : BaseBuffer(capacity)
		{
			AddPure(BL_STX);
		};

		void CompleteWithETX(void)
		{
			AddPure(BL_ETX);
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

	private:
		void Add(uint8_t newByte)
		{
			if(IsCtrlChar(newByte))
			{
				AddPure(BL_DLE);
			}
			AddPure(newByte);
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
};

class UnmaskBuffer : public BaseBuffer
{
	public:
		UnmaskBuffer(size_t capacity) : BaseBuffer(capacity)
		{
			Clear();
		};

		void Add(uint8_t newByte)
		{
				AddPure(newByte);
				AddToCrc(newByte);
		};

		void Clear(void)
		{
			BaseBuffer::Clear();
			mPrePreCrc = mPreCrc = 0;
			mLastWasDLE = false;
		};

		void CheckAndRemoveCrc(bool crcInLittleEndian) throw (FatalError)
		{
			if(0x0000 == GetCrc16(crcInLittleEndian))
			{
				if(2 > mLength) throw FatalError("Buffer underrun in UnmaskBuffer");
				mLength -= 2;
			}
			else
			{
				Clear();				
			}
		};

		/*
		 * @return true if end of response reached (marked by an ETX), else false
		 */
		bool Unmask(const uint8_t* pInput, size_t bytesMasked, bool checkCrc, bool crcInLittleEndian)
		{
			while(bytesMasked-- > 0)
			{
				if(mLastWasDLE)
				{
					mLastWasDLE = false;
					Add(*pInput);
				}
				else
				{
					switch(*pInput)
					{
						case BL_ETX:
							Trace(ZONE_INFO, "Detect ETX\n");
							if(checkCrc)
							{
								CheckAndRemoveCrc(crcInLittleEndian);
							}
							return true;
						case BL_DLE:
							mLastWasDLE = true;
							break;
						case BL_STX:
							Trace(ZONE_INFO, "Detect STX\n");
							Clear();
							break;
						default:
							Add(*pInput);
							break;
					}
				}
				pInput++;
			}
			return false;
		};

	private:
		uint16_t mPrePreCrc;
		uint16_t mPreCrc;
		bool mLastWasDLE;

		void AddToCrc(uint8_t newByte)
		{
			mPrePreCrc = mPreCrc;
			mPreCrc = mCrc;
			Crc_AddCrc16(newByte, &mCrc);
		};

		uint16_t GetCrc16(bool crcInLittleEndian) const
		{
			if(crcInLittleEndian)
			{
				uint16_t crc = mPrePreCrc;
				Crc_AddCrc16(mData[mLength-1], &crc);
				Crc_AddCrc16(mData[mLength-2], &crc);
				return crc;
			}
			return mCrc;
		};
};
#endif /* #ifndef _MASK_BUFFER_H_ */

