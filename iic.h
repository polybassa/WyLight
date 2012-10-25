/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
 
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
 
 #ifndef _IIC_H_
 #define _IIC_H_
 
 void I2C_Init();
 void I2C_Write(uns8 slaveaddr,uns8 dataaddr, uns8 data);
 uns8 I2C_Read(uns8 slaveaddr,uns8 readaddr);
 
 
 #endif /*_IIC_H_*/