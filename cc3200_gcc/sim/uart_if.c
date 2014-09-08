/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.
 
 This file is part of WyLight.
 
 WyLight is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 WyLight is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "uart_if.h"

void 
InitTerm()
{

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
Message(char *str)
{
#ifndef NOTERM
	printf("%s", str);
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
}

//*****************************************************************************
//
//! Get the Command string from UART
//!
//! \param  pucBuffer is the command store to which command will be populated
//! \param  ucBufLen is the length of buffer store available
//!
//! \return Length of the bytes received. -1 if buffer length exceeded.
//! 
//*****************************************************************************
int
GetCmd(char *pcBuffer, unsigned int uiBufLen)
{
	if (NULL != fgets(pcBuffer, uiBufLen, stdin)) {
		return strlen(pcBuffer);
	} else {
		return -1;
	}
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
int Report(char *pcFormat, ...)
{
 int iRet = 0;
#ifndef NOTERM

  char *pcBuff, *pcTemp;
  int iSize = 256;
 
  va_list list;
  pcBuff = (char*)malloc(iSize);
  if(pcBuff == NULL)
  {
	  return -1;
  }
  while(1)
  {
      va_start(list,pcFormat);
      iRet = vsnprintf(pcBuff,iSize,pcFormat,list);
      va_end(list);
      if(iRet > -1 && iRet < iSize)
      {
          break;
      }
      else
      {
          iSize*=2;
          if((pcTemp=realloc(pcBuff,iSize))==NULL)
          { 
              Message("Could not reallocate memory\n\r");
              iRet = -1;
              break;
          }
          else
          {
              pcBuff=pcTemp;
          }
          
      }
  }
  Message(pcBuff);
  free(pcBuff);
  
#endif
  return iRet;
}
