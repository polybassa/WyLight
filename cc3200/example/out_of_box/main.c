//*****************************************************************************
// main.c - Implementing Out of box features
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
//! \addtogroup oob
//! @{
//
//****************************************************************************

#include <stdlib.h>
#include <string.h>

#include "simplelink.h"
#include "netcfg.h"
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "utils.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "osi.h"

#include "device_status.h"
#include "smartconfig.h"
#include "tmp006drv.h"
#include "bma222drv.h"
#include "pinmux.h"
#include "gpio_if.h"
#include "uart_if.h"
#include "i2c_if.h"

#define OOB_TASK_PRIORITY				(1)
#define SPAWN_TASK_PRIORITY				(9)
#define OSI_STACK_SIZE					(1024)
#define SSID_NAME               		"cc3200demo"
#define AP_SSID_LEN_MAX         		(32)

#define SH_GPIO_3       				(3)  /* P58 - Device Mode */

#define AUTO_CONNECTION_TIMEOUT_COUNT   (50)   /* 5 Sec */
#define SL_STOP_TIMEOUT                 (30)


const char pcDigits[] = "0123456789";

unsigned char POST_token[] = "__SL_P_ULD";
unsigned char GET_token_TEMP[]  = "__SL_G_UTP";
unsigned char GET_token_ACC[]  = "__SL_G_UAC";
unsigned char GET_token_UIC[]  = "__SL_G_UIC";

#define ROLE_INVALID (-5)

//*****************************************************************************
// CC3200 Network State Machine Definitions
//*****************************************************************************
enum MCUNetworkStateEnum
{
    MCU_SLHost_UNINIT       = 0x0001, // CC3200 NW Driver Uninitialized
    MCU_SLHost_INIT         = 0x0002, // CC3200 NW Driver Initialized
    MCU_AP_ASSOC            = 0x0004, // CC3200 Associated to AP
    MCU_IP_ALLOC            = 0x0008, // CC3200 has IP Address
    MCU_IP_LEASED           = 0x0010, // CC3200 Server Initialized
    MCU_SERVER_INIT         = 0x0020, // CC3200 Server Initialized
    MCU_CLIENT_CONNECTED    = 0x0040, // CC3200 Client Connected to Server
    MCU_PING_COMPLETE       = 0x0080, // Ping to AP or server completed
    MCU_CONNECTION_FAIL     = 0x0100, // Wlan connection failed
    MCU_SMARTCONFIG_START   = 0x0200, // SMARTCONFIG STARTED
    MCU_SMARTCONFIG_STOP    = 0x0400  // SMART CONFIG Stopped
};
//****************************************************************************
//                      GLOBAL VARIABLES
//****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

//*****************************************************************************
// Variable related to Connection status
//*****************************************************************************
volatile unsigned short g_usMCNetworkUstate = 0;

unsigned int g_uiIpAddress = 0;
int g_iInternetAccess = -1;
unsigned char g_ucConnectTimeout =0;
unsigned char g_ucDryerRunning = 0;
int g_uiSimplelinkRole = ROLE_INVALID;
unsigned int g_uiDeviceModeConfig = ROLE_STA; //default is STA mode

typedef enum
{
  LED_OFF = 0,
  LED_ON,
  LED_BLINK
}eLEDStatus;
unsigned char g_ucLEDStatus = LED_OFF;

#ifdef USE_FREERTOS
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
void
vApplicationTickHook( void )
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
void
vAssertCalled( const char *pcFile, unsigned long ulLine )
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
void
vApplicationIdleHook( void )
{

}

//*****************************************************************************
//
//! Application provided stack overflow hook function.
//!
//! \param  handle of the offending task
//! \param  name  of the offending task
//!
//! \return none
//!
//*****************************************************************************
void
vApplicationStackOverflowHook( OsiTaskHandle *pxTask, signed char *pcTaskName)
{
	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}

void vApplicationMallocFailedHook()
{
    while(1)
  {
    // Infinite loop;
  }
}
#endif


