//*****************************************************************************
// Copyright (C) 2014 Texas Instruments Incorporated
//
// All rights reserved. Property of Texas Instruments Incorporated.
// Restricted rights to use, duplicate or disclose this code are
// granted through contract.
// The program may not be used without the written permission of
// Texas Instruments Incorporated or against the terms and conditions
// stipulated in the agreement under which this program has been supplied,
// and under no circumstances can it be used with non-TI connectivity device.
//
//*****************************************************************************


//*****************************************************************************
//
// Application Name     - XMPP Client
// Application Overview - This example code demonstrate how to connect and work
//						  with Extensible Messaging and Presence Protocol(XMPP)
//						  server using CC32xx. This code doesn't implement a
//						  full XMPP SW stack\library.
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_XMPP_Reference_Application
// or
// docs\examples\CC32xx_XMPP_Reference_Application.pdf
//
//*****************************************************************************


// Simplelink includes
#include "simplelink.h"

// driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "interrupt.h"

// free-rtos/ ti_rtos includes
#include "osi.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

// common interface includes
#include "gpio_if.h"
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "xmpp.h"
#include "pinmux.h"


#ifdef NOTERM
#define UART_PRINT(x, ...)
#else
#define UART_PRINT Report
#endif

#define APPLICATION_NAME        "WLAN STATION"
#define APPLICATION_VERSION     "1.0.0"
#define SUCCESS                 0
//
// Values for below macros shall be modified as per access-point(AP) properties
// SimpleLink device will connect to following AP when application is executed
//
#define SSID_NAME           "cc3200demo"    /* AP SSID */
#define SECURITY_TYPE       SL_SEC_TYPE_OPEN/* Security type (OPEN or WEP or WPA) */
#define SECURITY_KEY        ""              /* Password of the secured AP */
#define SSID_LEN_MAX        32
#define BSSID_LEN_MAX       6

#define SL_STOP_TIMEOUT     30
// IP addressed of XMPP server.
// Should be in long format, E.g: 0xadc2467d == 173.194.70.125
#define XMPP_IP_ADDR    0xadc2467d
#define XMPP_DST_PORT		5223
#define BUF_SIZE        	1400
#define SPAWN_TASK_PRIORITY 9
#define REMOTE_USERID_LEN   50
#define RECV_MSG_LEN        50
#define OSI_STACK_SIZE		3000
//
// Values for below macros shall be modified as per User's email-ID & domain name
// user-name password given here might be change in future
//
#define CLIENT_USER_NAME    "cc32xx.apps"
#define CLIENT_PASSWORD     "cc3200apps"
#define XMPP_DOMAIN_NAME    "gmail.com"
#define XMPP_RESOURCE       "work"



// Loop forever, user can change it as per application's requirement
#define LOOP_FOREVER(line_number) \
            {\
                while(1); \
            }

// check the error code and handle it
#define ASSERT_ON_ERROR(line_number, error_code) \
            {\
                if (error_code < 0) return error_code;\
            }

//Status bits - These are used to set/reset the corresponding bits in
// given variable
typedef enum{
    STATUS_BIT_CONNECTION =  0, // If this bit is Set SimpleLink device is
                                // connected to the AP

    STATUS_BIT_IP_AQUIRED       // If this bit is Set SimpleLink device has
                                 // acquired IP

}e_StatusBits;

// Application specific status/error codes
typedef enum{
    // Choosing -0x7D0 to avoid overlap w/ host-driver's error codes
    LAN_CONNECTION_FAILED = -0x7D0,
    INTERNET_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
    DEVICE_NOT_IN_STATION_MODE = INTERNET_CONNECTION_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

#define SET_STATUS_BIT(status_variable, bit)  status_variable |= (1<<(bit))
#define CLR_STATUS_BIT(status_variable, bit)  status_variable &= ~(1<<(bit))
#define CLR_STATUS_BIT_ALL(status_variable)   (status_variable = 0)
#define GET_STATUS_BIT(status_variable, bit)  (0 != (status_variable & \
                                                                (1<<(bit))))

#define IS_CONNECTED(status_variable)         GET_STATUS_BIT(status_variable, \
                                                       STATUS_BIT_CONNECTION)
