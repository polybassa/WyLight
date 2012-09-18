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
#include "trace.h"

#include "assert.h"

BlProxy::BlProxy(const ClientSocket* const pSock)
	: mSock(pSock)
{
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
	const unsigned char* pNext = pRequest;
	const unsigned char* pEnd = pNext + requestSize;
	Trace_String("Masking..");
	/* mask control characters in request */
	while((bufferSize < (BL_MAX_MESSAGE_LENGTH - 1)) && (pNext <= pEnd))
	{
		if(IsCtrlChar(*pNext))
		{
			*pCur = BL_DLE;
			pCur++; bufferSize++;
		}
		*pCur = *pNext;	
		pCur++; bufferSize++;
		pNext++;
	}	
	assert(0);
	Trace_String(" done\n");
	int numRetries = BL_MAX_RETRIES;
	do
	{
		/* sync with bootloader */
		mSock->Send(BL_SYNC, sizeof(BL_SYNC));
		if(0 != mSock->Recv(pResponse, responseSize, BL_RESPONSE_TIMEOUT_TMMS))
		{
			/* synchronized -> send request */
			if(static_cast<int>(requestSize) != mSock->Send(pRequest, requestSize))
			{
				/* send failed */
				return -1;
			}
		
			/* receive response */
			int bytesReceived = mSock->Recv(recvBuffer, sizeof(recvBuffer), BL_RESPONSE_TIMEOUT_TMMS);
			if(bytesReceived > 1)
			{
				/* remove STX and DLE from message */
				pCur = pResponse;
				pNext = recvBuffer;
				pEnd = recvBuffer + bytesReceived;

				/* remove STX from buffer */	
				while((pNext < pEnd) && (BL_STX == *pNext))
				{
					pNext++;
				}

				/* remove BL_DLE from buffer */
				do
				{
					if(BL_DLE == *pNext)
					{
						pNext++;
					}
					*pCur = *pNext;
					pCur++;
					pNext++;
				}while(pNext < pEnd);
				return pCur - pResponse;
			}
 		}
	}while(0 < --numRetries);

	/* to many retries */
	return -1;
}

