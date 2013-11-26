/*
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

namespace WyLight {

	static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

	static const timeval RESPONSE_TIMEOUT = {5, 0}; // three seconds timeout for fragmented responses from pic
	static const timeval SYNC_TIMEOUT = {0, 500000};

	ComProxy::ComProxy(const TcpSocket& sock)
		: mSock(sock)
	{}

	size_t ComProxy::Recv(uint8_t *pBuffer, const size_t length, timeval *pTimeout, bool checkCrc, bool crcInLittleEndian) const throw (ConnectionTimeout)
	{
		timeval endTime, now;
		gettimeofday(&endTime, NULL);
		timeval_add(&endTime, pTimeout);
		UnmaskBuffer recvBuffer {length};

		do {
			const size_t bytesMasked = mSock.Recv(pBuffer, length - recvBuffer.Size(), pTimeout);
			if(recvBuffer.Unmask(pBuffer, bytesMasked, checkCrc, crcInLittleEndian)) {
				memcpy(pBuffer, recvBuffer.Data(), recvBuffer.Size());
				return recvBuffer.Size();
			}
			gettimeofday(&now, NULL);
		}
		while(timeval_sub(&endTime, &now, pTimeout));
		throw ConnectionTimeout("Receive response timed out");
	}

	size_t ComProxy::Send(const BlRequest& req, uint8_t *pResponse, size_t responseSize, bool doSync) const throw(ConnectionTimeout, FatalError)
	{
			Trace(ZONE_INFO, "%zu pure bytes\n", req.GetSize());
		return Send(req.GetData(), req.GetSize(), pResponse, responseSize, req.CheckCrc(), doSync);
	}

	size_t ComProxy::Send(const FwCommand& cmd, response_frame *pResponse, size_t responseSize) const throw(ConnectionTimeout, FatalError)
	{
		return Send(cmd.GetData(), cmd.GetSize(), reinterpret_cast<uint8_t *>(pResponse), responseSize, true, false, false);
	}

	size_t ComProxy::Send(const uint8_t *pRequest, const size_t requestSize, uint8_t *pResponse, size_t responseSize, bool checkCrc, bool doSync, bool crcInLittleEndian) const throw(ConnectionTimeout, FatalError)
	{
		if(doSync) {
			if(SyncWithTarget() != BL_IDENT) {
				throw FatalError("Target in wrong mode");
			}
		}

		/* mask control characters in request and add crc */
		MaskBuffer maskBuffer {BL_MAX_MESSAGE_LENGTH};
		maskBuffer.Mask(pRequest, pRequest + requestSize, crcInLittleEndian);
		if(maskBuffer.Size() != mSock.Send(maskBuffer.Data(), maskBuffer.Size())) {
			throw FatalError("mSock.Send() failed");
		}

		/* wait for a response? */
		if((NULL == pResponse) || (0 == responseSize)) {
			Trace(ZONE_INFO, "waiting for no response-> exiting...\n");
			return 0;
		}

		/* receive response */
		timeval timeout = RESPONSE_TIMEOUT;
		return Recv(pResponse, responseSize, &timeout, checkCrc, crcInLittleEndian);
	}

	size_t ComProxy::SyncWithTarget(void) const throw(FatalError)
	{
		uint8_t recvBuffer[BL_MAX_MESSAGE_LENGTH];
		std::fill(recvBuffer, recvBuffer + sizeof(recvBuffer), 0);
		timeval timeout;
		int numRetries = BL_MAX_RETRIES;
		do
		{
			if(0 > --numRetries) {
				throw FatalError("SyncWithTarget failed, too many reties");
			}
			Trace(ZONE_INFO, "SYNC...\n");
			mSock.Send(BL_SYNC, sizeof(BL_SYNC));
			timeout = SYNC_TIMEOUT;
		}
		while(0 == mSock.Recv(recvBuffer, sizeof(recvBuffer), &timeout));

		return recvBuffer[0];
	}
} /* namespace WyLight */
