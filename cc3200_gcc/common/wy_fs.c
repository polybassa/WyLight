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

enum FileStatus {
	EMPTY = 0, INVALID, VALID
};

struct File {
	enum FileStatus Status;
	unsigned char Name[MAX_FILENAME_LEN];
};

static unsigned int computeAdress(unsigned char *pFileName) {
	unsigned int sum = 0;
	while (*pFileName) {
		sum += *pFileName++;
	}
	return sum % (MAX_NUM_FILES - 1);
}

static inline unsigned int incAdress(unsigned int adress) {
	return ++adress % (MAX_NUM_FILES - 1);
}

static long openFileSystem(const unsigned long access) {
	static const unsigned char FS_NAME[] = FILESYSTEMNAME;
	long hdl;
	if (sl_FsOpen(FS_NAME, access, 0, &hdl)) {
		// File Doesn't exit create a new file
		int errno = sl_FsOpen(FS_NAME, FS_MODE_OPEN_CREATE(MAX_NUM_FILES * sizeof(struct File), 0), 0, &hdl);
		if (errno) {
			// Something went wrong, clean up
			sl_FsDel(FS_NAME, 0);
			return SL_FS_ERR_ALLOC;
		}
		wy_FsClose(hdl, 0, 0, 0);
		errno = sl_FsOpen(FS_NAME, FS_MODE_OPEN_WRITE, 0, &hdl);
		if (errno) {
			// Something went wrong, clean up
			sl_FsDel(FS_NAME, 0);
			return SL_FS_ERR_ALLOC;
		}

		unsigned char emptyData[sizeof(struct File)];
		memset(emptyData, 0xff, sizeof(emptyData));
		unsigned int i;
		for (i = 0; i < MAX_NUM_FILES; i++) {
			if (sizeof(emptyData) != wy_FsWrite(hdl, i * sizeof(emptyData), emptyData, sizeof(emptyData)))
		}

		wy_FsClose(hdl, 0, 0, 0);
		errno = sl_FsOpen(FS_NAME, access, 0, &hdl);
		if (errno) {
			// Something went wrong, clean up
			sl_FsDel(FS_NAME, 0);
			return SL_FS_ERR_ALLOC;
		}
	}
	return hdl;
}

static long addFileNameToFilesystem(unsigned char *pFileName) {
	long hdl = openFileSystem(FS_MODE_OPEN_READ);
	if (hdl < 0) return hdl;  // contains ERRORCODE

	const unsigned int FileNameLen = sl_min(MAX_FILENAME_LEN, sl_Strlen(pFileName));

	unsigned int adress = computeAdress(pFileName);
	long retVal = SL_FS_ERR_ALLOC;
	unsigned int round = 0;
	for (; round < MAX_NUM_FILES; round++) {
		unsigned long offset = adress * sizeof(struct File);
		struct File tempFile;
		unsigned char buffer[sizeof(struct File)];
		int tempVal = wy_FsRead(hdl, offset, buffer, sizeof(struct File));
		if (sizeof(struct File) != tempVal) {
			retVal = SL_FS_ERR_FAILED_TO_READ;
			goto close_and_return;
		}
		memcpy(&tempFile, buffer, sizeof(struct File));

		if (tempFile.Status != VALID) {
			tempFile.Status = VALID;
			wy_FsClose(hdl, 0, 0, 0);
			hdl = openFileSystem(FS_MODE_OPEN_WRITE);

			memcpy(tempFile.Name, pFileName, FileNameLen);
			if (sizeof(struct File) == wy_FsWrite(hdl, offset, (unsigned char *) &tempFile, sizeof(struct File))) {
				retVal = SL_FS_OK;
			} else {
				retVal = SL_FS_ERROR_FAILED_TO_WRITE;
			}
			wy_FsWrite(hdl, sizeof(struct File) * (MAX_NUM_FILES - 1), (unsigned char *) &tempFile,
					sizeof(struct File));
			goto close_and_return;
		} else if (0 == memcmp(tempFile.Name, pFileName, FileNameLen)) {
			retVal = SL_FS_OK; // FileName already exists
			goto close_and_return;
		} else {
			adress = incAdress(adress);
		}
	}

	close_and_return: wy_FsClose(hdl, 0, 0, 0);
	return retVal;
}

