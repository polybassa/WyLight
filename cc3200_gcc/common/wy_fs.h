/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.
 
 This file is part of Wifly_Light.
 
 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef __WY_FS_H__
#define	__WY_FS_H__

#include "fs.h"

#define BLOCKSIZE 4096
#define MAX_NUM_FILES 32
#define FLASHSIZE 0xfffff		// 1MB
#define MAX_FILENAME_LEN 63

/*!
    \brief open file for read or write from/to storage device
    
    \param[in]      pFileName                  File Name buffer pointer  
    \param[in]      AccessModeAndMaxSize       Options: As described below
    \param[in]      pToken                     input Token for read, output Token for write
    \param[out]     pFileHandle      Pointing on the file and used for read and write commands to the file     
     
     AccessModeAndMaxSize possible input                                                                        \n
     FS_MODE_OPEN_READ                                        - Read a file                                                                  \n
	 FS_MODE_OPEN_WRITE                                       - Open for write for an existing file                                          \n
     FS_MODE_OPEN_CREATE(maxSizeInBytes,accessModeFlags)      - Open for creating a new file. Max file size is defined in bytes.             \n
                                                                For optimal FS size, use max size in 4K-512 bytes (e.g. 3584,7680)           \n
                                                                Several access modes can be combined together from SlFileOpenFlags_e  

    \return         On success, zero is returned. On error, negative is returned    
    
    \sa             sl_FsRead sl_FsWrite sl_FsClose       
    \note           belongs to \ref basic_api       
    \warning        This API is going to be modified on the next release
	\par            Example:
	\code
    wy_FsOpen("FileName.html",FS_MODE_OPEN_CREATE(3584,_FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE) ,NULL, &FileHandle);
	
	wy_FsOpen("FileName.html",FS_MODE_OPEN_READ,NULL, &FileHandle);
	\endcode
*/
long wy_FsOpen(unsigned char *pFileName,unsigned long AccessModeAndMaxSize, unsigned long *pToken,long *pFileHandle);
/*!
    \brief close file in storage device
    
    \param[in]      FileHdl   Pointer to the file (assigned from sl_FsOpen) 
    \param[in]      pCeritificateFileName, NULL terminated
    \param[in]      pSignature 
    \param[in]      SignatureLen
    \return         On success, zero is returned. On error, negative is returned    
    
    \sa             sl_FsRead sl_FsWrite sl_FsOpen        
    \note           belongs to \ref basic_api       
    \warning
	\par            Example:
	\code			
	wy_FsClose(FileHandle,0,0,0);
	\endcode
*/

int wy_FsClose(long FileHdl, unsigned char* pCeritificateFileName,unsigned char* pSignature ,unsigned long SignatureLen);


/*!
    \brief read block of data from a file in storage device
    
    \param[in]      FileHdl Pointer to the file (assigned from sl_FsOpen)    
    \param[in]      Offset  Offset to specific read block
    \param[out]     pData   Pointer for the received data
    \param[in]      Len     Length of the received data
     
    \return         On success, returns the number of read bytes. On error, negative number is returned    
    
    \sa             sl_FsClose sl_FsWrite sl_FsOpen        
    \note           belongs to \ref basic_api       
    \warning     
	\par            Example:
	\code	
	Status = wy_FsRead(FileHandle, 0, &readBuff[0], readSize);
	\endcode
*/

long wy_FsRead(long FileHdl, unsigned long Offset, unsigned char* pData, unsigned long Len);


/*!
    \brief write block of data to a file in storage device
    
    \param[in]      FileHdl  Pointer to the file (assigned from sl_FsOpen)  
    \param[in]      Offset   Offset to specific block to be written
    \param[in]      pData    Pointer the transmitted data to the storage device
    \param[in]      Len      Length of the transmitted data
     
    \return         On success, returns the number of written bytes. On error, negative number is returned
    
    \sa                     
    \note           belongs to \ref basic_api       
    \warning     
	\par            Example:
	\code	
	Status = wy_FsWrite(FileHandle, 0, &buff[0], readSize);
	\endcode
*/

long wy_FsWrite(long FileHdl, unsigned long Offset, unsigned char* pData, unsigned long Len);


/*!
    \brief get info on a file
    
    \param[in]      pFileName    File's name
    \param[in]      Token        File's token
    \param[out]     pFsFileInfo Returns the File's Information: flags,file size, allocated size and Tokens 
     
    \return         On success, zero is returned. On error, negative is returned    
    
    \sa             sl_FsOpen        
    \note           belongs to \ref basic_api       
    \warning        
	\par            Example:
	\code	
	Status = wy_FsGetInfo("FileName.html",Token,&FsFileInfo);
	\endcode
*/

int wy_FsGetInfo(unsigned char *pFileName,unsigned long Token,SlFsFileInfo_t* pFsFileInfo);

/*!
    \brief delete specific file from a storage or all files from a storage (format)
    
    \param[in]      pFileName    File's Name 
    \param[in]      Token        File's token     
    \return         On success, zero is returned. On error, negative is returned    
    
    \sa                     
    \note           belongs to \ref basic_api       
    \warning     
	\par            Example:
	\code	
	Status = wy_FsDel("FileName.html",Token);
	\endcode
*/

int wy_FsDel(unsigned char *pFileName,unsigned long Token);

/*!
 \brief delete all files from a storage (format)
 
 \return         On success, zero is returned. On error, negative is returned
 
 \sa
 \note           belongs to \ref basic_api
 \warning
	\par            Example:
	\code
	Status = wy_FsFormat();
	\endcode
 */

int wy_FsFormat(void);

#endif /*  __FS_H__ */
