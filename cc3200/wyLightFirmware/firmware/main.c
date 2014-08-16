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
#include "rom.h"
#include "rom_map.h"
#include "pinmux.h"

//Application Includes
#include "wifi.h"
#include "broadcast.h"
#include "wylightAdaption.h"

//Common interface includes
#include "network_if.h"

#define APPLICATION_NAME        "WyLight Firmware"
#define APPLICATION_VERSION     "1.0.0"
#define SUCCESS                 0

#define OSI_STACK_SIZE        1024

//****************************************************************************
//                      FUNCTION PROTOTYPES
//****************************************************************************
extern void Main_Task(void *pvParameters);
extern void Broadcast_Task(void *pvParameters);
extern void TcpServer_Task(void *pvParameters);
extern void UdpServer_Task(void *pvParameters);
extern void WyLight_Task(void *pvParameters);

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
static void DisplayBanner(char * AppName);
static void BoardInit(void);

//
// GLOBAL VARIABLES -- Start
//
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
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
	/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
	//
	// Set vector table base
	//
#if defined(ccs)
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
	DisplayBanner(APPLICATION_NAME);

	//
	// Simplelinkspawntask
	//
	VStartSimpleLinkSpawnTask(9);

	osi_TaskCreate(Main_Task, (signed portCHAR *) "Main", OSI_STACK_SIZE, NULL, 1, NULL);
	osi_TaskCreate(Broadcast_Task,(signed portCHAR *) "Broadcast",OSI_STACK_SIZE, NULL, 5, NULL);
	osi_TaskCreate(TcpServer_Task, (signed portCHAR *) "TcpServer", OSI_STACK_SIZE, NULL, 2, NULL);
	osi_TaskCreate(UdpServer_Task, (signed portCHAR *) "UdpServer", OSI_STACK_SIZE, NULL, 2, NULL);

	osi_start();

	while (1)
		;

}
