/*
 * firmware_download.c
 *
 *  Created on: 07.08.2014
 *      Author: nw
 */
#include "firmware_download.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "shamd5.h"
#include "hw_shamd5.h"
#include "simplelink.h"
#include "uart_if.h"
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "rom_map.h"
#include "prcm.h"
#include "utils.h"

#define PREFIX_BUFFER 			"GET /"
#define POST_BUFFER				" HTTP/1.1\nHost: www.wylight.de\nConnection: close\nAccept-Encoding: gzip, deflate\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n\n"
#define HOST_NAME				"www.wylight.de"

#define HTTP_FILE_NOT_FOUND    	"404 Not Found" /* HTTP file not found response */
#define HTTP_STATUS_OK         	"200 OK"  /* HTTP status ok response */
#define HTTP_CONTENT_LENGTH    	"Content-Length:"  /* HTTP content length header */
#define HTTP_CONNECTION        	"Connection:" /* HTTP Connection header */
#define HTTP_CONNECTION_CLOSE  	"close"  /* HTTP Connection header value */
#define HTTP_END_OF_HEADER		"\r\n\r\n"  /* string marking the end of headers in response */

#define DNS_RETRY				5 /* No of DNS tries */

#define MAX_BUFF_SIZE  			1460
#define BLOCKSIZE		 		64 /* Write block size for write to MD5SHA module */

#define UART_PRINT          	Report

//
// GLOBAL VARIABLES -- Start
//
// Flags to check that interrupts were successfully generated.
volatile bool g_bContextReadyFlag;
volatile bool g_bParthashReadyFlag;
volatile bool g_bInputReadyFlag;
volatile bool g_bOutputReadyFlag;
int g_iSockID;
//
// GLOBAL VARIABLES -- End
//
//*****************************************************************************

//
// FUNCTION PROTOTYPES -- Start
//
void SHAMD5IntHandler(void);
//
// FUNCTION PROTOTYPES -- End
//
//*****************************************************************************

unsigned long GetServerIP(/*const char *hostName*/) {

	char *hostName = HOST_NAME;
	unsigned long destinationServerIP = 0;
	unsigned int retry = 0;
	int retVal = 0;

	do {
		retry++;
		retVal = sl_NetAppDnsGetHostByName(hostName, strlen(hostName), &destinationServerIP, SL_AF_INET);
		if (0 > retVal) {
			UART_PRINT("Getting IP for Host Name failed\r\n");
		}
		if (retry > (DNS_RETRY - 1)) {
			UART_PRINT("Couldn't get the IP of the host, Exiting application\r\n");
			return 0;
		}
	} while (retry < DNS_RETRY && 0 > retVal);
	return destinationServerIP;
}