#define IS_IP_ACQUIRED(status_variable)       GET_STATUS_BIT(status_variable, \
                                                       STATUS_BIT_IP_AQUIRED)


//
// GLOBAL VARIABLES -- Start
//
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

unsigned long  g_ulStatus = 0;//SimpleLink Status
unsigned long  g_ulGatewayIP = 0; //Network Gateway IP address
unsigned char  g_ucConnectionSSID[SSID_LEN_MAX+1]; //Connection SSID
unsigned char  g_ucConnectionBSSID[BSSID_LEN_MAX]; //Connection BSSID
unsigned int g_IpObtained = 0;
//
// GLOBAL VARIABLES -- End
//



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
//! \brief The Function Handles WLAN Events
//!
//! \param[in]  pWlanEvent - Pointer to WLAN Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    switch(pWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        {
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);

            //
            // Information about the connected AP (like name, MAC etc) will be
            // available in 'sl_protocol_wlanConnectAsyncResponse_t'-Applications
            // can use it if required
            //
            //  sl_protocol_wlanConnectAsyncResponse_t *pEventData = NULL;
            // pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
            //

            // Copy new connection SSID and BSSID to global parameters
            memcpy(g_ucConnectionSSID,pWlanEvent->EventData.
                   STAandP2PModeWlanConnected.ssid_name,
                   pWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
            memcpy(g_ucConnectionBSSID,
                   pWlanEvent->EventData.STAandP2PModeWlanConnected.bssid,
                   SL_BSSID_LENGTH);

            UART_PRINT("[WLAN EVENT] STA Connected to the AP: %s ,\
                BSSID: %x:%x:%x:%x:%x:%x\n\r",
                      g_ucConnectionSSID,g_ucConnectionBSSID[0],
                      g_ucConnectionBSSID[1],g_ucConnectionBSSID[2],
                      g_ucConnectionBSSID[3],g_ucConnectionBSSID[4],
                      g_ucConnectionBSSID[5]);
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            sl_protocol_wlanConnectAsyncResponse_t*  pEventData = NULL;

            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

            pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

            // If the user has initiated 'Disconnect' request,
            //'reason_code' is SL_USER_INITIATED_DISCONNECTION
            if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                UART_PRINT("[WLAN EVENT]Device disconnected from the AP: %s,"
                "BSSID: %x:%x:%x:%x:%x:%x on application's request \n\r",
                           g_ucConnectionSSID,g_ucConnectionBSSID[0],
                           g_ucConnectionBSSID[1],g_ucConnectionBSSID[2],
                           g_ucConnectionBSSID[3],g_ucConnectionBSSID[4],
                           g_ucConnectionBSSID[5]);
            }
            else
            {
                UART_PRINT("[WLAN ERROR]Device disconnected from the AP AP: %s,"
                "BSSID: %x:%x:%x:%x:%x:%x on an ERROR..!! \n\r",
                           g_ucConnectionSSID,g_ucConnectionBSSID[0],
                           g_ucConnectionBSSID[1],g_ucConnectionBSSID[2],
                           g_ucConnectionBSSID[3],g_ucConnectionBSSID[4],
                           g_ucConnectionBSSID[5]);
            }
            memset(g_ucConnectionSSID,0,sizeof(g_ucConnectionSSID));
            memset(g_ucConnectionBSSID,0,sizeof(g_ucConnectionBSSID));
        }
        break;

        default:
        {
            UART_PRINT("[WLAN EVENT] Unexpected event [0x%x]\n\r",
                       pWlanEvent->Event);
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
        {
            SlIpV4AcquiredAsync_t *pEventData = NULL;

            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

            //Ip Acquired Event Data
            pEventData = &pNetAppEvent->EventData.ipAcquiredV4;

            //Gateway IP address
            g_ulGatewayIP = pEventData->gateway;

            UART_PRINT("[NETAPP EVENT] IP Acquired: IP=%d.%d.%d.%d , "
            "Gateway=%d.%d.%d.%d\n\r",
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,3),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,2),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,1),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,0),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,3),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,2),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,1),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,0));
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
//! \brief This function handles HTTP server events
//!
//! \param[in]  pServerEvent - Contains the relevant event information
//! \param[in]    pServerResponse - Should be filled by the user with the
//!                                      relevant response information
//!
//! \return None
//!
//****************************************************************************
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse)
{
    // Unused in this application
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
       switch( pSock->Event )
    {
        case SL_NETAPP_SOCKET_TX_FAILED:
            switch( pSock->EventData.status )
            {
                case SL_ECLOSE:
                    UART_PRINT("[SOCK ERROR] - close socket (%d) operation "
                    "failed to transmit all queued packets\n\n",
                           pSock->EventData.sd);
                    break;
                default:
                    UART_PRINT("[SOCK ERROR] - TX FAILED : socket %d , reason"
                        "(%d) \n\n",
                           pSock->EventData.sd, pSock->EventData.status);
            }
            break;

        default:
            UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n",pSock->Event);
    }
}


