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
// Application Name     - Antenna Selection
// Application Overview - CC3200 Antenna Selection is method by which user can
//                        configure the WLAN RF Antenna on the CC3200 device
//                        from a Browser
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_Antenna_Selection_Application
// or
// docs\examples\CC32xx_Antenna_Selection_Application.pdf
//
//*****************************************************************************


//****************************************************************************
//
//! \addtogroup antenna_selection
//! @{
//
//****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simplelink includes
#include "simplelink.h"

//driverlib includes
#include "datatypes.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "rom_map.h"
#include "interrupt.h"
#include "utils.h"
#include "prcm.h"
#include "gpio.h"

//Free_rtos/ti-rtos includes
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

#ifdef NOTERM
#define UART_PRINT(x, ...)
#define DBG_PRINT(x)
#else
#define UART_PRINT Report
#define DBG_PRINT  Message
#endif

#define APPLICATION_NAME        "Antenna Selection"
#define APPLICATION_VERSION     "1.0.0"
#define SUCCESS                 0

#define PAD_MODE_MASK        0x0000000F
#define PAD_STRENGTH_MASK    0x000000E0
#define PAD_TYPE_MASK        0x00000310

#define APP_TASK_PRIORITY              1
#define SPAWN_TASK_PRIORITY            9
#define OSI_STACK_SIZE                 2048
#define CONNECTION_TIMEOUT_COUNT       5000 /* 5sec */
#define TOKEN_ARRAY_SIZE               5
#define STRING_TOKEN_SIZE              10
#define RSSI_ARRAY_SIZE                5
#define STRING_IMAGE_SIZE              16

#define AP_SSID_LEN_MAX                33
#define SL_STOP_TIMEOUT                30

#define PIN_29              0x0000001C
#define PIN_30              0x0000001D
#define GPIO_ANTENNA_1      (26)
#define GPIO_ANTENNA_2      (27)

#define REG_PAD_CONFIG_26   (0x4402E108)
#define REG_PAD_CONFIG_27   (0x4402E10C)

#define SCAN_TABLE_SIZE   20

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

unsigned char g_ucIpObtained = 0, g_ucProfileAdded = 1, g_ucConnectionStatus = 0;
unsigned char g_ucConnectedToConfAP = 0, g_ucAntSelectDone = 0;
unsigned int g_uiConnectTimeoutCnt =0, g_uiIpAddress = 0;

unsigned char g_ucPriority;
char g_cWlanSSID[AP_SSID_LEN_MAX];
char g_cWlanSecurityKey[50];
SlSecParams_t g_SecParams;
Sl_WlanNetworkEntry_t g_netEntries[SCAN_TABLE_SIZE];
Sl_WlanNetworkEntry_t g_netEntriesAnt2[SCAN_TABLE_SIZE];
Sl_WlanNetworkEntry_t g_netEntriesCommon[5];

//SSID Token Antenna 1
char g_cTokenGetAnt1 [TOKEN_ARRAY_SIZE+1][STRING_TOKEN_SIZE] = {"__SL_G_US0",
                            "__SL_G_US1","__SL_G_US2", "__SL_G_US3",
                            "__SL_G_US4", "__SL_G_US5"};

//RSSI Token Antenna 1
char g_cTokenGetRssiAnt1 [TOKEN_ARRAY_SIZE][STRING_TOKEN_SIZE] = {"__SL_G_UR1",
                      "__SL_G_UR2", "__SL_G_UR3", "__SL_G_UR4", "__SL_G_UR5"};

//SSID Token Antenna 2
char g_cTokenGetAnt2 [TOKEN_ARRAY_SIZE][STRING_TOKEN_SIZE] = {"__SL_G_USA",
                    "__SL_G_USB", "__SL_G_USC", "__SL_G_USD", "__SL_G_USE"};

//RSSI Token Antenna 2
char g_cTokenGetRssiAnt2 [TOKEN_ARRAY_SIZE][STRING_TOKEN_SIZE] = {"__SL_G_URA",
                        "__SL_G_URB", "__SL_G_URC", "__SL_G_URD", "__SL_G_URE"};
//RSSI Indicator Image
char g_cRssiImg[RSSI_ARRAY_SIZE][STRING_IMAGE_SIZE] = {"images/sig1.jpg",
                                        "images/sig2.jpg","images/sig3.jpg",
                                        "images/sig4.jpg","images/sigE.jpg"};
