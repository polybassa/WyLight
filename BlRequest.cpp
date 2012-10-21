/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
 
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

#include "BlRequest.h"
#include "crc.h"
#include "trace.h"

/**
 * This makro is used in BlProxy::MaskControlCharacters and requires some
 * implicit parameter:
 * @param bytesWritten counter of bytes in output buffer
 * @param outputLength size of output buffer
 * @param pOutput output buffer
 * @param _BYTE_ the byte we have to mask and writte to buffer
 */
#define MaskAndAddByteToOutput(_BYTE_) { \
	if(IsCtrlChar(_BYTE_)) { \
		if(++bytesWritten > outputLength) return 0; \
		*pOutput = BL_DLE; \
		pOutput++; \
	} \
	if(++bytesWritten > outputLength) return 0; \
	*pOutput = _BYTE_; \
	pOutput++; \
}

BlProxy::BlProxy(const ClientSocket* const pSock)
	: mSock(pSock)
{
}

size_t BlProxy::MaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength) const
{
	const unsigned char* const pInputEnd = pInput + inputLength;
	size_t bytesWritten = 0;
	unsigned short crc = 0;

	/* skip first character since it is the command type byte */
	if(++bytesWritten > outputLength) return 0;
	*pOutput = *pInput;
	pOutput++;
	pInput++;	

	while(pInput < pInputEnd)
	{
		MaskAndAddByteToOutput(*pInput);
		Crc_AddCrc16(*pInput, &crc);
		pInput++;
	}

	// add crc to output
	MaskAndAddByteToOutput((unsigned char)(crc & 0xff));
	MaskAndAddByteToOutput((unsigned char)(crc >> 8));	
	return bytesWritten;
}

size_t BlProxy::UnmaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength) const
{
	if(outputLength < inputLength)
	{
		return 0;
	}
	const unsigned char* const pInputEnd = pInput + inputLength;
	unsigned short crc = 0;

	/* skip first character since its the command type byte */
	size_t bytesWritten = 1;
	*pOutput = *pInput;
	pOutput++;
	pInput++;

	/* read two bytes ahead to find crc */
	if(pInput >= pInputEnd) return 0;
	if(*pInput == BL_DLE)
	{
		pInput++;
	}
	if(pInput >= pInputEnd) return 0;
	unsigned char next = *pInput;
	pInput++;

	if(pInput >= pInputEnd) return 0;
	if(*pInput == BL_DLE)
	{
		pInput++;
	}
	if(pInput >= pInputEnd) return 0;
	unsigned char postNext = *pInput;
	pInput++;


	while(pInput < pInputEnd)
	{
		if(*pInput == BL_DLE)
		{
			pInput++;
		}
		*pOutput = next;
		pOutput++;
		bytesWritten++;
		Crc_AddCrc16(next, &crc);
		next = postNext;
		postNext = *pInput;
		pInput++;
	}

	// check and remove crc
	Trace_Hex(postNext);
	Trace_Hex(next);
	Trace_Hex(crc >> 8);
	Trace_Hex(crc & 0xff);
	if(crc != ((postNext << 8) | next))
	{
		Trace_String(__FUNCTION__);
		Trace_String(" crc failed\n");
		return 0;
	}
	return bytesWritten;
}

int BlProxy::Send(BlRequest& req, unsigned char* pResponse, size_t responseSize) const
{
	return Send(req.GetData(), req.GetSize(), pResponse, responseSize);
}

int BlProxy::Send(const unsigned char* pRequest, const size_t requestSize, unsigned char* pResponse, size_t responseSize) const
{
	unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
	unsigned char recvBuffer[BL_MAX_MESSAGE_LENGTH];
	size_t bufferSize = 0;
	unsigned char* pCur = buffer;
	unsigned char* pNext;
	const unsigned char* pEnd;;

	/* mask control characters in request and add crc */
	bufferSize = MaskControlCharacters(pRequest, requestSize, buffer, sizeof(buffer));
	if((0 == bufferSize) || (bufferSize == sizeof(buffer)))
	{
		Trace_String("BlProxy::Send: MaskControlCharacters() failed\n");
		return 0;
	}

	/* add BL_ETX to the end of buffer */
	buffer[bufferSize] = BL_ETX;
	bufferSize++;

	int numRetries = BL_MAX_RETRIES;
	do
	{
		/* sync with bootloader */
		mSock->Send(BL_SYNC, sizeof(BL_SYNC));
		if(0 != mSock->Recv(recvBuffer, sizeof(recvBuffer), BL_RESPONSE_TIMEOUT_TMMS))
		{
			/* synchronized -> send request */
			if(static_cast<int>(bufferSize) != mSock->Send(buffer, bufferSize))
			{
				Trace_String("BlProxy::Send: socket->Send() failed\n");
				return 0;
			}

			/* wait for a response? */
			if((0 == pResponse) || (0 == responseSize))
			{
				return 0;
			}

			/* receive response */
			int bytesReceived = mSock->Recv(recvBuffer, sizeof(recvBuffer), BL_RESPONSE_TIMEOUT_TMMS);

			if(bytesReceived > 1)
			{
				/* remove STX from message */
				pCur = pResponse;
				pNext = recvBuffer;
				pEnd = recvBuffer + bytesReceived;
				while((pNext < pEnd) && (BL_STX == *pNext))
				{
					pNext++; bytesReceived--;
				}

				/* remove BL_ETX from buffer */
				if(0 == bytesReceived)
				{
					return 0;
				}
				bytesReceived--;

				/* remove BL_DLE and check crc from buffer */
				return UnmaskControlCharacters(pNext, bytesReceived, pResponse, responseSize);
			}
 		}
	}while(0 < --numRetries);

	Trace_String("BlProxy::Send: Too many retries\n");
	return -1;
}

