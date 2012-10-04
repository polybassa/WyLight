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

#include <stdio.h>
#include "trace.h"

#ifdef __cplusplus
#include <assert.h>
#else
#define assert(EXPRESSION) if(!(EXPRESSION)) {errors++; printf("Assert(" #EXPRESSION ") failed\n");}
#endif

#define RunTest(RUN, FUNC) { \
	if(RUN) { \
		int _errors= FUNC(); \
		printf(#FUNC"() run with %d errors\n", _errors); \
		numErrors+= _errors; numTests++; \
	} else { \
		numSkipped++; \
	} \
}

#define UnitTestMainBegin(X) int numErrors = 0; int numSkipped = 0; int numTests = 0;
#define UnitTestMainEnd(X) { \
	printf("%s run %d Tests (%d skipped | %d errors)\n", __FILE__, numTests, numSkipped, numErrors); \
	return numErrors; \
}

