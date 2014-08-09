//*****************************************************************************
// main.c
//
// Reference code to demonstrate Audio Streaming and Playback.
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
// Application Name     -   Wifi Audio Application
// Application Overview -   This is a sample application demonstrating 
//                          Bi-directional audio transfer.Audio is streamed
//                          from one LP and rendered on another LP over wifi. 
//
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_Wifi_Audio_Application
// or
// doc\examples\CC32xx Wifi Audio Application.pdf
//
//*****************************************************************************

//****************************************************************************
//
//! \addtogroup wifi_audio_app
//! @{
//
//****************************************************************************
#include <stdlib.h>
#include <string.h>

//SimpleLink includes
#include "simplelink.h"

// free-rtos/ ti_rtos includes
#include "osi.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

// Hardware & DriverLib library includes.
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "uart_if.h"
#include "pinmux.h"
#include "network.h"
#include "circ_buff.h"
#include "i2c_if.h"
#include "mcasp_if.h"
#include "i2s.h"
#include "ti_codec.h"
#include "udma_if.h"
#include "gpio.h"
#include "gpio_if.h"
#include "udma.h"
#include "pcm_handler.h"
#include "control.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"
#include "utils.h"




//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************
#define OSI_STACK_SIZE          1024
//******************************************************************************
//			    GLOBAL VARIABLES
//******************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

unsigned int clientIP;  
tCircularBuffer *pTxBuffer;
tCircularBuffer *pRxBuffer;
tUDPSocket g_UdpSock;
unsigned long g_ulMcASPStatus = 0;
unsigned long g_ulRxCounter = 0;
unsigned long g_ulTxCounter = 0;
unsigned long g_ulZeroCounter = 0;
unsigned long g_ulValue = 0;
int iCounter,i = 0;
OsiTaskHandle g_SpeakerTask = NULL ;
OsiTaskHandle g_MicTask = NULL ;

//******************************************************************************
//			        FUNCTION DECLARATIONS
//******************************************************************************
extern void Speaker( void *pvParameters );
extern void Microphone( void *pvParameters );
extern void Network( void *pvParameters );
//*****************************************************************************
//
//! Application defined hook (or callback) function - the tick hook.
//! The tick interrupt can optionally call this
//!
//! \param  none
//! 
//! \return none
//!
//*****************************************************************************
void
vApplicationTickHook( void )
{
}

//*****************************************************************************
//
//! Application defined hook (or callback) function - assert
//!
//! \param  none
//! 
//! \return none
//!
//*****************************************************************************
void
vAssertCalled( const char *pcFile, unsigned long ulLine )
{
    while(1)
    {

    }
}

//*****************************************************************************
//
//! Application defined idle task hook
//! 
//! \param  none
//! 
//! \return none
//!
//*****************************************************************************
void
vApplicationIdleHook( void )
{

}

//*****************************************************************************
//
//! Application provided stack overflow hook function.
//!
//! \param  handle of the offending task
//! \param  name  of the offending task
//! 
//! \return none
//!
//*****************************************************************************
void
vApplicationStackOverflowHook( OsiTaskHandle *pxTask, signed char *pcTaskName)
{
    ( void ) pxTask;
    ( void ) pcTaskName;

    for( ;; );
}

void vApplicationMallocFailedHook()
{
    while(1)
  {
    // Infinite loop;
  }
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
void
BoardInit(void)
{
    /* In case of TI-RTOS vector table is initialize by OS itself */
    #ifndef USE_TIRTOS
    //
    // Set vector table base
    //
    #if defined(ccs)
        MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
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
//******************************************************************************
//							MAIN FUNCTION
//******************************************************************************
void main()
{   

    BoardInit();
    
    //
    // Pinmux Configuration
    //
    PinMuxConfig();
	
    //
    // Initialising the UART terminal
    //
    InitTerm();

    //
    // Create RX and TX Buffer
    //
    pTxBuffer = CreateCircularBuffer(TX_BUFFER_SIZE);
    pRxBuffer = CreateCircularBuffer(RX_BUFFER_SIZE);
    
    //
    // Initialising the I2C Interface
    //    
    I2C_IF_Open(1);

    //
    // Configure Audio Codec
    //     
    ConfigureAudioCodec(CODEC_I2S_WORD_LEN_24);
       
    GPIO_IF_LedConfigure(LED2|LED3);

	GPIO_IF_LedOff(MCU_RED_LED_GPIO);
	GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);	
    
    //
    // Configure PIN_01 for GPIOOutput
    //
    MAP_PinTypeGPIO(PIN_01, PIN_MODE_0, false);
    MAP_GPIODirModeSet(GPIOA1_BASE, 0x4, GPIO_DIR_MODE_OUT);
    
    //
    // Configure PIN_02 for GPIOOutput
    //
    MAP_PinTypeGPIO(PIN_02, PIN_MODE_0, false);
    MAP_GPIODirModeSet(GPIOA1_BASE, 0x8, GPIO_DIR_MODE_OUT);
    
    
    //Turning off Green,Orange LED after i2c writes completed - First Time
    GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);
    GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
    
    //
    // Initialize the Audio(I2S) Module
    //    
    AudioCapturerInit();
  
    //
    // Initialize the DMA Module
    //    
    UDMAInit();
    UDMAChannelSelect(UDMA_CH4_I2S_RX, NULL);
    UDMAChannelSelect(UDMA_CH5_I2S_TX, NULL);
   
    //
    // Setup the DMA Mode
    //     
    SetupPingPongDMATransferTx();
    SetupPingPongDMATransferRx();
    
    //
    // Setup the Audio In/Out
    //     
    AudioCapturerSetupDMAMode(DMAPingPongCompleteAppCB_opt, CB_EVENT_CONFIG_SZ);
    AudioCaptureRendererConfigure();
    
    // 
    // Start Audio Tx/Rx
    //     
    Audio_Start();
    
    //
    // Start the simplelink thread
    //
    VStartSimpleLinkSpawnTask(9);  

    //
    // Start the Network Task
    //    
    osi_TaskCreate( Network, (signed char*)"NetworkTask",OSI_STACK_SIZE, NULL, 1, NULL );

    //
    // Start the Control Task
    //     
    ControlTaskCreate();
    
    //
    // Start the Microphone Task
    //       
    osi_TaskCreate( Microphone,(signed char*)"MicroPhone", OSI_STACK_SIZE, NULL, 1, &g_MicTask );

    //
    // Start the Speaker Task
    //
    osi_TaskCreate( Speaker, (signed char*)"Speaker",OSI_STACK_SIZE, NULL, 1, &g_SpeakerTask ); 
    
    //
    // Start the task scheduler
    //
    osi_start();      
}
    
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
