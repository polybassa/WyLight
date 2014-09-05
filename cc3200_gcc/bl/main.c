/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

 This file is part of WyLight.

 WyLight is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 WyLight is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"

// SimpleLink includes
#include "simplelink.h"

// driverlib includes
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "utils.h"
#include "interrupt.h"

// common interface includes
#include "uart_if.h"
#include "pinmux.h"
#include "wy_bl_network_if.h"
#include "gpio_if.h"

// wylight includes
#include "bootloader.h"
#include "firmware_loader.h"

#define UART_PRINT          Report

//
// GLOBAL VARIABLES -- Start
//
extern void (* const g_pfnVectors[])(void);

static const char APP_NAME[] = "WyLight Bootloader";
const unsigned long BOOTLOADER_VERSION;
// GLOBAL VARIABLES -- End

extern void TcpServer(void);

static void DisplayBanner(const char *const AppName)
{
	UART_PRINT("\n\n\n\r");
	UART_PRINT("\t\t *************************************************\n\r");
	UART_PRINT("\t\t	  CC3200 %s Application       \n\r", AppName);
	UART_PRINT("\t\t	        %s, %s          \n\r", __DATE__, __TIME__);
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

static int ReadJumper() {
	unsigned int GPIOPort;
	unsigned char GPIOPin;

//Read GPIO
	GPIO_IF_GetPortNPin(SH_GPIO_3, &GPIOPort, &GPIOPin);
	return GPIO_IF_Get(SH_GPIO_3, GPIOPort, GPIOPin);
}

int main() {
	// Board Initialization
	BoardInit();

	// Configure the pinmux settings for the peripherals exercised
	PinMuxConfig();

	// Configuring UART
	InitTerm();

	GPIO_IF_LedConfigure(LED1 | LED2 | LED3);
	GPIO_IF_LedOff(MCU_ALL_LED_IND);

	DisplayBanner(APP_NAME);

	Network_IF_InitDriver();

	// Starting the CC3200 networking layers
	Network_IF_StartSimpleLinkAsAP();

	GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);

	if (ReadJumper() == 0) {

		if (ERROR == LoadAndExecuteFirmware()) {
			UART_PRINT("Firmware corrupt\r\n");
			GPIO_IF_LedOn(MCU_RED_LED_GPIO);
		}
	}
	GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);

	TcpServer();
	StartFirmware();
}