int GetFileFromServer(const int socket, const char* filename, const char* filenameInFlash) {
	int transfer_len = 0;
	long retVal = 0;
	long fileHandle = -1;
	unsigned long Token = 0, bytesReceived = 0;
	unsigned char *pBuff = 0;
	unsigned long file_size = 0;
	unsigned char buffer[MAX_BUFF_SIZE + 1];

	UART_PRINT("Start downloading the file %s\r\n", filename);

	memset(buffer, 0, sizeof(buffer));

	// Puts together the HTTP GET string.
	strcpy((char *) buffer, PREFIX_BUFFER);
	strcat((char *) buffer, filename);
	strcat((char *) buffer, POST_BUFFER);

	// Send the HTTP GET string to the opened TCP/IP socket.
	transfer_len = sl_Send(socket, buffer, strlen((const char *) buffer), 0);
	if (transfer_len < 0) {
		// error
		UART_PRINT("Socket Send Error\r\n");
		return -1;
	}

	memset(buffer, 0, sizeof(buffer));

	// get the reply from the server in buffer.
	transfer_len = sl_Recv(socket, buffer, MAX_BUFF_SIZE, 0);

	if (transfer_len > 0) {
		// Check for 404 return code
		if (strstr((const char *) buffer, HTTP_FILE_NOT_FOUND) != 0) {
			UART_PRINT("File not found, check the file and try again\r\n");
			return -1;
		}

		// if not "200 OK" return error
		if (strstr((const char *) buffer, HTTP_STATUS_OK) == 0) {
			UART_PRINT("Error during downloading the file\r\n");
			return -1;
		}

		// check if content length is transfered with headers
		pBuff = (unsigned char *) strstr((const char *) buffer, HTTP_CONTENT_LENGTH);
		if (pBuff != 0) {
			pBuff += strlen(HTTP_CONTENT_LENGTH);

			while (*pBuff != 0x0d)
				pBuff++;
			unsigned long multiplicant = 1;

			pBuff--;
			while (*pBuff != ' ') {
				file_size += (*pBuff - '0') * multiplicant;
				multiplicant *= 10;
				pBuff--;
			}

			if (file_size == 0) {
				UART_PRINT("\nContent Length not detected.\r\n");
				return -1;
			}

			UART_PRINT("Content Length %d\r\n", file_size);
		} else {
			UART_PRINT("\nContent Length not detected.\r\n");
			return -1;
		}

		// "\r\n\r\n" marks the end of headers
		pBuff = (unsigned char *) strstr((const char *) buffer, HTTP_END_OF_HEADER);
		if (pBuff == 0) {
			UART_PRINT("Invalid response\r\n");
			return -1;
		}
		// Increment by 4 to skip "\r\n\r\n"
		pBuff += 4;

		// Adjust buffer data length for header size
		transfer_len -= (pBuff - buffer);
	}

	// Delete eventually existing temporary file
	sl_FsDel((unsigned char *) filenameInFlash, Token);
	// Create a temporary file to save the downloaded file
	// open a user file for writing
	retVal = sl_FsOpen((unsigned char *) filenameInFlash, FS_MODE_OPEN_WRITE, &Token, &fileHandle);
	if (retVal < 0) {
		// File Doesn't exit create a new of file_size KB file
		retVal = sl_FsOpen((unsigned char *) filenameInFlash, FS_MODE_OPEN_CREATE(file_size, _FS_FILE_PUBLIC_WRITE), &Token, &fileHandle);
		if (retVal < 0) {
			UART_PRINT("Error during opening the temporary file\r\n");
			return -1;
		}
	}
	while (bytesReceived < file_size) {
		retVal = sl_FsWrite(fileHandle, bytesReceived, (unsigned char *) pBuff, transfer_len);
		if (retVal < 0) {
			retVal = sl_FsClose(fileHandle, 0, 0, 0);
			retVal = sl_FsDel((unsigned char *) filenameInFlash, Token);
			UART_PRINT("Error during writing the file\r\n");
			return -1;
		}
		bytesReceived += transfer_len;
		if (bytesReceived == file_size) break;

		memset(buffer, 0, sizeof(buffer));
		transfer_len = sl_Recv(socket, buffer, MAX_BUFF_SIZE, 0);
		pBuff = buffer;
	}

	// Close the opened file
	retVal = sl_FsClose(fileHandle, 0, 0, 0);

	if (bytesReceived != file_size) {
		UART_PRINT("Error While File Download\r\n");
		retVal = sl_FsDel((unsigned char *) filenameInFlash, Token);
		return -1;
	} else {
		UART_PRINT("\nDownloading File Completed\r\n");
	}
	return 0;
}


//*****************************************************************************
//
//! SHAMD5IntHandler - Interrupt Handler which handles different interrupts from
//! different sources
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void SHAMD5IntHandler(void) {
	uint32_t ui32IntStatus;
	//
	// Read the SHA/MD5 masked interrupt status.
	//
	ui32IntStatus = MAP_SHAMD5IntStatus(SHAMD5_BASE, true);
	if (ui32IntStatus & SHAMD5_INT_CONTEXT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_CONTEXT_READY);
		g_bContextReadyFlag = true;
	}
	if (ui32IntStatus & SHAMD5_INT_PARTHASH_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_PARTHASH_READY);
		g_bParthashReadyFlag = true;
	}
	if (ui32IntStatus & SHAMD5_INT_INPUT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_INPUT_READY);
		g_bInputReadyFlag = true;
	}
	if (ui32IntStatus & SHAMD5_INT_OUTPUT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_OUTPUT_READY);
		g_bOutputReadyFlag = true;
	}
}

