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
#include "func_times.h"
#include "number.h"
#include "obj.h"

obj_t * func_times(obj_t *args, obj_t *env) {

	mpz_t prodz;
	mpz_t opz;
	char *s;

	obj_t *cur;

	if (!IS_LIST(args)) {
		/* no arguments should return 1 */
		return alloc_atom(strdup("1"));
	}

	mpz_init_set_str(prodz, "1", 10);

	for (cur = args; IS_LIST(cur); cur = CDR(cur)) {

		if (IS_INT(CAR(cur))) {
			s = strdup(ATOM(CAR(cur)));
			mpz_init_set_str(opz, s, 10);
			mpz_mul(prodz, prodz, opz);
			mpz_clear(opz);
			free(s);
		} else {
			mpz_clear(prodz);
			fprintf(stdout, "TIMES: expecting INT arguments.\n");
			return alloc_fail();
		}
	}

	s = mpz_get_str(NULL, 10, prodz);
	mpz_clear(prodz);

	return number_filter(alloc_atom(s));
}
