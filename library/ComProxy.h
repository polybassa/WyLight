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

#ifndef _COM_PROXY_H_
#define _COM_PROXY_H_

#include "BlRequest.h"
#include "ClientSocket.h"
#include "trace.h"
#include "FwCommand.h"
#include "wifly_cmd.h"

namespace WyLight {

	class ComProxy
	{
	public:
		/*
		 * Create a new object for communication with PIC bootloader and firmware
		 * @param sock reference to a tcp wrapper socket with an established connection to the WLAN module
		 */
		ComProxy(const TcpSocket& sock);

		/*
		 * Send a request to the PIC bootloader and wait for a response
		 * @param req reference to the bootloader request
		 * @param pResponse pointer to a buffer for the bootloader response
		 * @param responseSize size of the response buffer
		 * @param doSync if true a uart baudrate synchronisation is forced before request is send
		 * @return number of bytes read into pResponse or 0 if crc check fails
		 * @throw ConnectionTimeout if a timeout occurred
		 * @throw FatalError if synchronisation or sending to socket failed
		 */
		size_t Send(const BlRequest& req, uint8_t *pResponse, size_t responseSize, bool doSync = true) const throw(ConnectionTimeout, FatalError);

		/*
		 * Send a request to the PIC firmware and wait for a response
		 * @param request FwCommand object with a firmware command frame
		 * @param pResponse pointer to buffer for the response frame
		 * @param responseSize size of the response buffer
		 * @return number of bytes read into pResponse or 0 if crc check fails
		 * @throw ConnectionTimeout if a timeout occurred
		 * @throw FatalError if sending to socket failed
		 */
		size_t Send(const FwCommand& request, response_frame *pResponse, size_t responseSize) const throw(ConnectionTimeout, FatalError);

		/*
		 * Send a byte sequence to force a uart baud rate synchronisation between WLAN module and PIC
		 * @return mode of target: BL_IDENT for Bootloader mode, FW_IDENT for Firmware mode
		 * @throw FatalError if synchronisation fails
		 */
		size_t SyncWithTarget(void) const throw(FatalError);

	private:
		/*
		 * Reference to a tcp socket with an established connection to a wifly module
		 */
		const TcpSocket& mSock;

		/*
		 * Receive data on the TcpSocket @see mSock, unmask the control characters and write the plain message into pBuffer
		 * @param pBuffer to store the read data
		 * @param length of the buffer pBuffer is pointing to
		 * @param pTimeout maximum time to wait for a response use NULL to wait indefinitely (default)
		 * @param checkCrc if true the crc of the response will be checked, 0 is returned if crc was wrong
		 * @param crcInLittleEndian if true the crc is assumed to be in little endian byte order like the bootloader will send it. if false the byte order is assumed to be big endian
		 * @return the number of bytes received or 0 if the crc check fails
		 * @throw ConnectionTimeout if response timed out
		 */
		size_t Recv(uint8_t *pBuffer, size_t length, timeval *pTimeout = NULL, bool checkCrc = true, bool crcInLittleEndian = true) const throw(ConnectionTimeout);

		/*
		 * Send a bootloader or firmware pRequest to the PIC
		 * @param pRequest buffer containing the request to send
		 * @param requestSize size of the request buffer
		 * @param pResponse pointer to a buffer for the response
		 * @param responseSize size of the response buffer
		 * @param checkCrc if true the crc of the response will be checked and 0 returned if crc check fails
		 * @param doSync if true a uart baudrate synchronisation is forced before request is send (only valid for bootloader requests
		 * @param crcInLittleEndian if true the crc is assumed in little endian byte order
		 * @return number of bytes read into pResponse or 0 if crc check fails
		 * @throw ConnectionTimeout if a timeout occurred
		 * @throw FatalError if synchronisation or sending to socket failed
		 */
		size_t Send(uint8_t const *pRequest, const size_t requestSize, uint8_t *pResponse, size_t responseSize, bool checkCrc, bool doSync, bool crcInLittleEndian = true) const throw(ConnectionTimeout, FatalError);
	};
}
#endif /* #ifndef _COM_PROXY_H_ */

