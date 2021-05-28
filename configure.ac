#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ddb], [0.1.0], [https://github.com/CiYong/ddb])
AC_CONFIG_SRCDIR([src/ddb.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])

AC_CONFIG_MACRO_DIRS([m4])
m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AX_CXX_COMPILE_STDCXX_11([ext], [optional])

# Libtool configuration for different targets
AM_PROG_AR
AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T


AC_CONFIG_FILES([Makefile])

AC_OUTPUT
