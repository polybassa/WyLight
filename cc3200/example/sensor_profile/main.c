//*****************************************************************************
// main.c
//
// Implementing the sensor profiling using power mgmt framework
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

//****************************************************************************
//
//! \addtogroup sensor_profile
//! @{
//
//****************************************************************************
#include <string.h>
#include "hw_ints.h"
#include "hw_memmap.h"
#include "simplelink.h"
#include "pinmux.h"
#include "rom_map.h"

//middleware includes
#include "cc_types.h"
#include "rtc_hal.h"
#include "gpio_hal.h"
#include "uart_drv.h"
#include "cc_timer.h"
#include "cc_pm_ops.h"

//driverlib includes
#include "interrupt.h"
#include "prcm.h"
#include "utils.h"
#include "wdt.h"

//OS includes
#include "osi.h"
#ifndef USE_TIRTOS
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#endif
#include "wdt_if.h"

#define SPAWN_TASK_PRIORITY     (9)
#define SSID_NAME              "cc3200demo"
#define GPIO_13                 13
#define GPIO_17                 17
#define GPIO_SRC_WKUP			GPIO_13
#define APP_UDP_PORT            5001
#define HIB_DUR_SEC			    60
#define HIB_DUR_NSEC			0
#define TRAFFIC_DUR_SEC         5
#define TRAFFIC_DUR_NSEC        0
#define SERVER_IP_ADDRESS       0xC0A80067
#define FOREVER                 1
#define BUFF_SIZE               1400
#define SL_STOP_TIMEOUT         30
#define CONNECTION_RETRIES		5
#define WD_PERIOD_MS 				10000
#define MAP_SysCtlClockGet 			80000
#define MILLISECONDS_TO_TICKS(ms) 	(MAP_SysCtlClockGet * (ms))
#define OSI_STACK_SIZE          1024

#define UART_PRINT(x)           uart_write(g_tUartHndl, x, strlen(x))

enum ap_events{
	EVENT_CONNECTION = 0x1,
	EVENT_DISCONNECTION = 0x2,
	EVENT_IP_ACQUIRED = 0x4,
	WDOG_EXPIRED = 0x8,
	CONNECTION_FAILED = 0x10
};

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
void sl_WlanEvtHdlr(SlWlanEvent_t *pSlWlanEvent);
void sl_NetAppEvtHdlr(SlNetAppEvent_t *pNetAppEvent);
static void Display31xxBanner();
void PrintIPAddr(unsigned int uiIpaddr);
int WlanConnect();
void WlanDisconnect();
int gpio_intr_hndlr(int gpio_num);
void TimerCallback(void* pvParam);
cc_hndl SetTimer();
cc_hndl SetTimerAsWkUp();
cc_hndl SetGPIOAsWkUp();
static void BoardInit();
void TimerGPIOTask(void *pvParameters);
void SwitchToStaMode(int iMode);

//*****************************************************************************
//                      GLOBAL VARIABLES
//****************************************************************************
#ifndef USE_TIRTOS
#if defined(ccs) || defined(gcc)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
#endif

unsigned char g_ucTrafficEnable = 0;
unsigned long long g_ullTrafficDelay = 0;
unsigned char g_ucIsTrafficDelayCal = 0;
cc_hndl g_tUartHndl;
unsigned long g_ulIpAddr = 0;
unsigned char g_ucFeedWatchdog = 0;
unsigned char g_ucWdogCount = 0;
OsiMsgQ_t g_tWaitForHib = 0;
OsiMsgQ_t g_tConnection = 0;
unsigned char g_cBsdBuf[BUFF_SIZE];

extern void lp3p0_setup_power_policy(int power_policy);
extern int platform_init();

//*****************************************************************************
//
//! Display31xxBanner
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void
Display31xxBanner()
{
    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t   *********************************************\n\r");
    UART_PRINT("\t\t          CC3200 Sensor Profile Application   \n\r");
    UART_PRINT("\t\t   *********************************************\n\r");
    UART_PRINT("\n\n\n\r");

}

//*****************************************************************************
//
//! Application defined hook (or callback) function - the tick hook.
//! The tick interrupt can optionally call this
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationTickHook( void )
{
}

