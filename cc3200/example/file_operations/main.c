//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//*****************************************************************************
//
// Application Name     -   File operations example
// Application Overview -   This example demonstate the file operations that 
//                          can be performed by the applications. The 
//                          application use the serial-flash as the storage 
//                          medium.
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_File_Operations
// or
// docs\examples\CC32xx File Operations.pdf
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup filesystem_demo
//! @{
//
//*****************************************************************************
#include <stdlib.h>
#include <string.h>

// Simplelink includes
#include "simplelink.h"

//Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"

//Common interface includes
#include "gpio_if.h"
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "pinmux.h"

#ifdef NOTERM
#define UART_PRINT(x, ...)
#else
#define UART_PRINT Report
#endif

#define APPLICATION_NAME		"FILE OPERATIONS"
#define APPLICATION_VERSION     "1.0.0"
#define SUCCESS                 0

// Loop forever, user can change it as per application's requirement
#define LOOP_FOREVER(line_number) \
            {\
                while(1); \
            }

#define SL_MAX_FILE_SIZE        64L*1024L       /* 64KB file */
#define BUF_SIZE                2048
#define USER_FILE_NAME          "fs_demo.txt"
#define SL_STOP_TIMEOUT         30
#define RED_LED_GPIO            9
#define GREEN_LED_GPIO          11

//
// GLOBAL VARIABLES -- Start
//
unsigned char gaucCmpBuf[BUF_SIZE];
const unsigned char gaucOldMacDonald[] =
		"Old MacDonald had a farm,E-I-E-I-O, \
And on his farm he had a cow, \
E-I-E-I-O, \
With a moo-moo here, \
And a moo-moo there, \
Here a moo, there a moo, \
Everywhere a moo-moo. \
Old MacDonald had a farm, \
E-I-E-I-O. \
Old MacDonald had a farm, \
E-I-E-I-O, \
And on his farm he had a pig, \
E-I-E-I-O, \
With an oink-oink here, \
And an oink-oink there, \
Here an oink, there an oink, \
Everywhere an oink-oink. \
Old MacDonald had a farm, \
E-I-E-I-O. \
Old MacDonald had a farm, \
E-I-E-I-O, \
And on his farm he had a duck, \
E-I-E-I-O, \
With a quack-quack here, \
And a quack-quack there, \
Here a quack, there a quack, \
Everywhere a quack-quack. \
Old MacDonald had a farm, \
E-I-E-I-O. \
Old MacDonald had a farm, \
E-I-E-I-O, \
And on his farm he had a horse, \
E-I-E-I-O, \
With a neigh-neigh here, \
And a neigh-neigh there, \
Here a neigh, there a neigh, \
Everywhere a neigh-neigh. \
Old MacDonald had a farm, \
E-I-E-I-O. \
Old MacDonald had a farm, \
E-I-E-I-O, \
And on his farm he had a donkey, \
E-I-E-I-O, \
With a hee-haw here, \
And a hee-haw there, \
Here a hee, there a hee, \
Everywhere a hee-haw. \
Old MacDonald had a farm, \
E-I-E-I-O. \
Old MacDonald had a farm, \
E-I-E-I-O, \
And on his farm he had some chickens, \
E-I-E-I-O, \
With a cluck-cluck here, \
And a cluck-cluck there, \
Here a cluck, there a cluck, \
Everywhere a cluck-cluck. \
Old MacDonald had a farm, \
E-I-E-I-O.";

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
// SimpleLink Asynchronous Event Handlers -- Start
//*****************************************************************************

//*****************************************************************************
//
//! This function gets triggered when HTTP Server receives Application
//! defined GET and POST HTTP Tokens.
//!
//! \param pHttpServerEvent Pointer indicating http server event
//! \param pHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent,
		SlHttpServerResponse_t *pSlHttpServerResponse) {

}

//*****************************************************************************
//
//!  \brief This function handles WLAN events
//!
//!  \param[in] pSlWlanEvent is the event passed to the handler
//!
//!  \return None
//
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {

}

//*****************************************************************************
//
//!  \brief This function handles events for IP address 
//!           acquisition via DHCP indication
//!
//!  \param[in] pNetAppEvent is the event passed to the handler
//!
//!  \return None
//
//*****************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {

}

//*****************************************************************************
//
//! This function handles socket events indication
//!
//! \param[in]      pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock) {

}