int ComputeSHA(const char *sourceFile, uint8_t *resultHash) {
	long sfileHandle = -1;
	unsigned long sToken = 0;
	long retVal = 0;
	unsigned long bytesRead = 0;
	unsigned long readsize = 0;
	uint8_t buffer[BLOCKSIZE];
	SlFsFileInfo_t sFileInfo;

	UART_PRINT("\r\nStarted comuting hash\r\n");

	//Enable MD5SHA module
	PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
	MAP_SHAMD5IntRegister(SHAMD5_BASE, SHAMD5IntHandler);

	//reset modul
	PRCMPeripheralReset(PRCM_DTHE);

	//clear flags
	g_bContextReadyFlag = false;
	g_bInputReadyFlag = false;

	//Enable Interrupts
	SHAMD5IntEnable(SHAMD5_BASE, SHAMD5_INT_CONTEXT_READY | SHAMD5_INT_PARTHASH_READY | SHAMD5_INT_INPUT_READY | SHAMD5_INT_OUTPUT_READY);

	//wait for context ready flag.
	while (!g_bContextReadyFlag)
		;

	//Configure SHA/MD5 module
	SHAMD5ConfigSet(SHAMD5_BASE, SHAMD5_ALGO_SHA1);

	// get file size
	retVal = sl_FsGetInfo((unsigned char *) sourceFile, sToken, &sFileInfo);
	if (retVal < 0) {
		// File Doesn't exit create a new of 45 KB file
		UART_PRINT("Error during opening the source file\r\n");
		return -1;
	}

	// open the source file for reading
	retVal = sl_FsOpen((unsigned char *) sourceFile, FS_MODE_OPEN_READ, &sToken, &sfileHandle);
	if (retVal < 0) {
		UART_PRINT("Error during opening the source file\r\n");
		return -1;
	}

	SHAMD5DataLengthSet(SHAMD5_BASE, (uint32_t) sFileInfo.FileLen);

	// Copy the files from temporary file to original file
	// If user file has checksum which can be used to verify the temporary
	// file then file should be verified before copying
	while (bytesRead < sFileInfo.FileLen) {
		if ((sFileInfo.FileLen - bytesRead) > BLOCKSIZE) readsize = BLOCKSIZE;
		else readsize = (sFileInfo.FileLen - bytesRead);

		memset(buffer, 0, sizeof(buffer));
		retVal = sl_FsRead(sfileHandle, bytesRead, (unsigned char *) buffer, readsize);
		if (retVal < 0) {
			// Error close the file and delete the temporary file
			retVal = sl_FsClose(sfileHandle, 0, 0, 0);
			UART_PRINT("Error during reading the file\r\n");
			return -1;
		}
		SHAMD5DataWrite(SHAMD5_BASE, buffer);
		bytesRead += readsize;
	}

	// Close the opened files
	retVal = sl_FsClose(sfileHandle, 0, 0, 0);
	if (retVal < 0) {
		// Error close the file and delete the temporary file
		UART_PRINT("Error during close the file\r\n");
		return -1;
	}
	UART_PRINT("Hash successfully\r\n");
	SHAMD5ResultRead(SHAMD5_BASE, resultHash);
	return 0;
}

int VerifyFile(const char *fileToVerify, const char *fileWithSHAHash) {
	unsigned long sToken = 0;
	long sfileHandle = -1;
	uint8_t firstHash[20];
	uint8_t secoundHash[21];
	long retVal;
	memset(firstHash, 0, sizeof(firstHash));
	memset(secoundHash, 0, sizeof(secoundHash));

	// get hash of file to verify
	if (ComputeSHA(fileToVerify, firstHash) < 0) {
		UART_PRINT("Error computing SHA1SUM of %s\r\n", fileToVerify);
		return -1;
	}

	// open the source file for reading
	retVal = sl_FsOpen((unsigned char *) fileWithSHAHash, FS_MODE_OPEN_READ, &sToken, &sfileHandle);
	if (retVal < 0) {
		UART_PRINT("Error during opening the source file %s\r\n", fileWithSHAHash);
		return -1;
	}

	// transform the hash from file with sha hash to byte values. The Sha1Sum is decoded in ascii chars
	unsigned int i;
	for (i = 0; i < sizeof(firstHash); i++) {
		retVal = sl_FsRead(sfileHandle, i * 2, (unsigned char *) &secoundHash[i], 2);
		if (retVal < 0) {
			// Error close the file and delete the temporary file
			retVal = sl_FsClose(sfileHandle, 0, 0, 0);
			UART_PRINT("Error during reading the file\r\n");
			return -1;
		}
		uint8_t tempValue;
		if (secoundHash[i] > '9') tempValue = (secoundHash[i] - 'a' + 10) << 4;
		else tempValue = (secoundHash[i] - '0') << 4;

		if (secoundHash[i + 1] > '9') tempValue += (secoundHash[i + 1] - 'a' + 10);
		else tempValue += (secoundHash[i + 1] - '0');
		secoundHash[i] = tempValue;
	}

	// Close the opened files
	retVal = sl_FsClose(sfileHandle, 0, 0, 0);
	if (retVal < 0) {
		// Error close the file and delete the temporary file
		UART_PRINT("Error during close the file\r\n");
		return -1;
	}

	if (memcmp(secoundHash, firstHash, sizeof(firstHash)) != 0) {
		UART_PRINT("\r\nHash-Sums are different\r\n");
		return -1;
	}
	return 0;
}
