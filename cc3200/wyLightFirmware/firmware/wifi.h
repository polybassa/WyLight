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


// Simplelink includes
#ifndef extern
#include "simplelink.h"
#endif

//Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "datatypes.h"
#include "uart.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"

//Free_rtos/ti-rtos includes
#ifndef extern
#include "FreeRTOS.h"
#include "task.h"
#include "osi.h"
#endif

//Common interface includes
#include "uart_if.h"

#define ATTEMPTING_TO_CONNECT_TO_AP			"Attempting to auto connect to AP"
#define ATTEMPTING_TO_ACQUIRE_IP			"Attempting to acquire IP"
#define NOT_CONNECTED_TO_AP					"Not connected to AP"

#define SL_STOP_TIMEOUT         30
#define SUCCESS					0

//
// Values for below macros shall be modified as per access-point(AP) properties
// SimpleLink device will connect to following AP when application is executed
//
#define SSID_NAME           "InternetBox"    /* AP SSID */
#define SECURITY_TYPE       SL_SEC_TYPE_WPA/* Security type (OPEN or WEP or WPA) */
#define SECURITY_KEY        "F!r5tUn!ver5alCybernet!c-K!net!cUltra-M!croProgrammer"              /* Password of the secured AP */
#define SSID_LEN_MAX        32
#define BSSID_LEN_MAX       6
