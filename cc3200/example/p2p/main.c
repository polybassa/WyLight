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
// Application Name     - P2P
// Application Overview - This sample example demostrates how CC3200 connects
//                          to a P2P device. The application starts a TCP server
//                          on port 5001 and waits for P2P device to connect and
//                          send data on it.
//
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_P2P_Application
// or
// docs\examples\CC32xx_P2P_Application.pdf
//
//*****************************************************************************


//****************************************************************************
//
//! \addtogroup p2p
//! @{
//
//****************************************************************************


// simplelink includes
#include "simplelink.h"

//driverlib includes
#include "utils.h"
#include "hw_ints.h"
#include "hw_types.h"
#include "interrupt.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "pinmux.h"

// common interface includes
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "gpio_if.h"


#ifdef NOTERM
#define UART_PRINT(x, ...)
#else
#define UART_PRINT Report
#endif
#define APPLICATION_VERSION     "1.0.0"
#define SUCCESS                 0

#define P2P_REMOTE_DEVICE   "remote-p2p-device"
#define P2P_DEVICE_NAME     "cc3200-p2p-device"
#define SECURITY_TYPE       SL_SEC_TYPE_P2P_PBC
#define SECURITY_KEY        ""
#define P2P_CONFIG_VALUE    "1-0050F204-1"
#define Delay(x)            MAP_UtilsDelay(x * 8000)
#define UNUSED(x)           (x = x)


#define SL_STOP_TIMEOUT     30
#define APP_NAME            "P2P Demo"
#define LISENING_CHANNEL    11
#define REGULATORY_CLASS    81
#define OPERATING_CHANNEL   6
#define TCP_PACKET_COUNT    1000
#define PORT_NUM            5001      /* Port to be used  by TCP server*/
#define BUF_SIZE            1400

#define P2P_ROLE_TYPE_NEGOTIATE

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


#define SET_STATUS_BIT(status_variable, bit) status_variable |= (1<<(bit))
#define CLR_STATUS_BIT(status_variable, bit) status_variable &= ~(1<<(bit))
#define CLR_STATUS_BIT_ALL(status_variable)   (status_variable = 0)
#define GET_STATUS_BIT(status_variable, bit) (0 != (status_variable & (1<<(bit))))

#define IS_NW_PROCSR_ON(status_variable)      GET_STATUS_BIT(status_variable,\
                                                            STATUS_BIT_NWP_INIT)
#define IS_CONNECTED(status_variable)         GET_STATUS_BIT(status_variable,\
                                                          STATUS_BIT_CONNECTION)
#define IS_IP_LEASED(status_variable)         GET_STATUS_BIT(status_variable,\
                                                           STATUS_BIT_IP_LEASED)
#define IS_IP_ACQUIRED(status_variable)       GET_STATUS_BIT(status_variable,\
                                                          STATUS_BIT_IP_AQUIRED)
#define IS_P2P_DEV_FOUND(status_variable)     GET_STATUS_BIT(status_variable,\
                                                       STATUS_BIT_P2P_DEV_FOUND)
#define IS_P2P_REQ_RCVD(status_variable)      GET_STATUS_BIT(status_variable,\
                                                    STATUS_BIT_P2P_REQ_RECEIVED)
#define IS_CONNECT_FAILED(status_variable)    GET_STATUS_BIT(status_variable,\
                                                   STATUS_BIT_CONNECTION_FAILED)
#define IS_PING_DONE(status_variable)         GET_STATUS_BIT(status_variable,\
                                                           STATUS_BIT_PING_DONE)

            
// Status bits - These are used to set/reset the corresponding bits in 
// given variable

