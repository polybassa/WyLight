//*****************************************************************************
// main.c
//
// Camera Application 
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
// Application Name     -   Camera Application
// Application Overview -   This is a sample application demonstrating camera on
//                          a CC3200 Launchpad setup. Micron MT9D111 
//                          camera sensor driver is used in this example 
//                          
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_Camera_Application
// or
// doc\examples\CC32xx Camera Application.pdf
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup camera_app
//! @{
//
//*****************************************************************************
#include "datatypes.h"
#include "rom.h"
#include "rom_map.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "simplelink.h"
#include "interrupt.h"
#include "prcm.h"
#include "camera_app.h"
#include "uart_if.h"
#include "udma_if.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
//*****************************************************************************
// Macros
//*****************************************************************************
#define SPAWN_TASK_PRIORITY             9
#define OSI_STACK_SIZE                  1024

//Common interface includes
#include "gpio_if.h"
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "pinmux.h"



#define APPLICATION_NAME        "CAMERA_APPLICATION"
#define APPLICATION_VERSION     "1.0.0"
#define SUCCESS                 0
//*****************************************************************************
//                      GLOBAL VARIABLES
//*****************************************************************************
volatile unsigned char g_CaptureImage = 0;
// Vector table entry
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
//****************************************************************************
//                      FUNCTION DEFINITIONS                          
//****************************************************************************
void CameraService(void *pvParameters);
extern void StartCamera();
extern void CaptureImage(void);

//***************************************************************************** 
// Callback function for FreeRTOS 
//*****************************************************************************
#ifdef USE_FREERTOS
void vApplicationStackOverflowHook( OsiTaskHandle pxTask, signed char *pcTaskName )
{
    while(1)
    {
      // Infinite loop;
    }
}

void vApplicationTickHook( void )
{

}

void vApplicationIdleHook( void )
{
      
}

void vAssertCalled( const char *pcFile, unsigned long ulLine )
{
    while(1)
    {
       
    }
}

void vApplicationMallocFailedHook( void )
{
      
}

#endif

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
                               SlHttpServerResponse_t *pSlHttpServerResponse)
{
    switch (pSlHttpServerEvent->Event)
    {
       case SL_NETAPP_HTTPPOSTTOKENVALUE:
        {
              if (0 == memcmp (pSlHttpServerEvent->
                       EventData.httpPostData.token_name.data, "__SL_P_U.C", 
                    pSlHttpServerEvent->EventData.httpPostData.token_name.len))
              {
                 g_CaptureImage = 1;
              }
        }
          break;
        default:
          break;
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
static void BoardInit(void)
{
// In case of TI-RTOS vector table is initialize by OS itself
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs) || defined(gcc)
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
//!     Camera Service starts here
//!    
//! \param     pvParameters     Parametrs to the task's entry function 
//! \return                     None                         
//
//***************************************************************************** 

void CameraService(void *pvParameters)
{
    StartCamera();  
    osi_TaskDelete(NULL);  
}
//*****************************************************************************
//
//!     Main()
//!     Initializes the system 
//!     Spawns CameraService and other resources
//!    
//!	\param                      None  
//!     \return                     On Sucess, zero is returned                         
//
//*****************************************************************************

int main()
{
  //
  // Initialize Board configurations
  //
  BoardInit();
  
  //
  // Configuring UART
  //
#ifndef NOTERM
    InitTerm();
#endif  //NOTERM

  // 
  // Simplelinkspawntask 
  //
  VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);
  //
  // Spawn 'CameraService' task 
  //
  osi_TaskCreate(CameraService,(signed char *) "CameraService", 
                                             OSI_STACK_SIZE, NULL, 1, NULL);
  
  osi_start();
  return 0;
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
