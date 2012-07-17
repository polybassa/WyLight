/**
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

#include "commandstorage.h"
#include "error.h"
#include "RingBuf.h"
#include "usart.h"

struct ErrorBits gERROR;

void throw_errors()
{
	if(RingBufHasError) 
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		ClearCmdBuf();
		USARTsend_str(" ERROR: Receivebuffer full");
		// *** Re-init the Ringbuffer to get a consistent commandstring and reset error
		RingBufInit();
	}
	if(gERROR.crc_failure)
	{
		USARTsend_str(" ERROR: CRC-Check failed");
		gERROR.crc_failure = 0;
	}
	if(gERROR.eeprom_failure)
	{
		USARTsend_str(" ERROR: EEPROM is full");
		gERROR.eeprom_failure = 0;
	}
}
