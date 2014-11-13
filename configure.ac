#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ergo-telescope], [1.0], [matthew.d.weger@gmail.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE           
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
CXXFLAGS="$CXXFLAGS -std=c++0x"

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h stdint.h stdlib.h string.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_OUTPUT(Makefile src/Makefile)
