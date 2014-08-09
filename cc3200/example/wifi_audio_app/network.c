//*****************************************************************************
// network.c
//
// Network Interface
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

#include "simplelink.h"
#include "protocol.h"
#include "network.h"
#include "hw_types.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "gpio_if.h"
//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************

#define ROLE_INVALID                    (-5)
#define AP_SSID_LEN_MAX                 (33)
#define LED_OFF                         (0)
#define LED_ON                          (1)
#define SH_GPIO_9                       (9)            /* Red */
#define SH_GPIO_11                      (11)           /* Green */
#define SH_GPIO_25                      (25)           /* Yellow */
#define AUTO_CONNECTION_TIMEOUT_COUNT   (50)           /* 5 Sec */

//*****************************************************************************
//                      GLOBAL VARIABLES
//*****************************************************************************
int g_uiSimplelinkRole = ROLE_INVALID;
extern tUDPSocket g_UdpSock;
unsigned int g_uiIpObtained = 0;
unsigned int g_uiConnectionStatus = 0;
unsigned int g_uiIpAddress = 0;
unsigned int g_smartConfigDone = 0;
unsigned int g_smartConfigStoped = 0;
unsigned int g_ConnectTimeout =0;
unsigned int g_uiDeviceModeConfig = ROLE_STA; //default is STA mode

//*****************************************************************************
//
//! mDNS_Task function
//!
//! \param  None
//!
//! \return None
//!
//*****************************************************************************
void mDNS_Task()
{
    int iretvalmDNS;
    unsigned int pAddr;
    unsigned long usPort;
    unsigned short	 ulTextLen = 200;
    char cText[201]; 

 
    //UnRegister mDNS Service if done Previously
    iretvalmDNS = sl_NetAppMDNSUnRegisterService("CC3200._audio._udp.local",
                              (unsigned char)strlen("CC3200._audio._udp.local"));

    while(1)
    {  
        iretvalmDNS = 1;
        
        //Read mDNS service.
        while(iretvalmDNS)
        {
            ulTextLen = 200;
            iretvalmDNS = sl_NetAppDnsGetHostByService("CC3200._audio._udp.local",
                                 (unsigned char)strlen("CC3200._audio._udp.local"),
                                  SL_AF_INET,(unsigned long *)&pAddr,&usPort,
                                  &ulTextLen,&cText[0]);
        }
        if(iretvalmDNS == 0 && (pAddr!=INVALID_CLIENT_ADDRESS) && (pAddr!=g_uiIpAddress))
        {               
             //Speaker Detected - Add Client
             g_UdpSock.Client.sin_family = AF_INET;
             g_UdpSock.Client.sin_addr.s_addr = htonl(pAddr);
             g_UdpSock.Client.sin_port = htons(usPort);
             g_UdpSock.iClientLength = sizeof(g_UdpSock.Client);
        }
         
             MAP_UtilsDelay(80*1000*100);
	}	
}

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
void sl_WlanEvtHdlr(SlWlanEvent_t *pSlWlanEvent)
{
    switch(pSlWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
           g_uiConnectionStatus = 1;           
           break;
        case SL_WLAN_DISCONNECT_EVENT:
           g_uiConnectionStatus = 0;
           break;
        case SL_WLAN_SMART_CONFIG_START_EVENT:
        	/* SmartConfig operation finished */
        	/*The new SSID that was acquired is: pWlanEventHandler->EventData.smartConfigStartResponse.ssid */
        	/* We have the possiblity that also a private token was sent to the Host:
        	 *  if (pWlanEventHandler->EventData.smartConfigStartResponse.private_token_len)
        	 *    then the private token is populated: pWlanEventHandler->EventData.smartConfigStartResponse.private_token
        	 */
            g_smartConfigDone = 1;
            g_smartConfigStoped = 1;
            break;
       case SL_WLAN_SMART_CONFIG_STOP_EVENT:
    	   /* SmartConfig stop operation was completed */
             g_smartConfigDone = 0;
             g_smartConfigStoped = 1;
            break;
        default:
            break;
    }
}

