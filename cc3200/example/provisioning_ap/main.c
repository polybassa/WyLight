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
// Application Name     - Provisioning AP
// Application Overview - CC3200 AP Provisioning is method by which user can
//						  configure the AP information on the CC3200 device from
//						  a Browser. The CC3200 device scans the Access Points,
//						  stores them in the memory and create an HTML page with
//						  the list of APs. User can open the HTML page and select
//						  the AP which he/she wants to configure.
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_Provisioning_AP
// or
// docs\examples\CC32xx_Provisioning_AP.pdf
//
//*****************************************************************************


//****************************************************************************
//
//! \addtogroup ap_provisioning
//! @{
//
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simplelink includes
#include "simplelink.h"


//driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "pin.h"
#include "utils.h"
#include "prcm.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

// common interface file
#ifndef NOTERM
#include "uart_if.h"
#endif

#ifdef NOTERM
#define UART_PRINT(x, ...)
#else
#define UART_PRINT Report
#endif


#define APP_TASK_PRIORITY         1
#define SPAWN_TASK_PRIORITY       9
#define OSI_STACK_SIZE		      2048
#define CONNECTION_TIMEOUT_COUNT  5000  /* 5sec */
#define TOKEN_ARRAY_SIZE          6
#define STRING_TOKEN_SIZE         10
#define AP_SSID_LEN_MAX           33
#define SL_STOP_TIMEOUT           30


// Loop forever, user can change it as per application's requirement
#define LOOP_FOREVER(line_number) \
            {\
                while(1); \
            }

// check the error code and handle it
#define ASSERT_ON_ERROR(line_number, error_code) \
            {\
                if(error_code < 0) return error_code;\
            }



