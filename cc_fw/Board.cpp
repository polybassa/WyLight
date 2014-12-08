/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.
 
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

#include "Board.h"

//Driverlib includes
#include "hw_ints.h"
#include "hw_types.h"
#include "interrupt.h"
#include "prcm.h"
#include "rom_map.h"
#include "pinmux.h"
#include "osi.h"
//Common interface includes
#include "uart_if.h"

#define UART_PRINT	Report

extern void (* const g_pfnVectors[])(void);

Board::Board() {
		// Set vector table base
		MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);
		
		// Enable Processor
		MAP_IntMasterEnable();
		MAP_IntEnable(FAULT_SYSTICK);
		
		PRCMCC3200MCUInit();
		
		//UART driver initialisations
		PinMuxConfig();
		InitTerm();
		ClearTerm();
		
		VStartSimpleLinkSpawnTask(9);
	
		UART_PRINT("\r\n---------------------------\r\n");
	}
