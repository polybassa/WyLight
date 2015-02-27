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

#ifndef cc3200

#include "usart.h"
#include "spi.h"
#include "crc.h"

#else /* cc3200 */

#include "wy_crc.h"
#include "uart_if.h"

#endif /* cc3200 */

#include "ScriptCtrl.h"
#include "CommandIO.h"
#include "trace.h"
#include "RingBuf.h"
#include "error.h"
#include "Version.h"
#include "wifly_cmd.h"
#include "rtc.h"

bank2 struct CommandBuffer g_CmdBuf;
bank5 struct response_frame g_ResponseBuf;
static bit g_Odd_STX_Received;

/** PRIVATE METHODES **/

#ifdef cc3200

static void UART_Send(const uns8 data)
{
    if (RingBuf_HasError(&g_RingBuf_Tx)) {
        Trace_String("g_RingBuf_Tx Error \r\n");
        RingBuf_Init(&g_RingBuf_Tx);
    }
    RingBuf_Put(&g_RingBuf_Tx, data);
}

#define Rtc_Ctl(x, y)
#define Timer_PrintCycletime(x, y) 0
#define Trace_Print(x, y) 0
#define SPI_Send(x) 0
#endif

static void WriteByte(uns8 byte)
{
    if (g_CmdBuf.counter < sizeof(g_CmdBuf.buffer)) {
        g_CmdBuf.buffer[g_CmdBuf.counter] = byte;
        g_CmdBuf.counter++;
        Crc_AddCrc(byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
    } else {
        CommandIO_Error();
    }
}

static void DeleteBuffer()
{
    g_CmdBuf.counter = 0;
    Crc_NewCrc(&g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
}

static void CheckForFwIdentMessage()
{
    g_Odd_STX_Received = !g_Odd_STX_Received;
    if (g_Odd_STX_Received == FALSE)
        UART_Send(FW_IDENT);
}

/** PUBLIC METHODES **/

void CommandIO_Init()
{
    g_CmdBuf.state = CS_WaitForSTX;
    DeleteBuffer();
    g_Odd_STX_Received = FALSE;
#ifdef cc3200
    Crc_Init();
#endif
}

void CommandIO_Error()
{
    CommandIO_CreateResponse(&g_ResponseBuf, g_CmdBuf.buffer[1], BAD_PACKET);
    CommandIO_SendResponse(&g_ResponseBuf);
    CommandIO_Init();
}

/** STATEMACHINE FOR GetCommands:
 * All ASCII-Chars are seperatet in 4 Groups
 *      Group1: STX
 *      Group2: ETX
 *      Group3: DLE
 *      Group4: All Elements of ASCII-Table without STX,ETX,DLE. I will call it CHAR in further description
 *
 * The Statemachine has 4 different states
 *      state 0: Wait for STX                           --> representet from CS_WaitForSTX
 *              read DLE or ETX or CHAR         --> new state = state 0 (nothing happens)
 *				read STX						--> new state = state 1
 *
 *      state 1: Read mask character			--> representet from CS_UnMaskChar
 *              read STX or ETX or DLE or CHAR	--> new state = state 3, save byte to commandbuffer, increment counter
 *
 *      state 2: Save Char                              --> representet from CS_SaveChar
 *              read CHAR						--> new state = state 3, save CHAR to commandbuffer, increment counter
 *              read DLE						--> new state = state 2
 *              read STX						--> new state = state 1
 *              read ETX						--> new state = state 0, do CRC-check, save dataframe
 *
 * **/

void CommandIO_GetCommands()
{
    if (RingBuf_HasError(&g_RingBuf)) {
        Trace_String(ERROR_RECEIVEBUFFER_FULL);//RingbufferFull
        // *** if a RingBufError occure, I have to throw away the current command,
        // *** because the last byte was not saved. Commandstring is inconsistent
        RingBuf_Init(&g_RingBuf);
        CommandIO_Error();
        return;
    }

    while (!RingBuf_IsEmpty(&g_RingBuf)) {
        // *** get new_byte from ringbuffer
        uns8 new_byte = RingBuf_Get(&g_RingBuf);

        switch (g_CmdBuf.state) {
        case CS_WaitForSTX:
            if (new_byte == STX) {
                CheckForFwIdentMessage();
                DeleteBuffer();
                g_CmdBuf.state = CS_SaveChar;
            }
            break;

        case CS_UnMaskChar:
            WriteByte(new_byte);
            g_CmdBuf.state = CS_SaveChar;
            break;

        case CS_SaveChar:
            if (new_byte == DLE) {
                g_CmdBuf.state = CS_UnMaskChar;
                break;
            }
            if (new_byte == STX) {
                CheckForFwIdentMessage();
                DeleteBuffer();
                break;
            }
            if (new_byte == ETX) {
                /* Setup statemachine for new state */
                g_Odd_STX_Received = FALSE;
                g_CmdBuf.state = CS_WaitForSTX;

                /* Set default answer value */
                ErrorCode mRetValue = BAD_PACKET;

                /* CRC Check */
                if ((0 == g_CmdBuf.CrcL) && (0 == g_CmdBuf.CrcH)) {
                    // [0] contains cmd_frame->cmd. Reply this cmd as response to client
#ifndef __CC8E__
                    mRetValue = (ErrorCode)ScriptCtrl_Add((struct led_cmd*)&g_CmdBuf.buffer[0]);
#else
                    mRetValue = ScriptCtrl_Add(&g_CmdBuf.buffer[0]);
#endif
                    if (mRetValue == NO_RESPONSE)
                        /* do not send a response if client does not want an echo */
                        break;
                } else {
                    mRetValue = CRC_CHECK_FAILED;
                }
                /* send response */
                CommandIO_CreateResponse(&g_ResponseBuf, g_CmdBuf.buffer[0], mRetValue);
                CommandIO_SendResponse(&g_ResponseBuf);
                break;
            }
            WriteByte(new_byte);
            break;
        }
    }
}

void CommandIO_SendResponse(struct response_frame* mFrame)
{
    uns8 crcH, crcL, tempByte, * pData;
    uns16 frameLength;

    frameLength = mFrame->length;

    pData = (uns8*)mFrame;

    Crc_NewCrc(&crcH, &crcL);

    UART_Send(STX);

    while (frameLength > 0) {
        frameLength--;
        tempByte = *pData++;
        Crc_AddCrc(tempByte, &crcH, &crcL);
        if ((tempByte == STX) || (tempByte == DLE) || (tempByte == ETX))
            UART_Send(DLE);
        UART_Send(tempByte);
    }
    if ((crcH == STX) || (crcH == DLE) || (crcH == ETX))
        UART_Send(DLE);
    UART_Send(crcH);
    if ((crcL == STX) || (crcL == DLE) || (crcL == ETX))
        UART_Send(DLE);
    UART_Send(crcL);
    UART_Send(ETX);
}

#define SPI_LOOPBACK_TESTVALUE 0x54

void CommandIO_CreateResponse(struct response_frame* mFrame, uns8 cmd, ErrorCode mState)
{
    mFrame->cmd = cmd;
    mFrame->state = mState;
    mFrame->length = sizeof(uns8) + sizeof(ErrorCode) + sizeof(uns16);

    switch (cmd) {
    case GET_RTC:
        {
            Rtc_Ctl(RTC_RD_TIME, &mFrame->data.time);
            mFrame->length += sizeof(struct rtc_time);
            break;
        };

    case GET_CYCLETIME:
        {
            uns8 bytesPrint =
                Timer_PrintCycletime(&(mFrame->data.max_cycle_times[0]), sizeof(struct response_frame) - 4);
            mFrame->length += bytesPrint;
            break;
        };

    case GET_TRACE:
        {
            uns8 bytesPrint = Trace_Print(&(mFrame->data.trace_string[0]), sizeof(struct response_frame) - 4);
            mFrame->length += bytesPrint;
            break;
        };

    case GET_FW_VERSION:
        {
            uns16 tempVersion = Version_Print();
            mFrame->data.versionData = tempVersion;
            mFrame->length += sizeof(uns16);
            break;
        }

    case GET_LED_TYP:
        if (SPI_LOOPBACK_TESTVALUE == SPI_Send(SPI_LOOPBACK_TESTVALUE))
            mFrame->data.ledTyp = LED_TYP_WS2801;
        else
            mFrame->data.ledTyp = LED_TYP_RGB;
        mFrame->length += sizeof(uns8);
        break;

    default:
        break;
    }
}
