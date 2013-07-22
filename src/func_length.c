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

#include <assert.h>
#include <gmp.h>
#include <stdio.h>

#include "func_length.h"
#include "number.h"
#include "obj.h"

obj_t * func_length(obj_t *args, obj_t *env) {

	char *s;

	mpz_t lenz;
	mpz_t onez;

	obj_t *cur;

	if (!IS_LIST(args)) {
		fprintf(stdout, "LENGTH: expected argument list\n");
		return alloc_fail();
	} else if (!IS_LIST(CAR(args))) {
		fprintf(stdout, "LENGTH: expected list argument\n");
		return alloc_fail();
	} else if (!IS_NIL(CDR(args))) {
		fprintf(stdout, "LENGTH: expected 1 argument\n");
		return alloc_fail();
	}

	mpz_init_set_str(lenz, "0", 10);
	mpz_init_set_str(onez, "1", 10);

	for (cur = CAR(args); IS_LIST(cur); cur = CDR(cur)) {
		mpz_add(lenz, lenz, onez);
	}

	s = mpz_get_str(NULL, 10, lenz);
	mpz_clear(onez);
	mpz_clear(lenz);

	return number_filter(alloc_atom(s));
}
