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

#ifndef _TIMEVAL_H_
#define _TIMEVAL_H_

#include <sys/time.h>

inline void timeval_add(timeval *ref, const timeval *diff)
{
	if(!diff) return;
	ref->tv_usec += diff->tv_usec;
	ref->tv_sec += diff->tv_sec;
	ref->tv_sec += ref->tv_usec / 1000000;
	ref->tv_usec = ref->tv_usec % 1000000;
}

/**
 * @param a should be >= than <b>
 * @param b should be <= than <a>
 * @param result if not NULL result will be a-b
 * @return true if result == NULL or a >= b
 */
inline bool timeval_sub(const timeval *a, const timeval *b, timeval *result)
{
	if(!result) return true;

	long long micros = (a->tv_sec - b->tv_sec) * 1000000 + a->tv_usec - b->tv_usec;
	result->tv_sec = (long)(micros / 1000000);
	result->tv_usec = micros % 1000000;
	return micros >= 0;
}
#endif /* #ifndef _TIMEVAL_H_ */