//*****************************************************************************
//
//! Application defined hook (or callback) function - assert
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vAssertCalled( const char *pcFile, unsigned long ulLine )
{
	while(1)
	{
	}
}

//*****************************************************************************
//
//! Application defined idle task hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationIdleHook( void )
{
    cc_idle_task_pm();
}

//*****************************************************************************
//
//! Application defined malloc fail hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
	while(1)
	{
	}
}

//*****************************************************************************
//
//! Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook()
{
	while(1)
	{
	}
}

//*****************************************************************************
//
//! Print the IP address in the dotted decimal form
//!
//! \param  uiIpaddr is the IP address of the device in integer form
//!
//! \return none
//!
//*****************************************************************************
void PrintIPAddr(unsigned int uiIpaddr)
{
	char pcIpString[16];
	unsigned char ucLen1, ucLen2, ucLen3, ucLen4;
	ucLen1 = sprintf(pcIpString, "%u", (uiIpaddr & 0xFF000000)>>24);
	ucLen2 = sprintf((pcIpString + ucLen1), ".%u", (uiIpaddr & 0x00FF0000)>>16);
	ucLen3 = sprintf((pcIpString + ucLen1 + ucLen2), ".%u",
					 (uiIpaddr & 0x0000FF00)>>8);
	ucLen4 = sprintf((pcIpString + ucLen1 + ucLen2 + ucLen3), ".%u\0",
					 (uiIpaddr & 0x000000FF));
	UNUSED(ucLen4);
	UART_PRINT(pcIpString);
}

//****************************************************************************
//
//! \brief This function handles WLAN events
//!
//! \param[in]  pSlWlanEvent is the event passed to the handler
//!
//! \return	None
//
//****************************************************************************
void sl_WlanEvtHdlr(SlWlanEvent_t *pSlWlanEvent)
{
	unsigned char ucQueueMsg = 0;
	switch(pSlWlanEvent->Event)
	{
	case SL_WLAN_CONNECT_EVENT:
		ucQueueMsg = (unsigned char)EVENT_CONNECTION;
		if(g_tConnection != 0)
		{
			osi_MsgQWrite(&g_tConnection, &ucQueueMsg, OSI_WAIT_FOREVER);
		}
		else
		{
            UART_PRINT("Error: sl_WlanEvtHdlr: Queue does not exist\n\r");
			while(FOREVER);
		}
		UART_PRINT("C\n\r");
		break;
	case SL_WLAN_DISCONNECT_EVENT:
		ucQueueMsg = (unsigned char)EVENT_DISCONNECTION;
		if(g_tConnection != 0)
		{
			osi_MsgQWrite(&g_tConnection, &ucQueueMsg, OSI_WAIT_FOREVER);
		}
		else
		{
            UART_PRINT("Error: sl_WlanEvtHdlr: Queue does not exist\n\r");
			while(FOREVER);
		}
		UART_PRINT("D\n\r");
		break;
	default:
		break;
	}
}

//****************************************************************************
//
//! \brief This function handles events for IP address acquisition via DHCP
//!		   indication
//!
//! \param[in]    pNetAppEvent is the event passed to the handler
//!
//! \return     None
//
//****************************************************************************
void sl_NetAppEvtHdlr(SlNetAppEvent_t *pNetAppEvent)
{
	unsigned char ucQueueMsg = 0;
	switch(pNetAppEvent->Event)
	{
	case SL_NETAPP_IPV4_ACQUIRED:
	case SL_NETAPP_IPV6_ACQUIRED:
		g_ulIpAddr = pNetAppEvent->EventData.ipAcquiredV4.ip;
		ucQueueMsg = (unsigned char)EVENT_IP_ACQUIRED;
		if(g_tConnection != 0)
		{
			osi_MsgQWrite(&g_tConnection, &ucQueueMsg, OSI_WAIT_FOREVER);
		}
		else
		{
            UART_PRINT("Error: sl_NetAppEvtHdlr: Queue does not exist\n\r");
			while(FOREVER);
		}
		UART_PRINT("IP: ");
		PrintIPAddr(g_ulIpAddr);
		UART_PRINT("\n\r");
		break;
	default:
		break;
	}
}

