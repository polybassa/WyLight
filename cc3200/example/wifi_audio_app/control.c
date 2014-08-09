//*****************************************************************************
// control.c
//
// Audio Control APIs
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

//****************************************************************************
//
//! \addtogroup wifi_audio_app
//! @{
//
//****************************************************************************
#include <stdio.h>
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "interrupt.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "gpio_if.h"
#include "osi.h"
#include "control.h"
#include "simplelink.h"
#include "utils.h"
#include "network.h"
//*****************************************************************************
//                      GLOBAL VARIABLES
//*****************************************************************************
P_AUDIO_HANDLER g_pAudioInControlHdl;
P_AUDIO_HANDLER g_pAudioOutControlHdl;

OsiTaskHandle g_AudioControlTask;
OsiMsgQ_t g_ControlMsgQueue;

typedef struct
{
    //Queue_Elem _elem;
    P_OSI_SPAWN_ENTRY pEntry;
    void* pValue;
}tTxMsg;

volatile unsigned char g_ucMicStartFlag = 0;
volatile unsigned char g_ucSpkrStartFlag = 0;
//*****************************************************************************
//
//! MicroPhone Control Routine
//!
//! \param  pValue - pointer to a memory structure that is passed 
//!         to the interrupt handler.
//!
//! \return None
//
//*****************************************************************************
void MicroPhoneControl(void* pValue)
{
    int iCount=0;
    unsigned long ulPin5Val = 1; 
    
    //Check whether GPIO Level is Stable As No Debouncing Circuit in LP
    for(iCount=0;iCount<3;iCount++)
    {
        osi_Sleep(200);
        ulPin5Val = MAP_GPIOPinRead(GPIOA1_BASE,GPIO_PIN_5);
        if(ulPin5Val)
        {
            //False Alarm
            return;
        }
    }
    if (g_ucMicStartFlag ==  0)
    {
        for(iCount = 0; iCount<3; iCount++)
        {
            //Blink LED 3 times to Indicate ON
            GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);
            osi_Sleep(50);
            GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);
            osi_Sleep(50);
        }
         g_ucMicStartFlag = 1;
        
     }
     else
     {
        //Blink LED 3 times to Indicate OFF
        for(iCount = 0; iCount<3; iCount++)
        {
            GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);
            osi_Sleep(50);
            GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);
            osi_Sleep(50);
        }
        g_ucMicStartFlag = 0;
     }
    
     //Enable GPIO Interrupt 
     MAP_GPIOIntClear(GPIOA1_BASE,GPIO_PIN_5);
     MAP_IntPendClear(INT_GPIOA1);
     MAP_IntEnable(INT_GPIOA1);
     MAP_GPIOIntEnable(GPIOA1_BASE,GPIO_PIN_5);

}

//*****************************************************************************
//
//! Speaker Control Routine
//!
//! \param  pValue - pointer to a memory structure that is passed to 
//!         the interrupt handler.
//!
//! \return None
//
//*****************************************************************************

void SpeakerControl(void* pValue)
{
    int iCount=0;
    unsigned long ulPin6Val = 1;
    
    //Check whether GPIO Level is Stable As No Debouncing Circuit in LP
    for(iCount=0;iCount<3;iCount++)
    {
        osi_Sleep(200);
        ulPin6Val = MAP_GPIOPinRead(GPIOA2_BASE,GPIO_PIN_6);
        if(ulPin6Val)
        {
            //False Alarm
            return;
        }
    }
    
    if (g_ucSpkrStartFlag ==  0)
    {
#ifndef MULTICAST   
        //Un Register mDNS Service.
        sl_NetAppMDNSUnRegisterService("CC3200._audio._udp.local",
                    (unsigned char)strlen("CC3200._audio._udp.local"));
        //Registering for the mDNS service.              
        sl_NetAppMDNSRegisterService("CC3200._audio._udp.local",
                   (unsigned char)strlen("CC3200._audio._udp.local"),"multicast",
                   (unsigned char)strlen("multicast"),AUDIO_PORT,1000,0);
#endif 
        
        //Blink LED 3 times to Indicate ON
        for(iCount = 0; iCount<3; iCount++)
        {
            GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
            osi_Sleep(50);
            GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
            osi_Sleep(50);
        }                     
        g_ucSpkrStartFlag = 1;
    }
    else
    {
        //Un Register mDNS Service.
        sl_NetAppMDNSUnRegisterService("CC3200._audio._udp.local",
                     (unsigned char)strlen("CC3200._audio._udp.local"));

        //Blink LED 3 times to Indicate OFF
        for(iCount = 0; iCount<3; iCount++)
        {
            GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
            osi_Sleep(50);
            GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
            osi_Sleep(50);
        }
        g_ucSpkrStartFlag = 0;            
    }
    
    //Enable GPIO Interrupt
    MAP_GPIOIntClear(GPIOA2_BASE,GPIO_PIN_6);
    MAP_IntPendClear(INT_GPIOA2);
    MAP_IntEnable(INT_GPIOA2);
    MAP_GPIOIntEnable(GPIOA2_BASE,GPIO_PIN_6);
}

