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
// Application Name     - Antenna Selection
// Application Overview - The objective of this sample code is to demonstrate
//						  how to connect to a server to download a file and save
//						  it on the serial flash. This scheme can be used by
//						  App developer to update any file over the network.
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_File_Download_Application
// or
// docs\examples\CC32xx_File_Download_Application.pdf
//
//*****************************************************************************

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"

// SimpleLink includes
#include "simplelink.h"

// driverlib includes
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "utils.h"
#include "interrupt.h"

// common interface includes
#include "uart_if.h"
#include "pinmux.h"

// wylight includes
#include "firmware_download.h"
#include "BroadcastTransmitter.h"


#define UART_PRINT          Report
#define APP_NAME            "File Download"

//
// Values for below macros shall be modified as per access-point(AP) properties
// SimpleLink device will connect to following AP when application is executed
//
#define SSID_NAME           "InternetBox"    /* AP SSID */
#define SECURITY_TYPE       SL_SEC_TYPE_WPA/* Security type (OPEN or WEP or WPA) */
#define SECURITY_KEY        "F!r5tUn!ver5alCybernet!c-K!net!cUltra-M!croProgrammer"              /* Password of the secured AP */
#define SSID_LEN_MAX        32
#define BSSID_LEN_MAX       6

#define SUCCESS             0
#define SL_STOP_TIMEOUT     30

// File on the serial flash to be replaced
#define FILE_NAME "/sys/mcuimg.bin"

enum {
	CONNECTED = 0x1, IP_ACQUIRED = 0x2
} e_Stauts;

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
typedef enum {
	STATUS_BIT_CONNECTION = 0, // If this bit is Set SimpleLink device is
							   // connected to the AP

	STATUS_BIT_IP_AQUIRED        // If this bit is Set SimpleLink device has
								 // acquired IP

} e_StatusBits;

// Application specific status/error codes
typedef enum {
	// Choosing -0x7D0 to avoid overlap w/ host-driver's error codes
	LAN_CONNECTION_FAILED = -0x7D0,
	INTERNET_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
	DEVICE_NOT_IN_STATION_MODE = INTERNET_CONNECTION_FAILED - 1,

	STATUS_CODE_MAX = -0xBB8
} e_AppStatusCodes;

#define SET_STATUS_BIT(status_variable, bit)  status_variable |= (1<<(bit))
#define CLR_STATUS_BIT(status_variable, bit)  status_variable &= ~(1<<(bit))
#define GET_STATUS_BIT(status_variable, bit)  (0 != (status_variable & \
                                                                (1<<(bit))))

#define IS_CONNECTED(status_variable)         GET_STATUS_BIT(status_variable, \
                                                       STATUS_BIT_CONNECTION)
#define IS_IP_ACQUIRED(status_variable)       GET_STATUS_BIT(status_variable, \
                                                       STATUS_BIT_IP_AQUIRED)

//
// GLOBAL VARIABLES -- Start
//
#if defined(ccs) || defined(gcc)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

