/*
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

#ifndef __WIFLYCOLOR_H_
#define __WIFLYCOLOR_H_

#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdint.h>

namespace WyLight {

	class WiflyColor
	{
	private:
		union
		{
			uint8_t asBytes[4];
			uint32_t asBigEndianLong;
		};
	public:
		static const uint32_t RED = 0xffff0000;
		static const uint32_t GREEN = 0xff00ff00;
		static const uint32_t BLUE = 0xff0000ff;
		static const uint32_t WHITE = 0xffffffff;
		static const uint32_t BLACK = 0xff000000;


		/**
		 * use a stringstream to convert hex ascii string into machine bits
		 */
		static uint32_t ToARGB(const std::string& s) {
			uint32_t argb;
			std::stringstream converter;
			converter << std::hex << s;
			converter >> argb;
			return (uint32_t)argb;
		};

		WiflyColor(const uint32_t argbValue = 0) { argb(argbValue); };

		uint8_t red() const { return asBytes[1]; };
		void red(uint8_t value) { asBytes[1] = value; };

		uint8_t green() const { return asBytes[2]; };
		void green(uint8_t value) { asBytes[2] = value; };

		uint8_t blue() const { return asBytes[3]; };
		void blue(uint8_t value) { asBytes[3] = value; };

		uint32_t argb() const { return ntohl(asBigEndianLong); };
		void argb(uint32_t argbValue) { asBigEndianLong = htonl(argbValue); };

		bool operator== (const WiflyColor& ref) const
		{
			return asBigEndianLong == ref.asBigEndianLong;
		};

		friend std::ostream& operator<< (std::ostream& out, const WiflyColor& ref) {
			return out << "0x" << std::hex << ref.argb();
		};

		friend std::istream& operator>> (std::istream& is, WiflyColor& ref)
		{
			std::string temp;
			is >> temp;
			ref.argb(WiflyColor::ToARGB(temp));
			return is;
		};
	};
}
#endif /* _WIFLYCOLOR_H_ */
