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

#include "wy_fs.h"
#include "trace.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

enum FileStatus {
	EMPTY = 0, INVALID, VALID
};

struct File {
	enum FileStatus Status;
	unsigned char Name[MAX_FILENAME_LEN];
};

static struct File Filesystem[MAX_NUM_FILES];

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

static long openFileSystem() {
	static const unsigned char FS_NAME[] = FILESYSTEMNAME;
	long hdl;
	if (sl_FsOpen(FS_NAME, FS_MODE_OPEN_READ, 0, &hdl)) {
		// File Doesn't exit create a new file
		if (sl_FsOpen(FS_NAME, FS_MODE_OPEN_CREATE(sizeof(Filesystem), _FS_FILE_OPEN_FLAG_COMMIT), 0, &hdl)) {
			// Something went wrong, clean up
			sl_FsDel(FS_NAME, 0);
			return SL_FS_ERR_ALLOC;
		}
		sl_FsClose(hdl, 0, 0, 0);
		memset(Filesystem, 0, sizeof(Filesystem));
		return SL_FS_OK;
	}
	if (sizeof(Filesystem) != sl_FsRead(hdl, 0, (unsigned char *) Filesystem, sizeof(Filesystem))) {
		sl_FsClose(hdl, 0, 0, 0);
		return SL_FS_ERR_FAILED_TO_READ;
	}
	sl_FsClose(hdl, 0, 0, 0);
	return SL_FS_OK;
}

static long closeFileSystem() {
	static const unsigned char FS_NAME[] = FILESYSTEMNAME;
	long hdl;
	if (sl_FsOpen(FS_NAME, FS_MODE_OPEN_WRITE, 0, &hdl)) {
		return SL_FS_ERR_FILE_NOT_EXISTS;
	}
	if (sizeof(Filesystem) != sl_FsWrite(hdl, 0, (unsigned char *) Filesystem, sizeof(Filesystem))) {
		sl_FsClose(hdl, 0, 0, 0);
		return SL_FS_ERROR_FAILED_TO_WRITE;
	}
	sl_FsClose(hdl, 0, 0, 0);
	return SL_FS_OK;
}

static long addFileNameToFilesystem(unsigned char *pFileName) {
	long errno = openFileSystem();
	if (errno) return errno;

	const unsigned int FileNameLen = sl_min(MAX_FILENAME_LEN, sl_Strlen(pFileName));

	unsigned int address = computeAdress(pFileName);
	long retVal = SL_FS_ERR_ALLOC;
	unsigned int round = 0;

	for (; round < MAX_NUM_FILES; round++) {
		if (Filesystem[address].Status != VALID) {
			Filesystem[address].Status = VALID;
			memcpy(Filesystem[address].Name, pFileName, FileNameLen);
			Filesystem[address].Name[FileNameLen] = 0x00;
			retVal = SL_FS_OK;
			goto close_and_return;
		} else if (0 == memcmp(Filesystem[address].Name, pFileName, FileNameLen)) {
			retVal = SL_FS_OK; // FileName already exists
			goto close_and_return;
		} else {
			address = incAdress(address);
		}
	}

	close_and_return: errno = closeFileSystem();
	return sl_min(errno, retVal);
}

static long removeFileNameFromFilesystem(unsigned char *pFileName) {
	long errno = openFileSystem();
	if (errno) return errno;

	const unsigned int FileNameLen = sl_min(MAX_FILENAME_LEN, sl_Strlen(pFileName));

	unsigned int address = computeAdress(pFileName);
	long retVal = SL_FS_ERR_FILE_NOT_EXISTS;
	unsigned int round = 0;

	for (; round < MAX_NUM_FILES; round++) {
		if (Filesystem[address].Status != INVALID || Filesystem[address].Status != VALID) {
			retVal = SL_FS_OK;
			goto close_and_return;
		} else if (Filesystem[address].Status == VALID
				&& 0 == memcmp(Filesystem[address].Name, pFileName, FileNameLen)) {
			Filesystem[address].Status = INVALID;
			memset(Filesystem[address].Name, 0, sizeof(MAX_FILENAME_LEN));
			retVal = SL_FS_OK;
			goto close_and_return;
		} else {
			address = incAdress(address);
		}
	}

	close_and_return: errno = closeFileSystem();
	return sl_min(errno, retVal);
}

long wy_FsOpen(unsigned char *pFileName, unsigned long AccessModeAndMaxSize, unsigned long *pToken, long *pFileHandle) {

	const unsigned long mask = _FS_MODE_ACCESS_MASK << _FS_MODE_ACCESS_OFFSET;
	const unsigned long access = (AccessModeAndMaxSize & mask) >> _FS_MODE_ACCESS_OFFSET;

#ifdef SIMULATOR
	long retVal1 = sl_FsOpen(pFileName, AccessModeAndMaxSize, pToken, pFileHandle);
#endif
	
	long retVal2 = SL_FS_OK;

	if (access == _FS_MODE_OPEN_CREATE || access == _FS_MODE_OPEN_WRITE_CREATE_IF_NOT_EXIST) {
		retVal2 = addFileNameToFilesystem(pFileName);
	}
#ifdef SIMULATOR
	return retVal2 ? retVal2 : retVal1;
#else
	return retVal2 ? retVal2 : sl_FsOpen(pFileName, AccessModeAndMaxSize, pToken, pFileHandle);
#endif
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
	long retVal = removeFileNameFromFilesystem(pFileName);
	if (retVal) {
		return retVal;
	}
	return sl_FsDel(pFileName, Token);
}

int wy_FsFormat(void) {
	long errno = openFileSystem();
	if (errno) return errno;

	long retVal = SL_FS_ERR_FILE_NOT_EXISTS;
	unsigned int address = 0;
	for (; address < MAX_NUM_FILES; address++) {
		if (Filesystem[address].Status == VALID) {
			retVal = sl_FsDel(Filesystem[address].Name, 0);
			if (retVal) {
				goto close_and_return;
			}

			Filesystem[address].Status = INVALID;
			memset(Filesystem[address].Name, 0, sizeof(MAX_FILENAME_LEN));
		}
	}
	retVal = SL_FS_OK;
	close_and_return: errno = closeFileSystem();
	return sl_min(errno, retVal);
}
#ifdef DEBUG
// remove this function in productive code
int wy_FsPrintFileList(void) {
	long errno = openFileSystem();
	if (errno) return errno;

	unsigned int address = 0;
	for (; address < MAX_NUM_FILES; address++) {
		Trace(ZONE_VERBOSE, "%d: ", address);
		if (Filesystem[address].Status == VALID) {
			Trace(ZONE_VERBOSE,"%s", Filesystem[address].Name);
		}
		Trace(ZONE_VERBOSE,"\r\n");
	}
	return SL_FS_OK;
}
#endif