//****************************************************************************
//
//!	\brief This function handles WLAN events
//!
//! \param  pSlWlanEvent is the event passed to the handler
//!
//! \return None
//
//****************************************************************************
void sl_WlanEvtHdlr(SlWlanEvent_t *pSlWlanEvent)
{
  switch(((SlWlanEvent_t*)pSlWlanEvent)->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        	g_usMCNetworkUstate |= MCU_AP_ASSOC;
           break;
        case SL_WLAN_DISCONNECT_EVENT:
        	g_usMCNetworkUstate &= ~ MCU_AP_ASSOC;
           break;
        case SL_WLAN_SMART_CONFIG_START_EVENT:
        	/* SmartConfig operation finished */
        	/*The new SSID that was acquired is: pWlanEventHandler->EventData.smartConfigStartResponse.ssid */
        	/* We have the possiblity that also a private token was sent to the Host:
        	 *  if (pWlanEventHandler->EventData.smartConfigStartResponse.private_token_len)
        	 *    then the private token is populated: pWlanEventHandler->EventData.smartConfigStartResponse.private_token
        	 */
        	g_usMCNetworkUstate |= MCU_SMARTCONFIG_START;
        	g_usMCNetworkUstate &= ~MCU_SMARTCONFIG_STOP;
        	break;
       case SL_WLAN_SMART_CONFIG_STOP_EVENT:
    	   /* SmartConfig stop operation was completed */
    	   g_usMCNetworkUstate |= MCU_SMARTCONFIG_STOP;
    	   g_usMCNetworkUstate &= ~MCU_SMARTCONFIG_START;
            break;
        default:
          break;
    }
}


