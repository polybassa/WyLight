#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([WyLight], [0.1], [nils.weiss@wylight.de])
AC_CONFIG_SRCDIR([android/app/src/main/jni/WiflyControlJni.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_SUBST(AM_CXXFLAGS, [-std=c++0x])
AC_SUBST(AM_CXXFLAGS, [-pedantic])
AC_SUBST(AM_CXXFLAGS, [-Wall])

AC_PATH_PROG(WINE, wine,[not_found])
AC_PATH_PROG(CPPCHECKER, cppcheck, [not_found])
AC_PATH_PROG(NDK, ndk-build, [not_found])
AC_PATH_PROG(DOXYGEN, doxygen, [not_found])
AC_PATH_PROG(GIT, git, [not_found])
AC_PATH_PROG(UNCRUSTIFY, uncrustify, [not_found])

AC_DEFUN(AC_PATH_PROG_CC8E, [
AC_PATH_PROG(PIC_CC8E, [CC8E.EXE], [not_found],[$PATH:/Users/weitys1/Dropbox/Wifly_Light/CC8E:/home/gpb/cc8efree:/home/weitys1/Dropbox/Wifly_Light/CC8E])
if test x"${PIC_CC8E}" == x"not_found" ; then
    AC_MSG_RESULT([Searching for CC8E with find!])
	PIC_CC8E=$(find ~ -name 'CC8E.EXE' -print -quit)
else
	AC_MSG_RESULT([CC8E found!])
fi ])
AC_PATH_PROG_CC8E

# Checks for libraries.
AC_CHECK_LIB([GL], glAlphaFunc, [AC_SUBST(GL,found)],[AC_SUBST(GL,not_found)])
AC_CHECK_LIB([GLU], gluBeginCurve, [AC_SUBST(GLU,found)],[AC_SUBST(GLU,not_found)])
AC_CHECK_LIB([glut], glutBitmapCharacter, [AC_SUBST(glut,found)],[AC_SUBST(glut,not_found)])
AC_CHECK_LIB([pthread], pthread_create)
AC_CHECK_LIB([thread], sleep_for)
AC_CHECK_LIB([crypto], SHA256)

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([clock_gettime floor getcwd gettimeofday inet_ntoa localtime_r memset pow select socket sqrt strstr strtoul])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
