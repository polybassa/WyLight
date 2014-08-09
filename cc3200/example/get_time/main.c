//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************


//*****************************************************************************
//
// Application Name     -   Get Time
// Application Overview -   Get Time application connects to a SNTP server,
//                            request for time, process the data and displays
//                            the time on Hyperterminal. The communication between
//                            the device and server is based on the Network Time
//                            Protocol (NTP)
//
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_Info_Center_Get_Time_Application
// or
// docs\examples\CC32xx_Info_Center_Get_Time_Application.pdf
//
//*****************************************************************************


//****************************************************************************
//
//! \addtogroup get_time
//! @{
//
//****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// simplelink includes
#include "datatypes.h"
#include "socket.h"

// driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "interrupt.h"
#include "prcm.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "timer.h"
#include "utils.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

// common interface includes
#include "network_if.h"
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "gpio_if.h"
#include "timer_if.h"
#include "udma_if.h"
#include "pinmux.h"


#ifdef NOTERM
#define UART_PRINT(x, ...)
#else
#define UART_PRINT Report
#endif

#define APP_NAME                "Get Time"
#define APPLICATION_VERSION     "1.0.0"

//
// Values for below macros shall be modified per the access-point's (AP) properties
// SimpleLink device will connect to following AP when the application is executed
//
#define SSID_NAME               "cc3200demo"      /* AP SSID */
#define SECURITY_TYPE           SL_SEC_TYPE_OPEN  /* Security type (OPEN or WEP or WPA)*/
#define SECURITY_KEY            ""                /* Password of the secured AP */

#define SPAWN_TASK_PRIORITY     9

#define TIME2013                3565987200      /* 113 years + 28 days(leap) */
#define YEAR2013                2013
#define SEC_IN_MIN              60
#define SEC_IN_HOUR             3600
#define SEC_IN_DAY              86400

#define GMT_DIFF_TIME_HRS       5
#define GMT_DIFF_TIME_MINS      30

#define SLEEP_TIME              80000000
#define OSI_STACK_SIZE          2048

//
// GLOBAL VARIABLES -- Start
//
unsigned short g_usTimerInts;
SlSecParams_t SecurityParams = {0};

#if defined(ccs) || defined(gcc)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

//!    ######################### list of SNTP servers #################################
//!    ##
//!    ##          hostname             |        IP       |       location
//!    ## -----------------------------------------------------------------------------
//!    ##   ntp.inode.at                | 195.58.160.5    |       Vienna
//!    ##   ntp3.proserve.nl            | 212.204.198.85  |       Amsterdam
//!    ##   ntp.spadhausen.com          | 109.168.118.249 |       Milano - Italy
//!    ##   ntp3.tcpd.net               | 23.23.128.218   |       London, UK
//!    ##   dmz0.la-archdiocese.net     | 209.151.225.100 |       Los Angeles, CA
//!    ##   Optimussupreme.64bitVPS.com | 216.128.88.62   |       Brooklyn, New York
//!    ##   ntp.mazzanet.id.au          | 203.206.205.83  |       Regional Victoria, Australia
//!    ##   a.ntp.br                    | 200.160.0.8     |       Sao Paulo, Brazil
//!    ################################################################################
const char g_acSNTPserver[30] = "ntp.inode.at"; //Add any one of the above servers

// Tuesday is the 1st day in 2013 - the relative year
const char g_acDaysOfWeek2013[7][3] = {{"Tue"},
                                    {"Wed"},
                                    {"Thu"},
                                    {"Fri"},
                                    {"Sat"},
                                    {"Sun"},
                                    {"Mon"}};

const char g_acMonthOfYear[12][3] = {{"Jan"},
                                  {"Feb"},
                                  {"Mar"},
                                  {"Apr"},
                                  {"May"},
                                  {"Jun"},
                                  {"Jul"},
                                  {"Aug"},
                                  {"Sep"},
                                  {"Oct"},
                                  {"Nov"},
                                  {"Dec"}};

const char g_acNumOfDaysPerMonth[12] = {31, 28, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};

const char g_acDigits[] = "0123456789";

struct
{
    unsigned long ulDestinationIP;
    int iSockID;
    unsigned long ulElapsedSec;
    short isGeneralVar;
    unsigned long ulGeneralVar;
    unsigned long ulGeneralVar1;
    char acTimeStore[30];
    char *pcCCPtr;
    unsigned short uisCCLen;
}g_sAppData;