//****************************************************************************
//
//!	\brief This function handles events for IP address acquisition via DHCP
//!		   indication
//!
//! \param  pNetAppEvent is the event passed to the Handler
//!
//! \return None
//
//****************************************************************************
void sl_NetAppEvtHdlr(SlNetAppEvent_t *pNetAppEvent)
{

    switch( pNetAppEvent->Event )
	{
	   case SL_NETAPP_IPV4_ACQUIRED:
		  g_usMCNetworkUstate |= MCU_IP_ALLOC;
          g_uiIpAddress = pNetAppEvent->EventData.ipAcquiredV4.ip;
          break;
       case SL_NETAPP_IP_LEASED:
    	   g_usMCNetworkUstate |= MCU_IP_LEASED;
           break;
	   default:
		 break;
	}
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
//! itoa
//!
//!	@brief  Convert integer to ASCII in decimal base
//!
//!     @param  cNum is input integer number to convert
//!     @param  cString is output string
//!
//!     @return number of ASCII parameters
//!
//!
//
//*****************************************************************************
static unsigned short itoa(char cNum, char *cString)
{
	char* ptr;
	char uTemp = cNum;
	unsigned short length;

	// value 0 is a special case
	if (cNum == 0)
	{
		length = 1;
		*cString = '0';

		return length;
	}

	// Find out the length of the number, in decimal base
	length = 0;
	while (uTemp > 0)
	{
		uTemp /= 10;
		length++;
	}

	// Do the actual formatting, right to left
	uTemp = cNum;
	ptr = cString + length;
	while (uTemp > 0)
	{
		--ptr;
		*ptr = pcDigits[uTemp % 10];
		uTemp /= 10;
	}

	return length;
}


//*****************************************************************************
//
//! ReadAccSensor
//!
//!	@brief  Read Accelerometer Data from Sensor
//!
//!
//!     @return none
//!
//!
//
//*****************************************************************************
void ReadAccSensor()
{
    //Define Accelerometer Threshold to Detect Movement
    const short csAccThreshold	= 5;

    signed char cAccXT1,cAccYT1,cAccZT1;
    signed char cAccXT2,cAccYT2,cAccZT2;
    signed short sDelAccX, sDelAccY, sDelAccZ;
    int iRet = -1;
    int iCount = 0;
      
    iRet = BMA222ReadNew(&cAccXT1, &cAccYT1, &cAccZT1);
    if(iRet)
    {
        //In case of error/ No New Data return
        return;
    }
    for(iCount=0;iCount<2;iCount++)
    {
        MAP_UtilsDelay((90*80*1000)); //30msec
        iRet = BMA222ReadNew(&cAccXT2, &cAccYT2, &cAccZT2);
        if(iRet)
        {
            //In case of error/ No New Data continue
            iRet = 0;
            continue;
        }

        else
        {                       
            sDelAccX = abs((signed short)cAccXT2 - (signed short)cAccXT1);
            sDelAccY = abs((signed short)cAccYT2 - (signed short)cAccYT1);
            sDelAccZ = abs((signed short)cAccZT2 - (signed short)cAccZT1);

            //Compare with Pre defined Threshold
            if(sDelAccX > csAccThreshold || sDelAccY > csAccThreshold ||
               sDelAccZ > csAccThreshold)
            {
                //Device Movement Detected, Break and Return
                g_ucDryerRunning = 1;
                break;
            }
            else
            {
                //Device Movement Static
                g_ucDryerRunning = 0;
            }
        }
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
void sl_HttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse)
{
	switch (pSlHttpServerEvent->Event)
	{
		case SL_NETAPP_HTTPGETTOKENVALUE:
		{
			UINT8 *ptr;

			ptr = pSlHttpServerResponse->ResponseData.token_value.data;
			pSlHttpServerResponse->ResponseData.token_value.len = 0;
			if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, GET_token_TEMP, strlen((const char *)GET_token_TEMP)) == 0)
			{
				float fCurrentTemp;
				TMP006DrvGetTemp(&fCurrentTemp);
				char cTemp = (char)fCurrentTemp;
				short sTempLen = itoa(cTemp,(char*)ptr);
				ptr[sTempLen++] = ' ';
				ptr[sTempLen++] = 'F';
				ptr[sTempLen] = '\0';
				pSlHttpServerResponse->ResponseData.token_value.len += sTempLen;

			}

			if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, GET_token_UIC, strlen((const char *)GET_token_UIC)) == 0)
			{
				if(g_iInternetAccess==0)
					strcpy((char*)pSlHttpServerResponse->ResponseData.token_value.data,"1");
				else
					strcpy((char*)pSlHttpServerResponse->ResponseData.token_value.data,"0");
				pSlHttpServerResponse->ResponseData.token_value.len =  2;
			}

			if(memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, GET_token_ACC, strlen((const char *)GET_token_ACC)) == 0)
			{

				ReadAccSensor();
				if(g_ucDryerRunning)
				{
					strcpy((char*)pSlHttpServerResponse->ResponseData.token_value.data,"Running");
					pSlHttpServerResponse->ResponseData.token_value.len += strlen("Running") + 1;
				}
				else
				{
					strcpy((char*)pSlHttpServerResponse->ResponseData.token_value.data,"Stopped");
					pSlHttpServerResponse->ResponseData.token_value.len += strlen("Stopped") + 1;
				}
			}



		}
			break;

		case SL_NETAPP_HTTPPOSTTOKENVALUE:
		{
			UINT8 led;
			UINT8 *ptr = pSlHttpServerEvent->EventData.httpPostData.token_name.data;

			//g_ucLEDStatus = 0;
			if(memcmp(ptr, POST_token, strlen((const char *)POST_token)) == 0)
			{
				ptr = pSlHttpServerEvent->EventData.httpPostData.token_value.data;
				if(memcmp(ptr, "LED", 3) != 0)
					break;
				ptr += 3;
				led = *ptr;
				ptr += 2;
				if(led == '1')
				{
					if(memcmp(ptr, "ON", 2) == 0)
					{
						GPIO_IF_LedOn(MCU_RED_LED_GPIO);
                                                g_ucLEDStatus = LED_ON;

					}
					else if(memcmp(ptr, "Blink", 5) == 0)
					{
						GPIO_IF_LedOn(MCU_RED_LED_GPIO);
						g_ucLEDStatus = LED_BLINK;
					}
					else
					{
						GPIO_IF_LedOff(MCU_RED_LED_GPIO);
                                                g_ucLEDStatus = LED_OFF;
					}
				}
				else if(led == '2')
				{
					if(memcmp(ptr, "ON", 2) == 0)
					{
						GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
					}
					else if(memcmp(ptr, "Blink", 5) == 0)
					{
						GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
						g_ucLEDStatus = 1;
					}
					else
					{
						GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
					}
				}

			}
		}
			break;
		default:
			break;
	}
}

