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


#include "Version.h"

struct cmd_get_fw_version g_Version;

void Version_Init(void)
{
	uns8 temp = verStr[0] - '0';
	temp *= 100;
	g_Version.major = temp;
	
	temp = verStr[1] - '0';
	temp *= 10;
	g_Version.major += temp;
	
	g_Version.major += verStr[2] - '0';
	
	temp = verStr[4] - '0';
	temp *= 100;
	g_Version.minor = temp;
	
	temp = verStr[5] - '0';
	temp *= 10;
	g_Version.minor += temp;
	
	g_Version.minor += verStr[6] - '0';

}