//*****************************************************************************
//
//! Control for Wifi Audio Player Microphone
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
static void MICStartStopControl()
{    
     tTxMsg sMsg;
     sMsg.pEntry = &MicroPhoneControl;
     sMsg.pValue = NULL;
     osi_MsgQWrite(&g_ControlMsgQueue,&sMsg,OSI_NO_WAIT);     
}             

//*****************************************************************************
//
//! Control for Wifi Audio Player Speaker
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
static void SpeakerStartStopControl()
{  
    tTxMsg sMsg;
    sMsg.pEntry = &SpeakerControl;
    sMsg.pValue = NULL;
    osi_MsgQWrite(&g_ControlMsgQueue,&sMsg,OSI_NO_WAIT);                   
}
       

//*****************************************************************************
//
//! GPIO Interrupt Handler for S3 button
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
static void MICButtonHandler()
{
    unsigned long ulPinState =  GPIOIntStatus(GPIOA1_BASE,1);
    if(ulPinState & GPIO_PIN_5)
    {  
        //Clear and Disable GPIO Interrupt
        MAP_GPIOIntDisable(GPIOA1_BASE,GPIO_PIN_5);
        MAP_GPIOIntClear(GPIOA1_BASE,GPIO_PIN_5);        
        MAP_IntDisable(INT_GPIOA1);
        
        //Call Speaker Handler
        if(g_pAudioInControlHdl)
        {
            g_pAudioInControlHdl();
        }
    }
              
}
//*****************************************************************************
//
//! GPIO Interrupt Handler for S2 button
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
static void SpeakerButtonHandler()
{
    unsigned long ulPinState =  GPIOIntStatus(GPIOA2_BASE,1);
    if(ulPinState & GPIO_PIN_6)
    {
        //Clear and Disable GPIO Interrupt
        MAP_GPIOIntDisable(GPIOA2_BASE,GPIO_PIN_6);
        MAP_GPIOIntClear(GPIOA2_BASE,GPIO_PIN_6);
        MAP_IntDisable(INT_GPIOA2);
        
        //Call Speaker Handler
        if(g_pAudioOutControlHdl)
        {
            g_pAudioOutControlHdl();
        }
    }
}

//*****************************************************************************
//
//!  \brief Initialize Push Button GPIO
//!
//! \param[in] pAudioInControl               GPIO Interrupt Handler
//! \param[in] pAudioOutControl              GPIO Interrupt Handler

//!
//! \return none
//!
//! \brief  Initializes Audio Player Push Button Controls
//
//*****************************************************************************
void InitControl(P_AUDIO_HANDLER pAudioInControl,P_AUDIO_HANDLER pAudioOutControl)
{
    //
    // Set Interrupt Type for GPIO
    // 
    MAP_GPIOIntTypeSet(GPIOA1_BASE,GPIO_PIN_5,GPIO_FALLING_EDGE);
    MAP_GPIOIntTypeSet(GPIOA2_BASE,GPIO_PIN_6,GPIO_FALLING_EDGE);

    //
    // Store Interrupt handlers
    //
    g_pAudioInControlHdl = pAudioInControl;
    g_pAudioOutControlHdl = pAudioOutControl;
    
    //
    // Register Interrupt handler
    //
    osi_InterruptRegister(INT_GPIOA1,(P_OSI_INTR_ENTRY)MICButtonHandler,INT_PRIORITY_LVL_1);
    osi_InterruptRegister(INT_GPIOA2,(P_OSI_INTR_ENTRY)SpeakerButtonHandler,INT_PRIORITY_LVL_1);
    
    //
    // Enable Interrupt
    //
    MAP_GPIOIntClear(GPIOA1_BASE,GPIO_PIN_5);
    MAP_GPIOIntEnable(GPIOA1_BASE,GPIO_INT_PIN_5);
    MAP_GPIOIntClear(GPIOA2_BASE,GPIO_PIN_6);
    MAP_GPIOIntEnable(GPIOA2_BASE,GPIO_INT_PIN_6);

}

//*****************************************************************************
//
//! AudioControlTask Task Routine
//!
//! \param pvParameters     Parameters to the task's entry function
//!
//! \return None
//
//*****************************************************************************
void AudioControlTask(void *pvParameters)
{
    tTxMsg Msg; 
    int ret=0;
	
    for(;;)
    {
        ret = osi_MsgQRead( &g_ControlMsgQueue, &Msg, OSI_WAIT_FOREVER );
        if(OSI_OK==ret)
        {
            Msg.pEntry(Msg.pValue);
        }
    }
}

//*****************************************************************************
//
//! Control Create Routine
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************


void ControlTaskCreate()
{
    InitControl(MICStartStopControl,SpeakerStartStopControl);
    
    osi_MsgQCreate(&g_ControlMsgQueue,"g_ControlMsgQueue",sizeof(tTxMsg),1);
    osi_TaskCreate(AudioControlTask, (signed char*)"AudioControlTask",2048, NULL, 1, &g_AudioControlTask );
}

//*****************************************************************************
//
//! Control Destroy Routine
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************


void ControlTaskDestroy()
{
    osi_MsgQDelete(&g_ControlMsgQueue);
    osi_TaskDelete(&g_AudioControlTask);
    g_ControlMsgQueue = NULL;
    g_AudioControlTask = NULL;
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