//*****************************************************************************
//
//! This function gets triggered when HTTP Server receives Application
//! defined GET and POST HTTP Tokens.
//!
//! \param pSlHttpServerEvent Pointer indicating http server event
//! \param pSlHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void sl_HttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent,
                           SlHttpServerResponse_t *pSlHttpServerResponse)
{
}

//*****************************************************************************
//
//! This function handles socket events indication
//!
//! \param[in]      pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    //
    // This application doesn't work w/ socket - Events are not expected
    //

}

//*****************************************************************************
//
//! The interrupt handler for the watchdog timer
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
void WatchdogIntHandler(void)
{
    //
    // If we have been told to stop feeding the watchdog, return immediately
    // without clearing the interrupt.  This will cause the system to reset
    // next time the watchdog interrupt fires.
    //
    if(!g_ucFeedWatchdog)
    {
        return;
    }

    unsigned char ucQueueMsg = 0;
    if(g_ucWdogCount < CONNECTION_RETRIES)
    {
    	g_ucWdogCount++;
		ucQueueMsg = (unsigned char)WDOG_EXPIRED;
		MAP_WatchdogIntClear(WDT_BASE);
    }
    else
    {
        g_ucFeedWatchdog = 0;
    	ucQueueMsg = (unsigned char)CONNECTION_FAILED;
        MAP_WatchdogIntClear(WDT_BASE);
    }
    if(g_tConnection != 0)
    {
        osi_MsgQWrite(&g_tConnection, &ucQueueMsg, OSI_NO_WAIT);
    }
    else
    {
        UART_PRINT("Error: WatchdogIntHandler: Queue does not exist\n\r");
    }
}

//*****************************************************************************
//
//! \brief Connecting to a WLAN Accesspoint
//! This function connects to the required AP (SSID_NAME).
//! This code example assumes the AP doesn't use WIFI security.
//! The function will return only once we are connected
//! and have acquired IP address
//!
//! \param[in] None
//!
//! \return 0 means success, -1 means failure
//!
//! \note
//!
//! \warning    If the WLAN connection fails or we don't aquire an IP address,
//!             We will be stuck in this function forever.
//
//*****************************************************************************
int WlanConnect()
{
	int iRetCode = 0;
	int iRetVal = 0;
	int iConnect = 0;
	unsigned char ucQueueMsg = 0;
    SlSecParams_t secParams;

    secParams.Key = "";
    secParams.KeyLen = sizeof(secParams.Key);
    secParams.Type = SL_SEC_TYPE_OPEN;

	//
	// Enable the peripherals used by this example.
	//
	MAP_PRCMPeripheralClkEnable(PRCM_WDT, PRCM_RUN_MODE_CLK);

	//
	// Set up the watchdog interrupt handler.
	//
	WDT_IF_Init(WatchdogIntHandler, MILLISECONDS_TO_TICKS(WD_PERIOD_MS));

	//
	// Start the timer. Once the timer is started, it cannot be disable.
	//
	MAP_WatchdogEnable(WDT_BASE);
	g_ucFeedWatchdog = 1;
	while(!(ucQueueMsg & (EVENT_IP_ACQUIRED|CONNECTION_FAILED)))
	{
		UART_PRINT("Trying to connect to AP: ");
		UART_PRINT(SSID_NAME);
		UART_PRINT("\n\r");
		sl_WlanConnect(SSID_NAME, strlen(SSID_NAME), 0, &secParams, 0);
		iConnect = 0;
		do{
			osi_MsgQRead(&g_tConnection, &ucQueueMsg, OSI_WAIT_FOREVER);

			switch(ucQueueMsg)
            {
                case EVENT_CONNECTION:
                    iConnect = 1;
                    break;

                case EVENT_IP_ACQUIRED:
                    iRetVal = 0;
                    break;

                case WDOG_EXPIRED:
                    
                    //
                    // disconnect from the Access Point
                    //
                    if(iConnect)
                    {
                        WlanDisconnect();
                    }

                    //
                    // stop the simplelink with reqd. timeout value (30 ms)
                    //
                    sl_Stop(SL_STOP_TIMEOUT);
                    
                    UART_PRINT("sl stop\n\r");
                    
                    MAP_UtilsDelay(8000);

                    //
                    // starting the simplelink
                    //
                    sl_Start(NULL, NULL, NULL);
                    
                    UART_PRINT("sl start\n\r");
                    break;

                case EVENT_DISCONNECTION:
                    iConnect = 0;
                    break;

                case CONNECTION_FAILED:
                    iRetVal = -1;
                    break;

                default:
                    UART_PRINT("unexpected event\n\r");
                    break;
			}
		}while(ucQueueMsg == (unsigned char)EVENT_CONNECTION);
	}
	iRetCode = MAP_WatchdogRunning(WDT_BASE);
	if(iRetCode)
	{
	   WDT_IF_DeInit();
	   MAP_PRCMPeripheralClkDisable(PRCM_WDT, PRCM_RUN_MODE_CLK);
	}
    return(iRetVal);
}

