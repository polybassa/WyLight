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
#include <unistd.h>
#include <stdlib.h>

#define NUM_TEST_LOOPS 255

/**************** includes and functions for wrapping ****************/
#include "wy_fs.h"

#define DELETE_CMD "rm "
#define DELETE_FILESYSTEM DELETE_CMD FILESYSTEMNAME

/* test RTC commands */
int ut_wy_fs_create_file(void)
{
	unsigned char *filename = (unsigned char *)__FUNCTION__;
	TestCaseBegin();
	long hdl;
	CHECK(0 == wy_FsOpen(filename,FS_MODE_OPEN_CREATE(100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), 0, &hdl));
	CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	remove((const char *)filename);
	TestCaseEnd();
}

int ut_wy_fs_create_and_del_file(void)
{
	unsigned char *filename = (unsigned char *)__FUNCTION__;
	TestCaseBegin();
	long hdl;
	CHECK(0 != wy_FsOpen(filename, _FS_FILE_PUBLIC_READ, 0, &hdl));
	CHECK(0 == wy_FsOpen(filename, FS_MODE_OPEN_CREATE( 100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), 0, &hdl));
	CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	CHECK(0 == wy_FsOpen(filename, _FS_FILE_PUBLIC_READ, 0, &hdl));
	CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	CHECK(0 == wy_FsDel(filename, 0));
	CHECK(0 != wy_FsOpen(filename, _FS_FILE_PUBLIC_READ, 0, &hdl));
	TestCaseEnd();
}

int ut_wy_fs_max_file_creation(void)
{
	
	TestCaseBegin();
	unsigned char *filename_pre = (unsigned char *)__FUNCTION__;
	unsigned char buffer[100];
	long hdl;
	system(DELETE_FILESYSTEM);
	
	for (unsigned int i = 0; i < MAX_NUM_FILES - 1; i++) {
		sprintf((char *)buffer, "%s_%d", filename_pre, i);
		CHECK(0 == wy_FsOpen(buffer, FS_MODE_OPEN_CREATE(100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), 0, &hdl));
		CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	}
	// Cleanup
	sprintf((char *)buffer, "rm %s*", filename_pre);
	system((const char *)buffer);
	
	TestCaseEnd();
}

int ut_wy_fs_to_much_file_creation(void)
{
	
	TestCaseBegin();
	unsigned char *filename_pre = (unsigned char *)__FUNCTION__;
	char buffer[100];
	long hdl;
	system(DELETE_FILESYSTEM);
	
	for (unsigned int i = 0; i < MAX_NUM_FILES - 1; i++) {
		sprintf((char *)buffer, "%s_%d", filename_pre, i);
		CHECK(0 == wy_FsOpen((unsigned char *)buffer, FS_MODE_OPEN_CREATE( 100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), 0, &hdl));
		CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	}
	
	sprintf(buffer, "%s_%d", filename_pre, MAX_NUM_FILES - 1);
	CHECK(0 != wy_FsOpen((unsigned char *)buffer, FS_MODE_OPEN_CREATE( 100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), 0, &hdl));
	
	// Cleanup
	sprintf((char *)buffer, "rm %s*", filename_pre);
	system((const char *)buffer);
	
	TestCaseEnd();
}

int ut_wy_fs_complete_delete(void)
{
	
	TestCaseBegin();
	unsigned char *filename_pre = (unsigned char *)__FUNCTION__;
	unsigned char buffer[100];
	long hdl;
	system(DELETE_FILESYSTEM);
	
	for (unsigned int i = 0; i < MAX_NUM_FILES - 1; i++) {
		sprintf((char *)buffer, "%s_%d", filename_pre, i);
		CHECK(0 == wy_FsOpen(buffer, FS_MODE_OPEN_CREATE( 100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), 0, &hdl));
		CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	}
	
	for (unsigned int i = 0; i < MAX_NUM_FILES - 1; i++) {
		sprintf((char *)buffer, "%s_%d", filename_pre, i);
		CHECK(0 == wy_FsDel(buffer, 0));
		CHECK(0 != wy_FsOpen(buffer, _FS_FILE_PUBLIC_READ, 0, &hdl));
	}
	TestCaseEnd();
}

int ut_wy_fs_format(void)
{
	
	TestCaseBegin();
	unsigned char *filename_pre = (unsigned char *)__FUNCTION__;
	unsigned char buffer[100];
	long hdl;
	system(DELETE_FILESYSTEM);
	
	for (unsigned int i = 0; i < MAX_NUM_FILES - 1; i++) {
		sprintf((char *)buffer, "%s_%d", filename_pre, i);
		CHECK(0 == wy_FsOpen(buffer, FS_MODE_OPEN_CREATE( 100, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), 0, &hdl));
		CHECK(0 == wy_FsClose(hdl, 0, 0, 0));
	}
	
	CHECK(0 == wy_FsFormat());
	
	for (unsigned int i = 0; i < MAX_NUM_FILES - 1; i++) {
		sprintf((char *)buffer, "%s_%d", filename_pre, i);
		CHECK(0 != wy_FsOpen(buffer, _FS_FILE_PUBLIC_READ, 0, &hdl));
	}
	TestCaseEnd();
}


int main(int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_wy_fs_create_file);
	RunTest(true, ut_wy_fs_create_and_del_file);
	RunTest(true, ut_wy_fs_max_file_creation);
	RunTest(true, ut_wy_fs_to_much_file_creation);
	RunTest(true, ut_wy_fs_complete_delete);
	RunTest(true, ut_wy_fs_format);
	
	// Cleanup
	system(DELETE_FILESYSTEM);
	UnitTestMainEnd();
}

