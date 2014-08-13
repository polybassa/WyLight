//*****************************************************************************
// Copyright (C) 2014 Texas Instruments Incorporated
//
// All rights reserved. Property of Texas Instruments Incorporated.
// Restricted rights to use, duplicate or disclose this code are
// granted through contract.
// The program may not be used without the written permission of
// Texas Instruments Incorporated or against the terms and conditions
// stipulated in the agreement under which this program has been supplied,
// and under no circumstances can it be used with non-TI connectivity device.
//
//*****************************************************************************

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
