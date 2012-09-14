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

#include "rtc.h"
#include "trace.h"
#include "iic.h"
#ifndef X86
#include "INLINE.H"
#endif

//*********************** PRIVATE FUNCTIONS *********************************************

uns8 BcdToBin(uns8 BcdValue)
{
	uns8 retValue = 0x00;
	
	retValue = BcdValue & 0x0f;
	BcdValue = BcdValue >> 4;
	BcdValue = BcdValue * 0x0a;
	
	return retValue + BcdValue;
}

uns8 BinToBcd(uns8 BinValue)
{
	uns8 onesValue, tensValue, tempValue;
	
	onesValue = 0x00;
	tensValue = 0x00;
	
	if(BinValue > 9 )
	{
		tensValue = BinValue / 0x0a;
		tempValue = tensValue * 10;
		BinValue -= tempValue;
	}
	onesValue = BinValue;
	tensValue = tensValue << 4;
	
	return tensValue + onesValue;
}

//*********************** PUBLIC FUNCTIONS *********************************************


struct rtc_time g_RtcTime;

void Rtc_Init(void)
{
	I2C_Init();
	
	I2C_Write(RTC,0x00,0x00);		//Make sure that the TEST-Bits in the RTC-Device are set to zero
	I2C_Write(RTC,0x01,0x00);		//Disable Interrupts in the RTC-Device
} 

void Rtc_Ctl(enum RTC_request req,struct rtc_time *pRtcTime)
{
	uns8 temp;
	switch(req)
	{
		case RTC_RD_TIME:
		{
			temp = BcdToBin( I2C_Read(RTC, 0x02) & 0b01111111);
			pRtcTime->tm_sec = temp;
			temp = BcdToBin( I2C_Read(RTC, 0x03) & 0b01111111);
			pRtcTime->tm_min = temp;
			temp = BcdToBin( I2C_Read(RTC, 0x04) & 0b00111111);
			pRtcTime->tm_hour = temp;	
			temp = BcdToBin( I2C_Read(RTC, 0x05) & 0b00111111);
			pRtcTime->tm_mday = temp;
			temp = BcdToBin( I2C_Read(RTC, 0x06) & 0b00000111);
			pRtcTime->tm_wday = temp;
			temp = BcdToBin( I2C_Read(RTC, 0x07) & 0b00011111);
			temp -= 1;
			pRtcTime->tm_mon = temp;
			temp = BcdToBin( I2C_Read(RTC, 0x08) & 0b11111111);
			pRtcTime->tm_year = temp;
		}break;
		case RTC_SET_TIME:
		{
			temp = BinToBcd(pRtcTime->tm_sec);
			I2C_Write(RTC,0x02,( temp ));
			temp = BinToBcd(pRtcTime->tm_min);
			I2C_Write(RTC,0x03,( temp ));
			temp = BinToBcd(pRtcTime->tm_hour);
			I2C_Write(RTC,0x04,( temp ));
			temp = BinToBcd(pRtcTime->tm_mday);
			I2C_Write(RTC,0x05,( temp ));
			temp = BinToBcd(pRtcTime->tm_wday);
			I2C_Write(RTC,0x06,( temp ));
			temp = BinToBcd((pRtcTime->tm_mon + 1));
			I2C_Write(RTC,0x07,( temp ));
			temp = BinToBcd(pRtcTime->tm_year);
			I2C_Write(RTC,0x08,( temp ));
		}break;
	}
}

