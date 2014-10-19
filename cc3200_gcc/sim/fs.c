/*
 * fs.c - CC31xx/CC32xx Host Driver Implementation
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "fs.h"
#include <stdlib.h>

/*****************************************************************************/
/*  sl_FsOpen */ 
/*****************************************************************************/

unsigned long g_CreateFileSize = 0;
unsigned char g_FileName[127];

unsigned long _GetCreateFsMode(unsigned long maxSizeInBytes,unsigned long accessFlags)
{
	g_CreateFileSize = maxSizeInBytes;
	return _FS_MODE(_FS_MODE_OPEN_WRITE_CREATE_IF_NOT_EXIST,  0, 0, 0);
}


#if _SL_INCLUDE_FUNC(sl_FsOpen)
long sl_FsOpen(const unsigned char *pFileName,unsigned long AccessModeAndMaxSize, unsigned long *pToken,long *pFileHandle)
{
	strcpy((char *)g_FileName, (const char *)pFileName);
	if (g_CreateFileSize) {
		unsigned char* mem = (unsigned char*)malloc(g_CreateFileSize);
		memset(mem, 0, g_CreateFileSize);
		FILE *hdl = fopen((const char *)pFileName, "wb");
		fwrite(mem, 1, g_CreateFileSize, hdl);
		fclose(hdl);
		free(mem);
		g_CreateFileSize = 0;
	}
	
	if (access((const char *)pFileName, F_OK))
		return -1;
	else
		return 0;
}
#endif

/*****************************************************************************/
/* sl_FsClose */ 
/*****************************************************************************/
#if _SL_INCLUDE_FUNC(sl_FsClose)
int sl_FsClose(long FileHdl, unsigned char* pCeritificateFileName,unsigned char* pSignature ,unsigned long SignatureLen)
{
	return 0;
}
#endif


/*****************************************************************************/
/* sl_FsRead */ 
/*****************************************************************************/
#if _SL_INCLUDE_FUNC(sl_FsRead)
long sl_FsRead(long FileHdl, unsigned long Offset, unsigned char* pData, unsigned long Len)
{
	FILE *pHandle = fopen((const char *)g_FileName, "rb");
	if (pHandle == NULL) {
		return -1;
	}
	
	fseek(pHandle, Offset, SEEK_SET);
	size_t retVal = fread(pData, 1, Len, pHandle);
	fclose(pHandle);

    return (long)retVal;
}
#endif

/*****************************************************************************/
/* sl_FsWrite */ 
/*****************************************************************************/
#if _SL_INCLUDE_FUNC(sl_FsWrite)
long sl_FsWrite(long FileHdl, unsigned long Offset, unsigned char* pData, unsigned long Len)
{
	FILE *pHandle = fopen((const char *)g_FileName, "rb");
	if (pHandle == NULL) {
		return -1;
	}
	
	fseek(pHandle, 0, SEEK_END);
	long fsize = ftell(pHandle);
	fseek(pHandle, 0, SEEK_SET);
	
	unsigned char *buffer = malloc(fsize);
	fread(buffer, fsize, 1, pHandle);
	
	memcpy(&buffer[Offset], pData, Len);
	
	pHandle = freopen((const char *)g_FileName, "wb", pHandle);
	long retVal = -1;
	if(1 == fwrite(buffer, fsize, 1, pHandle)) {
		retVal = Len;
	}
	fclose(pHandle);
	
    return retVal;
}
#endif

/*****************************************************************************/
/* sl_FsGetInfo */ 
/*****************************************************************************/

#if _SL_INCLUDE_FUNC(sl_FsGetInfo)
int sl_FsGetInfo(unsigned char *pFileName,unsigned long Token,SlFsFileInfo_t* pFsFileInfo)
{
	
	FILE * pHandle = fopen((const char *)pFileName, "rb");
	if (pHandle == NULL) {
		return -1;
	}
	fseek(pHandle, 0, SEEK_END);
	
	long size = ftell(pHandle);
	fclose(pHandle);
	memset(pFsFileInfo, 0, sizeof(SlFsFileInfo_t));

    pFsFileInfo->FileLen      = size;
    pFsFileInfo->AllocatedLen = size;
	return 0;
}
#endif

/*****************************************************************************/
/* sl_FsDel */ 
/*****************************************************************************/
#if _SL_INCLUDE_FUNC(sl_FsDel)
int sl_FsDel(unsigned char *pFileName,unsigned long Token)
{
	int status = remove((const char *)pFileName);
	return  status;
}
#endif
