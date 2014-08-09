//*****************************************************************************
// main.c
//
// demonstrates aes features on CC3200 device
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
//! \addtogroup aes_demo
//! @{
//
//*****************************************************************************

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hw_aes.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_common_reg.h"
#include "hw_types.h"
#include "rom.h"
#include "rom_map.h"
#include "aes.h"
#include "interrupt.h"
#include "prcm.h"
#include "uart_if.h"
#include "uart.h"
#include "pinmux.h"
#include "utils.h"
#include "pin.h"
#include "aes_vector.h"
#include "aes_userinput.h"
#include "uart_if.h"

//
// Undefine UserInput for taking default values from DESVector.h
//
#define USER_INPUT
#define UART_PRINT 	Report
#define FOREVER		1
#define APP_NAME	"AES Reference"

// Interrupt Flags
//
static volatile bool g_bContextInIntFlag;
static volatile bool g_bDataInIntFlag;
static volatile bool g_bContextOutIntFlag;
static volatile bool g_bDataOutIntFlag;
//*****************************************************************************
//                      GLOBAL VARIABLES
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

/****************************************************************************/
/*                      LOCAL FUNCTION PROTOTYPES                           */
/****************************************************************************/
void AESCrypt(unsigned int uiConfig,unsigned int uiKeySize,unsigned int *puiKey1,unsigned int *puiData,
		unsigned int *puiResult,unsigned int uiDataLength,unsigned int *uiIV);

void AESIntHandler(void);

unsigned int * LoadDefaultValues(unsigned int ui32Config,unsigned int *uiConfig,unsigned int *uiKeySize,
		unsigned int **uiIV,unsigned int **puiKey1,unsigned int *uiDataLength,unsigned int **puiResult);

static void BoardInit(void);


//*****************************************************************************
//
//! AES Crypt Function
//!
//! This function Configures Key,Mode and carries out Encryption/Decryption in 
//!                                                  CPU mode
//! \param uiConfig - Configuration Value
//! \param uiKeySize - KeySize used.(128,192 or 256 bit)
//! \param puiKey1 - Key Used
//! \param puiData - Input Data
//! \param puiResult - Resultant Output Data
//! \param uiDataLength - Input Data Length
//! \param uiIV - Initialization Vector used
//!
//! \return none
//
//*****************************************************************************
void 
AESCrypt(unsigned int uiConfig,unsigned int uiKeySize,unsigned int *puiKey1,unsigned int *puiData,
		unsigned int *puiResult,unsigned int uiDataLength,unsigned int *uiIV)
{
    //
    // Step1: Reset the Module
    // Step2: Enable Interrupts
    // Step3: Wait for Context Ready Inteerupt
    // Step4: Set the Configuration Parameters (Direction,AES Mode and Key Size)
    // Step5: Set the Initialization Vector
    // Step6: Write Key
    // Step7: Start the Crypt Process
    //
    MAP_PRCMPeripheralReset(PRCM_DTHE);
    
    //
    // Clear the flags.
    //
    g_bContextInIntFlag = false;
    g_bDataInIntFlag = false;
    g_bContextOutIntFlag = false;
    g_bDataOutIntFlag = false;
    
    //
    // Enable all interrupts.
    //
    MAP_AESIntEnable(AES_BASE, AES_INT_CONTEXT_IN |
	                     AES_INT_CONTEXT_OUT | AES_INT_DATA_IN |
	                     AES_INT_DATA_OUT);
    
    //
    // Wait for the context in flag, the flag will be set in the Interrupt handler.
    //
    while(!g_bContextInIntFlag)
    {
    }
    
    //
    // Configure the AES module with direction (encryption or decryption) and the key size.
    //
    MAP_AESConfigSet(AES_BASE, uiConfig |uiKeySize);
    
    //
    // Write the initial value registers if needed, depends on the mode.
    //
    if(((uiConfig & AES_CFG_MODE_M) == AES_CFG_MODE_CBC) ||
	           ((uiConfig & AES_CFG_MODE_M) == AES_CFG_MODE_CFB) ||
	           ((uiConfig & AES_CFG_MODE_M) == AES_CFG_MODE_CTR) ||
	           ((uiConfig & AES_CFG_MODE_M) == AES_CFG_MODE_ICM) 
	           )
    {
	MAP_AESIVSet(AES_BASE, (unsigned char *)uiIV);
    }
    
    //
    // Write key1.
    //
    MAP_AESKey1Set(AES_BASE,(unsigned char *)puiKey1,uiKeySize);
    
    //
    // Start Crypt Process
    //
    MAP_AESDataProcess(AES_BASE, (unsigned char *)puiData, (unsigned char *)puiResult, uiDataLength);
}

