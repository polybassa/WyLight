/**
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.
 
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
#include "timeval.h"
#include "trace.h"
#include "wifly_cmd.h"
#ifdef DEBUG
#include <iostream>
#endif

static const timeval RESPONSE_TIMEOUT = {3, 0}; // three seconds timeout for framented responses from pic

/**
 * This makro is used in ComProxy::MaskControlCharacters and requires some
 * implicit parameter:
 * @param bytesWritten counter of bytes in output buffer
 * @param outputLength size of output buffer
 * @param pOutput output buffer
 * @param _BYTE_ the byte we have to mask and write to buffer
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

ComProxy::ComProxy(uint32_t addr, uint16_t port)
	: mSock(addr, port)
{
}

size_t ComProxy::MaskControlCharacters(const uint8_t* pInput, size_t inputLength, uint8_t* pOutput, size_t outputLength, bool crcInLittleEndian) const
{
	const uint8_t* const pInputEnd = pInput + inputLength;
	size_t bytesWritten = 0;
	uint16_t crc = 0;

	while(pInput < pInputEnd)
	{
		MaskAndAddByteToOutput(*pInput);
		Crc_AddCrc16(*pInput, &crc);
		pInput++;
	}

	// add crc to output
	if(crcInLittleEndian)
	{
		MaskAndAddByteToOutput((uint8_t)(crc & 0xff));
		MaskAndAddByteToOutput((uint8_t)(crc >> 8));
	}
	else
	{
		MaskAndAddByteToOutput((uint8_t)(crc >> 8));
		MaskAndAddByteToOutput((uint8_t)(crc & 0xff));
	}
	return bytesWritten;
}

size_t ComProxy::UnmaskControlCharacters(const uint8_t* pInput, size_t inputLength, uint8_t* pOutput, size_t outputLength, bool checkCrc, bool crcInLittleEndian) const
{
	if(outputLength < inputLength)
	{
		return 0;
	}
	const uint8_t* const pInputEnd = pInput + inputLength;
	/* The bootloader sends the low byte of crc first, so we cannot just add
	 * all bytes to the checksum and compare it to zero.
	 * Now, our approach is to save the two latest crc's and parse the whole
	 * input buffer. When everything was read prepreCrc contains the crc before
	 * the first crc byte from data stream was added to crc.
	 */
	uint16_t crc = 0;
	uint16_t preCrc = 0;
	uint16_t prepreCrc = 0;
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

size_t ComProxy::Recv(uint8_t* pBuffer, size_t length, timeval* pTimeout, bool checkCrc, bool crcInLittleEndian) const
{
	timeval endTime, now;
	gettimeofday(&endTime, NULL);
	timeval_add(&endTime, pTimeout);
	uint8_t* const pBufferBegin = pBuffer;
	uint16_t crc = 0;
	uint16_t preCrc = 0;
	uint16_t prepreCrc = 0;
	bool lastWasDLE = false;

	// TODO refactor this with code in commandstorage. It should be identical to the fw receive implementation
	do {
		size_t bytesMasked = mSock.Recv(pBuffer, length, pTimeout);
#ifdef DEBUG
		std::cout << std::endl << __FILE__ << "::" << __FUNCTION__
		<< "(): Bytes masked: " << bytesMasked;
		std::cout << std::endl;
#endif
		uint8_t* pInput = pBuffer;
		while(bytesMasked-- > 0)
		{
			if(lastWasDLE)
			{
				lastWasDLE = false;
				prepreCrc = preCrc;
				preCrc = crc;
				Crc_AddCrc16(*pInput, &crc);
				*pBuffer = *pInput;
				pBuffer++;
				length--;
			}
			else
			{
				if(BL_DLE == *pInput)
				{
					lastWasDLE = true;
				}
				else if (BL_ETX == *pInput)
				{
#ifdef DEBUG
					std::cout << std::endl << __FILE__ << "::" << __FUNCTION__
					<< "(): Detect ETX ";
					std::cout << std::endl;
#endif
					if(!checkCrc)
					{
						return pBuffer - pBufferBegin;
					}

					if(crcInLittleEndian)
					{
						Crc_AddCrc16(pBuffer[-1], &prepreCrc);
						Crc_AddCrc16(pBuffer[-2], &prepreCrc);
						crc = prepreCrc;
					}
#ifdef DEBUG
					std::cout << std::endl << __FILE__ << "::" << __FUNCTION__
					<< "(): Crc: " << std::hex << crc << " Returnvalue: " <<  (pBuffer - 2) - pBufferBegin;
					std::cout << std::endl;
#endif
					return (0 != crc) ? 0 : (pBuffer - 2) - pBufferBegin;
				}
				else if (BL_STX == *pInput)
				{
					pBuffer = pBufferBegin;
#ifdef DEBUG
					std::cout << std::endl << __FILE__ << "::" << __FUNCTION__
					<< "(): Detect STX ";
					std::cout << std::endl;
#endif
				}
				else
				{
					prepreCrc = preCrc;
					preCrc = crc;
					Crc_AddCrc16(*pInput, &crc);
					*pBuffer = *pInput;
					pBuffer++;
					length--;
				}
			}
			pInput++;
		}
		gettimeofday(&now, NULL);
	} while(timeval_sub(&endTime, &now, pTimeout));
	return 0;
}

