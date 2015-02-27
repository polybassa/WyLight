/*
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

#ifdef cc3200
#include "socket.h"
#include <stdbool.h>

#ifdef write
#undef write
#endif
#endif

#include "platform.h"

#include "ledstrip.h"
#include "ScriptCtrl.h"
#include "CommandIO.h"
#include "eeprom.h"
#include "error.h"
#include "trace.h"

/**************** private functions/ macros *****************/
/**
 * Helper to calculate an eeprom address from a command pointer
 */
#define ScriptBufAddr(x) ((uns16)EEPROM_SCRIPTBUF_BASE + ((uns16)(x) * (uns16)sizeof(struct led_cmd)))

/**
 * Helper to increment a ScriptBuf pointer
 */
#define ScriptBufInc(x) ((x + 1) & SCRIPTCTRL_NUM_CMD_MAX)

/**
 * Setter for ScriptBuf.inLoop
 */
#define ScriptBufSetInLoop(x) { \
        Eeprom_Write(EEPROM_SCRIPTBUF_INLOOP, x); \
        gScriptBuf.inLoop = x; \
}

/**
 * Setter for ScriptBuf.read
 */
#define ScriptBufSetRead(x) { \
        Eeprom_Write(EEPROM_SCRIPTBUF_READ, x); \
        gScriptBuf.read = x; \
}

/**
 * Setter for ScriptBuf.write
 */
#define ScriptBufSetWrite(x) { \
        Eeprom_Write(EEPROM_SCRIPTBUF_WRITE, x); \
        gScriptBuf.write = x; \
}

/**
 * Prototyp - Private function
 * save command to eeprom
 */
uns8 ScriptCtrl_Write(const struct led_cmd* pCmd);

/* private globals */
struct ScriptBuf gScriptBuf;
struct led_cmd nextCmd;

uns8 ScriptCtrl_Add(struct led_cmd* pCmd)
{
    /* We have to reject all commands until buffer was cleared completely */
    if (gScriptBuf.isClearing)
        return SCRIPTBUFFER_FULL;

    switch (pCmd->cmd) {
    case CLEAR_SCRIPT:
        //Trace_String("Clearing script buffer;");
        gScriptBuf.isClearing = TRUE;
        return OK;

    case LOOP_ON:
        gScriptBuf.loopStart[gScriptBuf.loopDepth] = gScriptBuf.write;
        gScriptBuf.loopDepth++;
        return ScriptCtrl_Write(pCmd);

    case LOOP_OFF:
        {
            gScriptBuf.loopDepth--;
            uns8 loopStart = gScriptBuf.loopStart[gScriptBuf.loopDepth];
            pCmd->data.loopEnd.startIndex = ScriptBufInc(loopStart);
            pCmd->data.loopEnd.depth = gScriptBuf.loopDepth;
            uns8 numLoops = pCmd->data.loopEnd.numLoops;
            pCmd->data.loopEnd.counter = numLoops;
            /*Trace_String("Add LOOP_OFF: ");
               Trace_Hex(gScriptBuf.write);
               Trace_Hex(pCmd->data.loopEnd.startIndex);
               Trace_Hex(pCmd->data.loopEnd.depth);
               Trace_Hex(pCmd->data.loopEnd.counter);
               Trace_String(";");*/
            uns8 retVal = ScriptCtrl_Write(pCmd);
#ifdef cc3200
            Eeprom_Save(true);
#endif
            return retVal;
        }

    case WAIT:
        return ScriptCtrl_Write(pCmd);

#ifndef cc3200
    case START_BL:
        CommandIO_CreateResponse(&g_ResponseBuf, START_BL, OK);
        CommandIO_SendResponse(&g_ResponseBuf);
        Platform_EnableBootloaderAutostart();
        softReset();
        /* never reach this */
        return OK;

#endif /* cc3200 */
#ifdef __CC8E__
    case GET_RTC:
        return OK;

    case SET_RTC:
        g_RtcTime.tm_year = pCmd->data.set_rtc.tm_year;
        g_RtcTime.tm_mon = pCmd->data.set_rtc.tm_mon;
        g_RtcTime.tm_mday = pCmd->data.set_rtc.tm_mday;
        g_RtcTime.tm_wday = pCmd->data.set_rtc.tm_wday;
        g_RtcTime.tm_hour = pCmd->data.set_rtc.tm_hour;
        g_RtcTime.tm_min = pCmd->data.set_rtc.tm_min;
        g_RtcTime.tm_sec = pCmd->data.set_rtc.tm_sec;
        Rtc_Ctl(RTC_SET_TIME, &g_RtcTime);
        return OK;

#endif /* #ifdef __CC8E__ */
    case SET_COLOR_DIRECT:
        Ledstrip_SetColorDirect((uns8*)&pCmd->data.set_color_direct.ptr_led_array);
        return NO_RESPONSE;

#ifdef __CC8E__
    case GET_CYCLETIME:
        return OK;

    case GET_TRACE:
        return OK;

#endif /* #ifdef __CC8E__ */
    case GET_FW_VERSION:
        return OK;

    case SET_FADE:
        return ScriptCtrl_Write(pCmd);

    case SET_GRADIENT:
        return ScriptCtrl_Write(pCmd);

    case GET_LED_TYP:
        return OK;

    default:
        return BAD_COMMAND_CODE;
    }
}

void ScriptCtrl_Clear(void)
{
    ScriptBufSetInLoop(FALSE);
    ScriptBufSetRead(EEPROM_SCRIPTBUF_BASE);
    ScriptBufSetWrite(EEPROM_SCRIPTBUF_BASE);
    gScriptBuf.execute = gScriptBuf.read;
    gScriptBuf.waitValue = 0;
    gScriptBuf.isClearing = FALSE;
}

