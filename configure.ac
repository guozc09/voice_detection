#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([voicedetection], [1.0], [guozc09@163.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects foreign])

VOICE_DETECTION_VERSION_MAJOR=1
VOICE_DETECTION_VERSION_MINOR=0
VOICE_DETECTION_VERSION_RELEASE=0
AC_SUBST([VOICE_DETECTION_VERSION_MAJOR])
AC_SUBST([VOICE_DETECTION_VERSION_MINOR])
AC_SUBST([VOICE_DETECTION_VERSION_RELEASE])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lvad':
AC_CHECK_LIB([vad], [main])

AC_ARG_WITH(system-lib-path,
	AS_HELP_STRING([--with-system-lib-path=SYSTEM_LIB_PATH], [System library path]),
	[SYSTEM_LIB_PATH="$withval"])
AC_SUBST([SYSTEM_LIB_PATH])

AC_ARG_WITH(system-include-path,
	AS_HELP_STRING([--with-system-include-path=SYSTEM_INCLUDE_PATH], [System include path]),
	[SYSTEM_INCLUDE_PATH="$withval"])
AC_SUBST([SYSTEM_INCLUDE_PATH])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h memory.h stdint.h stdlib.h string.h sys/ioctl.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memmove memset pow])

AC_CONFIG_FILES([Makefile
                 keyword_detection/kwd/Makefile
                 keyword_detection/kwd/src/Makefile
                 keyword_detection/Makefile
                 voice_activity_detection/Makefile
                 voice_activity_detection/vad/Makefile
                 voice_activity_detection/vad/src/Makefile])
AC_OUTPUT