// Application specific status/error codes
typedef enum{
    // Choosing this number to avoid overlap w/ host-driver's error codes
    LAN_CONNECTION_FAILED = -0x7D0,
    CLIENT_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
    DEVICE_NOT_IN_STATION_MODE = CLIENT_CONNECTION_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;



//
// GLOBAL VARIABLES -- Start
//
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif


unsigned char g_ucIpObtained = 0, g_ucProfileAdded = 1, g_ucConnectionStatus=0;
unsigned char g_ucConnectedToConfAP = 0, g_ucProvisioningDone = 0;
unsigned int g_uiConnectTimeoutCnt =0, g_uiIpAddress = 0;
unsigned char g_ucPriority, g_ucIpLeased = 0;
unsigned long g_ulStaIp = 0;
char g_cWlanSSID[AP_SSID_LEN_MAX];
char g_cWlanSecurityKey[50];
SlSecParams_t g_SecParams;
Sl_WlanNetworkEntry_t g_NetEntries[20];
char g_token_get [TOKEN_ARRAY_SIZE][STRING_TOKEN_SIZE] = {"__SL_G_US0",
										"__SL_G_US1", "__SL_G_US2","__SL_G_US3",
												    "__SL_G_US4", "__SL_G_US5"};
//
// GLOBAL VARIABLES -- End
//

//*****************************************************************************
//            Local Functions Prototypes
//*****************************************************************************
static long ConfigureSimpleLinkToDefaultState();
static void WlanConnect();


#ifdef USE_FREERTOS
//*****************************************************************************
// FreeRTOS User Hook Functions enabled in FreeRTOSConfig.h
//*****************************************************************************

//*****************************************************************************
//
//! \brief Application defined hook (or callback) function - assert
//!
//! \param[in]  pcFile - Pointer to the File Name
//! \param[in]  ulLine - Line Number
//!
//! \return none
//!
//*****************************************************************************
void
vAssertCalled( const char *pcFile, unsigned long ulLine )
{
    //Handle Assert here
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined idle task hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void
vApplicationIdleHook( void)
{
    //Handle Idle Hook for Profiling, Power Management etc
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    //Handle Memory Allocation Errors
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook( xTaskHandle *pxTask,
                                   signed portCHAR *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
#endif //USE_FREERTOS


//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- Start
//*****************************************************************************


//*****************************************************************************
//
//! On Successful completion of Wlan Connect, This function triggers Connection
//! status to be set.
//!
//! \param  pSlWlanEvent pointer indicating Event type
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent)
{
    switch(pSlWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        {
            g_ucConnectionStatus = 1;

            //
            // Information about the connected AP (like name, MAC etc) will be
            // available in 'sl_protocol_wlanConnectAsyncResponse_t'-Applications
            // can use it if required
            //
            //  sl_protocol_wlanConnectAsyncResponse_t *pEventData = NULL;
            // pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
            //
            //
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            sl_protocol_wlanConnectAsyncResponse_t*  pEventData = NULL;
	        g_ucConnectionStatus = 0;
	        g_ucIpObtained = 0;

            pEventData = &pSlWlanEvent->EventData.STAandP2PModeDisconnected;

            // If the user has initiated 'Disconnect' request,
            //'reason_code' is SL_USER_INITIATED_DISCONNECTION
            if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                UART_PRINT("Device disconnected from the AP on application's request \n\r");
            }
            else
            {
                UART_PRINT("Device disconnected from the AP on an ERROR..!! \n\r");
            }

        }
        break;

        case SL_WLAN_STA_CONNECTED_EVENT:
        {
            // when device is in AP mode and any client connects to device cc3xxx


            //
            // Information about the connected client (like SSID, MAC etc) will
            // be available in 'slPeerInfoAsyncResponse_t' - Applications
            // can use it if required
            //
            // slPeerInfoAsyncResponse_t *pEventData = NULL;
            // pEventData = &pSlWlanEvent->EventData.APModeStaConnected;
            //

        }
        break;

        case SL_WLAN_STA_DISCONNECTED_EVENT:
        {
            // when client disconnects from device (AP)


            //
            // Information about the connected client (like SSID, MAC etc) will
            // be available in 'slPeerInfoAsyncResponse_t' - Applications
            // can use it if required
            //
            // slPeerInfoAsyncResponse_t *pEventData = NULL;
            // pEventData = &pSlWlanEvent->EventData.APModestaDisconnected;
            //
        }
        break;

        default:
        {
            UART_PRINT("[WLAN EVENT] Unexpected event \n\r");
        }
        break;
    }
}

//*****************************************************************************
//
//! \brief This function handles network events such as IP acquisition, IP
//!           leased, IP released etc.
//!
//! \param[in]  pNetAppEvent - Pointer to NetApp Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    switch(pNetAppEvent->Event)
    {
        case SL_NETAPP_IPV4_ACQUIRED:
        case SL_NETAPP_IPV6_ACQUIRED:
        {
            g_ucIpObtained = 1;

        }
        break;

        case SL_NETAPP_IP_LEASED:
        {
        	g_ucIpLeased = 1;

            g_ulStaIp = (pNetAppEvent)->EventData.ipLeased.ip_address;

            UART_PRINT("[NETAPP EVENT] IP Leased to Client: IP=%d.%d.%d.%d , ",
            			SL_IPV4_BYTE(g_ulStaIp,3), SL_IPV4_BYTE(g_ulStaIp,2),
            			SL_IPV4_BYTE(g_ulStaIp,1), SL_IPV4_BYTE(g_ulStaIp,0));
        }
        break;

        default:
        {
            UART_PRINT("[NETAPP EVENT] Unexpected event [0x%x] \n\r",
                       pNetAppEvent->Event);
        }
        break;
    }
}

//*****************************************************************************
//
//! This function gets triggered when HTTP Server receives Application
//! defined GET and POST HTTP Tokens.
//!
//! \param pSlHttpServerEvent - Pointer indicating http server event
//! \param pSlHttpServerResponse - Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void sl_HttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent,
		SlHttpServerResponse_t *pSlHttpServerResponse)
{

    switch (pSlHttpServerEvent->Event)
    {
        case SL_NETAPP_HTTPGETTOKENVALUE:
        {

            if(0== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data,
                            g_token_get [0],
                            pSlHttpServerEvent->EventData.httpTokenName.len))
            {
                if(g_ucConnectedToConfAP == 1)
                {
                	// Important - Connection Status
                    memcpy(pSlHttpServerResponse->ResponseData.token_value.data,
                    		"TRUE",strlen("TRUE"));
                    pSlHttpServerResponse->ResponseData.token_value.len =
                    											strlen("TRUE");
                }
                else
                {
                	// Important - Connection Status
                    memcpy(pSlHttpServerResponse->ResponseData.token_value.data,
                    		"FALSE",strlen("FALSE"));
                    pSlHttpServerResponse->ResponseData.token_value.len =
																strlen("FALSE");
                }
            }

            if(0== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data,
                            g_token_get [1],
                            pSlHttpServerEvent->EventData.httpTokenName.len))
            {
            	// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                memcpy(pSlHttpServerResponse->ResponseData.token_value.data,
                		g_NetEntries[0].ssid,g_NetEntries[0].ssid_len);
                pSlHttpServerResponse->ResponseData.token_value.len =
													  g_NetEntries[0].ssid_len;
            }
            if(0== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data,
                            g_token_get [2],
                            pSlHttpServerEvent->EventData.httpTokenName.len))
            {
            	// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                memcpy(pSlHttpServerResponse->ResponseData.token_value.data,
                        g_NetEntries[1].ssid,g_NetEntries[1].ssid_len);
                pSlHttpServerResponse->ResponseData.token_value.len =
													   g_NetEntries[1].ssid_len;
            }
            if(0== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data,
                            g_token_get [3],
                            pSlHttpServerEvent->EventData.httpTokenName.len))
            {
            	// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                memcpy(pSlHttpServerResponse->ResponseData.token_value.data,
                        g_NetEntries[2].ssid,g_NetEntries[2].ssid_len);
                pSlHttpServerResponse->ResponseData.token_value.len =
													   g_NetEntries[2].ssid_len;
            }
            if(0== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data,
                            g_token_get [4],
                            pSlHttpServerEvent->EventData.httpTokenName.len))
            {
            	// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                memcpy(pSlHttpServerResponse->ResponseData.token_value.data,
                        g_NetEntries[3].ssid,g_NetEntries[3].ssid_len);
                pSlHttpServerResponse->ResponseData.token_value.len =
                									   g_NetEntries[3].ssid_len;
            }
            if(0== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data,
                            g_token_get [5],
                            pSlHttpServerEvent->EventData.httpTokenName.len))
            {
            	// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
            	memcpy(pSlHttpServerResponse->ResponseData.token_value.data,
                        g_NetEntries[4].ssid,g_NetEntries[4].ssid_len);
                pSlHttpServerResponse->ResponseData.token_value.len =
													  g_NetEntries[4].ssid_len;
            }

            else
                break;

      }
              break;

      case SL_NETAPP_HTTPPOSTTOKENVALUE:
      {

        if((0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                          "__SL_P_USC",
                 pSlHttpServerEvent->EventData.httpPostData.token_name.len)) &&
            (0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                          "Add", pSlHttpServerEvent->EventData.httpPostData.token_value.len)))
        {
            g_ucProfileAdded = 0;

        }
        if(0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                         "__SL_P_USD", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {
            memcpy(g_cWlanSSID,  pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                    pSlHttpServerEvent->EventData.httpPostData.token_value.len);
            g_cWlanSSID[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
        }

        if(0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                         "__SL_P_USE", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {

            if(pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '0')
            {
                g_SecParams.Type =  SL_SEC_TYPE_OPEN;//SL_SEC_TYPE_OPEN

            }
            else if(pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '1')
            {
                g_SecParams.Type =  SL_SEC_TYPE_WEP;//SL_SEC_TYPE_WEP

            }
            else if(pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '2')
            {
                g_SecParams.Type =  SL_SEC_TYPE_WPA;//SL_SEC_TYPE_WPA

            }
            else
            {
                g_SecParams.Type =  SL_SEC_TYPE_OPEN;//SL_SEC_TYPE_OPEN
            }
        }
        if(0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                         "__SL_P_USF", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {
            memcpy(g_cWlanSecurityKey,pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                    pSlHttpServerEvent->EventData.httpPostData.token_value.len);
            g_cWlanSecurityKey[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
            g_SecParams.Key = g_cWlanSecurityKey;
            g_SecParams.KeyLen = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
        }
        if(0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USG",
                         pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {
           g_ucPriority = pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] - 48;
        }
        if(0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                         "__SL_P_US0", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {
            g_ucProvisioningDone = 1;
        }
      }
          break;
      default:
          break;
    }
}




//*****************************************************************************
//
//! \brief This function handles General Events
//!
//! \param[in]     pDevEvent - Pointer to General Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    //
    // Most of the general errors are not FATAL are are to be handled
    // appropriately by the application
    //
    UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
               pDevEvent->EventData.deviceEvent.status,
               pDevEvent->EventData.deviceEvent.sender);
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
// SimpleLink Asynchronous Event Handlers -- End
//*****************************************************************************


//****************************************************************************
//
//!    \brief This function initializes the application variables
//!
//!    \param[in]  None
//!
//!    \return     None
//
//****************************************************************************
static void InitializeAppVariables()
{
	g_ucIpObtained = 0;
	g_ucProfileAdded = 1;
	g_ucConnectionStatus=0;
	g_ucConnectedToConfAP = 0;
	g_ucProvisioningDone = 0;
	g_uiConnectTimeoutCnt =0;
	g_uiIpAddress = 0;
}

//*****************************************************************************
//! \brief This function puts the device in its default state. It:
//!           - Set the mode to STATION
//!           - Configures connection policy to Auto and AutoSmartConfig
//!           - Deletes all the stored profiles
//!           - Enables DHCP
//!           - Disables Scan policy
//!           - Sets Tx power to maximum
//!           - Sets power policy to normal
//!           - TBD - Unregister mDNS services
//!
//! \param   none
//! \return  On success, zero is returned. On error, negative is returned
//*****************************************************************************
static long ConfigureSimpleLinkToDefaultState()
{
    SlVersionFull   ver = {0};

    unsigned char ucVal = 1;
    unsigned char ucConfigOpt = 0;
    unsigned char ucConfigLen = 0;
    unsigned char ucPower = 0;

    long lRetVal = -1;
    long lMode = -1;

    lMode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(__LINE__, lMode);

    // Get the device's version-information
    ucConfigOpt = SL_DEVICE_GENERAL_VERSION;
    ucConfigLen = sizeof(ver);
    lRetVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &ucConfigOpt,
                                &ucConfigLen, (unsigned char *)(&ver));
    ASSERT_ON_ERROR(__LINE__, lRetVal);

    UART_PRINT("Host Driver Version: %s\n\r",SL_DRIVER_VERSION);
    UART_PRINT("Build Version %d.%d.%d.%d.31.%d.%d.%d.%d.%d.%d.%d.%d\n\r",
    ver.NwpVersion[0],ver.NwpVersion[1],ver.NwpVersion[2],ver.NwpVersion[3],
    ver.ChipFwAndPhyVersion.FwVersion[0],ver.ChipFwAndPhyVersion.FwVersion[1],
    ver.ChipFwAndPhyVersion.FwVersion[2],ver.ChipFwAndPhyVersion.FwVersion[3],
    ver.ChipFwAndPhyVersion.PhyVersion[0],ver.ChipFwAndPhyVersion.PhyVersion[1],
    ver.ChipFwAndPhyVersion.PhyVersion[2],ver.ChipFwAndPhyVersion.PhyVersion[3]);

    // Set connection policy to Auto + SmartConfig
    //      (Device's default connection policy)
    lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION,
                                SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
    ASSERT_ON_ERROR(__LINE__, lRetVal);

    // Remove all profiles
    lRetVal = sl_WlanProfileDel(0xFF);
    ASSERT_ON_ERROR(__LINE__, lRetVal);

    // If the device is not in station-mode, try putting it in staion-mode
    if(ROLE_STA != lMode)
    {
        if(ROLE_AP == lMode)
        {
            // If the device is in AP mode, we need to wait for this event
            // before doing anything
            while(g_ucIpObtained == 0)
            {
#ifndef SL_PLATFORM_MULTI_THREADED
              _SlNonOsMainLoopTask();
#endif
            }
        }

        // Switch to STA role and restart
        lRetVal = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(__LINE__, lRetVal);

        lRetVal = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_ON_ERROR(__LINE__, lRetVal);

        InitializeAppVariables();

        lRetVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(__LINE__, lRetVal);

        // Check if the device is in station again
        if(ROLE_STA != lRetVal)
        {
            // We don't want to proceed if the device is not up in STA-mode
            return DEVICE_NOT_IN_STATION_MODE;
        }
    }

    //
    // Device in station-mode. Disconnect previous connection if any
    // The function returns 0 if 'Disconnected done', negative number if already
    // disconnected Wait for 'disconnection' event if 0 is returned, Ignore
    // other return-codes
    //
    lRetVal = sl_WlanDisconnect();
    if(0 == lRetVal)
    {
        // Wait
        while(g_ucConnectionStatus == 0)
        {
#ifndef SL_PLATFORM_MULTI_THREADED
              _SlNonOsMainLoopTask();
#endif
        }
    }

    // Enable DHCP client
    lRetVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&ucVal);
    ASSERT_ON_ERROR(__LINE__, lRetVal);

    // Disable scan
    ucConfigOpt = SL_SCAN_POLICY(0);
    lRetVal = sl_WlanPolicySet(SL_POLICY_SCAN , ucConfigOpt, NULL, 0);
    ASSERT_ON_ERROR(__LINE__, lRetVal);

    // Set Tx power level for station mode
    // Number between 0-15, as dB offset from max power - 0 will set max power
    ucPower = 0;
    lRetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
            WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *)&ucPower);
    ASSERT_ON_ERROR(__LINE__, lRetVal);

    // Set PM policy to normal
    lRetVal = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(__LINE__, lRetVal);

    lRetVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(__LINE__, lRetVal);

    InitializeAppVariables();

    return lRetVal; // Success
}


