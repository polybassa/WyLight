//*****************************************************************************
// main.c
//
// demonstrates FreeRTOS features on cc3200 device
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
//! \addtogroup freertos_demo
//! @{
//
//*****************************************************************************
//*****************************************************************************
//
// main.c - Sample FreeRTOS Application for creating 2 tasks
//
//*****************************************************************************

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "osi.h"

/* Hardware library includes. */
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "interrupt.h"
#include "rom.h"
#include "rom_map.h"
#include "uart.h"
#include "prcm.h"
#include "pin.h"
#include "utils.h"
#include "pinmux.h"
#include "portmacro.h"
#include "uart_if.h"

/*Simple Link inlcudes */
#include "datatypes.h"
#include "simplelink.h"


//*****************************************************************************
//                      MACRO DEFINITIONS
//*****************************************************************************
#define UART_PRINT              Report
#define SPAWN_TASK_PRIORITY		9
#define OSI_STACK_SIZE			512
#define APP_NAME				"FreeRTOS Demo"

/* The queue used to send strings to the task1. */
xQueueHandle xPrintQueue;
//*****************************************************************************
//                      GLOBAL VARIABLES
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

//*****************************************************************************
//                      LOCAL FUNCTION DEFINITIONS
//*****************************************************************************
static void vTestTask1( void *pvParameters );
static void vTestTask2( void *pvParameters );

/*
 * Hook functions that can get called by the kernel.
 */
void vApplicationStackOverflowHook( xTaskHandle *pxTask,
                                   signed portCHAR *pcTaskName );
void vApplicationTickHook( void );
void vAssertCalled( const char *pcFile, unsigned long ulLine );
void vApplicationIdleHook( void );
static void BoardInit();

//******************************************************************************
//
//! First test task
//!
//! \param pvParameters is the parameter passed to the task while creating it.
//!
//!	This Function
//!		1. Receive message from the Queue and display it on the terminal.
//!
//! \return none
//
//******************************************************************************
void vTestTask1( void *pvParameters )
{
   portCHAR *pcMessage;
    for( ;; )
    {
      /* Wait for a message to arrive. */
      xQueueReceive( xPrintQueue, &pcMessage, portMAX_DELAY );

      UART_PRINT("message = ");
      UART_PRINT(pcMessage);
      UART_PRINT("\n\r");
      MAP_UtilsDelay(2000000);
    }
}

//******************************************************************************
//
//! Second test task
//!
//! \param pvParameters is the parameter passed to the task while creating it.
//!
//!	This Function
//!		1. Creates a message and send it to the queue.
//!
//! \return none
//
//******************************************************************************
void vTestTask2( void *pvParameters )
{
   unsigned long ul_2;
   const portCHAR *pcInterruptMessage[4] = {"Welcome","to","CC32xx"
		   ,"development !\n"};
   ul_2 =0;
      
   for( ;; )
     {
       /* Queue a message for the print task to display on the UART CONSOLE. */
      xQueueSend( xPrintQueue, &pcInterruptMessage[ul_2 % 4], portMAX_DELAY );
      ul_2++;
      MAP_UtilsDelay(2000000);
     }
}

//*****************************************************************************
//
//! Application defined hook (or callback) function - the tick hook.
//! The tick interrupt can optionally call this
//! \param  none
//!
//! \return none
//!
//!
//*****************************************************************************
void
vApplicationTickHook( void )
{
}

//*****************************************************************************
//
//! Application for handling the assertion.
//! \param  none
//!
//! \return none
//!
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
//! \param  none
//!
//! \return none
//!
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
//! \param  handle and name of the offending task
//!
//! \return none
//!
//*****************************************************************************
void
vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName )
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
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void
DisplayBanner(char * AppName)
{

    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t	CC3200 %s Application       \n\r", AppName);
    Report("\t\t *************************************************\n\r");
    Report("\n\n\n\r");
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
static void
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

//*****************************************************************************
//
//!  main function handling the freertos_demo.
//!
//! \param  None
//!
//! \return none
//
//*****************************************************************************
int main( void )
{
    //
    // Initialize the board
    //
    BoardInit();

    PinMuxConfig();

    //
    // Initializing the terminal
    //
    InitTerm();

    //
    // Clearing the terminal
    //
    ClearTerm();

    //
    // Diasplay Banner
    //
    DisplayBanner(APP_NAME);

    //
    // Creating a queue for 10 elements.
    //
    xPrintQueue =xQueueCreate( 10, sizeof( unsigned portLONG ) );

    if( xPrintQueue == 0 )
    {
      // Queue was not created and must not be used.
      return 0;
    }
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

    //
    // Create the Queue Receive task
    //
    osi_TaskCreate( vTestTask1, ( signed portCHAR * ) "TASK1",
                OSI_STACK_SIZE, NULL, 1, NULL );

    //
    // Create the Queue Send task
    //
    osi_TaskCreate( vTestTask2, ( signed portCHAR * ) "TASK2",
                OSI_STACK_SIZE,NULL, 1, NULL );

    //
    // Start the task scheduler
    //
    osi_start();

    return 0;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