static long removeFileNameFromFilesystem(unsigned char *pFileName) {
	long hdl = openFileSystem(FS_MODE_OPEN_READ);
	if (hdl < 0) return hdl; // contains ERRORCODE

	const unsigned int FileNameLen = sl_min(MAX_FILENAME_LEN, sl_Strlen(pFileName));

	unsigned int adress = computeAdress(pFileName);
	long retVal = SL_FS_ERR_FILE_NOT_EXISTS;
	unsigned int round = 0;
	for (; round < MAX_NUM_FILES; round++) {
		const size_t offset = adress * sizeof(struct File);
		struct File tempFile;
		unsigned char buffer[sizeof(struct File)] = { 0 };

		if (sizeof(struct File) != wy_FsRead(hdl, offset, buffer, sizeof(buffer))) {
			retVal = SL_FS_ERR_FAILED_TO_READ;
			goto close_and_return;
		}

		memcpy(&tempFile, buffer, sizeof(buffer));

		if (tempFile.Status != INVALID || tempFile.Status != VALID) {
			retVal = SL_FS_OK;
			goto close_and_return;
		} else if (0 == memcmp(tempFile.Name, pFileName, FileNameLen)) {
			tempFile.Status = INVALID;
			sl_FsClose(hdl, 0, 0, 0);
			hdl = openFileSystem(FS_MODE_OPEN_WRITE);
			if (sizeof(struct File) == wy_FsWrite(hdl, offset, (unsigned char *) &tempFile, sizeof(struct File))) {
				retVal = SL_FS_OK;
			} else {
				retVal = SL_FS_ERROR_FAILED_TO_WRITE;
			}
			wy_FsWrite(hdl, sizeof(struct File) * (MAX_NUM_FILES - 1), (unsigned char *) &tempFile,
					sizeof(struct File));
			goto close_and_return;
		} else {
			adress = incAdress(adress);
		}
	}

	close_and_return: wy_FsClose(hdl, 0, 0, 0);
	return retVal;
}

long wy_FsOpen(unsigned char *pFileName, unsigned long AccessModeAndMaxSize, unsigned long *pToken, long *pFileHandle) {

	long retVal1 = sl_FsOpen(pFileName, AccessModeAndMaxSize, pToken, pFileHandle);

	const unsigned long mask = _FS_MODE_ACCESS_MASK << _FS_MODE_ACCESS_OFFSET;
	const unsigned long access = (AccessModeAndMaxSize & mask) >> _FS_MODE_ACCESS_OFFSET;

	if (access == _FS_MODE_OPEN_CREATE || access == _FS_MODE_OPEN_WRITE_CREATE_IF_NOT_EXIST) {
		long retVal2 = addFileNameToFilesystem(pFileName);
		return retVal2 ? retVal2 : retVal1;
	}
	return retVal1;
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
	long hdl = openFileSystem(FS_MODE_OPEN_READ);
	if (hdl < 0) return hdl; // contains ERRORCODE

	long retVal = SL_FS_ERR_FILE_NOT_EXISTS;
	unsigned int adress = 0;
	for (; adress < MAX_NUM_FILES; adress++) {
		const size_t offset = adress * sizeof(struct File);
		struct File tempFile;

		if (sizeof(struct File) != sl_FsRead(hdl, offset, (unsigned char *) &tempFile, sizeof(struct File))) {
			retVal = SL_FS_ERR_FAILED_TO_READ;
			goto close_and_return;
		}

		if (tempFile.Status == VALID) {
			retVal = sl_FsDel(tempFile.Name, 0);
			if (retVal) goto close_and_return;

			tempFile.Status = INVALID;
			sl_FsClose(hdl, 0, 0, 0);
			hdl = openFileSystem(FS_MODE_OPEN_WRITE);
			if (sizeof(struct File) != sl_FsWrite(hdl, offset, (unsigned char *) &tempFile, sizeof(struct File))) {
				retVal = SL_FS_ERROR_FAILED_TO_WRITE;
				goto close_and_return;
			}
			sl_FsClose(hdl, 0, 0, 0);
			hdl = openFileSystem(FS_MODE_OPEN_READ);
		}
	}
	retVal = SL_FS_OK;
	close_and_return: sl_FsClose(hdl, 0, 0, 0);
	return retVal;
}