int32_t ComProxy::Send(BlRequest& req, uint8_t* pResponse, size_t responseSize, bool doSync) const
{
	Trace_String("ComProxy::Send(BlRequest&): ");
	Trace_Number((uint32_t)req.GetSize());
	Trace_String("pure bytes\n");
	int32_t retval = Send(req.GetData(), req.GetSize(), pResponse, responseSize, req.CheckCrc(), doSync);
	return retval;
}

int32_t ComProxy::Send(const struct cmd_frame* pFrame, uint8_t* pResponse, size_t responseSize, bool doSync) const
{
	Trace_String("ComProxy::Send(const struct cmd_frame*): ");
	int32_t retval = Send(reinterpret_cast<const uint8_t*>(pFrame), pFrame->length, pResponse, responseSize, true, doSync, false);
	return retval;
}

int32_t ComProxy::Send(const uint8_t* pRequest, const size_t requestSize, uint8_t* pResponse, size_t responseSize, bool checkCrc, bool doSync, bool crcInLittleEndian) const
{
	uint8_t buffer[BL_MAX_MESSAGE_LENGTH];
	uint8_t recvBuffer[BL_MAX_MESSAGE_LENGTH];
	size_t bufferSize = 0;

	/* add leading STX */
	buffer[0] = BL_STX;
	bufferSize++;

	/* mask control characters in request and add crc */
	bufferSize += MaskControlCharacters(pRequest, requestSize, buffer + 1, sizeof(buffer) + 1, crcInLittleEndian);
	if(1 == bufferSize)
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
		timeval timeout;
		do
		{
			if(0 > --numRetries)
			{
				Trace_String("ComProxy::Send: Too many retries\n");
				return -1;
			}
			Trace_String("ComProxy::Send: SYNC...\n");
			mSock.Send(BL_SYNC, sizeof(BL_SYNC));
			timeout = RESPONSE_TIMEOUT;
		} while(0 == mSock.Recv(recvBuffer, sizeof(recvBuffer), &timeout));
	}

		{
			/* synchronized -> send request */
			if(bufferSize != mSock.Send(buffer, bufferSize))
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
			timeval timeout = RESPONSE_TIMEOUT;
			size_t bytesReceived = Recv(recvBuffer, sizeof(recvBuffer), &timeout, checkCrc, crcInLittleEndian);
			memcpy(pResponse, recvBuffer, bytesReceived);
			return bytesReceived;
 		}
}

bool ComProxy::Send(std::string const& telnetMessage) const
{
	static const timespec NANOSLEEP_TIME = {0, 500000000};
	static const unsigned char ENTER_CMD_MODE[] = {'$', '$', '$'}; 
	static const unsigned char EXIT_CMD_MODE[] = {'e', 'x', 'i', 't', '\n'};

	if(sizeof(ENTER_CMD_MODE) != mSock.Send(ENTER_CMD_MODE, sizeof(ENTER_CMD_MODE)))
		return false;
	// we need to wait at least 250ms after "$$$" to enter command mode
	nanosleep(&NANOSLEEP_TIME, NULL);

	if(telnetMessage.size() != mSock.Send(reinterpret_cast<const unsigned char*>(telnetMessage.data()), telnetMessage.size()))
		return false;

	return sizeof(EXIT_CMD_MODE) == mSock.Send(EXIT_CMD_MODE, sizeof(EXIT_CMD_MODE));

}