//*****************************************************************************
//
//! DeInitDriver
//! The function de-initializes a CC3200 device
//!  
//! \param  None
//!  
//! \return none
//
//*****************************************************************************
void
DeInitDriver(void)
{
    //
    // Disconnect from the AP
    //
    sl_WlanDisconnect();
    //
    // Stop the simplelink host
    //
    sl_Stop(SL_STOP_TIMEOUT);
}

void
DeInitDriverAP(void)
{
    //
    // Stop the simplelink host
    //
    sl_Stop(SL_STOP_TIMEOUT);

}
//*****************************************************************************
//
//! initDriver
//! The function initializes a CC3200 device and triggers it to start operation
//!  
//! \param  None
//!  
//! \return none
//
//*****************************************************************************
int
InitDriver(void)
{
    //
    // Start the simplelink host
    //
    return sl_Start(NULL,NULL,NULL);

}

static void WlanConnect()
{
    //Add Profile
    sl_WlanProfileAdd(g_cWlanSSID, strlen((char*)g_cWlanSSID), 0, &g_SecParams,
    							0,g_ucPriority,0);

    //Connecting to the Access point
    sl_WlanConnect(g_cWlanSSID, strlen((char*)g_cWlanSSID), 0, &g_SecParams, 0);

    //waiting for the device to connect to the AP and obtain ip address
    while(g_uiConnectTimeoutCnt<CONNECTION_TIMEOUT_COUNT &&
    		((g_ucConnectionStatus == 0) || (g_ucIpObtained == 0)))
    {
        osi_Sleep(1); //Sleep 1 millisecond
        g_uiConnectTimeoutCnt++;
    }

    g_uiConnectTimeoutCnt = 0;

}