//****************************************************************************
//
//!	\brief Connects to the Network in AP or STA Mode - If ForceAP Jumper is
//!                                             Placed, Force it to AP mode
//!
//! \return	                	None
//
//****************************************************************************
void ConnectToNetwork()
{
	char ucAPSSID[32];
    unsigned short len, config_opt;

    // staring simplelink
    g_uiSimplelinkRole =  sl_Start(NULL,NULL,NULL);

    // Device is in AP Mode and Force AP Jumper is not Connected
    if(((g_uiSimplelinkRole == ROLE_AP) || (g_uiSimplelinkRole == ROLE_P2P)) && g_uiDeviceModeConfig == ROLE_STA )
    {
        //Switch to STA Mode
    	sl_WlanSetMode(ROLE_STA);
        sl_Stop(SL_STOP_TIMEOUT);
        g_uiSimplelinkRole =  sl_Start(NULL,NULL,NULL);
    }

    //Device is in STA Mode and Force AP Jumper is Connected
    if(((g_uiSimplelinkRole == ROLE_STA) || (g_uiSimplelinkRole == ROLE_P2P)) && g_uiDeviceModeConfig == ROLE_AP )
    {
         //Switch to AP Mode
    	sl_WlanSetMode(ROLE_AP);
        sl_Stop(SL_STOP_TIMEOUT);
        g_uiSimplelinkRole =  sl_Start(NULL,NULL,NULL);
    }

    //No Mode Change Required
    if(g_uiSimplelinkRole == ROLE_AP)
    {
       //waiting for the AP to acquire IP address from Internal DHCP Server
       while (!(g_usMCNetworkUstate & MCU_IP_ALLOC))
       {

       }
       char iCount=0;
       //Read the AP SSID
       memset(ucAPSSID,'\0',AP_SSID_LEN_MAX);
       len = AP_SSID_LEN_MAX;
       config_opt = WLAN_AP_OPT_SSID;
       sl_WlanGet(SL_WLAN_CFG_AP_ID, &config_opt , &len, (unsigned char*) ucAPSSID);
       
       //Blink LED 3 times to Indicate AP Mode
       for(iCount=0;iCount<3;iCount++)
       {
           //Turn RED LED On
           GPIO_IF_LedOn(MCU_RED_LED_GPIO);
           osi_Sleep(400);
           
           //Turn RED LED Off
           GPIO_IF_LedOff(MCU_RED_LED_GPIO);
           osi_Sleep(400);
       }

    }
    else
    {
    //waiting for the device to Auto Connect
    while (((!(g_usMCNetworkUstate & MCU_AP_ASSOC)) || !(g_usMCNetworkUstate & MCU_IP_ALLOC))&&
           g_ucConnectTimeout < AUTO_CONNECTION_TIMEOUT_COUNT)
    {
        //Turn RED LED On
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
        osi_Sleep(50);
        
        //Turn RED LED Off
        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
        osi_Sleep(50);
        
        g_ucConnectTimeout++;
    }
    //Couldn't connect Using Auto Profile
    if(g_ucConnectTimeout == AUTO_CONNECTION_TIMEOUT_COUNT)
    {
        //Blink Red LED to Indicate Connection Error
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
        g_ucConnectTimeout &= ~MCU_AP_ASSOC;
        g_ucConnectTimeout &= ~MCU_IP_ALLOC;

        //Connect Using Smart Config
        SmartConfigConnect();

        //Waiting for the device to Auto Connect
        while (!(g_usMCNetworkUstate & MCU_AP_ASSOC) || !(g_usMCNetworkUstate & MCU_IP_ALLOC))
        {
            MAP_UtilsDelay(500);
        }
    }
    //Turn RED LED Off
    GPIO_IF_LedOff(MCU_RED_LED_GPIO);

    g_iInternetAccess = ConnectionTest();

    }
}


