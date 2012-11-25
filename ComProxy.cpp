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

#include "ComProxy.h"
#include "crc.h"
#include "trace.h"
#include "wifly_cmd.h"

/**
 * This makro is used in ComProxy::MaskControlCharacters and requires some
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

ComProxy::ComProxy(const ClientSocket* const pSock)
	: mSock(pSock)
{
}

size_t ComProxy::MaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength, bool crcInLittleEndian) const
{
	const unsigned char* const pInputEnd = pInput + inputLength;
	size_t bytesWritten = 0;
	unsigned short crc = 0;

	while(pInput < pInputEnd)
	{
		MaskAndAddByteToOutput(*pInput);
		Crc_AddCrc16(*pInput, &crc);
		pInput++;
	}

	// add crc to output
	if(crcInLittleEndian)
	{
		MaskAndAddByteToOutput((unsigned char)(crc & 0xff));
		MaskAndAddByteToOutput((unsigned char)(crc >> 8));
	}
	else
	{
		MaskAndAddByteToOutput((unsigned char)(crc >> 8));
		MaskAndAddByteToOutput((unsigned char)(crc & 0xff));
	}
	return bytesWritten;
}

size_t ComProxy::UnmaskControlCharacters(const unsigned char* pInput, size_t inputLength, unsigned char* pOutput, size_t outputLength, bool checkCrc, bool crcInLittleEndian) const
{
	if(outputLength < inputLength)
	{
		return 0;
	}
	const unsigned char* const pInputEnd = pInput + inputLength;
	/* The bootloader sends the low byte of crc first, so we cannot just add
	 * all bytes to the checksum and compare it to zero.
	 * Now, our approach is to save the two latest crc's and parse the whole
	 * input buffer. When everything was read prepreCrc contains the crc before
	 * the first crc byte from data stream was added to crc.
	 */
	unsigned short crc = 0;
	unsigned short preCrc = 0;
	unsigned short prepreCrc = 0;
	size_t bytesWritten = 0;

	/* unmask input buffer and calculate crc */
	while(pInput < pInputEnd)
	{
		if(*pInput == BL_DLE)
		{
			pInput++;
		}
		*pOutput = *pInput;
		prepreCrc = preCrc;
		preCrc = crc;
		Crc_AddCrc16(*pInput, &crc);
		pOutput++;
		bytesWritten++;
		pInput++;
	}

	/* for responses without crc we can finish here */
	if(!checkCrc)
	{
		return bytesWritten;
	}

	/* we should have read at least two crc bytes! */
	if(bytesWritten < 2)
	{
		return 0;
	}

	if(crcInLittleEndian)
	{
		/* know we have to take the last two bytes (which can be masked!) and compare
		 * them to the calculated checksum, by adding them in reverse order and
		 * comparing them with zero
		 */
		pInput--;
		Crc_AddCrc16(*pInput, &prepreCrc);
		pInput--;
		if(BL_DLE == *pInput)
		{
			pInput--;
		}
		Crc_AddCrc16(*pInput, &prepreCrc);
		crc = prepreCrc;
	}

	if(0 != crc)
	{
		Trace_String(__FUNCTION__);
		Trace_String(" check crc: ");
		Trace_Number(prepreCrc);
		Trace_String(" crc failed\n");
		return 0;
	}
	return bytesWritten - 2;
}

int ComProxy::Send(BlRequest& req, unsigned char* pResponse, size_t responseSize, bool doSync) const
{
	Trace_String("ComProxy::Send(BlRequest&): ");
	Trace_Number(req.GetSize());
	Trace_String("pure bytes\n");
	return Send(req.GetData(), req.GetSize(), pResponse, responseSize, req.CheckCrc(), doSync);
}

int ComProxy::Send(const struct cmd_frame* pFrame, unsigned char* pResponse, size_t responseSize, bool doSync) const
{
	Trace_String("ComProxy::Send(const struct cmd_frame*): ");	
	return Send(reinterpret_cast<const unsigned char*>(pFrame), pFrame->length + 2, pResponse, responseSize, true, doSync, false);
}

int ComProxy::Send(const unsigned char* pRequest, const size_t requestSize, unsigned char* pResponse, size_t responseSize, bool checkCrc, bool doSync, bool crcInLittleEndian) const
{
	unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
	unsigned char recvBuffer[BL_MAX_MESSAGE_LENGTH];
	size_t bufferSize = 0;
	unsigned char* pCur = buffer;
	unsigned char* pNext;
	const unsigned char* pEnd;;

	/* add leading STX */
	buffer[0] = BL_STX;
	bufferSize++;

	/* mask control characters in request and add crc */
	bufferSize = MaskControlCharacters(pRequest, requestSize, buffer, sizeof(buffer), crcInLittleEndian);
	if((0 == bufferSize) || (bufferSize == sizeof(buffer)))
	{
		Trace_String("ComProxy::Send: MaskControlCharacters() failed\n");
		return 0;
	}

	/* add BL_ETX to the end of buffer */
	buffer[bufferSize] = BL_ETX;
	bufferSize++;

	int numRetries = BL_MAX_RETRIES;

	/* sync with bootloader */
	if(doSync)
	{
		do
		{
			if(0 > --numRetries)
			{
				Trace_String("ComProxy::Send: Too many retries\n");
				return -1;
			}
			Trace_String("ComProxy::Send: SYNC...\n");
			mSock->Send(BL_SYNC, sizeof(BL_SYNC));
		} while(0 == mSock->Recv(recvBuffer, sizeof(recvBuffer), BL_RESPONSE_TIMEOUT_TMMS));
	}

		{
			/* synchronized -> send request */
			if(static_cast<int>(bufferSize) != mSock->Send(buffer, bufferSize))
			{
				Trace_String("ComProxy::Send: socket->Send() failed\n");
				return 0;
			}

			/* wait for a response? */
			if((0 == pResponse) || (0 == responseSize))
			{
				Trace_String("ComProxy::Send: waiting for no response-> exiting...\n");
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
					Trace_String("ComProxy::Send: no bytes received\n");
					return 0;
				}
				bytesReceived--;

				/* remove BL_DLE from buffer and check crc if requested */
				return UnmaskControlCharacters(pNext, bytesReceived, pResponse, responseSize, checkCrc);
			}
			Trace_String("ComProxy::Send: response to short\n");
			return 0;
 		}
}

