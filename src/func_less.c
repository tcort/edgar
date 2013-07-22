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

#define _BSD_SOURCE

#include <assert.h>
#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "const.h"
#include "func_less.h"
#include "obj.h"

obj_t * func_less(obj_t *args, obj_t *env) {

	int r;
	int first;
	mpz_t lastz;
	mpz_t curz;
	char *s;

	obj_t *cur;

	if (!IS_LIST(args)) {
		fprintf(stdout, "LESS: expected argument list\n");
		return alloc_fail();
	}

	first = 1;
	mpz_init_set_str(lastz, "0", 10);

	for (cur = args; IS_LIST(cur); cur = CDR(cur)) {

		if (IS_INT(CAR(cur))) {

			s = strdup(ATOM(CAR(cur)));

			if (first) {
				/* last not defined yet, (LESS 1) is always T */
				first = 0;
				r = -1;
			} else {
				mpz_init_set_str(curz, s, 10);
				r = mpz_cmp(lastz, curz);
				mpz_clear(curz);
			}

			mpz_clear(lastz);
			mpz_init_set_str(lastz, s, 10);

			free(s);

			if (r >= 0) {
				mpz_clear(lastz);
				return alloc_nil();
			}

		} else {
			mpz_clear(lastz);
			fprintf(stdout, "LESS: expecting INT arguments.\n");
			return alloc_fail();
		}
	}

	mpz_clear(lastz);

	return alloc_t();
}
