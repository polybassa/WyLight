/*
 Copyright (C) 2013 Nils Weiss, Patrick Bruenn.
 
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

#include "Version.h"
#include "VersionFile.h"

const char verStr[] = VER_STRING;

uns8 Version_Print(uns8 *pArray, const uns8 arraySize)
{
	uns8 i = 0;
	uns8 tempByte;
	while(i < sizeof(VER_STRING) && i < arraySize)
	{
	    tempByte = verStr[i];
		*pArray = tempByte;
		pArray++;
		i++;
	}
	return i;
}