unsigned long g_ulStatus = 0;                                //SimpleLink Status
unsigned long g_ulDestinationIP; // IP address of destination server
unsigned long g_ulGatewayIP = 0; //Network Gateway IP address
unsigned char g_ucConnectionSSID[SSID_LEN_MAX + 1]; //Connection SSID
unsigned char g_ucConnectionBSSID[BSSID_LEN_MAX]; //Connection BSSID
int g_iSockID;
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
//! \param[in]  pWlanEvent - Pointer to WLAN Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent) {
	switch (pWlanEvent->Event) {
	case SL_WLAN_CONNECT_EVENT: {
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
		memcpy(g_ucConnectionSSID, pWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_name, pWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
		memcpy(g_ucConnectionBSSID, pWlanEvent->EventData.STAandP2PModeWlanConnected.bssid,
		SL_BSSID_LENGTH);

		UART_PRINT("[WLAN EVENT] STA Connected to the AP: %s ,"
				" BSSID: %x:%x:%x:%x:%x:%x\n\r", g_ucConnectionSSID, g_ucConnectionBSSID[0], g_ucConnectionBSSID[1], g_ucConnectionBSSID[2], g_ucConnectionBSSID[3],
				g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
	}
		break;

	case SL_WLAN_DISCONNECT_EVENT: {
		sl_protocol_wlanConnectAsyncResponse_t* pEventData = NULL;

		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

		pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

		// If the user has initiated 'Disconnect' request,
		//'reason_code' is SL_USER_INITIATED_DISCONNECTION
		if (SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code) {
			UART_PRINT("[WLAN EVENT]Device disconnected from the AP: %s,"
					"BSSID: %x:%x:%x:%x:%x:%x on application's request \n\r", g_ucConnectionSSID, g_ucConnectionBSSID[0], g_ucConnectionBSSID[1], g_ucConnectionBSSID[2],
					g_ucConnectionBSSID[3], g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
		} else {
			UART_PRINT("[WLAN ERROR]Device disconnected from the AP AP: %s,"
					"BSSID: %x:%x:%x:%x:%x:%x on an ERROR..!! \n\r", g_ucConnectionSSID, g_ucConnectionBSSID[0], g_ucConnectionBSSID[1], g_ucConnectionBSSID[2],
					g_ucConnectionBSSID[3], g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
		}
		memset(g_ucConnectionSSID, 0, sizeof(g_ucConnectionSSID));
		memset(g_ucConnectionBSSID, 0, sizeof(g_ucConnectionBSSID));
	}
		break;

	default: {
		UART_PRINT("[WLAN EVENT] Unexpected event [0x%x]\n\r", pWlanEvent->Event);
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
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {
	switch (pNetAppEvent->Event) {
	case SL_NETAPP_IPV4_ACQUIRED: {
		SlIpV4AcquiredAsync_t *pEventData = NULL;

		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

		//Ip Acquired Event Data
		pEventData = &pNetAppEvent->EventData.ipAcquiredV4;

		//Gateway IP address
		g_ulGatewayIP = pEventData->gateway;

		UART_PRINT("[NETAPP EVENT] IP Acquired: IP=%d.%d.%d.%d , "
				"Gateway=%d.%d.%d.%d\n\r", SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip, 3), SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip, 2),
				SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip, 1), SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip, 0),
				SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway, 3), SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway, 2),
				SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway, 1), SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway, 0));
	}
		break;

	default: {
		UART_PRINT("[NETAPP EVENT] Unexpected event [0x%x] \n\r", pNetAppEvent->Event);
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
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent, SlHttpServerResponse_t *pHttpResponse) {
	UART_PRINT("[SERVER CALLBACK]");
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
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) {
	//
	// Most of the general errors are not FATAL are are to be handled
	// appropriately by the application
	//
	UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n", pDevEvent->EventData.deviceEvent.status, pDevEvent->EventData.deviceEvent.sender);
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
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock) {
	//
	// This application doesn't work w/ socket - Events are not expected
	//
	switch (pSock->Event) {
	case SL_NETAPP_SOCKET_TX_FAILED:
		switch (pSock->EventData.status) {
		case SL_ECLOSE:
			UART_PRINT("[SOCK ERROR] - close socket (%d) operation "
					"failed to transmit all queued packets\n\n", pSock->EventData.sd);
			break;
		default:
			UART_PRINT("[SOCK ERROR] - TX FAILED : socket %d , reason"
					"(%d) \n\n", pSock->EventData.sd, pSock->EventData.status);
		}
		break;

	default:
		UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n", pSock->Event);
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
static void InitializeAppVariables() {
	g_ulStatus = 0;
	g_ulGatewayIP = 0;
	memset(g_ucConnectionSSID, 0, sizeof(g_ucConnectionSSID));
	memset(g_ucConnectionBSSID, 0, sizeof(g_ucConnectionBSSID));
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

static long ConfigureSimpleLinkToDefaultState() {
	SlVersionFull ver = { 0 };

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
	lRetVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &ucConfigOpt, &ucConfigLen, (unsigned char *) (&ver));
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	UART_PRINT("Host Driver Version: %s\n\r", SL_DRIVER_VERSION);
	UART_PRINT("Build Version %d.%d.%d.%d.31.%d.%d.%d.%d.%d.%d.%d.%d\n\r", ver.NwpVersion[0], ver.NwpVersion[1], ver.NwpVersion[2], ver.NwpVersion[3],
			ver.ChipFwAndPhyVersion.FwVersion[0], ver.ChipFwAndPhyVersion.FwVersion[1], ver.ChipFwAndPhyVersion.FwVersion[2], ver.ChipFwAndPhyVersion.FwVersion[3],
			ver.ChipFwAndPhyVersion.PhyVersion[0], ver.ChipFwAndPhyVersion.PhyVersion[1], ver.ChipFwAndPhyVersion.PhyVersion[2], ver.ChipFwAndPhyVersion.PhyVersion[3]);

	// Set connection policy to Auto + SmartConfig
	//      (Device's default connection policy)
	lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Remove all profiles
	lRetVal = sl_WlanProfileDel(0xFF);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// If the device is not in station-mode, try putting it in staion-mode
	if (ROLE_STA != lMode) {
		if (ROLE_AP == lMode) {
			// If the device is in AP mode, we need to wait for this event
			// before doing anything
			while (!IS_IP_ACQUIRED(g_ulStatus)) {
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

		lRetVal = sl_Start(0, 0, 0);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		// Check if the device is in station again
		if (ROLE_STA != lRetVal) {
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
	if (0 == lRetVal) {
		// Wait
		while (IS_CONNECTED(g_ulStatus)) {
#ifndef SL_PLATFORM_MULTI_THREADED
			_SlNonOsMainLoopTask();
#endif
		}
	}

	// Enable DHCP client
	lRetVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE, 1, 1, &ucVal);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Disable scan
	ucConfigOpt = SL_SCAN_POLICY(0);
	lRetVal = sl_WlanPolicySet(SL_POLICY_SCAN, ucConfigOpt, NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Set Tx power level for station mode
	// Number between 0-15, as dB offset from max power - 0 will set max power
	ucPower = 0;
	lRetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
	WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *) &ucPower);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Set PM policy to normal
	lRetVal = sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
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
//! \param  Status value
//!
//! \return  None
//!
//! \warning    If the WLAN connection fails or we don't aquire an IP
//!            address, It will be stuck in this function forever.
//
//****************************************************************************
static long WlanConnect() {
	SlSecParams_t secParams = { 0 };
	long lRetVal = 0;

	secParams.Key = SECURITY_KEY;
	secParams.KeyLen = strlen(SECURITY_KEY);
	secParams.Type = SECURITY_TYPE;

	lRetVal = sl_WlanConnect(SSID_NAME, strlen(SSID_NAME), 0, &secParams, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Wait for WLAN Event
	while ((!IS_CONNECTED(g_ulStatus)) || (!IS_IP_ACQUIRED(g_ulStatus))) {
		// wait till connects to an AP
		_SlNonOsMainLoopTask();
	}

	return SUCCESS;

}

//****************************************************************************
//
//! Create an endpoint for communication and initiate connection on socket.*/
//!
//! \brief Create connection with server
//!
//! This function opens a socket and create the endpoint communication with server
//!
//! \param[in]      DestinationIP - IP address of the server
//!
//! \return         socket id for success and negative for error
//
//****************************************************************************
int CreateConnection(unsigned long DestinationIP) {
	SlSockAddrIn_t Addr;
	int Status = 0;
	int AddrSize = 0;
	int SockID = 0;

	Addr.sin_family = SL_AF_INET;
	Addr.sin_port = sl_Htons(80);
	Addr.sin_addr.s_addr = sl_Htonl(DestinationIP);

	AddrSize = sizeof(SlSockAddrIn_t);

	SockID = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, 0);
	if (SockID < 0) {
		/* Error */
		UART_PRINT("Error while opening the socket\r\n");
		return -1;
	}

	Status = sl_Connect(SockID, (SlSockAddr_t *) &Addr, AddrSize);
	if (Status < 0) {
		/* Error */
		UART_PRINT("Error during connection with server\r\n");
		sl_Close(SockID);
		return -1;
	}
	return SockID;
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
static void DisplayBanner(char * AppName) {

	UART_PRINT("\n\n\n\r");
	UART_PRINT("\t\t *************************************************\n\r");
	UART_PRINT("\t\t	  CC3200 %s Application       \n\r", AppName);
	UART_PRINT("\t\t	        %s, %s          \n\r", __DATE__, __TIME__);
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
static void BoardInit(void) {
	/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
	//
	// Set vector table base
	//
#if defined(ccs) || defined(gcc)
	MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);
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

int main() {
	int retRes = 0;
	long lRetVal = -1;

	//
	// Board Initialization
	//
	BoardInit();

	//
	// Configure the pinmux settings for the peripherals exercised
	//
	PinMuxConfig();

	//
	// Configuring UART
	//
	InitTerm();

	//
	// Display banner
	//
	DisplayBanner(APP_NAME);

	UART_PRINT("\n\rStarted file download sample Application.");
	UART_PRINT("\n\r\n\r");

	InitializeAppVariables();

	//
	// Following function configure the device to default state by cleaning
	// the persistent settings stored in NVMEM (viz. connection profiles &
	// policies, power policy etc)
	//
	// Applications may choose to skip this step if the developer is sure
	// that the device is in its desired state at start of applicaton
	//
	// Note that all profiles and persistent settings that were done on the
	// device will be lost
	//
	lRetVal = ConfigureSimpleLinkToDefaultState();
	if (lRetVal < 0) {
		if (DEVICE_NOT_IN_STATION_MODE == lRetVal) {
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
	if (lRetVal < 0 || ROLE_STA != lRetVal) {
		UART_PRINT("Failed to start the device \n\r");
		LOOP_FOREVER(__LINE__);
	}

	UART_PRINT("Device started as STATION \n\r");

	// Connecting to WLAN AP - Set with static parameters defined at the top
	// After this call we will be connected and have IP address
	WlanConnect();

	UART_PRINT("Connected to the AP: %s\r\n", SSID_NAME);

	//BroadcastTransmitter_init();

	// Create a TCP connection to the Web Server
	g_iSockID = CreateConnection(GetServerIP());

	if (g_iSockID < 0) return -1;

	UART_PRINT("Connection to server created successfully\r\n");
	// Download the file, verify the file and replace the exiting file
	if (GetFileFromServer(g_iSockID, "firmware.bin", "/temp/firmware.bin") < 0) {
		retRes = sl_Close(g_iSockID);
		UART_PRINT("Error downloading file\r\n");
		return -1;
	}

	retRes = sl_Close(g_iSockID);
	if (0 > retRes) {
		UART_PRINT("Error during closing socket\r\n");
		return -1;
	}

	g_iSockID = CreateConnection(GetServerIP());

	if (g_iSockID < 0) return -1;

	UART_PRINT("Connection to server created successfully\r\n");
	if (GetFileFromServer(g_iSockID, "firmware.sha", "/temp/firmware.sha") < 0) {
		retRes = sl_Close(g_iSockID);
		UART_PRINT("Error downloading file\r\n");
		return -1;
	}

	retRes = sl_Close(g_iSockID);
	if (0 > retRes) {
		UART_PRINT("Error during closing socket\r\n");
		return -1;
	}

	retRes = VerifyFile("/temp/firmware.bin", "/temp/firmware.sha");
	if (retRes < 0) {
		UART_PRINT("Error verifying files\r\n");
	}
	retRes = ReplaceFile("/temp/firmware.bin", FILE_NAME);
	if (retRes < 0) {
		UART_PRINT("Error during replacing the file\r\n");
	}

	UART_PRINT("\nThank you\r\n");
	// Stop the CC3200 device
	sl_Stop(0xFF);

	LOOP_FOREVER(__LINE__);

}

