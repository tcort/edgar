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
#include <stdio.h>

#include <assert.h>
#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "const.h"
#include "builtin.h"
#include "env.h"
#include "eval.h"
#include "obj.h"

obj_t * func_atom(obj_t *args, obj_t *env) {

	if (list_length(args) != 1) {
		fprintf(stdout, "ATOM: expected 1 argument\n");
		return alloc_fail();
	}

	if (IS_ATOM(CAR(args))) {
		return alloc_t();
	} else {
		return alloc_nil();
	}
}

obj_t * func_car(obj_t *args, obj_t *env) {

	if (list_length(args) != 1) {
		fprintf(stdout, "CAR: expected 1 argument\n");
		return alloc_fail();
	}

	if (!IS_LIST(CAR(args))) {
		fprintf(stdout, "CAR: expected list argument\n");
		return alloc_fail();
	}

	return clone_obj(CAR(CAR(args)));
}

obj_t * func_cdr(obj_t *args, obj_t *env) {

	if (list_length(args) != 1) {
		fprintf(stdout, "CDR: expected 1 argument\n");
		return alloc_fail();
	}

	if (!IS_LIST(CAR(args))) {
		fprintf(stdout, "CDR: expected list argument\n");
		return alloc_fail();
	}

	return clone_obj(CDR(CAR(args)));
}

obj_t * func_cond(obj_t *args, obj_t *env) {

	obj_t *pred, *exp, *cur, *pair, *result;

	if (!IS_LIST(args)) {
		fprintf(stdout, "COND: expected argument list\n");
		return alloc_fail();
	}

	for (cur = args; IS_LIST(cur); cur = CDR(cur)) {

		pair = CAR(cur);

		if (IS_LIST(pair)) {
			pred = CAR(pair);
			exp = CADR(pair);

			result = eval(pred, env);

			if (IS_T(result)) {
				free_obj(result);
				result = eval(exp, env);
				return result;
			}
			free_obj(result);

		} else {
			fprintf(stdout, "COND: expecting list of lists\n");
		}
	}

	return alloc_nil();
}

obj_t * func_cons(obj_t *args, obj_t *env) {

	obj_t *x, *y;

	if (list_length(args) != 2) {
		fprintf(stdout, "CONS: expected 2 arguments\n");
		return alloc_fail();
	}
	
	if (IS_NIL(CADR(args)) || !IS_LIST(CADR(args))) {
		fprintf(stdout, "CONS: expected 2nd argument to be a list\n");
		return alloc_fail();
	}

	x = clone_obj(CAR(args));
	y = clone_obj(CADR(args));

	return alloc_list(x, y);
}

obj_t * func_equal(obj_t *args, obj_t *env) {

	if (list_length(args) != 2) {
		fprintf(stdout, "EQUAL: expected 2 arguments\n");
		return alloc_fail();
	}

	return compare_obj(CAR(args), CADR(args));
}

obj_t * func_greater(obj_t *args, obj_t *env) {

	int r;

	if (list_length(args) != 2) {
		fprintf(stdout, "GREATER: expected 2 argument\n");
		return alloc_fail();
	}

	r = cmp(CAR(args), CADR(args), "GREATER");
	if (r == 77) {
		return alloc_fail();
	} else if (r > 0) {
		return alloc_t();
	} else {
		return alloc_nil();
	}
}

obj_t * func_int(obj_t *args, obj_t *env) {

	if (list_length(args) != 1) {
		fprintf(stdout, "INT: expected 1 argument\n");
		return alloc_fail();
	}

	if (IS_INT(CAR(args))) {
		return alloc_t();
	} else {
		return alloc_nil();
	}
}

obj_t * func_less(obj_t *args, obj_t *env) {

	int r;

	if (list_length(args) != 2) {
		fprintf(stdout, "LESS: expected 2 argument\n");
		return alloc_fail();
	}

	r = cmp(CAR(args), CADR(args), "LESS");
	if (r == 77) {
		return alloc_fail();
	} else if (r < 0) {
		return alloc_t();
	} else {
		return alloc_nil();
	}
}

obj_t * func_minus(obj_t *args, obj_t *env) {

	if (list_length(args) != 2) {
		fprintf(stdout, "MINUS: expected 2 arguments\n");
		return alloc_fail();
	}

	return minus(CAR(args), CADR(args));
}

obj_t * func_plus(obj_t *args, obj_t *env) {

	if (list_length(args) != 2) {
		fprintf(stdout, "PLUS: expected 2 arguments\n");
		return alloc_fail();
	}

	return plus(CAR(args), CADR(args));
}

obj_t * func_quotient(obj_t *args, obj_t *env) {

	if (list_length(args) != 2) {
		fprintf(stdout, "QUOTIENT: expected 2 arguments\n");
		return alloc_fail();
	}

	return quotient(CAR(args), CADR(args));
}

obj_t * func_remainder(obj_t *args, obj_t *env) {

	if (list_length(args) != 2) {
		fprintf(stdout, "REMAINDER: expected 2 arguments\n");
		return alloc_fail();
	}

	return edgar_remainder(CAR(args), CADR(args));
}

obj_t * func_setq(obj_t *args, obj_t *env) {

	int r;

	if (list_length(args) != 2) {
		fprintf(stdout, "EQUAL: expected 2 arguments\n");
		return alloc_fail();
	}

	r = add_to_env(CAR(args), CAR(CDR(args)), env);

	if (r == 0) {
		return clone_obj(CAR(args));
	} else {
		return alloc_fail();
	}
}

obj_t * func_times(obj_t *args, obj_t *env) {

	if (list_length(args) != 2) {
		fprintf(stdout, "TIMES: expected 2 arguments\n");
		return alloc_fail();
	}

	return times(CAR(args), CADR(args));
}

obj_t * func_print(obj_t *args, obj_t *env) {
    print_list(args);
    fprintf(stdout,"\n");
    return alloc_nil();
}
