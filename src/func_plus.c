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
#include "func_plus.h"
#include "obj.h"

obj_t * func_plus(obj_t *args, obj_t *env) {

	mpz_t sumz;
	mpz_t opz;
	char *s;

	obj_t *cur;

	if (!IS_LIST(args)) {
		/* no arguments should return 0 */
		return alloc_atom(strdup("0"));
	}

	mpz_init_set_str(sumz, "0", 10);

	for (cur = args; IS_LIST(cur); cur = CDR(cur)) {

		if (IS_INT(CAR(cur))) {
			s = strdup(ATOM(CAR(cur)));
			if (s[0] == '+') {
				/* gmp doesn't like leading +, but 0 is OK */
				s[0] = '0';
			}
			mpz_init_set_str(opz, s, 10);
			mpz_add(sumz, sumz, opz);
			mpz_clear(opz);
			free(s);
		} else {
			mpz_clear(sumz);
			fprintf(stdout, "PLUS: expecting INT arguments.\n");
			return alloc_fail();
		}
	}

	s = mpz_get_str(NULL, 10, sumz);
	mpz_clear(sumz);

	return alloc_atom(s);
}
