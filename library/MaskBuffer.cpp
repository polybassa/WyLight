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

#include "MaskBuffer.h"
#include "trace.h"

namespace WyLight {

	static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_VERBOSE;

	void BaseBuffer::AddPure(uint8_t newByte)
	{
		if(mLength >= mCapacity) throw FatalError("BaseBuffer overflow");
		mData[mLength] = newByte;
		mLength++;
	}

	void MaskBuffer::Mask(const uint8_t *pInput, const uint8_t *const pInputEnd, const bool crcInLittleEndian)
	{
		while(pInput < pInputEnd)
		{
			AddWithCrc(*pInput);
			pInput++;
		}
		AppendCrc(crcInLittleEndian);
			AddPure(BL_ETX);
	}

	void MaskBuffer::Add(uint8_t newByte)
	{
		if(IsCtrlChar(newByte)) {
			AddPure(BL_DLE);
		}
			AddPure(newByte);
	}

	void MaskBuffer::AddWithCrc(uint8_t newByte)
	{
		Add(newByte);
		Crc_AddCrc16(newByte, &mCrc);
	}

	void MaskBuffer::AppendCrc(bool crcInLittleEndian)
	{
		if(crcInLittleEndian) {
			Add((uint8_t)(mCrc & 0xff));
			Add((uint8_t)(mCrc >> 8));
		} else {
			Add((uint8_t)(mCrc >> 8));
			Add((uint8_t)(mCrc & 0xff));
		}
	}

	void UnmaskBuffer::Add(uint8_t newByte)
	{
		AddPure(newByte);
		AddToCrc(newByte);
	}

	void UnmaskBuffer::Clear(void)
	{
		BaseBuffer::Clear();
		mPrePreCrc = mPreCrc = 0;
		mLastWasDLE = false;
	}

	void UnmaskBuffer::CheckAndRemoveCrc(bool crcInLittleEndian) throw (FatalError)
	{
		if(0x0000 == GetCrc16(crcInLittleEndian)) {
			if(2 > mLength) throw FatalError("Buffer underrun in UnmaskBuffer");
			mLength -= 2;
		} else {
			Clear();
		}
	}

	bool UnmaskBuffer::Unmask(const uint8_t *pInput, size_t bytesMasked, bool checkCrc, bool crcInLittleEndian)
	{
		while(bytesMasked-- > 0)
		{
			if(mLastWasDLE) {
				mLastWasDLE = false;
				Add(*pInput);
			} else {
				switch(*pInput)
				{
				case BL_ETX:
					Trace(ZONE_INFO, "Detect ETX\n");
					if(checkCrc) {
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
	}

	void UnmaskBuffer::AddToCrc(uint8_t newByte)
	{
		mPrePreCrc = mPreCrc;
		mPreCrc = mCrc;
			Crc_AddCrc16(newByte, &mCrc);
	}

	uint16_t UnmaskBuffer::GetCrc16(bool crcInLittleEndian) const
	{
		if(crcInLittleEndian) {
			uint16_t crc = mPrePreCrc;
			Crc_AddCrc16(mData[mLength - 1], &crc);
			Crc_AddCrc16(mData[mLength - 2], &crc);
			return crc;
		}
		return mCrc;
	}
} /* namespace WyLight */
