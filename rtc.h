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
 
 #ifndef _RTC_H_
 #define _RTC_H_
 
 #include "iic.h"
 
 #define RTC 0xA2		//IIC-Address of RTC Clock IC
 
 struct stTime{
	uns8 hours;
	uns8 minutes;
	uns8 secounds;
};

struct stDate{
	uns8 days;
	uns8 weekdays;
	uns8 months;
	uns8 years;
};

struct DateTime{
	struct stDate date;
	struct stTime time;
};

extern struct DateTime g_DateTime;

void Rtc_Init(void); 
 
void Rtc_GetTime(struct stTime *pTime);

void Rtc_GetDate(struct stDate *pDate); 

void Rtc_GetDateTime(struct DateTime *pDateTime);

void Rtc_SetTime(struct stTime *pTime);

void Rtc_SetDate(struct stDate *pDate); 

void Rtc_SetDateTime(struct DateTime *pDateTime);


 #endif /*_RTC_H_*/