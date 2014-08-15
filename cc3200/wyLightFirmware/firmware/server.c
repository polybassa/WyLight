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
#include "server.h"
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
//
// GLOBAL VARIABLES -- End
//

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
void TcpServer_Task(void *pvParameters);
void UdpServer_Task(void *pvParameters);
void StartTcpServer();

//*****************************************************************************
//
//! TcpServer_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler function to handle the TcpServer Socket
//
//*****************************************************************************

#define BUFFERSIZE 64

void TcpServer_Task(void *pvParameters) {

	// Inital Wait to give the main Task time to establish the wifi connection
	osi_Sleep(5000);

	while (1) {
		while (!IS_CONNECTED(g_ulStatus)) {
			osi_Sleep(500);
		}
		SlSockAddr_t LocalAddr, RemoteAddr;
		SlSocklen_t RemoteAddrLen;
		volatile int SocketTcpServer, SocketTcpChild;
		unsigned char buffer[BUFFERSIZE];

		LocalAddr.sa_family = SL_AF_INET;
		LocalAddr.sa_data[0] = ((SERVER_PORT >> 8) & 0xFF);
		LocalAddr.sa_data[1] = (SERVER_PORT & 0xFF);

		//all 0 => Own IP address
		memset(&LocalAddr.sa_data[2], 0, 4);

		SocketTcpServer = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, SL_IPPROTO_TCP);
		/*Blocking is default
		 int nonBlocking = 0;
		 sl_SetSockOpt(SocketTcpServer, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
		 */
		if (sl_Bind(SocketTcpServer, &LocalAddr, sizeof(LocalAddr)) < 0) {
			UART_PRINT(" Bind Error\n\r");
			sl_Close(SocketTcpServer);
			LOOP_FOREVER(__LINE__);
		}
		// Backlog = 1 to accept maximal 1 connection
		if (sl_Listen(SocketTcpServer, 1) < 0) {
			UART_PRINT(" Listen Error\n\r");
			sl_Close(SocketTcpServer);
			LOOP_FOREVER(__LINE__);
		}

		while (SocketTcpServer > 0) {
			SocketTcpChild = sl_Accept(SocketTcpServer, &RemoteAddr, &RemoteAddrLen);

			while (SocketTcpChild > 0) {
				UART_PRINT(" Connected TCP Client\r\n");
				memset(buffer, sizeof(buffer), 0);
				int bytesReceived = sl_Recv(SocketTcpChild, buffer, sizeof(buffer) - 1, 0);
				if (bytesReceived > 0 && IS_CONNECTED(g_ulStatus)) {
					// Received some bytes
					// TODO: Write received Bytes in global Buffer
					UART_PRINT("Received %d bytes:%s\r\n", bytesReceived, buffer);
				} else {
					// Error occured on child socket
					sl_Close(SocketTcpChild);
					SocketTcpChild = 0;
					break;
				}
			}
			if (!IS_CONNECTED(g_ulStatus)) {
				sl_Close(SocketTcpServer);
				SocketTcpServer = 0;
				break;
			}
		}
	}
}

//*****************************************************************************
//
//! UdpServer_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler function to handle the TcpServer Socket
//
//*****************************************************************************
void UdpServer_Task(void *pvParameters) {

	// Inital Wait to give the main Task time to establish the wifi connection
	osi_Sleep(5000);

	while (1) {
		while (!IS_CONNECTED(g_ulStatus)) {
			osi_Sleep(500);
		}
		SlSockAddr_t LocalAddr, RemoteAddr;
		SlSocklen_t RemoteAddrLen;
		volatile int SocketUdpServer;
		unsigned char buffer[BUFFERSIZE];

		LocalAddr.sa_family = SL_AF_INET;
		LocalAddr.sa_data[0] = ((SERVER_PORT >> 8) & 0xFF);
		LocalAddr.sa_data[1] = (SERVER_PORT & 0xFF);

		//all 0 => Own IP address
		memset(&LocalAddr.sa_data[2], 0, 4);

		SocketUdpServer = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, SL_IPPROTO_UDP);
		/*Blocking is default
		 int nonBlocking = 0;
		 sl_SetSockOpt(SocketTcpServer, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
		 */
		if (sl_Bind(SocketUdpServer, &LocalAddr, sizeof(LocalAddr)) < 0) {
			UART_PRINT(" Bind Error\n\r");
			sl_Close(SocketUdpServer);
			LOOP_FOREVER(__LINE__);
		}

		while (SocketUdpServer > 0) {
			UART_PRINT(" UDP Server started \r\n");
			memset(buffer, sizeof(buffer), 0);
			int bytesReceived = sl_RecvFrom(SocketUdpServer, buffer, sizeof(buffer) - 1, 0, &RemoteAddr, &RemoteAddrLen);
			if (bytesReceived > 0 && IS_CONNECTED(g_ulStatus)) {
				// Received some bytes
				// TODO: Write received Bytes in global Buffer
				UART_PRINT("Received %d bytes:%s\r\n", bytesReceived, buffer);
			}

			if (!IS_CONNECTED(g_ulStatus)) {
				sl_Close(SocketUdpServer);
				SocketUdpServer = 0;
				break;
			}
		}
	}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