void ScriptCtrl_Init(void)
{
    Eeprom_Init();
    gScriptBuf.inLoop = Eeprom_Read(EEPROM_SCRIPTBUF_INLOOP);
    gScriptBuf.read = Eeprom_Read(EEPROM_SCRIPTBUF_READ);
    gScriptBuf.write = Eeprom_Read(EEPROM_SCRIPTBUF_WRITE);
    gScriptBuf.execute = gScriptBuf.read;
    gScriptBuf.isRunning = TRUE;
}
//TODO Add a Methode to test the Errorbits and there responses

void ScriptCtrl_Run(void)
{
    /* delete command was triggered? */
    if (gScriptBuf.isClearing)
        ScriptCtrl_Clear();

    if (!gScriptBuf.isRunning) return;

    if (gScriptBuf.waitValue > 0)
        return;

    /* cmd available? */
    if (gScriptBuf.execute == gScriptBuf.write)
        return;

    /* read next cmd from buffer */
    uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
    Eeprom_ReadBlock((uns8*)&nextCmd, tempAddress, sizeof(nextCmd));

    switch (nextCmd.cmd) {
    case LOOP_ON:
        //Trace_String("LOOP_ON;");
        /* move execute pointer to the next command */
        gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
        ScriptBufSetInLoop(TRUE);
        break;

    case LOOP_OFF:
        if (LOOP_INFINITE == nextCmd.data.loopEnd.counter) {
            //Trace_String("End of infinite loop reached;");
            /* move execute pointer to the top of this loop */
            gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
        } else if (nextCmd.data.loopEnd.counter > 1) {
            /*Trace_String("normal loop iteration");
               //Trace_Hex(nextCmd.data.loopEnd.counter);
               //Trace_Hex(nextCmd.data.loopEnd.depth);
               Trace_String(";");*/
            /* update counter and set execute pointer to start of the loop */
            nextCmd.data.loopEnd.counter--;
            Eeprom_WriteBlock((uns8*)&nextCmd, tempAddress, sizeof(struct led_cmd));

            /* move execute pointer to the top of this loop */
            gScriptBuf.execute = nextCmd.data.loopEnd.startIndex;
        } else {
            if (0 == nextCmd.data.loopEnd.depth) {
                //Trace_String("End of top loop reached;");
                /* move execute pointer to the next command */
                gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);

                /* delete loop body from buffer */
                ScriptBufSetRead(gScriptBuf.execute);
                ScriptBufSetInLoop(FALSE);
            } else {
                //Trace_String("End of inner loop reached;");
                /* reinit counter for next iteration */
                nextCmd.data.loopEnd.counter = nextCmd.data.loopEnd.numLoops;
                uns16 tempAddress = ScriptBufAddr(gScriptBuf.execute);
                Eeprom_WriteBlock((uns8*)&nextCmd, tempAddress, sizeof(struct led_cmd));

                /* move execute pointer to the next command */
                gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
            }
        }
        break;

    case SET_FADE:
        Ledstrip_SetFade(&nextCmd.data.set_fade);
        if (nextCmd.data.set_fade.parallelFade == 0)
            gScriptBuf.waitValue = ntohs(nextCmd.data.set_fade.fadeTmms);

        /* move execute pointer to the next command */
        gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
        if (!gScriptBuf.inLoop)
            ScriptBufSetRead(gScriptBuf.execute);
        break;

    case SET_GRADIENT:
        Ledstrip_SetGradient(&nextCmd.data.set_gradient);
        if ((nextCmd.data.set_gradient.parallelAndOffset & 0x80) == 0)
            gScriptBuf.waitValue = ntohs(nextCmd.data.set_gradient.fadeTmms);

        /* move execute pointer to the next command */
        gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
        if (!gScriptBuf.inLoop)
            ScriptBufSetRead(gScriptBuf.execute);
        break;

    case WAIT:
        /* TODO we should disable interrupts while changing waitValue */
        gScriptBuf.waitValue = ntohs(nextCmd.data.wait.waitTmms);
        /* move execute pointer to the next command */
        gScriptBuf.execute = ScriptBufInc(gScriptBuf.execute);
        if (!gScriptBuf.inLoop)
            ScriptBufSetRead(gScriptBuf.execute);
        break;
    }
}

uns8 ScriptCtrl_Write(const struct led_cmd* pCmd)
{
    /* if we write a new command, we set the scriptCtrl to running
     * ATTENTION Check if this behaviour is acceptable when whe use an alarmCtrl */
    if (!gScriptBuf.isRunning) gScriptBuf.isRunning = TRUE;

    uns8 writeNext = ScriptBufInc(gScriptBuf.write);
    if (writeNext != gScriptBuf.read) {
        uns16 tempAddress = ScriptBufAddr(gScriptBuf.write);
        Eeprom_WriteBlock((const uns8*)pCmd, tempAddress, sizeof(struct led_cmd));
        ScriptBufSetWrite(writeNext);
        return OK;
    }
    return SCRIPTBUFFER_FULL;
}

void ScriptCtrl_DecrementWaitValue(void)
{
    if (gScriptBuf.waitValue > 0)
        gScriptBuf.waitValue = gScriptBuf.waitValue - 1;
}

void ScriptCtrl_CheckAndDecrementWaitValue(void)
{
    if (gScriptBuf.waitValue < 8)
        gScriptBuf.waitValue = 0;
}
