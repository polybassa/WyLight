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
#include "MaskBuffer.h"
#include "timeval.h"
#include "trace.h"
#include "wifly_cmd.h"

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

static const timeval RESPONSE_TIMEOUT = {3, 0}; // three seconds timeout for framented responses from pic


ComProxy::ComProxy(const TcpSocket& sock)
	: mSock(sock)
{
}


size_t ComProxy::Recv(uint8_t* pBuffer, const size_t length, timeval* pTimeout, bool checkCrc, bool crcInLittleEndian) const throw (ConnectionTimeout)
{
	timeval endTime, now;
	gettimeofday(&endTime, NULL);
	timeval_add(&endTime, pTimeout);
	UnmaskBuffer recvBuffer{length};

	do {
		size_t bytesMasked = mSock.Recv(pBuffer, length-recvBuffer.Size(), pTimeout);
		if(recvBuffer.Unmask(pBuffer, bytesMasked, checkCrc, crcInLittleEndian))
		{
			memcpy(pBuffer, recvBuffer.Data(), recvBuffer.Size());
			return recvBuffer.Size();
		}
		gettimeofday(&now, NULL);
	} while(timeval_sub(&endTime, &now, pTimeout));
	throw ConnectionTimeout("Receive response timed out");
}

int32_t ComProxy::Send(BlRequest& req, uint8_t* pResponse, size_t responseSize, bool doSync) const
{
	Trace(ZONE_INFO, "%zu pure bytes\n", req.GetSize());
	return Send(req.GetData(), req.GetSize(), pResponse, responseSize, req.CheckCrc(), doSync);
}

int32_t ComProxy::Send(const struct cmd_frame* pFrame, response_frame* pResponse, size_t responseSize, bool doSync) const
{
	return Send(reinterpret_cast<const uint8_t*>(pFrame), pFrame->length, reinterpret_cast<uint8_t*>(pResponse), responseSize, true, doSync, false);
}

int32_t ComProxy::Send(const uint8_t* pRequest, const size_t requestSize, uint8_t* pResponse, size_t responseSize, bool checkCrc, bool doSync, bool crcInLittleEndian) const
{
	MaskBuffer maskBuffer{BL_MAX_MESSAGE_LENGTH};
	uint8_t recvBuffer[BL_MAX_MESSAGE_LENGTH];

	/* mask control characters in request and add crc */
	maskBuffer.Mask(pRequest, pRequest + requestSize, crcInLittleEndian);
	maskBuffer.CompleteWithETX();


	int numRetries = BL_MAX_RETRIES;

	/* sync with bootloader */
	if(doSync)
	{
		Trace(ZONE_VERBOSE, "sync with bootloader\n");
		timeval timeout;
		do
		{
			if(0 > --numRetries)
			{
				Trace(ZONE_WARNING, "Too many retries\n");
				return -1;
			}
			Trace(ZONE_INFO, "SYNC...\n");
			mSock.Send(BL_SYNC, sizeof(BL_SYNC));
			timeout = RESPONSE_TIMEOUT;
		} while(0 == mSock.Recv(recvBuffer, sizeof(recvBuffer), &timeout));
	}
	
	Trace(ZONE_VERBOSE, "synchronized\n");
	/* synchronized -> send request */
	if(maskBuffer.Size() != mSock.Send(maskBuffer.Data(), maskBuffer.Size()))
	{
		Trace(ZONE_ERROR, "socket->Send() failed\n");
		return 0;
	}

	/* wait for a response? */
	if((0 == pResponse) || (0 == responseSize))
	{
		Trace(ZONE_INFO, "waiting for no response-> exiting...\n");
		return 0;
	}

	/* receive response */
	timeval timeout = RESPONSE_TIMEOUT;
	size_t bytesReceived = Recv(recvBuffer, sizeof(recvBuffer), &timeout, checkCrc, crcInLittleEndian);
	const size_t bytesToCopy = std::min(bytesReceived, responseSize);
	memcpy(pResponse, recvBuffer, bytesToCopy);
	Trace(ZONE_VERBOSE, "%zu bytes received %zu bytes copied\n", bytesReceived, bytesToCopy);
	return (int32_t)bytesReceived;
}

