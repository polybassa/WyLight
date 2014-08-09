//*****************************************************************************
// wdt_if.c
//
// watchdog timer interface APIs
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

#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "wdt.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "wdt_if.h"
#ifdef USE_TIRTOS
#include <stdio.h>
#include "osi.h"
#endif
/****************************************************************************/
/*                       FUNCTION DEFINITIONS                               */
/****************************************************************************/

//****************************************************************************
//
//! Initialize the watchdog timer
//!
//! \param fpAppWDTCB is the WDT interrupt handler to be registered
//! \param uiReloadVal is the reload value to be set to the WDT
//! 
//! This function  
//!        1. Initializes the WDT
//!
//! \return None.
//
//****************************************************************************
void WDT_IF_Init(fAPPWDTDevCallbk fpAppWDTCB,
                   unsigned int uiReloadVal)
{
    //
    // Unlock to be able to configure the registers
    //
    MAP_WatchdogUnlock(WDT_BASE);

#ifdef USE_TIRTOS
    osi_InterruptRegister(INT_WDT, fpAppWDTCB, INT_PRIORITY_LVL_1);
#else
    MAP_WatchdogIntRegister(WDT_BASE,fpAppWDTCB);
#endif

    //
    // Enable the interrupt
    //
    MAP_WatchdogIntEnable(WDT_BASE);

    //
    // Enable stalling of the watchdog timer during debug events
    //
    MAP_WatchdogStallEnable(WDT_BASE);

    //
    // Set the watchdog timer reload value
    //
    MAP_WatchdogReloadSet(WDT_BASE,uiReloadVal);
}
//****************************************************************************
//
//! DeInitialize the watchdog timer
//!
//! \param None
//! 
//! This function  
//!        1. DeInitializes the WDT
//!
//! \return None.
//
//****************************************************************************
void WDT_IF_DeInit()
{
    //
    // Unlock to be able to configure the registers
    //
    MAP_WatchdogUnlock(WDT_BASE);

	//
    // Disable stalling of the watchdog timer during debug events
    //
    MAP_WatchdogStallDisable(WDT_BASE);

	//
    // Clear the interrupt
    //
    MAP_WatchdogIntClear(WDT_BASE);

	//
    // Unregister the interrupt
    //
    MAP_WatchdogIntUnregister(WDT_BASE);
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
