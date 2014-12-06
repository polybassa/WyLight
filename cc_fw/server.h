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


#ifndef _SERVER_H_
#define _SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

//Common interface includes
#include "uart_if.h"

//Free_rtos/ti-rtos includes
#include "osi.h"

#define UART_PRINT 		   		Report
#define SERVER_PORT				2000

#ifndef SUCCESS
#define SUCCESS 				0
#endif

extern OsiTaskHandle *TcpServerTaskHandle;

void TcpServer_TaskInit(void);
void TcpServer_TaskQuit(void);
void TcpServer_TaskRun(void);
void TcpServer_Task(void *pvParameters);

extern OsiTaskHandle *UdpServerTaskHandle;

void UdpServer_TaskInit(void);
void UdpServer_TaskQuit(void);
void UdpServer_TaskRun(void);
void UdpServer_Task(void *pvParameters);
	
#ifdef __cplusplus
}
#endif

#endif /* _SERVER_H_ */


