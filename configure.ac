#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([WiflyLight], [0.1], [weitys1@aol.com])
AC_CONFIG_SRCDIR([android/WiflyLight/jni/WiflyControlJni.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_SUBST(AM_CXXFLAGS, [-std=c++11])
AC_SUBST(AM_CXXFLAGS, [-pedantic])
AC_SUBST(AM_CXXFLAGS, [-Wall])
AC_PATH_PROG(WINE, wine,[wine not found])
AC_PATH_PROG(NDK, ndk-build, [ndk not found])
AC_PATH_PROG(PIC_CC8E, [CC8E.EXE], [CC8E NOT FOUND],[$PATH:/Users:/home:/Users/weitys1/Dropbox/Wifly_Light/CC8E:/home/gpb/cc8efree])
AC_PATH_PROG(DOXYGEN, doxygen, [doxygen not found])
AC_PATH_PROG(GIT, git, [git not found])

# Checks for libraries.
AC_CHECK_LIB([GL], glAlphaFunc)
AC_CHECK_LIB([GLU], gluBeginCurve)
AC_CHECK_LIB([glut], glutBitmapCharacter)
AC_CHECK_LIB([pthread], pthread_create)

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([clock_gettime gettimeofday localtime_r memset select socket])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
