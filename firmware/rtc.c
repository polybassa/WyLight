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
#include "iic.h"
#include "trace.h"

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

    if (BinValue > 9) {
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
uns8 g_RtcAdress;

void Rtc_Init(void)
{
    I2C_Init();

    if (I2C_DetectSlave(RTC_MCP79410)) {
        Trace_String(" 79410 detected ");
        g_RtcAdress = RTC_MCP79410;
    } else if (I2C_DetectSlave(RTC_8564JE)) {
        Trace_String(" 8564JE detected");
        g_RtcAdress = RTC_8564JE;
    } else {
        Trace_String(" NO RTC detected");
        g_RtcAdress = 0x00;
    }

    if (g_RtcAdress == RTC_MCP79410) {
        uns8 temp;
        //set ST bit in RTC 0x00
        //set EXTOSC bit in RTC 0x00
        temp = I2C_Read(g_RtcAdress, 0x00);
        I2C_Write(g_RtcAdress, 0x00, temp | 0x88);
        //set enable Battery bit in RTC 0x03
        temp = I2C_Read(g_RtcAdress, 0x03);
        I2C_Write(g_RtcAdress, 0x03, temp | 0x08);

        //print statusregister
        Trace_String(" RTC 03:");
        Trace_Hex(I2C_Read(g_RtcAdress, 0x03));
    } else if (g_RtcAdress == RTC_8564JE) {
        I2C_Write(g_RtcAdress, 0x00, 0x00);       //Make sure that the TEST-Bits in the RTC-Device are set to zero
        I2C_Write(g_RtcAdress, 0x01, 0x00);       //Disable Interrupts in the RTC-Device
    }
}

void Rtc_Ctl(enum RTC_request req, struct rtc_time* pRtcTime)
{
    uns8 temp;

    switch (req) {
    case RTC_RD_TIME:
        if (g_RtcAdress == RTC_MCP79410) {
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x00) & 0x7F);
            pRtcTime->tm_sec = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x01) & 0x7F);
            pRtcTime->tm_min = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x02) & 0x3F);
            pRtcTime->tm_hour = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x04) & 0x3F);
            pRtcTime->tm_mday = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x03) & 0x07);
            pRtcTime->tm_wday = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x05) & 0x1F);
            temp -= 1;
            pRtcTime->tm_mon = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x06) & 0xFF);
            pRtcTime->tm_year = temp;
        } else if (g_RtcAdress == RTC_8564JE) {
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x02) & 0x7F);
            pRtcTime->tm_sec = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x03) & 0x7F);
            pRtcTime->tm_min = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x04) & 0x3F);
            pRtcTime->tm_hour = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x05) & 0x3F);
            pRtcTime->tm_mday = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x06) & 0x07);
            pRtcTime->tm_wday = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x07) & 0x1F);
            temp -= 1;
            pRtcTime->tm_mon = temp;
            temp = BcdToBin(I2C_Read(g_RtcAdress, 0x08) & 0xFF);
            pRtcTime->tm_year = temp;
        } else {
            Trace_String("NO RTC");
        }
        break;

    case RTC_SET_TIME:
        if (g_RtcAdress == RTC_MCP79410) {
            //clear ST Bit
            I2C_Write(g_RtcAdress, 0x00, 0x00);

            temp = BinToBcd(pRtcTime->tm_min);
            I2C_Write(g_RtcAdress, 0x01, (temp));
            temp = BinToBcd(pRtcTime->tm_hour);
            I2C_Write(g_RtcAdress, 0x02, (temp));
            temp = BinToBcd(pRtcTime->tm_mday);
            I2C_Write(g_RtcAdress, 0x04, (temp));
            temp = BinToBcd(pRtcTime->tm_wday);
            I2C_Write(g_RtcAdress, 0x03, (temp | 0x08));
            temp = BinToBcd((pRtcTime->tm_mon + 1));
            I2C_Write(g_RtcAdress, 0x05, (temp));
            temp = BinToBcd(pRtcTime->tm_year);
            I2C_Write(g_RtcAdress, 0x06, (temp));
            //set sec and ST bit
            temp = BinToBcd(pRtcTime->tm_sec);
            I2C_Write(g_RtcAdress, 0x00, (temp | 0x80));
        } else if (g_RtcAdress == RTC_8564JE) {
            temp = BinToBcd(pRtcTime->tm_sec);
            I2C_Write(g_RtcAdress, 0x02, (temp));
            temp = BinToBcd(pRtcTime->tm_min);
            I2C_Write(g_RtcAdress, 0x03, (temp));
            temp = BinToBcd(pRtcTime->tm_hour);
            I2C_Write(g_RtcAdress, 0x04, (temp));
            temp = BinToBcd(pRtcTime->tm_mday);
            I2C_Write(g_RtcAdress, 0x05, (temp));
            temp = BinToBcd(pRtcTime->tm_wday);
            I2C_Write(g_RtcAdress, 0x06, (temp));
            temp = BinToBcd((pRtcTime->tm_mon + 1));
            I2C_Write(g_RtcAdress, 0x07, (temp));
            temp = BinToBcd(pRtcTime->tm_year);
            I2C_Write(g_RtcAdress, 0x08, (temp));
        } else {
            Trace_String("NO RTC");
        }
        break;
    }
}
