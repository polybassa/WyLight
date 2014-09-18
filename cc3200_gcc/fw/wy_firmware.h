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


#ifndef _WY_FIRMWARE_H_
#define _WY_FIRMWARE_H_

//Common interface includes
#include "uart_if.h"

//Free_rtos/ti-rtos includes
#include "osi.h"

#define UART_PRINT 		   		Report

#ifndef SUCCESS
#define SUCCESS 				0
#endif

extern OsiTaskHandle WyLightFirmwareTaskHandle;
extern OsiTaskHandle WyLightGetCommandsTaskHandle;
extern OsiSyncObj_t FirmwareCanAccessFileSystemSemaphore;
extern OsiSyncObj_t NewDataAvailableSemaphore;

void WyLightFirmware_TaskInit(void);
void WyLightFirmware_Task(void *pvParameters);
void WyLightGetCommands_Task(void *pvParameters);

#endif /* _WY_FIRMWARE_H_ */


