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

typedef enum {
	EMPTY,
	INVALID,
	VALID
}FileNameStatus;

typedef struct {
	FileNameStatus Status;
	unsigned char Name[MAX_FILENAME_LEN];
} File;

static const char FILESYSTEM_NAME[] = "filesystem";

unsigned int computeAdress(unsigned char *pFileName) {
	unsigned int sum = 0;
	while(*pFileName) {
		sum += *pFileName++;
	}
	return sum % MAX_NUM_FILES;
}

void addFileNameToFilesystem(unsigned int adress) {
	//open filesystem
}

long wy_FsCreate(unsigned char *pFileName, unsigned long maxSize, unsigned long accessFlags, unsigned long *pToken, long *pFileHandle) {
	return sl_FsOpen(pFileName, FS_MODE_OPEN_CREATE(maxSize, accessFlags), pToken, pFileHandle);
}

inline long wy_FsOpen(unsigned char *pFileName,unsigned long AccessModeAndMaxSize, unsigned long *pToken,long *pFileHandle) {
	return sl_FsOpen(pFileName, AccessModeAndMaxSize, pToken, pFileHandle);
}

inline int wy_FsClose(long FileHdl, unsigned char* pCeritificateFileName,unsigned char* pSignature ,unsigned long SignatureLen) {
	return sl_FsClose(FileHdl, pCeritificateFileName, pSignature, SignatureLen);
}

inline long wy_FsRead(long FileHdl, unsigned long Offset, unsigned char* pData, unsigned long Len) {
	return sl_FsRead(FileHdl, Offset, pData, Len);
}

inline long wy_FsWrite(long FileHdl, unsigned long Offset, unsigned char* pData, unsigned long Len) {
	return sl_FsWrite(FileHdl, Offset, pData, Len);
}

inline int wy_FsGetInfo(unsigned char *pFileName,unsigned long Token,SlFsFileInfo_t* pFsFileInfo) {
	return sl_FsGetInfo(pFileName, Token, pFsFileInfo);
}

int wy_FsDel(unsigned char *pFileName,unsigned long Token) {
	return sl_FsDel(pFileName, Token);
}
