/*
 Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

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

#ifndef FIRMWARE_DOWNLOAD_H_
#define FIRMWARE_DOWNLOAD_H_

#include <stdint.h>

//****************************************************************************
//
//! \brief Resolve DNS Hostname
//!
//! This function resolve the DNS Hostname and returns the corresponding IP
//!
//! \param[in]      none
//!
//! \return         0 for error and positive value greater then 0 for a
//!					destination IP
//
//****************************************************************************
unsigned long GetServerIP(void);

//****************************************************************************
//
//! \brief Obtain the file from the server
//!
//! This function requests the file from the server and save it in
//! in serial flash.
//!
//! \param[in]      socket - connected socket to the host file server
//! \param[in]      filename - filename of the file on the server
//! \param[in]      filenameInFlash - name for the file in the internal flash
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
int GetFileFromServer(const int socket, const char* filename, const char* filenameInFlash);

//****************************************************************************
//
//! \brief Compute SHA1SUM
//!
//! This function computes the SHA1SUM of a given file
//!
//! \param[in]      sourceFile -- Source File of a file in the flash
//! \param[out]		resultHash -- Pointer to a 20 Byte Array for the result
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
int ComputeSHA(const char *sourceFile, uint8_t *resultHash);

//****************************************************************************
//
//! \brief Verify a File
//!
//! This function computes the checksum for a given file and compare's it with a
//! hash sum from a secound file
//! The secound file is generated from terminal sha1sum
//! Filecreation: sha1sum -b someBinary.bin > someBinary.sha
//!
//! \param[in]      sourceFile -- Source File of a file in the flash
//! \param[out]		resultHash -- Pointer to a 20 Byte Array for the result
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
int VerifyFile(const char *fileToVerify, const char *fileWithSHAHash);

//****************************************************************************
//
//! \brief Replace the existing file with the download file
//!
//! This function replaces two Files in the FlashFileSystem
//!
//! \param[in]      sourceFile
//! \param[in]		destinationFile
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
int ReplaceFile(const char *sourceFile, const char *destinationFile);

#endif /* FIRMWARE_DOWNLOAD_H_ */
