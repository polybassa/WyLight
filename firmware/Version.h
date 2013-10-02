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


#ifndef _Version_h
#define _Version_h
#include "wifly_cmd.h"
#include "VersionFile.h"

#define VERSION_STRING_ORIGIN 0x3000
#warning "Version String at 0x3000!!! check for overlapping code"
extern struct cmd_get_fw_version g_Version;
const char verStr[] = VER_STRING;

void Version_Init(void);


#endif
