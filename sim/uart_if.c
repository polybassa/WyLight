//*****************************************************************************
// uart_if.c
//
// uart interface file: Prototypes and Macros for UARTLogger
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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "hw_types.h"
#include "uart_if.h"

//*****************************************************************************
// Global variable indicating command is present
//*****************************************************************************
static unsigned long __Errorlog;

//*****************************************************************************
// Global variable indicating input length
//*****************************************************************************
unsigned int ilen=1;

//*****************************************************************************
//
//! Initialization
//!
//! This function
//!		1. Configures the UART to be used.
//!
//! \return none
//
//*****************************************************************************
void 
InitTerm()
{
  __Errorlog = 0;
}

//*****************************************************************************
//
//!	Outputs a character string to the console
//!
//! \param str is the pointer to the string to be printed
//!
//! This function
//!		1. prints the input string character by character on to the console.
//!
//! \return none
//
//*****************************************************************************
void 
Message(const char *str)
{
#ifndef NOTERM
    if(str != NULL)
    {
        while(*str!='\0')
        {
			printf("%c", *str++);
           // MAP_UARTCharPut(CONSOLE,*str++);
        }
    }
#endif
}

//*****************************************************************************
//
//!	Clear the console window
//!
//! This function
//!		1. clears the console window.
//!
//! \return none
//
//*****************************************************************************
void 
ClearTerm()
{
    Message("\33[2J\r");
}

//*****************************************************************************
//
//! Error Function
//!
//! \param 
//!
//! \return none
//! 
//*****************************************************************************
void 
Error(char *pcFormat, ...)
{
#ifndef NOTERM
    char  cBuf[256];
    va_list list;
    va_start(list,pcFormat);
    vsnprintf(cBuf,256,pcFormat,list);
    Message(cBuf);
#endif
    __Errorlog++;
}

//*****************************************************************************
//
//!	prints the formatted string on to the console
//!
//! \param format is a pointer to the character string specifying the format in
//!		   the following arguments need to be interpreted.
//! \param [variable number of] arguments according to the format in the first
//!         parameters
//! This function
//!		1. prints the formatted error statement.
//!
//! \return count of characters printed
//
//*****************************************************************************
int Report(const char *pcFormat, ...)
{
 int iRet = 0;
#ifndef NOTERM
  char pcBuff[256];
  int iSize = sizeof(pcBuff);
 
  va_list list;
  va_start(list,pcFormat);
  iRet = vsnprintf(pcBuff,iSize,pcFormat,list);
  va_end(list);

  if( iRet < 0 || iRet >= iSize) {
	  Message("Message to long\r\n");
	  return -1;
  }
  Message(pcBuff);
#endif
  return iRet;
}
