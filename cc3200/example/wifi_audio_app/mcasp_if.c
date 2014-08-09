//*****************************************************************************
// mcasp_if.c
//
//  MCASP interface APIs.
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
//! \addtogroup microphone
//! @{
//
//*****************************************************************************
/* Standard includes. */
#include <stdio.h>

/* Hardware & driverlib library includes. */
#include "hw_types.h"
#include "hw_mcasp.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "debug.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "udma.h"
#include "interrupt.h"
#include "prcm.h"
#include "uart.h"
#include "pin.h"
#include "i2s.h"

/* Demo app includes. */
#include "mcasp_if.h"
#include "gpio_if.h"
#include "network.h"

//*****************************************************************************
//
//! Returns the pointer to transfer Audio samples to be rendered
//!
//! \param None
//! 
//! This function  
//!        1. Returns the pointer to the data buffer
//!
//! \return None.
//
//*****************************************************************************
unsigned int* AudioRendererGetDMADataPtr()
{
    return (unsigned int *)(I2S_TX_DMA_PORT);
}

//*****************************************************************************
//
//! Returns the pointer to transfer Audio samples to be captured
//!
//! \param None
//! 
//! This function  
//!        1. Returns the pointer to the data buffer
//!
//! \return None.
//
//*****************************************************************************
unsigned int* AudioCapturerGetDMADataPtr()
{
    return (unsigned int *)(I2S_RX_DMA_PORT);
}



//*****************************************************************************
//
//! Initialize the audio capturer
//!
//! \param None
//! 
//! This function initializes 
//!        1. Initializes the McASP module
//!
//! \return None.
//
//*****************************************************************************
void AudioCapturerInit()
{
    //
    // Initialising the McASP
    //
    McASPInit();
}
//*****************************************************************************
//
//! Initialize McASP Interface
//!
//! \param None
//! 
//! This function initializes 
//!        1. Initializes the McASP module
//!
//! \return None.
//
//*****************************************************************************
void McASPInit()
{

    MAP_PRCMPeripheralClkEnable(PRCM_I2S,PRCM_RUN_MODE_CLK); 
    MAP_PRCMI2SClockFreqSet(512000);   
      //512000 = 16*2*16000Khz(Num of bytes * STEREO * 16000 sampling)
 
}

//*****************************************************************************
//
//! Setup the Audio capturer callback routine and the interval of callback.
//! On the invocation the callback is expected to fill the AFIFO with enough
//! number of samples for one callback interval.
//!
//! \param pfnAppCbHndlr is the callback handler that is invoked when 
//! \e ucCallbackEvtSamples space is available in the audio FIFO
//! \param ucCallbackEvtSamples is used to configure the callback interval
//! 
//! This function initializes 
//!        1. Initializes the interrupt callback routine
//!        2. Sets up the AFIFO to interrupt at the configured interval
//!
//! \return None.
//
//*****************************************************************************
void AudioCapturerSetupDMAMode(void (*pfnAppCbHndlr)(void), 
                              unsigned long ulCallbackEvtSamples)
{

    MAP_I2SIntEnable(I2S_BASE,I2S_INT_XDATA);
#ifdef USE_TIRTOS
    osi_InterruptRegister(INT_I2S, pfnAppCbHndlr, INT_PRIORITY_LVL_1);
#else
    MAP_I2SIntRegister(I2S_BASE,pfnAppCbHndlr);
#endif

    MAP_I2STxFIFOEnable(I2S_BASE,8,1);
    MAP_I2SRxFIFOEnable(I2S_BASE,8,1);

}

//*****************************************************************************
//
//! Initialize the AudioCaptureRendererConfigure
//!
//! \param None
//! 
//! This function initializes 
//!        1. Initialize I2S to work for 
//              a) 16bit,STEREO,16000 sampling( ie 512000 Hz bit clock)
//              b) DMA
//!
//! \return None.
//
//*****************************************************************************

void AudioCaptureRendererConfigure()
{

    MAP_I2SConfigSetExpClk(I2S_BASE,512000,512000,I2S_SLOT_SIZE_16|
                                       I2S_PORT_DMA);
    MAP_I2SSerializerConfig(I2S_BASE,I2S_DATA_LINE_1,I2S_SER_MODE_RX,
                                            I2S_INACT_LOW_LEVEL);
    MAP_I2SSerializerConfig(I2S_BASE,I2S_DATA_LINE_0,I2S_SER_MODE_TX,
                                            I2S_INACT_LOW_LEVEL);

}
//*****************************************************************************
//
//! Initialize the Audio_Start
//!
//! \param None
//! 
//! This function initializes 
//!        1. Enable the McASP module for TX and RX
//!
//! \return None.
//
//*****************************************************************************

void Audio_Start()
{
    MAP_I2SEnable(I2S_BASE,I2S_MODE_TX_RX_SYNC);

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
