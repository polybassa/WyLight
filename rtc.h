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

/** Have a look at: http://linux.die.net/man/4/rtc   **/

#define RTC 0xA2		/* IIC-Address of RTC Clock IC */

enum RTC_request{ RTC_SET_TIME, RTC_RD_TIME};

 /** *********************** rtc.h VARIABLES *********************************************/
 struct rtc_time{
	uns8 tm_sec;    /* seconds after the minute (0 to 59) */
	uns8 tm_min;    /* minutes after the hour (0 to 59) */
	uns8 tm_hour;   /* hours since midnight (0 to 23) */
	uns8 tm_mday;   /* day of the month (1 to 31) */
	uns8 tm_mon;    /* months since January (0 to 11) */
	uns8 tm_year;   /* years since 1900 */
	uns8 tm_wday;   /* days since Sunday (0 to 6 Sunday=0) */
//	uns8 tm_yday;   /** NOT SUPPORTED days since January 1 (0 to 365) */
//	uns8 tm_isdst;  /** NOT SUPPORTED Daylight Savings Time */
};


/** *********************** FUNCTIONS *********************************************/
extern struct rtc_time g_RtcTime;

void Rtc_Init(void); 

void Rtc_Ctl(enum RTC_request req,struct rtc_time *pRtcTime);

#endif /*_RTC_H_*/