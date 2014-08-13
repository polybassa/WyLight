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

#include <stdint.h>

// Simplelink includes
#include "simplelink.h"

//Driverlib includes
#include "utils.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

//Common interface includes
#include "network_if.h"

//Application Includes
#define extern
#include "broadcast.h"
#undef extern

//*****************************************************************************
//
//! \addtogroup serial_wifi
//! @{
//
//*****************************************************************************

//
// GLOBAL VARIABLES -- Start
//
extern unsigned long g_ulStatus;

struct __attribute__((__packed__)) BroadcastMessage {
	uint8_t MAC[6];
	uint8_t channel;
	uint8_t rssi;
	uint16_t port;
	uint32_t rtc;
	uint16_t battery;
	uint16_t gpio;
	uint8_t asciiTime[14];
	uint8_t version[28];
	uint8_t deviceId[32];
	uint16_t boottime;
	uint8_t sensors[16];

};

static struct BroadcastMessage mBroadcastMessage = { 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, //MAC
		0x07, //channel
		0x3f, //rssi
		0x00,
		0x00, //port
		0x00, 0x00, 0x24,
		0xb1, //rtc
		0x00,
		0x00, //battery
		0x0d,
		0x11, //gpio
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, //time
		0x57, 0x69, 0x46, 0x6c, 0x79, 0x20, 0x56, 0x65, 0x72, 0x20, 0x32, 0x2e, 0x33, 0x36, 0x2c, 0x20, 0x30, 0x38, 0x2d, 0x32, 0x32, 0x2d, 0x32, 0x30, 0x31, 0x32, 0x00,
		0x00, //version
		'W', 'i', 'F', 'l', 'y', '-', 'C', 'C', '3', '2', '0', '0', 0x00, '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', //deviceid
		0x00, 0x00, //boottime
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //sensors
		};
//
// GLOBAL VARIABLES -- End
//

#define CLR_STATUS_BIT_ALL(status_variable)  (status_variable = 0)

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
void Broadcast_Task(void *pvParameters);
int SendBroadcastMessage(void);

//*****************************************************************************
//
//! Broadcast_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler function to handle the Broadcast Messages
//
//*****************************************************************************

void Broadcast_Task(void *pvParameters) {

	osi_Sleep(5000);

	while (!IS_CONNECTED(g_ulStatus)) {
		osi_Sleep(500);
	}
	// Get MAC-Address for Broadcast Message
	unsigned char macAddressLen = SL_MAC_ADDR_LEN;
	sl_NetCfgGet(SL_MAC_ADDRESS_GET, NULL, &macAddressLen, (unsigned char *) &(mBroadcastMessage.MAC));

	// Set Client Port
	mBroadcastMessage.port = htons(2000);

	memset(&(mBroadcastMessage.deviceId), 0, sizeof(mBroadcastMessage.deviceId));
	const char tempDeviceId[] = "WyLightCC3200";
	mem_copy(&(mBroadcastMessage.deviceId), (void *) tempDeviceId, sizeof(mBroadcastMessage.deviceId));

	const char tempVersion[] = "wifly-EZX Ver 4.00.1, Apr 19";
	mem_copy(&(mBroadcastMessage.version), (void *) tempVersion, sizeof(mBroadcastMessage.version));

	while (1) {

		while (!IS_CONNECTED(g_ulStatus)) {
			osi_Sleep(500);
		}

		SlSockAddrIn_t sAddr;
		int iAddrSize;
		int iSockID;
		int iStatus;

		//filling the UDP server socket address
		sAddr.sin_family = SL_AF_INET;
		sAddr.sin_port = sl_Htons((unsigned short) PORT_NUM);
		sAddr.sin_addr.s_addr = sl_Htonl((unsigned int) IP_ADDR);

		iAddrSize = sizeof(SlSockAddrIn_t);

		// creating a UDP socket
		iSockID = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, 0);
		if (iSockID < 0) {
			// error
			UART_PRINT("ERROR: Couldn't aquire socket for Broadcast transmit\r\n");
			LOOP_FOREVER(__LINE__);
		}

		do {

			SlGetRxStatResponse_t wlanStatistics;

			if (sl_WlanRxStatGet(&wlanStatistics, 0) != 0) {
				UART_PRINT("ERROR: Couldn't get wlan statistics\r\n");
				break;
			}

			mBroadcastMessage.rssi = (uint8_t) wlanStatistics.AvarageMgMntRssi;
			mBroadcastMessage.rtc = htonl(wlanStatistics.GetTimeStamp);
			// Send Broadcast Message
			iStatus = sl_SendTo(iSockID, &mBroadcastMessage, sizeof(struct BroadcastMessage), 0, (SlSockAddr_t *) &sAddr, iAddrSize);

			if (iStatus <= 0) {
				UART_PRINT("ERROR: Failure during Broadcast transmit\r\n");
				break;
			}
			osi_Sleep(1500);
		} while (IS_CONNECTED(g_ulStatus) && iStatus > 0);

		sl_Close(iSockID);
	}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
