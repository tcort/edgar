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

#include "config.h"

#define _BSD_SOURCE

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

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

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

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

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

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

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

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

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

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

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

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) != 2) {
		fprintf(stdout, "EQUAL: expected 2 arguments\n");
		return alloc_fail();
	}

	return compare_obj(CAR(args), CADR(args));
}

obj_t * func_greater(obj_t *args, obj_t *env) {

	int r;

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) != 2) {
		fprintf(stdout, "GREATER: expected 2 argument\n");
		return alloc_fail();
	}

	r = edgar_cmp(CAR(args), CADR(args), "GREATER");
	if (r == 77) {
		return alloc_fail();
	} else if (r > 0) {
		return alloc_t();
	} else {
		return alloc_nil();
	}
}

obj_t * func_int(obj_t *args, obj_t *env) {

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

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

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) != 2) {
		fprintf(stdout, "LESS: expected 2 argument\n");
		return alloc_fail();
	}

	r = edgar_cmp(CAR(args), CADR(args), "LESS");
	if (r == 77) {
		return alloc_fail();
	} else if (r < 0) {
		return alloc_t();
	} else {
		return alloc_nil();
	}
}

obj_t * func_arith(obj_t *args, obj_t *env, obj_t * (*f)(obj_t *, obj_t *)) {
	obj_t * result = NULL, *cur, *rest;

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	for (cur = CAR(args), rest = CDR(args), result = clone_obj(cur); IS_LIST(rest); rest = CDR(rest)) {
		obj_t * new_result = f(result, CAR(rest));
		free_obj(result);
		result = new_result;

		if (IS_FAIL(result)) {
			break;
		}
	}

	return result;
}

obj_t * func_minus(obj_t *args, obj_t *env) {

	obj_t *result;

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) < 1) {
		fprintf(stdout, "MINUS: expected at least 1 argument\n");
		return alloc_fail();
	}

	if (list_length(args) == 1) {
		obj_t *def_val = alloc_atom(strdup("0"));

		result = edgar_minus(def_val, CAR(args));
		free_obj (def_val);
	} else {
		result = func_arith(args, env, edgar_minus);
	}

	return result;
}

obj_t * func_plus(obj_t *args, obj_t *env) {

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) < 1) {
		fprintf(stdout, "PLUS: expected at least 1 argument\n");
		return alloc_fail();
	}

	return func_arith(args, env, edgar_plus);
}

obj_t * func_quotient(obj_t *args, obj_t *env) {

	obj_t *result;

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) < 1) {
		fprintf(stdout, "QUOTIENT: expected at least 1 argument\n");
		return alloc_fail();
	}

	if (list_length(args) == 1) {
		obj_t *def_val = alloc_atom(strdup("1"));

		result = edgar_quotient(def_val, CAR(args));
		free_obj (def_val);
	} else {
		result = func_arith(args, env, edgar_quotient);
	}

	return result;
}

obj_t * func_remainder(obj_t *args, obj_t *env) {

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) < 2) {
		fprintf(stdout, "REMAINDER: expected at least 2 arguments\n");
		return alloc_fail();
	}

	return func_arith(args, env, edgar_remainder);
}

obj_t * func_setq(obj_t *args, obj_t *env) {

	int r;

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

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

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) < 1) {
		fprintf(stdout, "TIMES: expected at least 1 argument\n");
		return alloc_fail();
	}

	return func_arith(args, env, edgar_times);
}

obj_t * func_if(obj_t *args, obj_t *env) {

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	if (list_length(args) != 3) {
		fprintf(stdout, "IF: expected 3 arguments\n");
		return alloc_fail();
	}

	obj_t *predResult = eval(CAR(args), env);
	obj_t *onTrue = CAR(CDR(args));
	obj_t *onFalse = CAR(CDR(CDR(args)));

	if (IS_T(predResult)) {
		free_obj(predResult);
		return eval(onTrue, env);
	}

	free_obj(predResult);

	return eval(onFalse, env);
}

obj_t * func_print(obj_t *args, obj_t *env) {

	if (args == NULL || env == NULL) {
		return alloc_fail();
	}

	print_list(args);
	fprintf(stdout, "\n");

	return alloc_nil();
}
