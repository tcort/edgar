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

TESTS = \
	atom.sh		\
	car.sh		\
	cdr.sh		\
	cond.sh		\
	cons.sh		\
	defun.sh	\
	equal.sh	\
	greater.sh	\
	int.sh		\
	minus.sh	\
	misc.sh		\
	null.sh		\
	less.sh		\
	libedgar.sh	\
	plus.sh		\
	quote.sh	\
	quotient.sh	\
	remainder.sh	\
	setq.sh		\
	sqrt.sh		\
	times.sh

TESTS_ENVIRONMENT = \
	DIFF=$(DIFF)						\
	EDGAR=$(top_builddir)/src/edgar				\
	EDGAR_LIB_DIR=$(top_srcdir)/lib				\
	EDGAR_IN_DIR=$(top_srcdir)/tests			\
	EDGAR_OUT_DIR=$(top_builddir)/tests			\
	EDGAR_EX_DIR=$(top_srcdir)/tests			\
	TEST_RUNNER=$(top_srcdir)/tests/test-runner.sh		\
	VALGRIND=$(VALGRIND)					\
	WINE=$(WINE)

CLEANFILES = \
	atom.out	atom.out.stdout		atom.out.stderr		\
	car.out		car.out.stdout		car.out.stderr		\
	cdr.out		cdr.out.stdout		cdr.out.stderr		\
	cond.out	cond.out.stdout		cond.out.stderr		\
	cons.out 	cons.out.stdout		cons.out.stderr		\
	defun.out	defun.out.stdout	defun.out.stderr	\
	equal.out	equal.out.stdout	equal.out.stderr	\
	greater.out	greater.out.stdout	greater.out.stderr	\
	int.out		int.out.stdout		int.out.stderr		\
	minus.out	minus.out.stdout	minus.out.stderr	\
	misc.out	misc.out.stdout		misc.out.stderr		\
	null.out	null.out.stdout		null.out.stderr		\
	less.out	less.out.stdout		less.out.stderr		\
	libedgar.out	libedgar.out.stdout	libedgar.out.stderr	\
	plus.out	plus.out.stdout		plus.out.stderr		\
	quote.out	quote.out.stdout	quote.out.stderr	\
	quotient.out	quotient.out.stdout	quotient.out.stderr	\
	remainder.out	remainder.out.stdout	remainder.out.stderr	\
	setq.out	setq.out.stdout		setq.out.stderr		\
	sqrt.out	sqrt.out.stdout		sqrt.out.stderr		\
	times.out	times.out.stdout	times.out.stderr

EXTRA_DIST = test-runner.sh $(TESTS)		\
	atom.in		atom.ex			\
	car.in		car.ex			\
	cdr.in		cdr.ex			\
	cons.in		cons.ex			\
	cond.in		cond.ex			\
	defun.in	defun.ex		\
	equal.in	equal.ex		\
	greater.in	greater.ex		\
	int.in		int.ex			\
	minus.in	minus.ex		\
	misc.in		misc.ex			\
	null.in		null.ex			\
	less.in		less.ex			\
	libedgar.in	libedgar.ex		\
	plus.in		plus.ex			\
	quote.in	quote.ex		\
	quotient.in	quotient.ex		\
	remainder.in	remainder.ex		\
	setq.in		setq.ex			\
	sqrt.in		sqrt.ex			\
	times.in	times.ex