//*****************************************************************************
//
//! This function gets triggered when device acquires IP
//! status to be set. When Device is in DHCP mode recommended to use this.
//!
//! \param pNetAppEvent Pointer indicating device aquired IP 
//!
//! \return None
//!
//*****************************************************************************
void sl_NetAppEvtHdlr(SlNetAppEvent_t *pNetAppEvent)
{
    switch(pNetAppEvent->Event)     
    {
        case SL_NETAPP_IPV4_ACQUIRED:
          g_uiIpObtained = 1;
          g_uiIpAddress = pNetAppEvent->EventData.ipAcquiredV4.ip;
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
//! \param pHttpServerEvent Pointer indicating http server event
//! \param pHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void sl_HttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse)
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

#ifdef MULTICAST
//*****************************************************************************
//
//! Add to Multicast Group to receive Audio Stream 
//!
//! \param none
//!
//! \return None
//
//*****************************************************************************
void ReceiveMulticastPacket()
{
    SlSockIpMreq mreq;
    memset(&mreq,0,sizeof(SlSockIpMreq));

    // set interface
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    mreq.imr_multiaddr.s_addr = ADDR_MULTICAST_GROUP;

    // do membership call
    if (setsockopt(g_UdpSock.iSockDesc, IPPROTO_IP, IP_ADD_MEMBERSHIP,
     &mreq, sizeof(SlSockIpMreq)) == -1) {
            return;
       }	
}
#endif
//*****************************************************************************
//
//! Connect the Device to Network
//!
//! \param  None
//!
//! \return  None
//!
//*****************************************************************************

void ConnectToNetwork()
{
    //Start Simplelink Device 
    g_uiSimplelinkRole =  sl_Start(NULL,NULL,NULL);

    // Device is in AP Mode and Force AP Jumper is not Connected
    if(g_uiSimplelinkRole == ROLE_AP && g_uiDeviceModeConfig == ROLE_STA )
    {
        //Switch to STA Mode
        sl_WlanSetMode(ROLE_STA);
        sl_Stop(10);
        g_uiSimplelinkRole =  sl_Start(NULL,NULL,NULL);
    }
     
    //Device is in STA Mode and Force AP Jumper is Connected
    if(g_uiSimplelinkRole == ROLE_STA && g_uiDeviceModeConfig == ROLE_AP )
    {
        //Switch to AP Mode
        sl_WlanSetMode(ROLE_AP);
        sl_Stop(10);
        g_uiSimplelinkRole =  sl_Start(NULL,NULL,NULL);
    }
     
     //No Mode Change Required
    if(g_uiSimplelinkRole == ROLE_AP)
    {
        //waiting for the AP to acquire IP address from Internal DHCP Server
        while (g_uiIpObtained == 0)
    	{
            MAP_UtilsDelay(500);
    	}
         //Read the AP SSID
        unsigned char ucAPSSID[AP_SSID_LEN_MAX];
        unsigned short len = 32;
	unsigned short  config_opt = WLAN_AP_OPT_SSID;
        memset(ucAPSSID,'\0',AP_SSID_LEN_MAX);
	sl_WlanGet(SL_WLAN_CFG_AP_ID, &config_opt , &len, (unsigned char*) ucAPSSID);
        
    }
    else
    {   
        //waiting for the device to Auto Connect
        while (((g_uiConnectionStatus == 0) || (g_uiIpObtained == 0))&&
               g_ConnectTimeout < AUTO_CONNECTION_TIMEOUT_COUNT)
        {
            //Turn Green LED On       
            GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);            
            osi_Sleep(50);            
            //Turn Green LED Off
            GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);            
            osi_Sleep(50);
            
            g_ConnectTimeout++;
        }
        //Couldn't connect Using Auto Profile
        if(g_ConnectTimeout==AUTO_CONNECTION_TIMEOUT_COUNT)
        {
            g_uiConnectionStatus = 0;
            g_uiIpObtained = 0;  
            
            //Turn Green LED On       
            GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);  
            
            //Connect Using Smart Config
            SmartConfigConnect();

            //Waiting for the device to Auto Connect
            while (((g_uiConnectionStatus == 0) || (g_uiIpObtained == 0)))
            {
                MAP_UtilsDelay(500);              
            }
             
            //Turn Green LED Off      
            GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);
        }
    }
}
//*****************************************************************************
//
//! Create Socket and Bind to Local IP
//!
//! \param  None
//!
//! \return None
//!
//*****************************************************************************

void CreateUdpServer(tUDPSocket *pSock)
{
    int uiPort = AUDIO_PORT;
    pSock->iSockDesc = socket(AF_INET, SOCK_DGRAM, 0);
    pSock->Server.sin_family = AF_INET;
    pSock->Server.sin_addr.s_addr = htonl(INADDR_ANY);
    pSock->Server.sin_port = htons(uiPort);
    pSock->iServerLength = sizeof(pSock->Server);

    pSock->Client.sin_family = AF_INET;
    pSock->Client.sin_addr.s_addr = htonl(INVALID_CLIENT_ADDRESS);
    pSock->Client.sin_port = htons(uiPort);
    pSock->iClientLength = sizeof(pSock->Client);
    bind(pSock->iSockDesc,(struct sockaddr*)&(pSock->Server),
         pSock->iServerLength);
}

//*****************************************************************************
//
//! Network Task
//!
//! \param  pvParameters - Parameters to the task's entry function
//!
//! \return None
//!
//*****************************************************************************
void Network( void *pvParameters )
{
    //Connect to Network
    ConnectToNetwork();
    
    //Cread UDP Socket and Bind to Local IP Address
    CreateUdpServer(&g_UdpSock);
    
#ifdef MULTICAST  
    //Add to Multicast Group
    ReceiveMulticastPacket();
    //Delete the Networking Task as Service Discovery is not needed
    //osi_TaskDelete(NULL);
    //Waiting in while loop
    while(1)
    {
      MAP_UtilsDelay(50000);
    }
#else
    //Discover CC3200 Audio Devices  
    mDNS_Task();
#endif    

}
