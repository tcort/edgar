/*
 * edgar - a small LISP Interpreter written in C
 * Copyright (c) 2013 Thomas Cort <linuxgeek@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "obj.h"
#include "test.h"

/*
 * This file provides unit tests for edgar. To add a new test, simply create
 * a function that utilizes some part of edgar, use the assertion macros
 * from test.h to test it, and then add a call to the function to main().
 * Try to name your functions unit_test_FILE_FUNC(). For example, the
 * function unit_test_obj_alloc_t() is used to test the alloc_t() function
 * of obj.c.
 */

/**
 * Number of successful assertions. DO NOT MANIPULATE THIS VALUE DIRECTLY.
 * This value is "owned" by the macros in test.h.
 */
int pass_count = 0;

/**
 * Number of failed assertions. DO NOT MANIPULATE THIS VALUE DIRECTLY.
 * This value is "owned" by the macros in test.h.
 */
int fail_count = 0;

void unit_test_obj_alloc_t(void) {

	obj_t *t;

	t = alloc_t();

	assertNotNull("alloc_t() failed to allocate memory.", t);
	if (t == NULL) {
		return;
	}

	assertTrue("alloc_t() should have returned an atom.", IS_ATOM(t));
	assertTrue("alloc_t() should have return an atom equal to T.", IS_T(t));

	free_obj(t);
	t = NULL;

	return;
}

int main(int argc, char *argv[]) {

	if (argc != 1) {
		fprintf(stdout, "Usage: %s\n", argv[0]);
		return FAIL;
	}

	unit_test_obj_alloc_t();

	fprintf(stdout,
		"unittests: %d Assertions, %d Passed, %d Failed\n",
		(pass_count + fail_count), pass_count, fail_count);

	if (fail_count > 0) {
		return FAIL;
	}

	return PASS;
}