//*****************************************************************************
//
//! AES Interrupt Handler
//!
//! This function
//!		1. Handles Interrupts based on Interrupt Sources. Set Flags
//! \param none
//!
//! \return none
//
//*****************************************************************************

void
AESIntHandler(void)
{
    uint32_t uiIntStatus;
    
    //
    // Read the AES masked interrupt status.
    //
    uiIntStatus = MAP_AESIntStatus(AES_BASE, true);
    
    //
    // Set Different flags depending on the interrupt source.
    //
    if(uiIntStatus & AES_INT_CONTEXT_IN)
    {
        MAP_AESIntDisable(AES_BASE, AES_INT_CONTEXT_IN);
        g_bContextInIntFlag = true;
    }
    if(uiIntStatus & AES_INT_DATA_IN)
    {
        MAP_AESIntDisable(AES_BASE, AES_INT_DATA_IN);
        g_bDataInIntFlag = true;
    }
    if(uiIntStatus & AES_INT_CONTEXT_OUT)
    {
        MAP_AESIntDisable(AES_BASE, AES_INT_CONTEXT_OUT);
        g_bContextOutIntFlag = true;
    }
    if(uiIntStatus & AES_INT_DATA_OUT)
    {
        MAP_AESIntDisable(AES_BASE, AES_INT_DATA_OUT);
        g_bDataOutIntFlag = true;
    }
    

}

//*****************************************************************************
//
//! Function - Read Predefined Values. Populate Key,PlainText,Mode etc from 
//!   pre-defined Vectors
//!
//! \param ui32Config - Configuration Value (Direction | Mode |KeySize)
//! \out uiConfig - Configuration value 
//! \out uiKeySize - Key Size used
//! \out uiIV - Initialization Vector
//! \out puiKey1 - Key Used
//! \out uiDataLength - DataLength Used
//! \out puiResult - Result
//!
//! \return Returns /\e true on success or \e false on failure.
//
//*****************************************************************************
unsigned int *
LoadDefaultValues(unsigned int ui32Config,unsigned int *uiConfig,unsigned int *uiKeySize,
		unsigned int **uiIV,unsigned int **puiKey1,unsigned int *uiDataLength,unsigned int **puiResult)
       