//****************************************************************************
//
//!	\brief Read Force AP GPIO and Configure Mode - 1(Access Point Mode)
//!                                                  - 0 (Station Mode)
//!
//! \return	                	None
//
//****************************************************************************
static void ReadDeviceConfiguration()
{
	unsigned int uiGPIOPort;
	unsigned char pucGPIOPin;
	unsigned char ucPinValue;
        
        //Read GPIO
	GPIO_IF_GetPortNPin(SH_GPIO_3,&uiGPIOPort,&pucGPIOPin);
	ucPinValue = GPIO_IF_Get(SH_GPIO_3,uiGPIOPort,pucGPIOPin);
        
        //If Connected to VCC, Mode is AP
	if(ucPinValue == 1)
	{
            //AP Mode
            g_uiDeviceModeConfig = ROLE_AP;
	}
	else
	{
            //STA Mode
            g_uiDeviceModeConfig = ROLE_STA;
	}

}

//****************************************************************************
//
//!	\brief OOB Application Main Task - Initializes SimpleLink Driver and
//!                                              Handles HTTP Requests
//! \param[in]              	pvParameters is the data passed to the Task
//!
//! \return	                	None
//
//****************************************************************************
static void OOBTask(void *pvParameters)
{
    //Read Device Mode Configuration
    ReadDeviceConfiguration();

    //Connect to Network
    ConnectToNetwork();

    //Handle Async Events
    while(1)
    {
          //LED Actions
          if(g_ucLEDStatus == LED_ON)
          {
              GPIO_IF_LedOn(MCU_RED_LED_GPIO);
              osi_Sleep(500);
          }
          if(g_ucLEDStatus == LED_OFF)
          {
              GPIO_IF_LedOff(MCU_RED_LED_GPIO);
              osi_Sleep(500);
          }
    	 if(g_ucLEDStatus==LED_BLINK)
        {
            GPIO_IF_LedOn(MCU_RED_LED_GPIO);
            osi_Sleep(500);
            GPIO_IF_LedOff(MCU_RED_LED_GPIO);
            osi_Sleep(500);
        }
    }
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
#endif  //ccs
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif  //ewarm
    
#endif  //USE_TIRTOS
    
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

//****************************************************************************
//							MAIN FUNCTION
//****************************************************************************
void main()
{
    BoardInit();
    
     //
    // Configure the pinmux settings for the peripherals exercised
    //
    PinMuxConfig();

    PinConfigSet(PIN_58,PIN_STRENGTH_2MA|PIN_STRENGTH_4MA,PIN_TYPE_STD_PD);
    
    //
    // UART Init
    //
    InitTerm();
    
    //
    // LED Init
    //
    GPIO_IF_LedConfigure(LED1);
      
    //Turn Off the LEDs
    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
       
    //
    // I2C Init
    //
    I2C_IF_Open(I2C_MASTER_MODE_FST);

    //Init Temprature Sensor
    TMP006DrvOpen();

    //Init Accelerometer Sensor
    BMA222Open();
    
    //
    // Simplelinkspawntask
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);
    
    //
    // Create OOB Task
    //
    osi_TaskCreate(OOBTask, (signed char*)"OOBTask",OSI_STACK_SIZE, NULL, OOB_TASK_PRIORITY, NULL );

    //
    // Start OS Scheduler
    //
    osi_start();

    while (1)
    {

    }

}
