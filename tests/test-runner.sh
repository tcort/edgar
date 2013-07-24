#!/bin/sh
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

IN=${EDGAR_IN_DIR}/${1}.in
OUT=${EDGAR_OUT_DIR}/${1}.out
EX=${EDGAR_EX_DIR}/${1}.ex
LIBEDGAR=${EDGAR_LIB_DIR}/edgar.lisp

if test "x${DIFF}" = "x"; then
	# no diff program, skipping test
	exit 77
fi

if test "x${1}" = "xlibedgar"; then
ARGS="-l ${LIBEDGAR}"
else
ARGS=""
fi

if test "x${VALGRIND}" != "x"; then
	${VALGRIND} --tool=memcheck --leak-check=yes ${EDGAR} ${IN} 1> ${OUT}.stdout 2> ${OUT}.stderr
	grep -qs "All heap blocks were freed -- no leaks are possible" ${OUT}.stderr
	RESULT=$?
	if [ $RESULT -gt 0 ]
	then
		echo "Memory leak detected"
		exit 1
	fi

	grep -qs "ERROR SUMMARY: 0 errors from 0 contexts" ${OUT}.stderr
	RESULT=$?
	if [ $RESULT -gt 0 ]
	then
		echo "Memory error detected"
		exit 1
	fi
fi

cat ${IN} | ${WINE} ${EDGAR} ${ARGS} | sed -e 's/\r$//' > ${OUT}
${DIFF} -u ${EX} ${OUT}

RESULT=$?
if [ $RESULT -gt 0 ]
then
	exit 1
else
	exit 0
fi
