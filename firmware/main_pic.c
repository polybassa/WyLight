/**
   Copyright (C) 2012 - 2017 Nils Weiss, Patrick Bruenn.

   This file is part of WyLight.

   WyLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   WyLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#define DEBUG
#pragma optimize 1
#pragma sharedAllocation

#include "isr_pic.c"
#include "main_common.c"
#include "crc.c"
#include "eeprom.c"
#include "error.c"
#include "ledstrip.c"
#include "spi.c"
#include "timer.c"
#include "RingBuf.c"
#include "usart.c"
#include "CommandIO.c"
#include "platform.c"
#include "rtc.c"
#include "iic.c"
#include "ScriptCtrl.c"
#include "trace.c"
#include "Flash.c"
#include "Version.c"