//*****************************************************************************
//
//! \brief disconnection from the WLAN Accesspoint
//!
//! \param[in] None
//!
//! \return None
//!
//! \note
//!
//! \warning    If the WLAN disconnection fails, We will be stuck in this
//!             function forever.
//
//*****************************************************************************
void WlanDisconnect()
{
	unsigned char ucQueueMsg = 0;
	while(ucQueueMsg != (unsigned char)EVENT_DISCONNECTION)
	{
		sl_WlanDisconnect();
		osi_MsgQRead(&g_tConnection, &ucQueueMsg, OSI_WAIT_FOREVER);
	}
    return;
}

//*****************************************************************************
//
//! \brief callback function for gpio interrupt handler (no used here)
//!
//! \param gpio_num is the gpio number which has triggered the interrupt
//!
//! \return 0
//
//*****************************************************************************
int gpio_intr_hndlr(int gpio_num)
{
    return 0;
}

//*****************************************************************************
//
//! \brief callback function for timer interrupt handler
//!
//! \param vParam is a general void pointer (not used here)
//!
//! \return None
//
//*****************************************************************************
void TimerCallback(void* pvParam)
{
    g_ucTrafficEnable = 0;
    return;
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
    IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
  //
  // Enable Processor
  //
  MAP_IntMasterEnable();
  MAP_IntEnable(FAULT_SYSTICK);

  PRCMCC3200MCUInit();
}

//*****************************************************************************
//
//! \brief introduce required delay to manage desired udp tx rate
//!
//! \param  uiTrafficRate is desired udp tx rate in KBps.
//! \param  uiMsgSize is the size of each udp tx packet in Bytes.
//!
//! \return none
//!
//*****************************************************************************
void ManageDelay(unsigned int uiTrafficRate, unsigned int uiMsgSize)
{
    if(g_ucIsTrafficDelayCal == 0)
    {
        g_ullTrafficDelay = ((((1000 * (unsigned long long)uiMsgSize)* 80000)/
                            ((uiTrafficRate * 1024)*6) ));
        if(g_ullTrafficDelay <= 10840)
        {
            g_ullTrafficDelay = 0;
        }
        else
        {
            // Delay adjustment considering code execution time (based on test)
            g_ullTrafficDelay -= 10840;
        }
        // flag indicating the delay has been calculated would not need
        // recalculation.
        g_ucIsTrafficDelayCal = 1;
    }
    MAP_UtilsDelay(g_ullTrafficDelay);
}

//*****************************************************************************
//
//! \brief set GPIO as a wake up source from low power modes.
//!
//! \param none
//!
//! \return handle for the gpio used
//
//*****************************************************************************
cc_hndl SetGPIOAsWkUp()
{
    cc_hndl tGPIOHndl;
    //
    // setting up GPIO as a wk up source and configuring other related
    // parameters
    //
    tGPIOHndl = cc_gpio_open(GPIO_SRC_WKUP, GPIO_DIR_INPUT);
    cc_gpio_enable_notification(tGPIOHndl, GPIO_SRC_WKUP, INT_FALLING_EDGE,
                                (GPIO_TYPE_NORMAL | GPIO_TYPE_WAKE_SOURCE));
    return(tGPIOHndl);
}

