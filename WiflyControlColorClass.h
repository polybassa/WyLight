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

#ifndef _WIFLYCONTROL_LEDCOLOR_H_
#define _WIFLYCONTROL_LEDCOLOR_H_

#include <cstdint>
#include <netinet/in.h>

class WiflyControlColorClass
{	
	private:
		union
		{
		      uint8_t rgbaBytes[4];
		      uint32_t	rgbaLong;
		}data;
	public:
		WiflyControlColorClass(const uint32_t& newValue = 0x00);
		~WiflyControlColorClass(void);
	  
		const uint8_t red() const;
		void red(const uint8_t& newColor);
		
		const uint8_t green() const;
		void green(const uint8_t& newColor);
		
		const uint8_t blue() const;
		void blue(const uint8_t& newColor);
		
		const uint8_t alpha() const;
		void alpha(const uint8_t& newColor);
		
		const uint32_t rgba() const;
		void rgba(const uint32_t& newValue);
};

#endif /* _WIFLYCONTROL_LEDCOLOR_H_*/