//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- End
//*****************************************************************************

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
	// In case of TI-RTOS vector table is initialize by OS itself
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
//! \brief  the aim of this example code is to demonstrate File-system
//!          capabilities of the device.
//!         For simplicity, the serial flash is used as the device under test.
//!         The procedure includes the following steps:
//!            1) open a user file for writing
//!            2) write "Old MacDonalds" child song 37 times to get just below a 64KB file
//!            3) close the user file
//!            4) open the user file for reading
//!            5) read the data and compare with the stored buffer
//!            6) close the user file
//!
//! \param  None
//!
//! \return -1 or -2 in case of error, 0 otherwise
//!         Also, Green LED is turned solid in case of success
//!         Red LED is turned solid in case of failure
//
//*****************************************************************************
int main() {
	long lFileHandle, dFileHandle;
	unsigned long ulToken, dToken;
	int iRetVal, iLoopCnt;
	unsigned char policyVal;

	//
	// Initialize Board configurations
	//
	BoardInit();

	//
	// Configure the pinmux settings for the peripherals exercised
	//
	PinMuxConfig();

	//
	// Configure LEDs
	//
	GPIO_IF_LedConfigure(LED1 | LED3);

	GPIO_IF_LedOff(MCU_RED_LED_GPIO);
	GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);

	//
	// Initializing the CC3200 networking layers
	//
	iRetVal = sl_Start(NULL, NULL, NULL);
	if (iRetVal < 0) {
		GPIO_IF_LedOn(MCU_RED_LED_GPIO);
		return -1;
	}

	//
	// reset all network policies
	//
	sl_WlanPolicySet( SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0),
			&policyVal, 1 /*PolicyValLen*/);

	iRetVal = sl_FsDel((unsigned char*) "/sys/mcuimg.bin", 0);
	if (iRetVal < 0) {
		GPIO_IF_LedOn(MCU_RED_LED_GPIO);
		//return -1;
	}

	//
	//  create a user file
	//
	iRetVal = sl_FsOpen((unsigned char *) "/sys/mcuimg.bin",
			FS_MODE_OPEN_CREATE(65536,
					_FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE
							| _FS_FILE_OPEN_FLAG_VENDOR), &ulToken,
			&lFileHandle);
	//
	//  open a user file for writing
	//
	iRetVal = sl_FsOpen((unsigned char *) "mcuimg2.bin",
	FS_MODE_OPEN_READ, &dToken, &dFileHandle);
	if (iRetVal < 0) {
		iRetVal = sl_FsClose(dFileHandle, 0, 0, 0);
		GPIO_IF_LedOn(MCU_RED_LED_GPIO);
		return -1;
	}

	SlFsFileInfo_t img2Info;

	iRetVal = sl_FsGetInfo("mcuimg2.bin", dToken, &img2Info);

	if (iRetVal < 0) {
		iRetVal = sl_FsClose(dFileHandle, 0, 0, 0);
		GPIO_IF_LedOn(MCU_RED_LED_GPIO);
		return -1;
	}

	unsigned long bytesRead = 0;
	unsigned long bytesRead_current = 0;
	const unsigned long blocksize = 1024;
	unsigned long bytesLeft = img2Info.FileLen;
	long returnValue;

	unsigned char buffer[1024];

	while (bytesLeft) {

		returnValue = sl_FsRead(dFileHandle, bytesRead, buffer, blocksize);

		if (returnValue < 0) {
			GPIO_IF_LedOn(MCU_RED_LED_GPIO);
			return -1;
		}

		bytesRead_current = returnValue;

		returnValue = sl_FsWrite(lFileHandle, bytesRead, buffer,
				bytesRead_current);

		bytesRead += returnValue;

		if (returnValue < 0) {
			GPIO_IF_LedOn(MCU_RED_LED_GPIO);
			return -1;
		}

		bytesLeft -= returnValue;
	}

	//
	// close the user file
	//
	iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
	if (SL_RET_CODE_OK != iRetVal) {
		GPIO_IF_LedOn(MCU_RED_LED_GPIO);
		return -1;
	}

	iRetVal = sl_FsClose(dFileHandle, 0, 0, 0);
	if (SL_RET_CODE_OK != iRetVal) {
		GPIO_IF_LedOn(MCU_RED_LED_GPIO);
		return -1;
	}

	//
	// turn ON the green LED indicating success
	//
	GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);
	iRetVal = sl_Stop(SL_STOP_TIMEOUT);
	if (iRetVal < 0) {
		LOOP_FOREVER(__LINE__);
		//return -1; //Will not reach here
	}
	return 0;
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