{
    unsigned int *uiData;
    
    //
    // Populate all the out parameters from pre-defined vector
    //
    *uiConfig=ui32Config;
    
    //
    // Read Key and Key size
    //
    *puiKey1=psAESCBCTestVectors.pui32Key1;
    *uiKeySize=psAESCBCTestVectors.ui32KeySize;
    
    //
    // Read Initialization Vector
    //
    *uiIV=&psAESCBCTestVectors.pui32IV[0];
    
    //
    // Read Data Length and allocate Result and Data variables accordingly
    //
    *uiDataLength=psAESCBCTestVectors.ui32DataLength;
    *puiResult=(unsigned int*)malloc(*uiDataLength);
    if(*puiResult != NULL)
    {
        memset(*puiResult,0,*uiDataLength);
    }
    else
    {
        //Failed to allocate memory
        UART_PRINT("Failed to allocate memory");
        return 0;
    }
    uiData=(unsigned int*)malloc(*uiDataLength);
    if(uiData != NULL)
    {
        memset(uiData,0,*uiDataLength);
    }
    else
    {
        //Failed to allocate memory
        UART_PRINT("Failed to allocate memory");
        return 0;
    }
    //
    // Copy Plain Text or Cipher Text into the variable Data
    //
    if(ui32Config & AES_CFG_DIR_ENCRYPT)
        memcpy(uiData,psAESCBCTestVectors.pui32PlainText,*uiDataLength);
    else
         memcpy(uiData,psAESCBCTestVectors.pui32CipherText,*uiDataLength);
    
    return uiData; 
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
    Report("\t\t 	  CC3200 %s Application       \n\r", AppName);
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
//! main - calls Crypt function after populating either from pre- defined vector 
//! or from User
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void 
main()
{
    unsigned int uiConfig,uiKeySize,*puiKey1,*puiData,*puiResult,
    uiDataLength,uiIV[4]={0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c};

#ifdef USER_INPUT
    unsigned int ui32CharCount;
    unsigned char* pui8Result;
#else
    unsigned int *puiIV;
    puiIV=&uiIV[0];

#endif
    BoardInit();
    
    //
    // Configuring UART for Receiving input and displaying output
    // 1. PinMux setting
    // 2. Initialize UART
    // 3. Displaying Banner
    //
    PinMuxConfig();
    InitTerm();
    DisplayBanner(APP_NAME);

    //
    // Enable AES Module
    //
    MAP_PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
    
    //
    // Enable AES interrupts.
    //
    MAP_AESIntRegister(AES_BASE, AESIntHandler);
#ifdef USER_INPUT
    while(FOREVER)
    {
        //
        // Read values either from User or from Vector based on macro USER_INPUT defined or not
        //

        //
        // Read the values from the user over uart and Populate the variables
        //
        puiData = ReadFromUser(&uiConfig,&uiKeySize,&puiKey1,&uiDataLength,&puiResult);
        if(puiData==NULL)
        {
            UART_PRINT("\n\rInvalid Input. Please try again. \n\r");
            continue;
        }
#else
        //
        // Load Default values
        //
        puiData = LoadDefaultValues(AES_CFG_DIR_ENCRYPT | AES_CFG_MODE_CBC ,&uiConfig,&uiKeySize,&puiIV,&puiKey1,&uiDataLength,&puiResult);
#endif

        //
        // Carry out Encryption
        //
        UART_PRINT("\n\r Encryption in progress....");
        AESCrypt(uiConfig,uiKeySize,puiKey1,puiData,puiResult,uiDataLength,uiIV);
        UART_PRINT("\n\r Encryption done, cipher text created");
        
        //
        // Copy Result into Data Vector to continue with Decryption. and change config value
        //
        memcpy(puiData,puiResult,uiDataLength);
        uiConfig &= ~(1 << 2);
        //
        // Carry out Decryption
        //
        UART_PRINT("\n\r\n\r Decryption in progress....");
        AESCrypt(uiConfig,uiKeySize,puiKey1,puiData,puiResult,uiDataLength,uiIV);
        UART_PRINT("\n\r Decryption done");
        
        //
        // Display/Verify Result
        //

    #ifdef USER_INPUT
        //
        // Display Plain Text
        //
        UART_PRINT("\n\r Text after decryption ");
        pui8Result = (unsigned char *)puiResult;
        for(ui32CharCount=0;ui32CharCount<uiDataLength;ui32CharCount++)
        {
            UART_PRINT("%c",*(pui8Result+ui32CharCount));
        }
        UART_PRINT("\n\r");
    }
    #else
        //
        // Compare Cipher Text and Plain Text with the expected values from predefined vector
        //
        if(memcmp(puiData,psAESCBCTestVectors.pui32CipherText,psAESCBCTestVectors.ui32DataLength)==0)
        {
            UART_PRINT("\n\r\n\r Encryption verification Successful");
        }
        else
        {
            UART_PRINT("\n\r\n\r Error in Encryption");
        }

        if(memcmp(puiResult,psAESCBCTestVectors.pui32PlainText,psAESCBCTestVectors.ui32DataLength)==0)
        {
            UART_PRINT("\n\r Decryption verification Successful");
        }
        else
        {
            UART_PRINT("\n\r\n\r Error in Decryption");
        }
        while(FOREVER);
    #endif

}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
