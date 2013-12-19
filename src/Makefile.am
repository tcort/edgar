# edgar - a small LISP Interpreter written in C
# Copyright (c) 2013 Thomas Cort <linuxgeek@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AM_CPPFLAGS = -Wall -Wextra -pedantic -O2

bin_PROGRAMS = edgar

noinst_LIBRARIES = libedgar.a
libedgar_a_SOURCES = \
	apply.c			apply.h			\
				const.h			\
	env.c			env.h			\
	eval.c			eval.h			\
	fileio.c		fileio.h		\
	builtin.c		builtin.h		\
	number.c		number.h		\
	obj.c			obj.h			\
	parser.c		parser.h		\
	repl.c			repl.h			\
	scanner.c 		scanner.h

edgar_SOURCES = \
	main.c
edgar_LDADD = libedgar.a
