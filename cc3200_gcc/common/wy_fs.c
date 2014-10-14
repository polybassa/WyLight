/**
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

#include "wy_fs.h"
#include "uart_if.h"
#include "string.h"

#define UART_PRINT 	Report
#define SUCCESS 	0

#ifndef ERROR
#define ERROR -1
#endif

typedef enum {
	EMPTY, INVALID, VALID
} FileStatus;

typedef struct {
	FileStatus Status;
	unsigned char Name[MAX_FILENAME_LEN];
} File;

static unsigned int computeAdress(unsigned char *pFileName) {
	unsigned int sum = 0;
	while (*pFileName) {
		sum += *pFileName++;
	}
	return sum % MAX_NUM_FILES;
}

static inline unsigned int incAdress(unsigned int adress) {
	return ++adress % MAX_NUM_FILES;
}

static long openFileSystem(void) {
	const unsigned char FILESYSTEM_NAME[] = "filesystem";
	//open filesystem
	long fileHandle;
	if (sl_FsOpen((unsigned char*) FILESYSTEM_NAME, FS_MODE_OPEN_WRITE, 0, &fileHandle)) {
		// File Doesn't exit create a new file
		if (sl_FsOpen((unsigned char*) FILESYSTEM_NAME,
				FS_MODE_OPEN_CREATE(MAX_NUM_FILES * sizeof(File),
						_FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ
								| _FS_FILE_OPEN_FLAG_VENDOR), 0, &fileHandle)) {
			sl_FsDel((unsigned char*) FILESYSTEM_NAME, 0);
			UART_PRINT("Error during creating the destination file\r\n");
			return SL_FS_ERR_ALLOC;
		}
		UART_PRINT("File %s created\r\n", FILESYSTEM_NAME);
		unsigned char emptyData[MAX_NUM_FILES * sizeof(File)];
		memset(emptyData, 0, sizeof(emptyData));
		if(MAX_NUM_FILES * sizeof(File) != sl_FsWrite(fileHandle, 0, emptyData, MAX_NUM_FILES * sizeof(File))) {
			sl_FsClose(fileHandle, 0, 0, 0);
			UART_PRINT("Error setting up filesystem\r\n");
			return ERROR;
		}
	}
	return fileHandle;
}

static long addFileNameToFilesystem(unsigned char *pFileName) {
	const long hdl = openFileSystem();
	if (hdl < 0) return hdl;

	unsigned int adress = computeAdress(pFileName);
	long retVal = ERROR;
	File tempFile;
	printf("Add Filename to Filesystem\r\n");
	for (;;) {
		const size_t offset = adress * sizeof(File);
		if (sizeof(File) != sl_FsRead(hdl, offset, (unsigned char *) &tempFile, sizeof(File))) goto close_and_return;

		if (tempFile.Status == EMPTY || tempFile.Status == INVALID) {
			tempFile.Status = VALID;

			size_t fileNameLen = strlen((const char *) pFileName) + 1; // +1 for termination null
			if (fileNameLen > MAX_FILENAME_LEN) fileNameLen = MAX_FILENAME_LEN;

			memcpy(&tempFile.Name[0], pFileName, fileNameLen);
			if(sizeof(File) == sl_FsWrite(hdl, offset, (unsigned char *) &tempFile, sizeof(File)))
				retVal = SUCCESS;
			 else
				 retVal = ERROR;
			goto close_and_return;
		} else if (0 == memcmp(tempFile.Name, pFileName, strlen((const char *) pFileName))) {
			printf("Compare: %s == %s\r\n", &tempFile.Name[0], pFileName);
			retVal = SUCCESS; // FileName already exists
			goto close_and_return;
		} else {
			adress = incAdress(adress);
		}
	}

	close_and_return: sl_FsClose(hdl, 0, 0, 0);
	return retVal;
}

static long removeFileNameFromFilesystem(unsigned char *pFileName) {
	const long hdl = openFileSystem();
	if (hdl < 0) return hdl; // contains ERRORCODE

	unsigned int adress = computeAdress(pFileName);
	long retVal = ERROR;
	File tempFile;
	printf("Remove Filename from Filesystem\r\n");
	for (;;) {
		const size_t offset = adress * sizeof(File);
		if (sizeof(File) != sl_FsRead(hdl, offset, (unsigned char *) &tempFile, sizeof(File))) goto close_and_return;

		if (tempFile.Status == EMPTY) {
			retVal = SUCCESS;
			goto close_and_return;
		} else if (0 == memcmp(&tempFile.Name[0], pFileName, strlen((const char *) pFileName))) {
			printf("Compare: %s == %s\r\n", &tempFile.Name[0], pFileName);
			tempFile.Status = INVALID;
			if(sizeof(File) == sl_FsWrite(hdl, offset, (unsigned char *) &tempFile, sizeof(File)))
				retVal = SUCCESS;
			else
				retVal = ERROR;
			goto close_and_return;
		} else {
			adress = incAdress(adress);
		}
	}

	close_and_return: sl_FsClose(hdl, 0, 0, 0);
	return retVal;
}

long wy_FsCreateIfNotExists(unsigned char *pFileName, unsigned long maxSize, unsigned long accessFlags,
		unsigned long *pToken, long *pFileHandle) {
	if (addFileNameToFilesystem(pFileName)) {
		UART_PRINT("Failure in addFileNameToFilesystem\r\n");
		return ERROR;
	}
	return sl_FsOpen(pFileName, FS_MODE_OPEN_CREATE(maxSize, accessFlags), pToken, pFileHandle);
}

inline long wy_FsOpen(unsigned char *pFileName, unsigned long AccessModeAndMaxSize, unsigned long *pToken,
		long *pFileHandle) {
	return sl_FsOpen(pFileName, AccessModeAndMaxSize, pToken, pFileHandle);
}

inline int wy_FsClose(long FileHdl, unsigned char* pCeritificateFileName, unsigned char* pSignature,
		unsigned long SignatureLen) {
	return sl_FsClose(FileHdl, pCeritificateFileName, pSignature, SignatureLen);
}

inline long wy_FsRead(long FileHdl, unsigned long Offset, unsigned char* pData, unsigned long Len) {
	return sl_FsRead(FileHdl, Offset, pData, Len);
}

inline long wy_FsWrite(long FileHdl, unsigned long Offset, unsigned char* pData, unsigned long Len) {
	return sl_FsWrite(FileHdl, Offset, pData, Len);
}

inline int wy_FsGetInfo(unsigned char *pFileName, unsigned long Token, SlFsFileInfo_t* pFsFileInfo) {
	return sl_FsGetInfo(pFileName, Token, pFsFileInfo);
}

int wy_FsDel(unsigned char *pFileName, unsigned long Token) {
	if (removeFileNameFromFilesystem(pFileName)) {
		UART_PRINT("Failure in removeFileNameFromFilesystem\r\n");
		return ERROR;
	}
	return sl_FsDel(pFileName, Token);
}
