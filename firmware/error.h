/*
 Copyright (C) 2012 Nils Weiss, Patrick Brünn.

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

#ifndef _ERROR_H_
#define _ERROR_H_

#ifdef __CC8E__
typedef enum {
#else /* For GCC or CLANG on X86, ARM */
typedef enum __attribute__ ((__packed__)) {
#endif
	OK = 0x00,
	SCRIPTBUFFER_FULL,
	CRC_CHECK_FAILED,
	BAD_PACKET,                                     //ringbuffer or commandIo overflow
	BAD_COMMAND_CODE,
	NO_RESPONSE
}
ErrorCode;

void Error_Throw();
#endif /* #ifndef _ERROR_H_ */