typedef enum{
    STATUS_BIT_NWP_INIT = 0, // If this bit is set: Network Processor is 
                             // powered up
                             
    STATUS_BIT_CONNECTION,   // If this bit is set: the device is connected to 
                             // the AP or client is connected to device (AP)
                             
    STATUS_BIT_IP_LEASED,    // If this bit is set: the device has leased IP to 
                             // any connected client

    STATUS_BIT_IP_AQUIRED,   // If this bit is set: the device has acquired an IP
    
    STATUS_BIT_SMARTCONFIG_START, // If this bit is set: the SmartConfiguration 
                                  // process is started from SmartConfig app

    STATUS_BIT_P2P_DEV_FOUND,    // If this bit is set: the device (P2P mode) 
                                 // found any p2p-device in scan

    STATUS_BIT_P2P_REQ_RECEIVED, // If this bit is set: the device (P2P mode) 
                                 // found any p2p-negotiation request

    STATUS_BIT_CONNECTION_FAILED, // If this bit is set: the device(P2P mode)
                                  // connection to client(or reverse way) is failed

    STATUS_BIT_PING_DONE         // If this bit is set: the device has completed
                                 // the ping operation

}e_StatusBits;

/* Application specific status/error codes */
typedef enum{
    // Choosing this number to avoid overlap w/ host-driver's error codes 
    LAN_CONNECTION_FAILED = -0x7D0,  
    INTERNET_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
    DEVICE_NOT_IN_STATION_MODE = INTERNET_CONNECTION_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

union
{
    char BsdBuf[BUF_SIZE];
    UINT32 demobuf[BUF_SIZE/4];
} uBuf;



//
// GLOBAL VARIABLES -- Start
//
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif


unsigned long  g_ulDeviceIp = 0;
unsigned long  g_ulStatus = 0;
unsigned long  g_ulGatewayIP = 0;
const char digits[] = "0123456789";
char g_p2p_dev[MAXIMAL_SSID_LENGTH + 1];
//
// GLOBAL VARIABLES -- End
//


//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- Start
//*****************************************************************************


//*****************************************************************************
//
//! \brief The Function Handles WLAN Events
//!
//! \param[in] pWlanEvent pointer indicating Event type
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
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION_FAILED);

            //
            // Information about the connected AP (like name, MAC etc) will be
            // available in 'sl_protocol_wlanConnectAsyncResponse_t' - Applications
            // can use it if required
            //
            //  sl_protocol_wlanConnectAsyncResponse_t *pEventData = NULL;
            // pEventData = &pSlWlanEvent->EventData.STAandP2PModeWlanConnected;
            //
            //
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            sl_protocol_wlanConnectAsyncResponse_t*  pEventData = NULL;

            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

            pEventData = &pSlWlanEvent->EventData.STAandP2PModeDisconnected;

            // If the user has initiated 'Disconnect' request, 'reason_code'
            // is SL_USER_INITIATED_DISCONNECTION
            if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                UART_PRINT("Device disconnected from the AP on application's"
                           "request \n\r");
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
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION_FAILED);

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
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

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


        case SL_WLAN_P2P_DEV_FOUND_EVENT:
        {
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_P2P_DEV_FOUND);
            //
            // Information about P2P config details (like Peer device name, own
            // SSID etc) will be available in 'slPeerInfoAsyncResponse_t' -
            // Applications can use it if required
            //
            // slPeerInfoAsyncResponse_t *pEventData = NULL;
            // pEventData = &pSlWlanEvent->EventData.P2PModeDevFound;
            //
        }
        break;

        case SL_WLAN_P2P_NEG_REQ_RECEIVED_EVENT:
        {
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_P2P_REQ_RECEIVED);
            memset(g_p2p_dev, '\0', MAXIMAL_SSID_LENGTH + 1);
            memcpy(g_p2p_dev,pSlWlanEvent->EventData.P2PModeNegReqReceived.go_peer_device_name,
                    pSlWlanEvent->EventData.P2PModeNegReqReceived.go_peer_device_name_len);
            
            //
            // Information about P2P Negotiation req details (like Peer device
            // name, own SSID etc) will be available in
            // 'slPeerInfoAsyncResponse_t' - Applications can use it if required
            //
            // slPeerInfoAsyncResponse_t *pEventData = NULL;
            // pEventData = &pSlWlanEvent->EventData.P2PModeNegReqReceived;
            //
        }
        break;

        case SL_WLAN_CONNECTION_FAILED_EVENT:
        {
            // If device gets any connection failed event
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION_FAILED);
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
//! This function gets triggered when HTTP Server receives Application
//! defined GET and POST HTTP Tokens.
//!
//! \param pHttpServerEvent Pointer indicating http server event
//! \param pHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, 
               SlHttpServerResponse_t *pSlHttpServerResponse)
{
    /* Unused in this example */
}

