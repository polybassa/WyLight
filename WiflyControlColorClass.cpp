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

#include "WiflyControlColorClass.h"

WiflyControlColorClass::WiflyControlColorClass(const uint32_t& newValue)
{
	  data.rgbaLong = htonl(newValue);
}

WiflyControlColorClass::~WiflyControlColorClass(void)
{
}
	  	  
const uint8_t WiflyControlColorClass::red() const
{
	  return data.rgbaBytes[0];
}
void WiflyControlColorClass::red(const uint8_t& newColor)
{
	  data.rgbaBytes[0] = newColor;
}


const uint8_t WiflyControlColorClass::green() const
{
	  return data.rgbaBytes[1];
}
void WiflyControlColorClass::green(const uint8_t& newColor)
{
	  data.rgbaBytes[1] = newColor;
}


const uint8_t WiflyControlColorClass::blue() const
{
	  return data.rgbaBytes[2];
}
void WiflyControlColorClass::blue(const uint8_t& newColor)
{
	  data.rgbaBytes[2] = newColor;
}
		

const uint8_t WiflyControlColorClass::alpha() const
{
	  return data.rgbaBytes[3];
}
void WiflyControlColorClass::alpha(const uint8_t& newColor)
{
	  data.rgbaBytes[3] = newColor;
}
		

const uint32_t WiflyControlColorClass::rgba() const
{
	  return ntohl(data.rgbaLong);
}
void WiflyControlColorClass::rgba(const uint32_t& newValue)
{
	  data.rgbaLong = htonl(newValue);
}
