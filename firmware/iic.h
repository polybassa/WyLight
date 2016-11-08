/**
   Copyright (C) 2012 - 2016 Nils Weiss, Patrick Bruenn.

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

#ifndef _IIC_H_
#define _IIC_H_

void I2C_Init();
void I2C_Write(uns8 slaveaddr, const uns8 dataaddr, const uns8 data);
uns8 I2C_Read(uns8 slaveaddr, const uns8 readaddr);
uns8 I2C_DetectSlave(const uns8 slaveaddr);

#endif  /*_IIC_H_*/
