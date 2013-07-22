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
#include "func_length.h"
#include "func_quotient.h"
#include "obj.h"

obj_t * func_quotient(obj_t *args, obj_t *env) {

	mpz_t lenz;
	mpz_t quotz;
	mpz_t opz;
	char *s;
	int first;

	obj_t *cur;
	obj_t *len;
	obj_t *lenargs;

	first = 1;

	/* get list length */
	lenargs = alloc_list(clone_obj(args), alloc_nil());
	len = func_length(lenargs, env);
	mpz_init_set_str(lenz, ATOM(len), 10);
	free_obj(len);
	free_obj(lenargs);

	if (mpz_cmp_si(lenz, 0) == 0) {

		fprintf(stdout, "QUOTIENT: expected argument list\n");
		mpz_clear(lenz);
	
		return alloc_fail();
	} else if (mpz_cmp_si(lenz, 1) == 0) {
		first = 0;
	}

	mpz_clear(lenz);

	mpz_init_set_str(quotz, "1", 10);

	for (cur = args; IS_LIST(cur); cur = CDR(cur)) {

		if (IS_INT(CAR(cur))) {
			s = strdup(ATOM(CAR(cur)));
			if (s[0] == '+') {
				/* gmp doesn't like leading +, but 0 is OK */
				s[0] = '0';
			}
			mpz_init_set_str(opz, s, 10);

			if (first && mpz_cmp_si(quotz, 0) != 0) {
				first = 0;
				mpz_cdiv_q(quotz, opz, quotz);
			} else if (mpz_cmp_si(opz, 0) != 0) {
				mpz_cdiv_q(quotz, quotz, opz);
			} else {
				fprintf(stdout, "QUOTIENT: Divide by 0 Error.\n");
				mpz_clear(opz);
				mpz_clear(quotz);
				free(s);
				return alloc_fail();
			}

			mpz_clear(opz);
			free(s);
		} else {
			fprintf(stdout, "QUOTIENT: expecting INT arguments.\n");
			mpz_clear(quotz);
			return alloc_fail();
		}
	}

	s = mpz_get_str(NULL, 10, quotz);
	mpz_clear(quotz);

	return alloc_atom(s);
}
