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


#ifndef __BROADCAST_H_
#define __BROADCAST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//Common interface includes
#include "uart_if.h"

//Free_rtos/ti-rtos includes
#include "osi.h"

#ifndef SUCCESS
#define SUCCESS 	0
#endif

#define BC_PORT_NUM        	55555
#define UART_PRINT 		   	Report

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

extern OsiTaskHandle BroadcastTaskHandle;

void Broadcast_TaskInit(void);
void Broadcast_TaskQuit(void);
void Broadcast_TaskRun(void);
void Broadcast_Task(void *pvParameters);
	
#ifdef __cplusplus
}
#endif

#endif
