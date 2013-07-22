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
#include "func_remainder.h"
#include "number.h"
#include "obj.h"

obj_t * func_remainder(obj_t *args, obj_t *env) {

	int r;
	mpz_t op1z;
	mpz_t op2z;
	mpz_t result;
	char *s, *s1, *s2;

	obj_t *cur;

	if (!IS_LIST(args)) {
		fprintf(stdout, "REMAINDER: expected argument list\n");
		return alloc_fail();
	} else if (!IS_NIL(CDR(CDR(args)))) {
		fprintf(stdout, "REMAINDER: expected 2 arguments\n");
		return alloc_fail();
	} else if (!IS_INT(CAR(args)) && !IS_INT(CADR(args))) {
		fprintf(stdout, "REMAINDER: arguments must be INTs.\n");
		return alloc_fail();
	}

	s1 = strdup(ATOM(CAR(args)));
	mpz_init_set_str(op1z, s1, 10);

	s2 = strdup(ATOM(CADR(args)));
	mpz_init_set_str(op2z, s2, 10);

	if (mpz_cmp_si(op2z, 0) == 0) {
		mpz_clear(op1z);
		mpz_clear(op2z);
		free(s1);
		free(s2);
		fprintf(stdout, "REMAINDER: Divide by 0 Error.\n");
		return alloc_fail();
	}

	mpz_init(result);
	mpz_mod(result, op1z, op2z);

	mpz_clear(op1z);
	mpz_clear(op2z);

	free(s1);
	free(s2);

	s =  mpz_get_str(NULL, 10, result);
	mpz_clear(result);

	return number_filter(alloc_atom(s));
}
