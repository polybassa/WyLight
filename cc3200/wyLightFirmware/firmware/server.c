/*
 Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

 This file is part of Wifly_Light.

 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

//*****************************************************************************
//
//! \addtogroup wylight
//! @{
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

//WyLight adaption includes
//#include "RingBuf.h"

//Application Includes
#include "server.h"


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
extern struct RingBuffer g_RingBuf;
//
// GLOBAL VARIABLES -- End
//

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
void TcpServer_Task(void *pvParameters);
void UdpServer_Task(void *pvParameters);

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

	//RingBuf_Init(&g_RingBuf);

	while (1) {
		while (!IS_CONNECTED(g_ulStatus)) {
			osi_Sleep(500);
		}

		SlSockAddrIn_t RemoteAddr;
		SlSocklen_t RemoteAddrLen = sizeof(SlSockAddrIn_t);
		volatile int SocketTcpServer, SocketTcpChild;
		uint8_t buffer[BUFFERSIZE];

		const SlSockAddrIn_t LocalAddr = { .sin_family = SL_AF_INET, .sin_port = htons(SERVER_PORT), .sin_addr.s_addr = 0 };

		SocketTcpServer = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, SL_IPPROTO_TCP);
		/*Blocking is default
		 int nonBlocking = 0;
		 sl_SetSockOpt(SocketTcpServer, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
		 */
		if (sl_Bind(SocketTcpServer, (SlSockAddr_t *) &LocalAddr, sizeof(LocalAddr)) < 0) {
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
			SocketTcpChild = sl_Accept(SocketTcpServer, (SlSockAddr_t *) &RemoteAddr, &RemoteAddrLen);

			while (SocketTcpChild > 0) {
				UART_PRINT(" Connected TCP Client\r\n");
				memset(buffer, sizeof(buffer), 0);
				int bytesReceived = sl_Recv(SocketTcpChild, buffer, sizeof(buffer) - 1, 0);
				if (bytesReceived > 0 && IS_CONNECTED(g_ulStatus)) {
					// Received some bytes
					taskENTER_CRITICAL();
					// TODO: REMOVE this check if RingBuf run's stabel
					/*if (RingBuf_HasError(&g_RingBuf)) {
					 UART_Print("ERROR: Ringbuffer overflow");
					 taskENTER_CRITICAL();
					 LOOP_FOREVER(__LINE__);
					 }

					 unsigned int i;
					 for (i = 0; i < bytesReceived; i++) {
					 RingBuf_Put(&g_RingBuf, buffer[i]);
					 }*/
					taskEXIT_CRITICAL();
					UART_PRINT("Tcp: Received %d bytes:%s\r\n", bytesReceived, buffer);
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

	while (1) {
		while (!IS_CONNECTED(g_ulStatus)) {
			osi_Sleep(500);
		}
		SlSockAddrIn_t RemoteAddr;
		SlSocklen_t RemoteAddrLen = sizeof(SlSockAddrIn_t);
		volatile int SocketUdpServer;
		unsigned char buffer[BUFFERSIZE];

		const SlSockAddrIn_t LocalAddr = { .sin_family = SL_AF_INET, .sin_port = htons(SERVER_PORT), .sin_addr.s_addr = 0 };

		SocketUdpServer = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, SL_IPPROTO_UDP);
		//Blocking is default
		// int nonBlocking = 0;
		// sl_SetSockOpt(SocketUdpServer, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));

		if (sl_Bind(SocketUdpServer, (SlSockAddr_t *) &LocalAddr, sizeof(LocalAddr)) < 0) {
			UART_PRINT(" Bind Error\n\r");
			sl_Close(SocketUdpServer);
			LOOP_FOREVER(__LINE__);
		}

		while (SocketUdpServer > 0) {
			UART_PRINT(" UDP Server started \r\n");
			memset(buffer, sizeof(buffer), 0);
			int bytesReceived = sl_RecvFrom(SocketUdpServer, buffer, sizeof(buffer) - 1, 0, (SlSockAddr_t *) &RemoteAddr, &RemoteAddrLen);
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
