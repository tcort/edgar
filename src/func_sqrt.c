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
#include <stdlib.h>
#include <string.h>

#include "func_sqrt.h"
#include "obj.h"

obj_t * func_sqrt(obj_t *args, obj_t *env) {

	char *s;
	char *op;
	mpz_t sqrtz;
	mpz_t opz;

	if (!IS_LIST(args)) {
		fprintf(stdout, "SQRT: expected argument list\n");
		return alloc_fail();
	} else if (!IS_NIL(CDR(args))) {
		fprintf(stdout, "SQRT: expected 1 argument\n");
		return alloc_fail();
	} else if (!IS_INT(CAR(args))) {
		fprintf(stdout, "SQRT: argument must be an INT\n");
		return alloc_fail();
	}

	op = strdup(ATOM(CAR(args)));
	if (op[0] == '+') {
		/* gmp doesn't like leading +, but 0 is OK */
		op[0] = '0';
	} else if (op[0] == '-') {
		free(op);
		fprintf(stdout, "SQRT: argument must be non-negative\n");
		return alloc_fail();
	}

	mpz_init_set_str(sqrtz, "0", 10);
	mpz_init_set_str(opz, op, 10);
	
	mpz_sqrt(sqrtz, opz);

	mpz_clear(opz);
	free(op);

	s = mpz_get_str(NULL, 10, sqrtz);
	mpz_clear(sqrtz);

	return alloc_atom(s);
}