//*****************************************************************************
//
//! \brief set Timer as a wake up source from low power modes.
//!
//! \param none
//!
//! \return handle for the Timer setup as a wakeup source
//
//*****************************************************************************
cc_hndl SetTimerAsWkUp()
{
    cc_hndl tTimerHndl;
    struct cc_timer_cfg sRealTimeTimer;
    struct u64_time sInitTime, sIntervalTimer;
    //
    // setting up Timer as a wk up source and other timer configurations
    //
    sInitTime.secs = 0;
    sInitTime.nsec = 0;
    cc_rtc_set(&sInitTime);

    sRealTimeTimer.source = HW_REALTIME_CLK;
    sRealTimeTimer.timeout_cb = NULL;
    sRealTimeTimer.cb_param = NULL;

    tTimerHndl = cc_timer_create(&sRealTimeTimer);

    sIntervalTimer.secs = HIB_DUR_SEC;
    sIntervalTimer.nsec = HIB_DUR_NSEC;
    cc_timer_start(tTimerHndl, &sIntervalTimer, OPT_TIME_ABS_VALUE);
    return(tTimerHndl);
}

//*****************************************************************************
//
//! \brief set Timer for transmitting udp packets for desired time.
//!
//! \param none
//!
//! \return handle for the Timer setup
//
//*****************************************************************************
cc_hndl SetTimer()
{
    cc_hndl tTimerHndl;
    struct cc_timer_cfg sHighResTimer;
    struct u64_time sInitTime, sIntervalTimer;

    sInitTime.secs = 0;
    sInitTime.nsec = 0;
    cc_rtc_set(&sInitTime);

    sHighResTimer.source = HW_REALTIME_CLK;
    sHighResTimer.timeout_cb = TimerCallback;
    sHighResTimer.cb_param = NULL;

    tTimerHndl = cc_timer_create(&sHighResTimer);

    sIntervalTimer.secs = TRAFFIC_DUR_SEC;
    sIntervalTimer.nsec = TRAFFIC_DUR_NSEC;
    cc_timer_start(tTimerHndl, &sIntervalTimer, OPT_TIME_ABS_VALUE);
    return(tTimerHndl);
}

//*****************************************************************************
//
//! Check the device mode and switch to STATION(STA) mode
//! restart the NWP to activate STATION mode
//!
//! \param  iMode (device mode)
//!
//! \return None
//
//*****************************************************************************
void SwitchToStaMode(int iMode)
{
	if(iMode != ROLE_STA)
	{
	    sl_WlanSetMode(ROLE_STA);
	    MAP_UtilsDelay(80000);
	    sl_Stop(10);
	    MAP_UtilsDelay(80000);
	    sl_Start(0,0,0);
	}

}

