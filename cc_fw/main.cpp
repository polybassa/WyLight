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

//Driverlib includes
#include "hw_ints.h"
#include "hw_types.h"
#include "interrupt.h"
#include "prcm.h"
#include "rom_map.h"
#include "pinmux.h"
#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"

//Application Includes
#include "wifi.h"
#include "server.h"
#include "wy_firmware.h"
#include "pwm.h"
#include "BroadcastTransmitter.h"

//Common interface includes
#include "uart_if.h"

#include <string.h>

#define APPLICATION_NAME        "WyLight Firmware"
#define APPLICATION_VERSION     "1.0.0"
#define SUCCESS                 0
#define UART_PRINT				Report

//
// GLOBAL VARIABLES -- Start
//
extern void (* const g_pfnVectors[])(void);
BroadcastTransmitter broadcast;
//
// GLOBAL VARIABLES -- End
//

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
	UART_PRINT("\t\t       CC3200 %s Application       \n\r", AppName);
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
	//
	// Set vector table base
	//
	MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);
	//
	// Enable Processor
	//
	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();
}

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
extern "C" void vAssertCalled(const char *pcFile, unsigned long ulLine) {
	//Handle Assert here
	while (1) {
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
extern "C" void vApplicationIdleHook(void) {
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
extern "C" void vApplicationMallocFailedHook() {
	//Handle Memory Allocation Errors
	while (1) {
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
extern "C" void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName) {
	//Handle FreeRTOS Stack Overflow
	while (1) {
	}
}

//*****************************************************************************
//
//! main
//!
//! @param  none
//!
//! @return none
//!
//! @brief  Main function
//
//*****************************************************************************
int main(void) {

	//board initializations
	BoardInit();

	//UART driver initialisations
	PinMuxConfig();
	InitTerm();

	ClearTerm();
	DisplayBanner((char *)APPLICATION_NAME);
	
	//
	// Simplelinkspawntask
	//
	VStartSimpleLinkSpawnTask(9);
	
	/*__asm("		ldr r10, =__init_array_start\n"
		  "		ldr r11, =__init_array_end\n"
		  "globals_init_loop:	\n"
		  "		cmp     r10,r11\n"
		  "		bhs		done\n"
		  "		ldr   r12, [r10], #4\n"
		  "		blx   r12\n"
		  "		b     globals_init_loop\n"
		  "done: nop \n");*/

	Pwm_TaskInit();
	WlanSupport_TaskInit();
	TcpServer_TaskInit();
	UdpServer_TaskInit();
	WyLightFirmware_TaskInit();

	//
	// Simplelinkspawntask
	//
	VStartSimpleLinkSpawnTask(9);
	broadcast.init();
	
	osi_TaskCreate(WlanSupport_Task, (signed portCHAR *) "WlanSupport", OSI_STACK_SIZE, NULL, 8, WlanSupportTaskHandle);
	//osi_TaskCreate(Broadcast_Task, (signed portCHAR *) "Broadcast", OSI_STACK_SIZE, NULL, 1, BroadcastTaskHandle);
	osi_TaskCreate(TcpServer_Task, (signed portCHAR *) "TcpServer", OSI_STACK_SIZE, NULL, 5, TcpServerTaskHandle);
	osi_TaskCreate(UdpServer_Task, (signed portCHAR *) "UdpServer", OSI_STACK_SIZE, NULL, 6, UdpServerTaskHandle);
	osi_TaskCreate(WyLightFirmware_Task, (signed portCHAR *) "WyLightFirmware", OSI_STACK_SIZE, NULL, 7, WyLightFirmwareTaskHandle);
	osi_TaskCreate(WyLightGetCommands_Task, (signed portCHAR *) "GetCommands", OSI_STACK_SIZE, NULL, 6, WyLightGetCommandsTaskHandle);
	osi_TaskCreate(Pwm_Task, (signed portCHAR *) "PWM", OSI_STACK_SIZE, NULL, 2, &PwmTaskHandle);

	osi_start();

	while (1)
		;

}