//*****************************************************************************
//
//! This function gets triggered when device acquires IP
//! status to be set. When Device is in DHCP mode recommended to use this.
//!
//! \param pNetAppEvent Pointer indicating device acquired IP
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
            SlIpV4AcquiredAsync_t *pEventData = NULL;

            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

            //
            // Information about the IPv4 & IPv6 details (like IP, gateway,dns
            // etc) will be available in 'SlNetAppEventData_u' - Applications
            // can use it if required
            //
            // For IPv4:
            //
            // SlNetAppEventData_u *pEventData = NULL;
            // pEventData = &pNetAppEvent->EventData.ipAcquiredV4;
            //
            // For IPv6:
            //
            // SlNetAppEventData_u *pEventData = NULL;
            // pEventData = &pNetAppEvent->EventData.ipAcquiredV6;
            //


            //Ip Acquired Event Data
            g_ulDeviceIp = pNetAppEvent->EventData.ipAcquiredV4.ip;

            //Gateway IP address
            g_ulGatewayIP = pEventData->gateway;
			/*
            UART_PRINT("[NETAPP EVENT] IP Acquired: IP=%d.%d.%d.%d , "
                        "Gateway=%d.%d.%d.%d\n\r",
            SL_IPV4_BYTE(g_ulDeviceIp,3),
            SL_IPV4_BYTE(g_ulDeviceIp,2),
            SL_IPV4_BYTE(g_ulDeviceIp,1),
            SL_IPV4_BYTE(g_ulDeviceIp,0),
            SL_IPV4_BYTE(g_ulGatewayIP,3),
            SL_IPV4_BYTE(g_ulGatewayIP,2),
            SL_IPV4_BYTE(g_ulGatewayIP,1),
            SL_IPV4_BYTE(g_ulGatewayIP,0));
            */
        }
        break;

        case SL_NETAPP_IP_LEASED:
        {
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

            //
            // Information about the IP-Leased details (like IP-Leased,
            // lease-time, mac etc) will be available in 'SlIpLeasedAsync_t' -
            // Applications can use it if required
            //
            // SlIpLeasedAsync_t *pEventData = NULL;
            // pEventData = &pNetAppEvent->EventData.ipLeased;
            //

        }
        break;

        default:
        {
            UART_PRINT("[NETAPP EVENT] Unexpected event \n\r");
        }
        break;
    }
}


//*****************************************************************************
//
//!    \brief This function handles socket events indication
//!
//!    \param[in]      pSock is the event passed to the handler
//!
//!    \return         None
//
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    //
    // This application doesn't work w/ socket - Hence not handling these events
    //
    //UART_PRINT(" [SOCK EVENT] Unexpected event \n\r");

}