//*****************************************************************************
//
//! \brief Task Created by main fucntion. This task prints the wake up reason
//!        (from hibernate or from restart). start simplelink, set NWP power
//!        policy and connects to an AP. Creates UDP client and send UDP
//!        packets at around 1Mbit/sec for certain time. Disconnect form AP
//!        and stops the simplelink.Setup GPIO and Timer as wakeup source from
//!        low power modes. Go into HIBernate.
//!
//! \param pvParameters is a general void pointer (not used here).
//!
//! \return none
//
//*****************************************************************************
void TimerGPIOTask(void *pvParameters)
{
    cc_hndl tTimerHndl;
    cc_hndl tGPIOHndl;
    int iSockDesc = 0;
    int iRetVal = 0;
    int iCounter = 0;
    sockaddr_in sServerAddr;
    unsigned char *pcSendBuff;
    unsigned char cSyncMsg;

    // filling the buffer
    for (iCounter=0 ; iCounter<BUFF_SIZE ; iCounter++)
    {
        g_cBsdBuf[iCounter] = (char)(iCounter % 10);
    }
    pcSendBuff = g_cBsdBuf;

    if(MAP_PRCMSysResetCauseGet() == PRCM_POWER_ON)
    {
        //
        // Displays the Application Banner
        //
        Display31xxBanner();
    }
    else if(MAP_PRCMSysResetCauseGet() == PRCM_HIB_EXIT)
    {
        UART_PRINT("woken from hib\n\r");
    }
    else if(MAP_PRCMSysResetCauseGet() == PRCM_WDT_RESET)
    {
        UART_PRINT("woken from WDT Reset\n\r");
    }
    else
    {
        UART_PRINT("woken cause unknown\n\r");
    }

    //
    // starting the simplelink
    //
    iRetVal = sl_Start(NULL, NULL, NULL);

    //
    // Switch to STA mode if device is not in this mode
    //
    SwitchToStaMode(iRetVal);

    //
    // Set the power management policy of NWP
    //
    sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
    
    osi_MsgQCreate(&g_tConnection, NULL, sizeof( unsigned char ), 3);
    
    //
    // connecting to the Access Point
    //
    if(-1 == WlanConnect())
    {

        goto no_network_connection;
    }else{
    	UART_PRINT("Connected to AP\n\r");
    }
    
    //
    // creating a UDP socket
    //
    iSockDesc = sl_Socket(SL_AF_INET,SL_SOCK_DGRAM, 0);

    if(iSockDesc < 0)
    {
        UART_PRINT("sock error\n\r");
        while(FOREVER);
    }

    //
    // configure the UDP Server address
    //
    sServerAddr.sin_family = SL_AF_INET;
    sServerAddr.sin_port = sl_Htons(APP_UDP_PORT);
    sServerAddr.sin_addr.s_addr = sl_Htonl(SERVER_IP_ADDRESS);

    //
    // Set 5 sec timer allowing 5 sec of UDP Tx.
    //
    tTimerHndl = SetTimer();

    g_ucTrafficEnable = 1;
    while(g_ucTrafficEnable == 1)
    {
        //
        // sending message
        //
        iRetVal = sendto(iSockDesc, pcSendBuff,BUFF_SIZE, 0,
                        (struct sockaddr *)&sServerAddr,sizeof(sServerAddr));
        if(iRetVal < 0)
        {
            UART_PRINT("send error\n\r");
            while(FOREVER);
        }
        ManageDelay(128,BUFF_SIZE);
    }
    UART_PRINT("sent\n\r");

    //
    // stop and delete the timer
    //
    cc_timer_stop(tTimerHndl);
    cc_timer_delete(tTimerHndl);

    //
    //close the socket
    //
    close(iSockDesc);
    
    //
    // disconnect from the Access Point
    //
    WlanDisconnect();

no_network_connection:
    //
    // stop the simplelink with reqd. timeout value (30 ms)
    //
    sl_Stop(SL_STOP_TIMEOUT);

    //
    // setting Timer as one of the wakeup source
    //
    tTimerHndl = SetTimerAsWkUp();

    //
    // setting some GPIO as one of the wakeup source
    //
    tGPIOHndl = SetGPIOAsWkUp();

    /* handles, if required, can be used to stop the timer, but not used here*/
    UNUSED(tTimerHndl);
    UNUSED(tGPIOHndl);

    //
    // Setting up HIBERNATE as the lowest power mode for the system.
    //
    lp3p0_setup_power_policy(POWER_POLICY_HIBERNATE);

    //
    // idle wait will push the system into the lowest power mode(HIBERNATE).
    //
    osi_MsgQCreate(&g_tWaitForHib, NULL, sizeof( unsigned char ), 1);
    osi_MsgQRead(&g_tWaitForHib, &cSyncMsg, OSI_WAIT_FOREVER);

    //
    // infinite loop (must not reach here)
    //
    while(FOREVER);

}

//****************************************************************************
//							MAIN FUNCTION
//****************************************************************************
void main(void)
{
    //
    // Board Initialisation
    //
    BoardInit();

    //
    // Configure the pinmux settings for the peripherals exercised
    //
    PinMuxConfig();

    //
    // Initialize the platform
    //
    platform_init();

    //
    // Configuring UART
    //
    g_tUartHndl = uart_open(PRCM_UARTA0);

    //
    // Start the SimpleLink Host
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

    //
    // setting up timer and gpio as source for wake up from HIBERNATE
    //
    osi_TaskCreate(TimerGPIOTask,
                (const signed char *)"set wk_src for hibernate",
                OSI_STACK_SIZE, NULL, 1, NULL );

    //
    // Start the task scheduler
    //
    osi_start();

}

