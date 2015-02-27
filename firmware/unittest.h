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

#ifndef _UNITTEST_H_
#define _UNITTEST_H_

#include <stdio.h>
#include "trace.h"

#define CHECK(EXPRESSION) if (!(EXPRESSION)) { \
        errors++; \
        printf("ERROR %s:%d: %s() CHECK(" # EXPRESSION ") failed\n", __FILE__, __LINE__, __FUNCTION__); \
}

#define CHECK_MEMCMP(BUFFER, REF_DATA, REF_SIZE) { \
        CHECK(0 == memcmp(BUFFER, REF_DATA, REF_SIZE)); \
        BUFFER += REF_SIZE; \
}

#define NOT_IMPLEMENTED(X) { \
        printf("ERROR %s:%d: %s() NOT IMPLEMENTED\n", __FILE__, __LINE__, __FUNCTION__); \
        return 1; \
}

#define RunTest(RUN, FUNC) { \
        if (RUN) { \
            size_t _errors = FUNC(); \
            if (_errors != 0) { \
                printf(# FUNC "() run with %d errors\n", (int)_errors); \
                numErrors += _errors; \
            } \
            numTests++; \
        } \
        else { \
            numSkipped++; \
        } \
}

#define TestCaseBegin(X) size_t errors = 0;
#define TestCaseEnd(X) return errors;

#define UnitTestMainBegin(X) int numErrors = 0; int numSkipped = 0; int numTests = 0;
#define UnitTestMainEnd(X) do { \
        printf("%36s run %2d Tests (%2d skipped | %2d errors)\n", __FILE__, numTests, numSkipped, numErrors); \
        return numErrors; \
} while (0)
#endif /* #ifndef _UNITTEST_H_ */