//****************************************************************************
//
//!    \brief This function initializes the application variables
//!
//!    \param[in]  None
//!
//!    \return     0 on success, negative error-code on error
//
//****************************************************************************
static void InitializeAppVariables()
{
    g_ulStatus = 0;
    g_ulDeviceIp = 0;
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
//!  \brief Connecting to a WLAN Access point
//!
//!   This function connects to the required AP (SSID_NAME) with Security
//!   parameters specified in the form of macros at the top of this file
//!
//!   \param[in]              None
//!
//!   \return                 None
//!
//!   \warning    If the WLAN connection fails or we don't acquire an IP 
//!            address, It will be stuck in this function forever.
//
//****************************************************************************
static long WlanConnect()
{
    SlSecParams_t secParams = {0};
    long retVal = 0;

    secParams.Key = SECURITY_KEY;
    secParams.KeyLen = strlen(SECURITY_KEY);
    secParams.Type = SECURITY_TYPE;

    retVal = sl_WlanConnect(P2P_REMOTE_DEVICE, strlen(P2P_REMOTE_DEVICE), 0,
                            &secParams, 0);
    ASSERT_ON_ERROR(__LINE__, retVal);

    // Wait till Device acquired an IP in P2P mode
    while(! IS_P2P_REQ_RCVD(g_ulStatus))
    { 
        _SlNonOsMainLoopTask();
    }

    // Connect with the device requesting the connection
    retVal = sl_WlanConnect(g_p2p_dev, strlen(g_p2p_dev), 0, &secParams, 0);
    ASSERT_ON_ERROR(__LINE__, retVal);
    
#ifdef P2P_ROLE_TYPE_NEGOTIATE
    while(! IS_IP_ACQUIRED(g_ulStatus))
#else
    while(! IS_IP_LEASED(g_ulStatus))
#endif
    {
        _SlNonOsMainLoopTask();
        if(IS_CONNECT_FAILED(g_ulStatus))
        {
            // Error, connection is failed
            UART_PRINT("Connection Failed\r\n");
            return -1;
        }
    }

    return SUCCESS;
}

//****************************************************************************
//
//! \brief Display the IP Adderess of device
//!
//! \param[in]      none
//!
//! \return         none
//!
//
//****************************************************************************
static void DisplayIP()
{
    unsigned long mask, dns, gateway;
    unsigned char len, dhcpIsOn;

    if(g_ulDeviceIp == 0)
    {
        /* Device is in GO mode, Get the IP of Device */
        len = sizeof(_NetCfgIpV4Args_t);
		dhcpIsOn = 0;
		_NetCfgIpV4Args_t ipV4 = {0};
		sl_NetCfgGet(SL_IPV4_AP_P2P_GO_GET_INFO,&dhcpIsOn,&len,(unsigned char *)&ipV4);
		g_ulDeviceIp=ipV4.ipV4;
		mask=ipV4.ipV4Mask;
		gateway=ipV4.ipV4Gateway;
		dns=ipV4.ipV4DnsServer;
    }

    UNUSED(mask);
    UNUSED(gateway);
    UNUSED(dns);

    UART_PRINT("CC3200 Device IP : IP=%d.%d.%d.%d \n\r", SL_IPV4_BYTE(g_ulDeviceIp,3),
                SL_IPV4_BYTE(g_ulDeviceIp,2), SL_IPV4_BYTE(g_ulDeviceIp,1),
                SL_IPV4_BYTE(g_ulDeviceIp,0));
}

//****************************************************************************
//
//! \brief Opening a server side socket and receiving data
//!
//! This function opens a TCP socket in Listen mode and waits for an incoming
//! TCP connection. If a socket connection is established then the function
//! will try to read 1000 TCP packets from the connected client.
//!
//! \param[in]      port number on which the server will be listening on
//!
//! \return         0 on success, -1 on Error.
//!
//! \note           This function will wait for an incoming connection till one
//!                 is established
//
//****************************************************************************
static int BsdTcpServer(UINT16 Port)
{
    SlSockAddrIn_t  Addr;
    SlSockAddrIn_t  LocalAddr;

    int             idx;
    int             AddrSize;
    int             SockID;
    int             Status;
    int             newSockID;
    long            LoopCount = 0;
    long            nonBlocking = 1;

    for (idx=0 ; idx<BUF_SIZE ; idx++)
    {
        uBuf.BsdBuf[idx] = (char)(idx % 10);
    }

    LocalAddr.sin_family = SL_AF_INET;
    LocalAddr.sin_port = sl_Htons((UINT16)Port);
    LocalAddr.sin_addr.s_addr = 0;

    SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if( SockID < 0 )
    {
        /* error */
        return -1;
    }

    AddrSize = sizeof(SlSockAddrIn_t);
    Status = sl_Bind(SockID, (SlSockAddr_t *)&LocalAddr, AddrSize);
    if( Status < 0 )
    {
        /* error */
        sl_Close(SockID);
        return -1;
    }

    Status = sl_Listen(SockID, 0);
    if( Status < 0 )
    {
        sl_Close(SockID);
        return -1;
    }

    Status = sl_SetSockOpt(SockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING,
                           &nonBlocking, sizeof(nonBlocking));
    if( Status < 0 )
    {
        return -1;
    }
    newSockID = SL_EAGAIN;

    while( newSockID < 0 )
    {
        newSockID = sl_Accept(SockID, ( struct SlSockAddr_t *)&Addr,
                              (SlSocklen_t*)&AddrSize);
        if( newSockID == SL_EAGAIN )
        {
            /* Wait for 1 ms */
            Delay(1);
        }
        else if( newSockID < 0 )
        {
            sl_Close(SockID);
            return -1;
        }
    }

    while (LoopCount < TCP_PACKET_COUNT)
    {
        Status = sl_Recv(newSockID, uBuf.BsdBuf, BUF_SIZE, 0);
        if( Status <= 0 )
        {
            /* error */
            sl_Close(newSockID);
            sl_Close(SockID);
            return -1;
        }
        LoopCount++;
    }
    GPIO_IF_LedOn(MCU_EXECUTE_SUCCESS_IND);
    sl_Close(newSockID);
    sl_Close(SockID);

    return 0;
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

//*****************************************************************************
//
//! Check the device mode and switch to P2P mode
//! restart the NWP to activate P2P mode
//!
//! \param  None
//!
//! \return status code - Success:0, Failure:-ve
//
//*****************************************************************************
long StartDeviceInP2P()
{
    long retVal = -1;
    // Reset CC3200 Network State Machine
    InitializeAppVariables();

    //
    // Following function configure the device to default state by cleaning
    // the persistent settings stored in NVMEM (viz. connection profiles &
    // policies, power policy etc)
    //
    // Applications may choose to skip this step if the developer is sure
    // that the device is in its default state at start of application
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
    // Assumption is that the device is configured in station mode already
    // and it is in its default state
    //

    retVal = sl_Start(NULL,NULL,NULL);

    if (retVal < 0)
    {
        UART_PRINT("Failed to start the device \n\r");
        return -1;
    }
    else if(retVal != ROLE_P2P)
    {
        sl_WlanSetMode(ROLE_P2P);
        sl_Stop(10);

        // reset the Status bits
        CLR_STATUS_BIT_ALL(g_ulStatus);

        retVal = sl_Start(NULL,NULL,NULL);
        if(retVal < 0 || retVal != ROLE_P2P)
        {
            UART_PRINT("Failed to start the device \n\r");
            return -1;
        }
        else
        {
            UART_PRINT("Started SimpleLink Device: P2P Mode\n\r");
            return SUCCESS;
        }
    }
    else
    {
        return SUCCESS;
    }
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
    UART_PRINT("\t\t\t    CC3200 %s Application       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}

int main()
{

    unsigned char ucP2PParam[4];
    long lRetVal = -1;

    //
    // Initialize Board configurations
    //
    BoardInit();
    
    //
    // Pinmuxing for GPIO, UART
    //
    PinMuxConfig();
    
    //
    // configure LEDs
    //
    GPIO_IF_LedConfigure(LED1|LED2|LED3);
    
    // off all LEDs
    GPIO_IF_LedOff(MCU_ALL_LED_IND);
    
#ifndef NOTERM
    //
    // Configuring UART
    //
    InitTerm();
#endif

    
    //
    // Display the Application Banner
    //
    DisplayBanner(APP_NAME);

    UART_PRINT("Scan Wi-FI direct device in your handheld device\n\r");

    // Initializing the CC3200 device
    lRetVal = StartDeviceInP2P();
    if(lRetVal < 0)
    {
    	LOOP_FOREVER(__LINE__);
    }

    // Set any p2p option (SL_CONNECTION_POLICY(0,0,0,any_p2p,0)) to connect to
    // first available p2p device
    sl_WlanPolicySet(SL_POLICY_CONNECTION,SL_CONNECTION_POLICY(1,0,0,0,0),NULL,0);

    // Set the negotiation role (SL_P2P_ROLE_NEGOTIATE).
    // CC3200 will negotiate with remote device GO/client mode.
    // Other valid options are:
    //             - SL_P2P_ROLE_GROUP_OWNER
    //             - SL_P2P_ROLE_CLIENT
    sl_WlanPolicySet(SL_POLICY_P2P, SL_P2P_POLICY(SL_P2P_ROLE_NEGOTIATE,
            SL_P2P_NEG_INITIATOR_ACTIVE),NULL,0);


    // Set P2P Device name
    sl_NetAppSet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DEVICE_URN,
            strlen(P2P_DEVICE_NAME), (unsigned char *)P2P_DEVICE_NAME);

    // Set P2P device type
    sl_WlanSet(SL_WLAN_CFG_P2P_PARAM_ID, WLAN_P2P_OPT_DEV_TYPE,
            strlen(P2P_CONFIG_VALUE), (unsigned char*)P2P_CONFIG_VALUE);

    // setting P2P channel parameters
    ucP2PParam[0] = LISENING_CHANNEL;
    ucP2PParam[1] = REGULATORY_CLASS;
    ucP2PParam[2] = OPERATING_CHANNEL;
    ucP2PParam[3] = REGULATORY_CLASS;

    // Set P2P Device listen and open channel valid channels are 1/6/11
    sl_WlanSet(SL_WLAN_CFG_P2P_PARAM_ID, WLAN_P2P_OPT_CHANNEL_N_REGS,
                sizeof(ucP2PParam), ucP2PParam);

    // Restart as P2P device
    sl_Stop(SL_STOP_TIMEOUT);
    lRetVal = sl_Start(NULL,NULL,NULL);

    if(lRetVal < 0 || lRetVal != ROLE_P2P)
    {
        UART_PRINT("Failed to start the device \n\r");
        LOOP_FOREVER(__LINE__);
    }
    else
    {
        UART_PRINT("Connect to %s \n\r",P2P_DEVICE_NAME);
    }
    
    /* Connect to configure P2P device */
    lRetVal = WlanConnect();

    if(lRetVal == 0)
    {
        GPIO_IF_LedOn(MCU_IP_ALLOC_IND);
    }
    else
    {
        UART_PRINT("Reset the device and try again\n\r");
        LOOP_FOREVER(__LINE__);
    }

    DisplayIP();
    UART_PRINT("Send TCP packets from your handheld device to CC3200's IP\n\r");
    
    /*After calling this function, you can start sending data to CC3200 IP
    * address on PORT_NUM */
    if(!(IS_CONNECT_FAILED(g_ulStatus)))
        BsdTcpServer(PORT_NUM);

    UART_PRINT("Received TCP packets successfully \n\r");
    
    // revert Device into STA mode and power off Network Processor
    sl_WlanSetMode(ROLE_STA);
    sl_Stop(SL_STOP_TIMEOUT);
    UART_PRINT("Test passed, exiting application... \n\r");
    while(1)
    {
        _SlNonOsMainLoopTask();
    }
}
