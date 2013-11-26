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

#ifndef ___stl_patches_h
#define ___stl_patches_h
#if 0
#include "../config.h"
#ifndef HAVE_LIBTHREAD
#warning "Check for a new c++11 std::thread implementatin, which can handle std::this_thread::sleep_for(std::chrono::nanoseconds(X)) whithout using nanosleep"
#define _GLIBCXX_USE_NANOSLEEP
#endif
#endif
#ifdef ANDROID
#warning "Check for a new android-ndk > r9b to avoid using this selfmade implementation"
#include <sstream>
namespace std {
	template <class T>
	std::string to_string(T value)
	{
		std::stringstream converter;
		converter << value;
		return converter.str();
	}

	inline int stoi(const std::string& str, size_t *idx = 0, int base = 10)
	{
		std::stringstream converter(str);
		int i;
		converter >> i;
		return i;
	}

}
#endif /* #ifdef ANDROID */

#endif /* #ifndef ___stl_patches_h */

