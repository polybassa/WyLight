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
#include "INLINE.H"

//*********************** PRIVATE FUNCTIONS *********************************************
#ifndef X86
uns8 BcdToBin(uns8 BcdValue)
{
	uns8 retValue = 0x00;
	
	retValue = BcdValue & 0x0f;
	SWAPF(BcdValue,1);					//swap nipples
	BcdValue &= 0b00001111;				//Mask high nipple and VL-Bit
	BcdValue = BcdValue * 10;
	
	return retValue + BcdValue;
}

uns8 BinToBcd(uns8 BinValue);
#endif
//*********************** PUBLIC FUNCTIONS *********************************************


struct DateTime g_DateTime;

void Rtc_Init(void)
{
	I2C_Init();
	
	I2C_Write(RTC,0x00,0x00);		//Make sure that the TEST-Bits in the RTC-Device are set to zero
	I2C_Write(RTC,0x01,0x00);		//Disable Interrupts in the RTC-Device
} 

void Rtc_GetTime(struct stTime *pTime)
{
	uns8 temp;
	
	temp = BcdToBin( I2C_Read(RTC, 0x02) & 0b01111111);
	pTime->secounds = temp;
	temp = BcdToBin( I2C_Read(RTC, 0x03) & 0b01111111);
	pTime->minutes = temp;
	temp = BcdToBin( I2C_Read(RTC, 0x04) & 0b00111111);
	pTime->hours = temp;				
}

void Rtc_GetDate(struct stDate *pDate); 


