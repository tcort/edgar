dnl edgar - a small LISP Interpreter written in C
dnl Copyright (c) 2013, 2014 Thomas Cort <linuxgeek@gmail.com>
dnl
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.

dnl don't forget to update the man page version when bumping this version.
AC_INIT([edgar], [2.0], [Thomas Cort <linuxgeek@gmail.com>])
AC_COPYRIGHT([Copyright (C) 2013, 2014 Thomas Cort <linuxgeek@gmail.com>])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_SILENT_RULES([yes])

dnl
dnl Standard C Checks
dnl
AC_LANG([C])
AC_PROG_CC()
AC_PROG_RANLIB
AM_PROG_AR

dnl
dnl Standard Header Checks
dnl
AC_HEADER_STDC
AC_HEADER_ASSERT

dnl
dnl Check for Headers Used
dnl
AC_CHECK_HEADERS([assert.h ctype.h getopt.h stdio.h stdlib.h string.h unistd.h])

dnl
dnl Check for Functions Used
dnl
AC_CHECK_FUNCS([getopt_long])
AC_CHECK_FUNCS([assert exit memset])
AC_CHECK_FUNCS([isdigit toupper atol])
AC_CHECK_FUNCS([strcmp strdup strlen])
AC_CHECK_FUNCS([fileno isatty])
AC_CHECK_FUNCS([feof fflush fprintf])
AC_CHECK_FUNCS([getc ungetc])
AC_CHECK_FUNCS([malloc free])

dnl
dnl Check for gmp
dnl
AC_CHECK_LIB(gmp, __gmpz_init, ,
	[AC_MSG_ERROR([GNU MP not found, see http://gmplib.org/])])

dnl
dnl Check for diff(1) utility (needed to run the tests but not to compile)
dnl
AC_ARG_VAR([DIFF],[the 'diff' program to use for test output comparison])
AC_PATH_PROG([DIFF],[diff],[])
if test "x$DIFF" = "x"; then
	AC_MSG_NOTICE([==> diff command not found!])
	AC_MSG_NOTICE([==> Set DIFF variable if present in non-standard path!])
	AC_MSG_NOTICE([diff is mandatory to run the tests])
fi

dnl
dnl Check for cppcheck utility (needed to run some static analysis tests but not to compile)
dnl
AC_ARG_VAR([CPPCHECK],[the 'cppcheck' program is used for some static analysis])
AC_PATH_PROG([CPPCHECK],[cppcheck],[])
if test "x$CPPCHECK" = "x"; then
	AC_MSG_NOTICE([==> cppcheck command not found! See http://cppcheck.sourceforge.net/ ])
	AC_MSG_NOTICE([==> Set CPPCHECK variable if present in non-standard path!])
	AC_MSG_NOTICE([cppcheck static analysis will be skipped.])
fi

if test $cross_compiling = yes; then
	case "$host" in
		*-*-mingw* | *-*-cygwin)
			dnl
			dnl Check for wine
			dnl
			AC_ARG_VAR([WINE],[the 'wine' program for running mingw/cygwin executables.])
			AC_PATH_PROG([WINE],['wine'],[])
			if test "x$WINE" = "x"; then
				AC_MSG_NOTICE([==> wine command not found! see http://www.winehq.org/])
				AC_MSG_NOTICE([==> Set WINE variable if present in non-standard path!])
				AC_MSG_NOTICE([Tests will probably not work without wine.])
			fi
		;;
	esac
else
	dnl
	dnl Check for valgrind (needed to run memory tests but not to run functional tests)
	dnl
	AC_ARG_VAR([VALGRIND],[the 'valgrind' program to use for memory checking.])
	AC_PATH_PROG([VALGRIND],[valgrind],[])
	if test "x$VALGRIND" = "x"; then
		AC_MSG_NOTICE([==> valgrind command not found! See http://valgrind.org/ ])
		AC_MSG_NOTICE([==> Set VALGRIND variable if present in non-standard path!])
		AC_MSG_NOTICE([Memory leak/error testing will be skipped.])
	fi
fi

dnl short name for older file systems that only support 8.3 filenames
AC_CONFIG_HEADERS([config.h:config.in])

AC_CONFIG_FILES([Makefile examples/Makefile lib/Makefile man/Makefile src/Makefile tests/Makefile])

AC_OUTPUT