//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- End
//*****************************************************************************


//*****************************************************************************
//
//! \brief This function initializes the application variables
//!
//! \param    None
//!
//! \return None
//!
//*****************************************************************************
static void InitializeAppVariables()
{
    g_ulStatus = 0;
    g_ulGatewayIP = 0;
    memset(g_ucConnectionSSID,0,sizeof(g_ucConnectionSSID));
    memset(g_ucConnectionBSSID,0,sizeof(g_ucConnectionBSSID));
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
    if (ROLE_STA != lMode)
    {
        if (ROLE_AP == lMode)
        {
            // If the device is in AP mode, we need to wait for this event
            // before doing anything
            while(!IS_IP_ACQUIRED(g_ulStatus))
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

        // reset status bits
        CLR_STATUS_BIT_ALL(g_ulStatus);

        lRetVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(__LINE__, lRetVal);

        // Check if the device is in station again
        if (ROLE_STA != lRetVal)
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
        while(IS_CONNECTED(g_ulStatus))
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


//****************************************************************************
//
//! \brief Connecting to a WLAN Accesspoint
//!
//!  This function connects to the required AP (SSID_NAME) with Security
//!  parameters specified in te form of macros at the top of this file
//!
//! \param  None
//!
//! \return  None
//!
//! \warning    If the WLAN connection fails or we don't aquire an IP
//!            address, It will be stuck in this function forever.
//
//****************************************************************************
void WlanConnect()
{
   SlSecParams_t secParams;

   secParams.Key = "";
   secParams.KeyLen = 0;
   secParams.Type = SL_SEC_TYPE_OPEN;

   sl_WlanConnect(SSID_NAME,strlen(SSID_NAME),0,&secParams,0);

   while((!IS_CONNECTED(g_ulStatus)) || (!IS_IP_ACQUIRED(g_ulStatus)))
   {
	   // Toggle LEDs to Indicate Connection Progress
       GPIO_IF_LedOff(MCU_IP_ALLOC_IND);
       MAP_UtilsDelay(800000);
       GPIO_IF_LedOn(MCU_IP_ALLOC_IND);
       MAP_UtilsDelay(800000);

   }
   //
   // Red LED on to indicate AP connection
   //
   GPIO_IF_LedOn(MCU_IP_ALLOC_IND);
}

//*****************************************************************************
//
//! Configure the device as xmpp client
//!
//! \param  pvParameters pointer to parameters
//!
//! \return None
//
//*****************************************************************************
static void XmppClient(void *pvParameters)
{
  SlNetAppXmppOpt_t XmppOption;
  SlNetAppXmppUserName_t UserName;
  SlNetAppXmppPassword_t Password;
  SlNetAppXmppDomain_t Domain;
  SlNetAppXmppResource_t Resource;
  unsigned char pRemoteJid[REMOTE_USERID_LEN];
  unsigned char pRecvMessage[RECV_MSG_LEN];
  signed short  Status = 0;

  long lRetVal = -1;
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
  lRetVal = ConfigureSimpleLinkToDefaultState();
  if(lRetVal < 0)
  {
	  if (DEVICE_NOT_IN_STATION_MODE == lRetVal)
	  {
		  UART_PRINT("Failed to configure the device in its default state\n\r");
	  }

	  LOOP_FOREVER(__LINE__);
  }

  UART_PRINT("Device is configured in default state \n\r");

  //
  // Assumption is that the device is configured in station mode already
  // and it is in its default state
  //

  lRetVal = sl_Start(0, 0, 0);

  if (lRetVal < 0 || ROLE_STA != lRetVal)
  {
      UART_PRINT("Failed to start the device \n\r");
      LOOP_FOREVER(__LINE__);
  }

  UART_PRINT("Device started as STATION \n\r");

  // Connecting to WLAN AP - Set with static parameters defined at the top
  // After this call we will be connected and have IP address
  WlanConnect();


  // Configuring different parameters which are required for XMPP connection
  XmppOption.Port = XMPP_DST_PORT;
  XmppOption.Family = SL_AF_INET;
  XmppOption.SecurityMethod = SO_SECMETHOD_SSLV3;
  XmppOption.SecurityCypher = SECURE_MASK_SSL_RSA_WITH_RC4_128_SHA;
  XmppOption.Ip = XMPP_IP_ADDR;

  sl_NetAppXmppSet(SL_NET_APP_XMPP_ID, NETAPP_XMPP_ADVANCED_OPT, 
                   sizeof(SlNetAppXmppOpt_t), (unsigned char *)&XmppOption);

  // Configure Client's user name
  memcpy(UserName.UserName, CLIENT_USER_NAME, strlen(CLIENT_USER_NAME));
  UserName.Length = strlen(CLIENT_USER_NAME);

  sl_NetAppXmppSet(SL_NET_APP_XMPP_ID, NETAPP_XMPP_USER_NAME, UserName.Length, 
                   (unsigned char *)&UserName);
  // Configure Client's password
  memcpy(Password.Password, CLIENT_PASSWORD, strlen(CLIENT_PASSWORD));
  Password.Length = strlen(CLIENT_PASSWORD);

  sl_NetAppXmppSet(SL_NET_APP_XMPP_ID, NETAPP_XMPP_PASSWORD, Password.Length, 
                   (unsigned char *)&Password);

  memcpy(Domain.DomainName, XMPP_DOMAIN_NAME, strlen(XMPP_DOMAIN_NAME));
  Domain.Length = strlen(XMPP_DOMAIN_NAME);

  sl_NetAppXmppSet(SL_NET_APP_XMPP_ID, NETAPP_XMPP_DOMAIN, Domain.Length, 
                   (unsigned char *)&Domain);

  memcpy(Resource.Resource, XMPP_RESOURCE,strlen(XMPP_RESOURCE));
  Resource.Length = strlen(XMPP_RESOURCE);

  sl_NetAppXmppSet(SL_NET_APP_XMPP_ID, NETAPP_XMPP_RESOURCE, Resource.Length, 
                   (unsigned char *)&Resource);


  while(sl_NetAppXmppConnect() < 0)
  {
    MAP_UtilsDelay(10000);
  }

  Status = sl_NetAppXmppRecv(pRemoteJid, REMOTE_USERID_LEN, pRecvMessage, RECV_MSG_LEN );
  while(1)
  {
    while ( Status < 0)
    {
      MAP_UtilsDelay(10000);
      Status = sl_NetAppXmppRecv(pRemoteJid, REMOTE_USERID_LEN, pRecvMessage, RECV_MSG_LEN );
    }

    //
    // Toggle Orange LED to indicate if it gets some chat message
    //
    GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
    MAP_UtilsDelay(800000);
    GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
    Status = sl_NetAppXmppSend(pRemoteJid, strlen((char*)pRemoteJid),pRecvMessage,
                               strlen((char*)pRecvMessage) );  
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
  //
  // Set vector table base
  //
#if defined(ewarm)
  MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif

  //
  // Enable Processor
  //
  MAP_IntMasterEnable();
  MAP_IntEnable(FAULT_SYSTICK);

  PRCMCC3200MCUInit();
}

int main(void)
{
  //
  // initialize board configurations
  //
  BoardInit();

  //
  // Pinmux GPIO for LEDs
  //
  PinMuxConfig();

#ifndef NOTERM
    //
    // Configuring UART
    //
    InitTerm();
#endif


  //
  // Configure LEDs
  //
  GPIO_IF_LedConfigure(LED1|LED2|LED3);

  GPIO_IF_LedOff(MCU_ALL_LED_IND);

  //
  // Simplelinkspawntask
  //
  VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);
  
  osi_TaskCreate(XmppClient, (const signed char*)"XmppClient", OSI_STACK_SIZE, NULL, 1, NULL );
  
  osi_start();

  while(1)
  {

  }

}
