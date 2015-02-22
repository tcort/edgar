#!/bin/sh
# edgar - a small LISP Interpreter written in C
# Copyright (c) 2013, 2014, 2015 Thomas Cort <linuxgeek@gmail.com>
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

if test "x${CPPCHECK}" = "x"; then
	# no cppcheck program, skipping test
	exit 77
fi

${CPPCHECK} -I. --enable=all --error-exitcode=1 --quiet ${TOP_SRCDIR}/src/*.c
RESULT=$?
if [ $RESULT -gt 0 ]
then
	exit 1
else
	exit 0
fi
