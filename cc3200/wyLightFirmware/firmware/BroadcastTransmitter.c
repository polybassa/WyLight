/*
 * BroadcastTransmitter.c
 *
 *  Created on: 12.08.2014
 *      Author: nw
 */


#include "BroadcastTransmitter.h"

#include <stdio.h>
#include <stdint.h>
#include "hw_types.h"
#include "interrupt.h"
#include "hw_ints.h"
#include "hw_apps_rcm.h"
#include "hw_common_reg.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "hw_memmap.h"
#include "timer.h"
#include "timer_if.h"
#include "simplelink.h"
#include "wlan.h"
#include "uart_if.h"

#define IP_ADDR            0xffffffff /* 255.255.255.255 */
#define PORT_NUM           55555

#define UART_PRINT 		   Report

uint8_t capturedBroadcastMessage[110] = { 0x00, 0x0f, 0xb5, 0xb2, 0x57,
		0xfa, //MAC
		0x07, //channel
		0x3f, //rssi
		0x07,
		0xd0, //port
		0x00, 0x00, 0x24,
		0xb1, //rtc
		0x0b,
		0xff, //battery
		0x0d,
		0x11, //gpio
		0x54, 0x69, 0x6d, 0x65, 0x20, 0x4e, 0x4f, 0x54, 0x20, 0x53, 0x45, 0x54, 0x00,
		0x00, //time
		0x57, 0x69, 0x46, 0x6c, 0x79, 0x20, 0x56, 0x65, 0x72, 0x20, 0x32, 0x2e, 0x33, 0x36, 0x2c, 0x20, 0x30, 0x38, 0x2d, 0x32, 0x32, 0x2d, 0x32, 0x30, 0x31, 0x32, 0x00,
		0x00, //version
		'W', 'i', 'F', 'l', 'y', '-', 'E', 'Z', 'X', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', //deviceid
		0x4e, 0x00, //boottime
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //sensors
		};

//****************************************************************************
//
//! \brief Opening a UDP client side socket and sending data
//!
//! This function opens a UDP socket and tries to connect to a Server IP_ADDR
//!    waiting on port PORT_NUM.
//!    Then the function will send 1000 UDP packets to the server.
//!
//! \param[in]  port number on which the server will be listening on
//!
//! \return    0 on success, -1 on Error.
//
//****************************************************************************
int SendBroadcastMessage(void) {
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
		return -1;
	}

	iStatus = sl_SendTo(iSockID, capturedBroadcastMessage, sizeof(capturedBroadcastMessage), 0, (SlSockAddr_t *) &sAddr, iAddrSize);
	if (iStatus <= 0) {
		// error
		sl_Close(iSockID);
		return -1;
	}

	UART_PRINT("Sent Broadcast Message successfully\n\r");

	//closing the socket after sending 1000 packets
	sl_Close(iSockID);

	return 0;
}

void TimerBaseIntHandler(void) {
	//
	// Clear the timer interrupt.
	//
	Timer_IF_InterruptClear(TIMERA0_BASE);

	SendBroadcastMessage();

}

void BroadcastTransmitter_init(void) {

	Timer_IF_Init(PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0);

	Timer_IF_IntSetup(PRCM_TIMERA0, TIMERA0_BASE, TimerBaseIntHandler);

	Timer_IF_Start(TIMERA0_BASE, TIMER_A, PERIODIC_TEST_CYCLES * PERIODIC_TEST_LOOPS / 10);

}
