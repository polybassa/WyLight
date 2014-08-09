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
// Application Name     -   DeepSleep Networking
// Application Overview -   The objective of this application is to showcase 
//                          the deepsleep as a power saving tool in a 
//                          networking context (in this case a TCP server).
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_Deepsleep_Application
// or
// \docs\examples\CC32xx Deepsleep Application.pdf
//
//*****************************************************************************


//****************************************************************************
//
//! \addtogroup deepsleep_nw
//! @{
//
//****************************************************************************
#include <stdlib.h>

// Simplelink includes
#include "simplelink.h"

//Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "timer.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

//Common interface includes
#include "gpio_if.h"
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "pinmux.h"
#include "timer_if.h"
#include "nw_if.h"

#ifdef NOTERM
#define DBG_PRINT(x)
#else
#define DBG_PRINT Report
#endif

#define APPLICATION_NAME	"DeepSleep Networking"
#define APPLICATION_VERSION     "1.0.0"

//
// Values for below macros shall be modified per the access-point's (AP) properties
// SimpleLink device will connect to following AP when the application is executed
//
#define SSID_NAME               "cc3200demo"      /* AP SSID */
#define SECURITY_TYPE           SL_SEC_TYPE_OPEN  /* Security type (OPEN or WEP or WPA) */
#define SECURITY_KEY            ""                /* Password of the secured AP */

#define APP_TCP_PORT            5001
#define SYS_CLK                 80000000
#define MAX_BUF                 512
#define FOREVER                 1
#define SPAWN_TASK_PRIORITY     9
#define OSI_STACK_SIZE		3000

//
// GLOBAL VARIABLES -- Start
//

// Variable used in Timer Interrupt Handler
unsigned short g_usTimerInts;
// AP Security Parameters
SlSecParams_t SecurityParams = {0};

//
// GLOBAL VARIABLES -- End
//

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
int CreateTCPServerSocket(unsigned int uiPortNum);
void TimerPeriodicIntHandler(void);
void LedTimerConfigNStart();
void LedTimerDeinitStop();
static void DisplayBanner(char * AppName);
static void BoardInit(void);
void TCPServerTask(void *pvParameters);

#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

//****************************************************************************
//                      LOCAL FUNCTION DEFINITIONS
//***************************************************************************

//****************************************************************************
//
//! Creates a TCP socket and binds to it
//!
//! \param uiPortNum is the port number to bind to
//! 
//! This function  
//!    1. Creates a TCP socket and binds to it
//!
//! \return Soceket Descriptor, < 1 if error.
//
//****************************************************************************
int CreateTCPServerSocket(unsigned int uiPortNum)
{
    int iSockDecs;
    sockaddr_in sServerAddress;

    //
    // Create a TCP socket
    //
    if((iSockDecs = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
    {
        DBG_PRINT("Socket creation failed\n");
        return -1;
    }
    //
    // Bind - Assign a port to the socket
    //
    sServerAddress.sin_family = AF_INET;
    sServerAddress.sin_addr.s_addr = INADDR_ANY;
    sServerAddress.sin_port = htons(uiPortNum);
    if ( bind(iSockDecs, 
          (struct sockaddr*)&sServerAddress, 
           sizeof(sServerAddress)) != 0 )
    {
        DBG_PRINT("Bind failed\n");
        return -1;
    }

    return iSockDecs;
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
    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t 	CC3200 %s Application       \n\r", AppName);
    Report("\t\t *************************************************\n\r");
    Report("\n\n\n\r");
}

//****************************************************************************
//
//! Task function implementing the TCP server and showcasing the  
//! deepsleep functionality
//!
//! \param none
//! 
//! This function  
//!    1. Creates a TCP socket and binds to it
//!    2. Listens on the socket
//!    3. Accepts a client connection
//!    4. Receives a packet on the socket
//!    5. Closes the socket
//!
//! \return None.
//
//****************************************************************************
void TCPServerTask(void *pvParameters)
{
    int iSocketDesc, iClientFD;
    struct sockaddr_in sClientAddr;
    int iRecvLen;
    unsigned int iClientAddrLen = sizeof(sClientAddr);
    unsigned char aucRecvBuffer[MAX_BUF];

    DisplayBanner(APPLICATION_NAME);

    DBG_PRINT("DEEPSLEEP: Test Begin\n\r");

    //
    // GPIO Configuration
    //
    GPIO_IF_LedConfigure(LED1|LED3);

    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);	

    //
    // Start the driver
    //
    Network_IF_InitDriver(ROLE_STA);

    GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);
    LedTimerConfigNStart();

    // Initialize AP security params
    SecurityParams.Key = SECURITY_KEY;
    SecurityParams.KeyLen = strlen(SECURITY_KEY);
    SecurityParams.Type = SECURITY_TYPE;

    //
    // Connect to the Access Point
    //
    Network_IF_ConnectAP(SSID_NAME,SecurityParams);

    LedTimerDeinitStop();

    //
    // Switch ON RED LED to indicate that Device acquired an IP
    //
    GPIO_IF_LedOn(MCU_IP_ALLOC_IND);

    //
    // Create and Bind TCP server socket
    //
    iSocketDesc = CreateTCPServerSocket(APP_TCP_PORT);
    if(iSocketDesc < 0)
    {
        DBG_PRINT("DEEPSLEEP: Socket create failed\n");
        goto end;
    }
    DBG_PRINT("DEEPSLEEP: Socket created\n\r");
    DBG_PRINT("DEEPSLEEP: Listening on Socket...\n\r");

    //
    // Start Listening on the socket
    //
    if( listen(iSocketDesc, 5) != 0)
    {
        DBG_PRINT("DEEPSLEEP: Listen failed\n\r");
        goto end;
    }

    while(FOREVER)
    {
        //
        // Accept new client connections (Here just one)
        //
        DBG_PRINT("DEEPSLEEP: Waiting for client.......\n\r");
        iClientFD = accept(iSocketDesc, 
                       (struct sockaddr*)&sClientAddr, 
                       &iClientAddrLen);
        DBG_PRINT("DEEPSLEEP: Connected to client [0x%x] \n\r", 
                           htonl(sClientAddr.sin_addr.s_addr));

        do
        {
            //
            // Receive on the socket
            //
            iRecvLen = recv(iClientFD,aucRecvBuffer,MAX_BUF,0);

            if(iRecvLen > 0)
            {
                DBG_PRINT("DEEPSLEEP: received %d bytes\n\r", iRecvLen);
                DBG_PRINT("DEEPSLEEP: received message : %s\n\r", aucRecvBuffer);
            }
        }while(iRecvLen > 0);

        //
        // Close the client descriptor
        //
        close(iClientFD);
    }

    //
    // Close the socket. Commented as not expected to reach here
    //
    //close(iSocketDesc);

end:
    DBG_PRINT("DEEPSLEEP: Test Complete\n\r");
    //
    // Loop here
    //
    while(1);
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
#if defined(ccs)
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
//!    2. Invokes the LPDSTCPServerTask
//!
//! \return None.
//
//****************************************************************************
void main()
{
    //
    // Initialize the board
    //
    BoardInit();

    //
    // Configure the pinmux settings for the peripherals exercised
    //
    PinMuxConfig();

#ifndef NOTERM
    //
    // Configuring UART
    //
    InitTerm();
    ClearTerm();
#endif

    //
    // Start the SimpleLink Host
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

    //
    // Start the TCPServer task
    //
    osi_TaskCreate(TCPServerTask,
                    (const signed char *)"DeepSleep TCP",
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
