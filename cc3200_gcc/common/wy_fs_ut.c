/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

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

#include "unittest.h"
#include <stdbool.h>

#define NUM_TEST_LOOPS 255

/**************** includes and functions for wrapping ****************/
#include "wy_fs.h"

/* test RTC commands */
int ut_wy_fs_create_file(void)
{
	const unsigned char filename[] = "ut_wy_fs_create_file";
	TestCaseBegin();
	long hdl;
	CHECK(0 == wy_FsCreateIfNotExists(filename, 100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR, 0, &hdl));
	CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	TestCaseEnd();
}

int ut_wy_fs_create_and_del_file(void)
{
	const unsigned char filename[] = "ut_wy_fs_create_and_del_file";
	TestCaseBegin();
	long hdl;
	CHECK(0 > wy_FsOpen(filename, _FS_FILE_PUBLIC_READ, 0, &hdl));
	CHECK(0 == wy_FsCreateIfNotExists(filename, 100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR, 0, &hdl));
	CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	CHECK(0 == wy_FsOpen(filename, _FS_FILE_PUBLIC_READ, 0, 0));
	CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	CHECK(0 == wy_FsDel(filename, 0));
	CHECK(0 != wy_FsOpen(filename, _FS_FILE_PUBLIC_READ, 0, 0));
	TestCaseEnd();
}


int main(int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_wy_fs_create_file);
	RunTest(true, ut_wy_fs_create_and_del_file);
	UnitTestMainEnd();
}

