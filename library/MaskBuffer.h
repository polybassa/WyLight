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

#ifndef _MASK_BUFFER_H_
#define _MASK_BUFFER_H_

#include "BlRequest.h"
#include "crc.h"
#include "WiflyControlException.h"

namespace WyLight {

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

		const uint8_t *Data(void) const
		{
			return mData;
		};

		size_t Size(void) const
		{
			return mLength;
		};

	protected:
		const size_t mCapacity;
		uint8_t *mData;
		size_t mLength;
		uint16_t mCrc;

		void AddPure(uint8_t newByte);
	};

	class MaskBuffer : public BaseBuffer
	{
	public:
		MaskBuffer(size_t capacity) : BaseBuffer(capacity)
		{
			AddPure(BL_STX);
		};

		void Mask(const uint8_t *pInput, const uint8_t *const pInputEnd, const bool crcInLittleEndian = true);

	private:
		void Add(uint8_t newByte);
		void AddWithCrc(uint8_t newByte);
		void AppendCrc(bool crcInLittleEndian);
	};

	class UnmaskBuffer : public BaseBuffer
	{
	public:
		UnmaskBuffer(size_t capacity) : BaseBuffer(capacity)
		{
			Clear();
		};

		void Add(uint8_t newByte);
		void Clear(void);
		void CheckAndRemoveCrc(bool crcInLittleEndian) throw (FatalError);

		/*
		 * @return true if end of response reached (marked by an ETX), else false
		 */
		bool Unmask(const uint8_t *pInput, size_t bytesMasked, bool checkCrc, bool crcInLittleEndian);

	private:
		uint16_t mPrePreCrc;
		uint16_t mPreCrc;
		bool mLastWasDLE;

		void AddToCrc(uint8_t newByte);

		uint16_t GetCrc16(bool crcInLittleEndian) const;

	};
}
#endif /* #ifndef _MASK_BUFFER_H_ */