//
// GLOBAL VARIABLES -- End
//

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
void AntennaSelect(unsigned char ucAntNum);
unsigned char getRSSILevel(signed char rssi,char** pucRssiImg);
static long ConfigureSimpleLinkToDefaultState();


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
            g_ucConnectionStatus = 1;

            //
            // Information about the connected AP (like name, MAC etc) will be
            // available in 'sl_protocol_wlanConnectAsyncResponse_t'-Application
            // can use it if required
            //
            //  sl_protocol_wlanConnectAsyncResponse_t *pEventData = NULL;
            // pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
            //
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            sl_protocol_wlanConnectAsyncResponse_t*  pEventData = NULL;

            g_ucConnectionStatus = 0;

            pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

            // If the user has initiated 'Disconnect' request,
            //'reason_code' is SL_USER_INITIATED_DISCONNECTION
            if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                UART_PRINT("[WLAN EVENT] Device disconnected from the AP\n\r");

            }
            else
            {
                UART_PRINT("[WLAN ERROR] Device disconnected from the AP\n\r");
            }
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
        /* case SL_NETAPP_IPV6_ACQUIRED:   (enable it for IPv6 case) */
        {

            g_ucIpObtained = 1;
            g_uiIpAddress = pNetAppEvent->EventData.ipAcquiredV4.ip;

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
//! \param pSlHttpServerEvent Pointer indicating http server event
//! \param pSlHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void sl_HttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent,
        SlHttpServerResponse_t *pSlHttpServerResponse)
{
    char* pcRssiImgPath = NULL;
    unsigned char ucRssiImgPathLen = 0;
    switch (pSlHttpServerEvent->Event)
    {
          case SL_NETAPP_HTTPGETTOKENVALUE:
          {
              /* Print token name */
              DBG_PRINT("token value\n\r");

              if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data, 
                              g_cTokenGetAnt1 [0],
                              pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  if(g_ucConnectedToConfAP == 1)
                  {
                      memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                              "TRUE",strlen("TRUE")); // Important - Connection Status
                      pSlHttpServerResponse->ResponseData.token_value.len = strlen("TRUE");
                  }
                  else
                  {
                      memcpy (pSlHttpServerResponse->ResponseData.token_value.data, 
                              "FALSE",strlen("FALSE")); // Important - Connection Status
                      pSlHttpServerResponse->ResponseData.token_value.len = strlen("FALSE");
                  }
              }

              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt1 [1],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {                         
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntries[0].ssid,g_netEntries[0].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntries[0].ssid_len;

              }
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt1 [2],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntries[1].ssid,
                          g_netEntries[1].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntries[1].ssid_len;
              }
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt1 [3],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntries[2].ssid,g_netEntries[2].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntries[2].ssid_len;
              }
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt1 [4],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntries[3].ssid,g_netEntries[3].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntries[3].ssid_len;
              }
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt1 [5],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntries[4].ssid,g_netEntries[4].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntries[4].ssid_len;
              }
              
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetRssiAnt1 [0],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntries[0].rssi,&pcRssiImgPath);
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
                      
              }
              
             else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                  g_cTokenGetRssiAnt1 [1],
                                  pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntries[1].rssi,&pcRssiImgPath);
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
                     
              }
              
             else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                  g_cTokenGetRssiAnt1 [2],
                                  pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntries[2].rssi,&pcRssiImgPath);                                
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
              }
              
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetRssiAnt1[3],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntries[3].rssi,&pcRssiImgPath);
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
              }
              
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetRssiAnt1[4],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntries[4].rssi,&pcRssiImgPath);
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
              }
              
              //Antenna 2
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt2[0],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {                         
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntriesAnt2[0].ssid,g_netEntriesAnt2[0].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntriesAnt2[0].ssid_len;

              }
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt2[1],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntriesAnt2[1].ssid,g_netEntriesAnt2[1].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntriesAnt2[1].ssid_len;
              }
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt2[2],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntriesAnt2[2].ssid,g_netEntriesAnt2[2].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntriesAnt2[2].ssid_len;
              }
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt2[3],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntriesAnt2[3].ssid,g_netEntriesAnt2[3].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntriesAnt2[3].ssid_len;
              }
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetAnt2[4],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          g_netEntriesAnt2[4].ssid,g_netEntriesAnt2[4].ssid_len);
                  pSlHttpServerResponse->ResponseData.token_value.len = g_netEntriesAnt2[4].ssid_len;
              }
              
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetRssiAnt2[0],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntriesAnt2[0].rssi,&pcRssiImgPath);
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
                      
              }
              
             else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                  g_cTokenGetRssiAnt2[1],
                                  pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntriesAnt2[1].rssi,&pcRssiImgPath);
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
                     
              }
              
             else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                  g_cTokenGetRssiAnt2[2],
                                  pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                
                  ucRssiImgPathLen = getRSSILevel(g_netEntriesAnt2[2].rssi,&pcRssiImgPath);                                
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
              }
              
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetRssiAnt2[3],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntriesAnt2[3].rssi,&pcRssiImgPath);
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
              }
              
              else if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                                   g_cTokenGetRssiAnt2[4],
                                   pSlHttpServerEvent->EventData.httpTokenName.len))
              {
                  ucRssiImgPathLen = getRSSILevel(g_netEntriesAnt2[4].rssi,&pcRssiImgPath);
                  // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                  memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                          pcRssiImgPath,ucRssiImgPathLen);
                  pSlHttpServerResponse->ResponseData.token_value.len = ucRssiImgPathLen;
              }
                              

              else
                  DBG_PRINT("Token ...\n\r");

          }
                  break;

          case SL_NETAPP_HTTPPOSTTOKENVALUE:
          {

              /* Post request - print post values */
              DBG_PRINT("Post request\n\r");

              if ((0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                                "__SL_P_USC", 
                pSlHttpServerEvent->EventData.httpPostData.token_name.len)) &&
                (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                                  "Add",
                   pSlHttpServerEvent->EventData.httpPostData.token_value.len)))
              {
                  g_ucProfileAdded = 0;
              }
              if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                               "__SL_P_USD", 
                     pSlHttpServerEvent->EventData.httpPostData.token_name.len))
              {
                  memcpy (g_cWlanSSID,  
                    pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                    pSlHttpServerEvent->EventData.httpPostData.token_value.len);
                  g_cWlanSSID[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;

              }
              
              if(0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                              "__SL_P_U.D",
                   pSlHttpServerEvent->EventData.httpPostData.token_name.len))
              {
                  if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '1')
                  {
                      //Select Antenna 1
                      AntennaSelect(1);
                  }
                  else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '2')
                  {
                      //Select Antenna 2
                      AntennaSelect(2);
                  }
              }

              if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                               "__SL_P_USE",
                               pSlHttpServerEvent->EventData.httpPostData.token_name.len))
              {

                  if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '0')
                  {
                      g_SecParams.Type =  SL_SEC_TYPE_OPEN;//SL_SEC_TYPE_OPEN;

                  }
                  else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '1')
                  {
                      g_SecParams.Type =  SL_SEC_TYPE_WEP;//SL_SEC_TYPE_WEP;

                  }
                  else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '2')
                  {
                      g_SecParams.Type =  SL_SEC_TYPE_WPA;//SL_SEC_TYPE_WPA;

                  }
                  else
                  {
                      g_SecParams.Type =  SL_SEC_TYPE_OPEN;//SL_SEC_TYPE_OPEN;
                  }
              }
              if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                               "__SL_P_USF",
                               pSlHttpServerEvent->EventData.httpPostData.token_name.len))
              {
                  memcpy (g_cWlanSecurityKey,pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                          pSlHttpServerEvent->EventData.httpPostData.token_value.len);
                  
                  g_cWlanSecurityKey[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
                  g_SecParams.Key = g_cWlanSecurityKey;
                  g_SecParams.KeyLen = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
              }
              
              if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data, 
                                "__SL_P_USG",
                               pSlHttpServerEvent->EventData.httpPostData.token_name.len))
              {
                 g_ucPriority = pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] - 48;
              }
              
              if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                               "__SL_P_US0", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
              {
                  g_ucAntSelectDone = 1;
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
    g_ucIpObtained = 0;
    g_ucConnectionStatus = 0;
    g_ucConnectedToConfAP = 0;
    g_ucAntSelectDone = 0;
    g_uiConnectTimeoutCnt = 0;
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
    if (ROLE_STA != lMode)
    {
        if (ROLE_AP == lMode)
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

        // reset the status values
        g_ucIpObtained = 0;
        g_ucConnectionStatus = 0;
    
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
        while(g_ucConnectionStatus == 1)
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
    sl_Stop(10);
}

void
DeInitDriverAP(void)
{
    //
    // Stop the simplelink host
    //
    sl_Stop(10);

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
    long lRetVal = sl_Start(NULL,NULL,NULL);

    if (lRetVal < 0 )
    {
        UART_PRINT("Failed to start the device \n\r");
        LOOP_FOREVER(__LINE__);
    }
    else
    {
        return lRetVal;
    }

}

void WlanConnect()
{
    //Add Profile
    sl_WlanProfileAdd(g_cWlanSSID, strlen((char*)g_cWlanSSID), 0, &g_SecParams,
                        0,g_ucPriority,0);

    //Connecting to the Access point
    sl_WlanConnect(g_cWlanSSID, strlen((char*)g_cWlanSSID), 0, &g_SecParams, 0);

    //waiting for the device to connect to the AP and obtain ip address
    while (g_uiConnectTimeoutCnt<CONNECTION_TIMEOUT_COUNT && 
                ((g_ucConnectionStatus == 0) || (g_ucIpObtained == 0)))
    {
        osi_Sleep(1); //Sleep 1 millisecond
        g_uiConnectTimeoutCnt++;
    }

    g_uiConnectTimeoutCnt = 0;

}

void AntennaSelect(unsigned char ucAntNum)
{

    if(ucAntNum == 1)
    {
         MAP_GPIOPinWrite(GPIOA3_BASE, 0xC, 0x8);
    }
    else if(ucAntNum == 2)
    {
        MAP_GPIOPinWrite(GPIOA3_BASE, 0xC, 0x4);
    }

    return;
}


//*****************************************************************************
//
//! \brief This function Sort the Scan Result by RSSI
//!
//! \param[in]      netEntries - Scan Result Array
//! \param[in]      ucSSIDCount - Length of Scan Result Array
//!
//! \return         none
//!
//! \note
//!
//
//*****************************************************************************
void SortByRSSI(Sl_WlanNetworkEntry_t* netEntries, unsigned char ucSSIDCount)
{
    Sl_WlanNetworkEntry_t tTempNetEntry;
    unsigned char ucCount, ucSwapped;
    do{
        ucSwapped = 0;
        for(ucCount =0; ucCount < ucSSIDCount - 1; ucCount++)
        {
           if(netEntries[ucCount].rssi < netEntries[ucCount + 1].rssi)
           {
              tTempNetEntry = netEntries[ucCount];
              netEntries[ucCount] = netEntries[ucCount + 1];
              netEntries[ucCount + 1] = tTempNetEntry;
              ucSwapped = 1;
           }
        } //end for
     }while(ucSwapped);
}

//*****************************************************************************
//
//! \brief This function Get the Scan Result
//!
//! \param[in]      none
//!
//! \return         Size of Scan Result Array
//!
//! \note
//!
//
//*****************************************************************************
int GetScanResult(Sl_WlanNetworkEntry_t* netEntries )
{
    UINT8   policyOpt;
    UINT32  IntervalVal = 60;
    int retVal;

    policyOpt = SL_CONNECTION_POLICY(0, 0, 0, 0, 0);
    retVal = sl_WlanPolicySet(SL_POLICY_CONNECTION , policyOpt, NULL, 0);
    if(retVal < 0)
    {
        /* Error */
    }

    // enable scan
    policyOpt = SL_SCAN_POLICY(1);

    // set scan policy - this starts the scan
    retVal = sl_WlanPolicySet(SL_POLICY_SCAN , policyOpt,
                            (UINT8 *)(IntervalVal), sizeof(IntervalVal));

    // delay 1 second to verify scan is started
    osi_Sleep(1000);

    // retVal indicates the valid number of entries
    // The scan results are occupied in netEntries[]
    retVal = sl_WlanGetNetworkList(0, SCAN_TABLE_SIZE, netEntries);

    // Disable scan
    policyOpt = SL_SCAN_POLICY(0);

    // set scan policy - this stops the scan
    sl_WlanPolicySet(SL_POLICY_SCAN , policyOpt,
                            (UINT8 *)(IntervalVal), sizeof(IntervalVal));

    return retVal;

}

//****************************************************************************
//
//! Task function implements the Antenna Selection functionality
//!
//! \param none
//!
//! This function
//!    1. Starts Device in STA Mode
//!    2. Scans, Sort and Stores all the AP with Antenna 1
//!    3. Scans, Sort and Stores all the AP with Antenna 2
//!    4. Switch to AP Mode and Wait for AP Configuration from Browser
//!    5. Switch to STA Mode and Connect to Configured AP with Selected Antenna
//!
//! \return None.
//
//****************************************************************************
void AntennaSelection(void* pTaskParams)
{

   int iDeviceMode = 0;
   unsigned char ucCountSSID;
   unsigned char ucCountSSIDAnt2;
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

    //
    // Start the driver
    //
    iDeviceMode = InitDriver();

    if (iDeviceMode == ROLE_AP)
    {
        //Device in AP Mode, Wait for Initialization to Complete
        while (g_ucIpObtained == 0)
        {
            MAP_UtilsDelay(100);
        }
    }

    sl_WlanSetMode(ROLE_STA);


    if (iDeviceMode == ROLE_AP)
        DeInitDriverAP();
    else
        DeInitDriver();

    g_ucIpObtained = 0;
    g_ucConnectionStatus = 0;

    InitDriver();

    //Select Antenna 1
    AntennaSelect(1);

    //Get Scan Result
    ucCountSSID = GetScanResult(&g_netEntries[0]);

    //Select Antenna 2
    AntennaSelect(2);

    //Get Scan Result
    ucCountSSIDAnt2 = GetScanResult(&g_netEntriesAnt2[0]);

    //Sort Scan Result
    SortByRSSI(&g_netEntries[0],ucCountSSID);
    SortByRSSI(&g_netEntriesAnt2[0],ucCountSSIDAnt2);

  while(!g_ucAntSelectDone)
  {

    //Switch to AP Mode
    sl_WlanSetMode(ROLE_AP);

    DeInitDriver();
    g_ucIpObtained = 0;
    g_ucConnectionStatus = 0;


    //Initialize the SLHost Driver
    InitDriver();

    //Wait for Ip Acquired Event in AP Mode
    while (g_ucIpObtained == 0)
    {
        MAP_UtilsDelay(100);
    }

     //
    // Wait for AP Configuraiton, Open Browser and Configure AP
    //
    while (g_ucProfileAdded && !g_ucAntSelectDone)
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

  while (1)
  {

  }

}


unsigned char getRSSILevel(signed char rssi,char** pucRssiImg)
{
    unsigned char ucTmpRssi=0;
    if(pucRssiImg == NULL)
    {
        return 0;
    }

    ucTmpRssi = rssi+128;
    if(ucTmpRssi>0 && ucTmpRssi<50 )
    {
        *pucRssiImg = g_cRssiImg[0];
        return strlen(*pucRssiImg);
    }
    else if(ucTmpRssi>=50 && ucTmpRssi<75)
    {
        *pucRssiImg = g_cRssiImg[1];
        return strlen(*pucRssiImg);
    }
    else if(ucTmpRssi>=75 && ucTmpRssi<100)
    {
        *pucRssiImg = g_cRssiImg[2];
        return strlen(*pucRssiImg);
    }
    else
    {
        *pucRssiImg = g_cRssiImg[3];
        return strlen(*pucRssiImg);
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
//!    2. Invokes the AP Antenna Selection Task
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
    
    
    MAP_PRCMPeripheralClkEnable(PRCM_GPIOA3, PRCM_RUN_MODE_CLK);
    MAP_GPIODirModeSet(GPIOA3_BASE,0xC,GPIO_DIR_MODE_OUT);

    
    //
    // Configure PIN_29 for GPIOOutput
    //    
    HWREG(REG_PAD_CONFIG_26) = ((HWREG(REG_PAD_CONFIG_26) & ~(PAD_STRENGTH_MASK 
                        | PAD_TYPE_MASK)) | (0x00000020 | 0x00000000 ));
    
    //
    // Set the mode.
    //
    HWREG(REG_PAD_CONFIG_26) = (((HWREG(REG_PAD_CONFIG_26) & ~PAD_MODE_MASK) |  
                                                    0x00000000) & ~(3<<10));
    
    //
    // Set the direction
    //
    HWREG(REG_PAD_CONFIG_26) = ((HWREG(REG_PAD_CONFIG_26) & ~0xC00) | 0x00000800);
    
    
     //
    // Configure PIN_30 for GPIOOutput
    //
    HWREG(REG_PAD_CONFIG_27) = ((HWREG(REG_PAD_CONFIG_27) & ~(PAD_STRENGTH_MASK
                                | PAD_TYPE_MASK)) | (0x00000020 | 0x00000000 ));
    
    //
    // Set the mode.
    //
    HWREG(REG_PAD_CONFIG_27) = (((HWREG(REG_PAD_CONFIG_27) & ~PAD_MODE_MASK) |  
                                        0x00000000) & ~(3<<10));

    //
    // Set the direction
    //
    HWREG(REG_PAD_CONFIG_26) = ((HWREG(REG_PAD_CONFIG_27) & ~0xC00) | 0x00000800);
    
    
    //Enable Interrupt
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);
    
     //
    // Simplelinkspawntask
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);
    
    osi_TaskCreate(AntennaSelection, (signed char*)"AntennaSelection",1024,
                        NULL, APP_TASK_PRIORITY, NULL );
    
    osi_start();

    
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