//****************************************************************************
//
//! Task function implements the ProvisioningAP functionality
//!
//! \param none
//! 
//! This function  
//!    1. Starts Device in STA Mode
//!    2. Scans and Stores all the AP
//!    3. Switch to AP Mode and Wait for AP Configuration from Browser
//!    4. Switch to STA Mode and Connect to Configured AP
//!
//! \return None.
//
//****************************************************************************
void ProvisioningAP(void* pTaskParams)
{

    int iDeviceMode = 0;
    char message[100];
    unsigned long intervalInSeconds = 10;

    int Index,i;


    long retVal = -1;

	InitializeAppVariables();

	//
	// Following function configure the device to default state by cleaning
	// the persistent settings stored in NVMEM (viz. connection profiles &
	// policies, power policy etc)
	//
	// Applications may choose to skip this step if the developer is sure
	// that the device is in its default state at start of applicaton
	//
	// Note that all profiles and persistent settings that were done on the
	// device will be lost
	//
	retVal = ConfigureSimpleLinkToDefaultState();
	if(retVal < 0)
	{
	   if (DEVICE_NOT_IN_STATION_MODE == retVal)
		   UART_PRINT("Failed to configure the device in its default state \n\r");

	   LOOP_FOREVER(__LINE__);
	}

	UART_PRINT("Device is configured in default state \n\r");

    //
    // Start the driver
    //
    iDeviceMode = InitDriver();
    
    if(iDeviceMode == ROLE_AP)
    {
        //Device in AP Mode, Wait for Initialization to Complete
        while(g_ucIpObtained == 0)
        {
            MAP_UtilsDelay(100); 
        }
    }
    
    sl_WlanSetMode(ROLE_STA);
    

    if(iDeviceMode == ROLE_AP)
    	DeInitDriverAP();
    else
    	DeInitDriver();
    
    g_ucIpObtained = 0;
    g_ucConnectionStatus = 0;
    
    //MAP_UtilsDelay(10000000); // wait for scan to append msec
    InitDriver();
    
  
    while(!g_ucProvisioningDone)
    {   
      
         //Scan AP in STA mode
        sl_WlanPolicySet(SL_POLICY_SCAN,SL_SCAN_POLICY_EN(1), 
                 (unsigned char *)&intervalInSeconds,sizeof(intervalInSeconds));

        // wait for scan to complete
        MAP_UtilsDelay(20000000); 

        //Get Scan Result
        Index = sl_WlanGetNetworkList(0,20,&g_NetEntries[0]);

        for(i=0; i< Index; i++)
        {
             snprintf(message, 60, "%d) SSID %s\n\r",i,g_NetEntries[i].ssid);
        }
        //Switch to AP Mode
        sl_WlanSetMode(ROLE_AP);

        DeInitDriver();
        g_ucIpObtained = 0;
        g_ucConnectionStatus = 0;


        //Initialize the SLHost Driver
        InitDriver();

        //Wait for Ip Acquired Event in AP Mode
        while(g_ucIpObtained == 0)
        {
            MAP_UtilsDelay(100); 
        }

         //
        // Wait for AP Configuraiton, Open Browser and Configure AP
        //
        while(g_ucProfileAdded && !g_ucProvisioningDone)
        {
            MAP_UtilsDelay(100); 
        }

        g_ucProfileAdded = 1;

        //Switch to STA Mode
        sl_WlanSetMode(ROLE_STA);
          
        //AP Configured, Restart in STA Mode
        DeInitDriverAP();
           
        g_ucIpObtained = 0;
        g_ucConnectionStatus = 0;

        //MAP_UtilsDelay(10000000); 
        InitDriver();

        //
        // Connect to the Configured Access Point
        //
        WlanConnect();

        g_ucConnectedToConfAP = g_ucConnectionStatus;

        sl_WlanDisconnect();    

    }
    
   
    while(1)
    {

    }

}


//****************************************************************************
//
//! Main function
//!
//! \param none
//! 
//! This function  
//!    1. Invokes the SLHost task
//!    2. Invokes the AP Provisioning Task
//!
//! \return None.
//
//****************************************************************************
void main()
{
    //
    // Setup the interrupt vector table
    //
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
    
    //MCU Initialization
    PRCMCC3200MCUInit();

    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);
    
#ifndef NOTERM
    //
    // Configuring UART
    //
    InitTerm();
#endif

     //
    // Simplelinkspawntask
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);
    
    osi_TaskCreate(ProvisioningAP, (signed char*)"ProvisioningAP",
    		OSI_STACK_SIZE, NULL, APP_TASK_PRIORITY, NULL );
    
    osi_start();

    
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
