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

#define IP_ADDR            0xffffffff /* 255.255.255.255 */
#define PORT_NUM           55555

#define UART_PRINT 		   Report
