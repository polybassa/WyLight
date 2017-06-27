/*
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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//*********************** ENUMERATIONS *********************************************
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
//*********************** CONFIGURATION ********************************************
#define NUM_OF_LED 32

//*********************** useful macros ********************************************
#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

#ifdef __CC8E__
#include "platform_pic.h"
#elif defined(__XTENSA_EL__)
#include "platform_esp.h"
#else
#include "platform_linux.h"
#endif
#endif /* #ifndef _PLATFORM_H_ */
