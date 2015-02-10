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

#include "CC3200_Platform.h"

#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"

//Application Includes
#include "wifi.h"
#include "wy_firmware.h"
#include "pwm.h"
#include "BroadcastTransmitter.h"
#include "SimplelinkCustomer.h"
#include "SimplelinkServers.h"

#define APPLICATION_NAME        "WyLight Firmware"
#define APPLICATION_VERSION     "1.0.0"

//Override C++ new/delete operators to reduce memory footprint
void *operator new(size_t size) {
	return pvPortMalloc(size);
}

void *operator new[](size_t size) {
	return pvPortMalloc(size);
}

void operator delete(void *p) {
	vPortFree(p);
}

void operator delete[](void *p) {
	vPortFree(p);
}

extern "C" void abort(void)
{
    while (1);
}

// GLOBAL VARIABLES
const CC3200_Platform g_platform;
std::vector<SimplelinkCustomer*> SimplelinkCustomer::Customers;
const BroadcastTransmitter g_broadcast;
const UdpServer g_udpserver;
const TcpServer g_tcpserver;

//*****************************************************************************
// FreeRTOS User Hook Functions enabled in FreeRTOSConfig.h
//*****************************************************************************
extern "C" void vAssertCalled(const char *pcFile, unsigned long ulLine) {
	//Handle Assert here
	while (1) {
	}
}

extern "C" void vApplicationIdleHook(void) {
	//Handle Idle Hook for Profiling, Power Management etc
}

extern "C" void vApplicationMallocFailedHook() {
	//Handle Memory Allocation Errors
	while (1) {
	}
}

extern "C" void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName) {
	//Handle FreeRTOS Stack Overflow
	while (1) {
	}
}

int main(void) {

	Pwm_TaskInit();
	WlanSupport_TaskInit();
	WyLightFirmware_TaskInit();

	osi_TaskCreate(WlanSupport_Task, (signed portCHAR *) "WlanSupport", OSI_STACK_SIZE, NULL, 8, WlanSupportTaskHandle);
	osi_TaskCreate(WyLightFirmware_Task, (signed portCHAR *) "WyLightFirmware", OSI_STACK_SIZE, NULL, 7,
			WyLightFirmwareTaskHandle);
	osi_TaskCreate(WyLightGetCommands_Task, (signed portCHAR *) "GetCommands", OSI_STACK_SIZE, NULL, 6,
			WyLightGetCommandsTaskHandle);
	osi_TaskCreate(Pwm_Task, (signed portCHAR *) "PWM", OSI_STACK_SIZE, NULL, 2, &PwmTaskHandle);

	osi_start();
	while (1)
		;

}