//
// GLOBAL VARIABLES -- End
//



//*****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//*****************************************************************************
void GetNTPTimeTask(void *pvParameters);
static void GetSNTPTime(unsigned char ucGmtDiffHr, unsigned char ucGmtDiffMins);
static void DisplayBanner(char * AppName);


//*****************************************************************************
//
//! Gets the current time from the selected SNTP server
//!
//! \brief  This function obtains the NTP time from the server.
//!
//! \param  GmtDiffHr is the GMT Time Zone difference in hours
//! \param  GmtDiffMins is the GMT Time Zone difference in minutes
//!
//! \return None
//!
//
//*****************************************************************************
void GetSNTPTime(unsigned char ucGmtDiffHr, unsigned char ucGmtDiffMins)
{
  
/*
                            NTP Packet Header:


       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9  0  1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |LI | VN  |Mode |    Stratum    |     Poll      |   Precision    |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                          Root  Delay                           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                       Root  Dispersion                         |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                     Reference Identifier                       |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                    Reference Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                    Originate Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                     Receive Timestamp (64)                     |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                     Transmit Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                 Key Identifier (optional) (32)                 |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                                                                |
      |                 Message Digest (optional) (128)                |
      |                                                                |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
    char cDataBuf[48];
    int iRet = 0;
    SlSockAddr_t sAddr;
    SlSockAddrIn_t sLocalAddr;
    int iAddrSize;
    //
    // Send a query ? to the NTP server to get the NTP time
    //
    memset(cDataBuf, 0, sizeof(cDataBuf));
    cDataBuf[0] = '\x1b';

    sAddr.sa_family = AF_INET;
    // the source port
    sAddr.sa_data[0] = 0x00;
    sAddr.sa_data[1] = 0x7B;    // UDP port number for NTP is 123
    sAddr.sa_data[2] = (char)((g_sAppData.ulDestinationIP>>24)&0xff);
    sAddr.sa_data[3] = (char)((g_sAppData.ulDestinationIP>>16)&0xff);
    sAddr.sa_data[4] = (char)((g_sAppData.ulDestinationIP>>8)&0xff);
    sAddr.sa_data[5] = (char)(g_sAppData.ulDestinationIP&0xff);

    iRet = sl_SendTo(g_sAppData.iSockID,
                     cDataBuf,
                     sizeof(cDataBuf), 0,
                     &sAddr, sizeof(sAddr));
    if (iRet != sizeof(cDataBuf))
    {
        UART_PRINT("Could not send SNTP request\n\r\n\r");
        return;    // could not send SNTP request
    }

    //
    // Wait to receive the NTP time from the server
    //
    iAddrSize = sizeof(SlSockAddrIn_t);
    sLocalAddr.sin_family = SL_AF_INET;
    sLocalAddr.sin_port = 0;
    sLocalAddr.sin_addr.s_addr = 0;
    sl_Bind(g_sAppData.iSockID,
            (SlSockAddr_t *)&sLocalAddr,
            iAddrSize);

    iRet = sl_RecvFrom(g_sAppData.iSockID,
                       cDataBuf, sizeof(cDataBuf), 0,
                       (SlSockAddr_t *)&sLocalAddr,
                       (SlSocklen_t*)&iAddrSize);
    if (iRet <= 0)
    {
        UART_PRINT("Did not receive\n\r");
        return;
    }

    //
    // Confirm that the MODE is 4 --> server
    //
    if ((cDataBuf[0] & 0x7) != 4)    // expect only server response
    {
        UART_PRINT("Expecting response from Server Only!\n\r");
        return;    // MODE is not server, abort
    }
    else
    {
        char iIndex;

        //
        // Getting the data from the Transmit Timestamp (seconds) field
        // This is the time at which the reply departed the
        // server for the client
        //
        g_sAppData.ulElapsedSec = cDataBuf[40];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[41];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[42];
        g_sAppData.ulElapsedSec <<= 8;
        g_sAppData.ulElapsedSec += cDataBuf[43];

        //
        // seconds are relative to 0h on 1 January 1900
        //
        g_sAppData.ulElapsedSec -= TIME2013;

        //
        // in order to correct the timezone
        //
        g_sAppData.ulElapsedSec += (ucGmtDiffHr * SEC_IN_HOUR);
        g_sAppData.ulElapsedSec += (ucGmtDiffMins * SEC_IN_MIN);

        g_sAppData.pcCCPtr = &g_sAppData.acTimeStore[0];

        //
        // day, number of days since beginning of 2013
        //
        g_sAppData.isGeneralVar = g_sAppData.ulElapsedSec/SEC_IN_DAY;
        memcpy(g_sAppData.pcCCPtr,
               g_acDaysOfWeek2013[g_sAppData.isGeneralVar%7], 3);
        g_sAppData.pcCCPtr += 3;
        *g_sAppData.pcCCPtr++ = '\x20';

        //
        // month
        //
        g_sAppData.isGeneralVar %= 365;
        for (iIndex = 0; iIndex < 12; iIndex++)
        {
            g_sAppData.isGeneralVar -= g_acNumOfDaysPerMonth[iIndex];
            if (g_sAppData.isGeneralVar < 0)
                    break;
        }
        if(iIndex == 12)
        {
            iIndex = 0;
        }
        memcpy(g_sAppData.pcCCPtr, g_acMonthOfYear[iIndex], 3);
        g_sAppData.pcCCPtr += 3;
        *g_sAppData.pcCCPtr++ = '\x20';

        //
        // date
        // restore the day in current month
        //
        g_sAppData.isGeneralVar += g_acNumOfDaysPerMonth[iIndex];
        g_sAppData.uisCCLen = itoa(g_sAppData.isGeneralVar + 1,
                                   g_sAppData.pcCCPtr);
        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = '\x20';

        //
        // time
        //
        g_sAppData.ulGeneralVar = g_sAppData.ulElapsedSec%SEC_IN_DAY;

        // number of seconds per hour
        g_sAppData.ulGeneralVar1 = g_sAppData.ulGeneralVar%SEC_IN_HOUR;

        // number of hours
        g_sAppData.ulGeneralVar /= SEC_IN_HOUR;
        g_sAppData.uisCCLen = itoa(g_sAppData.ulGeneralVar,
                                   g_sAppData.pcCCPtr);
        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = ':';

        // number of minutes per hour
        g_sAppData.ulGeneralVar = g_sAppData.ulGeneralVar1/SEC_IN_MIN;

        // number of seconds per minute
        g_sAppData.ulGeneralVar1 %= SEC_IN_MIN;
        g_sAppData.uisCCLen = itoa(g_sAppData.ulGeneralVar,
                                   g_sAppData.pcCCPtr);
        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = ':';
        g_sAppData.uisCCLen = itoa(g_sAppData.ulGeneralVar1,
                                   g_sAppData.pcCCPtr);
        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;
        *g_sAppData.pcCCPtr++ = '\x20';

        //
        // year
        // number of days since beginning of 2013
        //
        g_sAppData.ulGeneralVar = g_sAppData.ulElapsedSec/SEC_IN_DAY;
        g_sAppData.ulGeneralVar /= 365;
        g_sAppData.uisCCLen = itoa(YEAR2013 + g_sAppData.ulGeneralVar,
                                   g_sAppData.pcCCPtr);
        g_sAppData.pcCCPtr += g_sAppData.uisCCLen;

        *g_sAppData.pcCCPtr++ = '\0';

        UART_PRINT("response from server: ");
        UART_PRINT((char *)g_acSNTPserver);
        UART_PRINT("\n\r");
        UART_PRINT(g_sAppData.acTimeStore);
        UART_PRINT("\n\r\n\r");
    }
}
//*****************************************************************************
//
//! Periodic Timer Interrupt Handler
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void
TimerPeriodicIntHandler(void)
{
    unsigned long ulInts;

    //
    // Clear all pending interrupts from the timer we are
    // currently using.
    //
    ulInts = MAP_TimerIntStatus(TIMERA0_BASE, true);
    MAP_TimerIntClear(TIMERA0_BASE, ulInts);

    //
    // Increment our interrupt counter.
    //
    g_usTimerInts++;
    if(!(g_usTimerInts & 0x1))
    {
        //
        // Off Led
        //
        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    }
    else
    {
        //
        // On Led
        //
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    }
}

//****************************************************************************
//
//! Function to configure and start timer to blink the LED while device is
//! trying to connect to an AP
//!
//! \param none
//!
//! return none
//
//****************************************************************************
void LedTimerConfigNStart()
{
    //
    // Configure Timer for blinking the LED for IP acquisition
    //
    Timer_IF_Init(PRCM_TIMERA0,TIMERA0_BASE,TIMER_CFG_PERIODIC,TIMER_A,0);
    Timer_IF_IntSetup(TIMERA0_BASE,TIMER_A,TimerPeriodicIntHandler);
    Timer_IF_Start(TIMERA0_BASE,TIMER_A,PERIODIC_TEST_CYCLES / 10);
}

//****************************************************************************
//
//! Disable the LED blinking Timer as Device is connected to AP
//!
//! \param none
//!
//! return none
//
//****************************************************************************
void LedTimerDeinitStop()
{
    //
    // Disable the LED blinking Timer as Device is connected to AP
    //
    Timer_IF_Stop(TIMERA0_BASE,TIMER_A);
    Timer_IF_DeInit(TIMERA0_BASE,TIMER_A);

}
//****************************************************************************
//
//! Task function implementing the gettime functionality using an NTP server
//!
//! \param none
//!
//! This function
//!    1. Initializes the required peripherals
//!    2. Initializes network driver and connects to the default AP
//!    3. Creates a UDP socket, gets the NTP server IP address using DNS
//!    4. Periodically gets the NTP time and displays the time
//!
//! \return None.
//
//****************************************************************************
void GetNTPTimeTask(void *pvParameters)
{
    int iSocketDesc;

    UART_PRINT("GET_TIME: Test Begin\n\r");

    //
    // Configure LED
    //
    GPIO_IF_LedConfigure(LED1|LED3);

    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);    


    //
    // Reset The state of the machine
    //
    Network_IF_ResetMCUStateMachine();

    //
    // Start the driver
    //
    Network_IF_InitDriver(ROLE_STA);

    LedTimerConfigNStart();

    // Initialize AP security params
    SecurityParams.Key = SECURITY_KEY;
    SecurityParams.KeyLen = strlen(SECURITY_KEY);
    SecurityParams.Type = SECURITY_TYPE;

    //
    // Connect to the Access Point
    //
    Network_IF_ConnectAP(SSID_NAME, SecurityParams);

    //
    // Disable the LED blinking Timer as Device is connected to AP
    //
    LedTimerDeinitStop();

    //
    // Switch ON RED LED to indicate that Device acquired an IP
    //
    GPIO_IF_LedOn(MCU_IP_ALLOC_IND);

    //
    // Create UDP socket
    //
    iSocketDesc = sl_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(iSocketDesc < 0)
    {
        UART_PRINT("Socket create failed\n\r");
        goto end;
    }
    UART_PRINT("Socket created\n\r");

    //
    // Get the NTP server host IP address using the DNS lookup
    //
    g_sAppData.ulDestinationIP = Network_IF_GetHostIP((char*)g_acSNTPserver);
    if( g_sAppData.ulDestinationIP != 0)
    {
        while(1)
        {
            //
            // Get the NTP time and display the time
            //
            GetSNTPTime(GMT_DIFF_TIME_HRS, GMT_DIFF_TIME_MINS);

            //
            // Wait a while before resuming
            //
            MAP_UtilsDelay(SLEEP_TIME);
        }
    }
    else
    {
        UART_PRINT("DNS lookup failed. \n\r");
    }

    //
    // Close the socket
    //
    close(iSocketDesc);
    UART_PRINT("Socket closed\n\r");

end:
    //
    // Stop the driver
    //
    Network_IF_DeInitDriver();

    UART_PRINT("GET_TIME: Test Complete\n\r");

    //
    // Loop here
    //
    while(1);
}

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void
DisplayBanner(char * AppName)
{

    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\t\t      CC3200 %s Application       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS

    //
    // Set vector table base
    //
#if defined(ccs) || defined(gcc)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif

    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

//****************************************************************************
//
//! Main function
//!
//! \param none
//!
//! This function
//!    1. Invokes the SLHost task
//!    2. Invokes the GetNTPTimeTask
//!
//! \return None.
//
//****************************************************************************
void main()
{
    //
    // Initialize Board configurations
    //
    BoardInit();

    //
    // Enable and configure DMA
    //
    UDMAInit();

    //
    // Pinmux for UART
    //
    PinMuxConfig();

    //
    // Configuring UART
    //
    InitTerm();

    //
    // Display Application Banner
    //
    DisplayBanner(APP_NAME);

    //
    // Start the SimpleLink Host
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

    //
    // Start the LPDSUDPBroadcast task
    //
    osi_TaskCreate(GetNTPTimeTask,
                    (const signed char *)"Get NTP Time",
                    OSI_STACK_SIZE,
                    NULL,
                    1,
                    NULL );

    //
    // Start the task scheduler
    //
    osi_start();
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
