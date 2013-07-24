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
#include <string.h>
#include <stdio.h>

#include "apply.h"
#include "func_cond.h"
#include "env.h"
#include "eval.h"
#include "obj.h"

/*
 * Evaluate an expression.
 */
obj_t * eval(obj_t * exp, obj_t *env) {

	obj_t *r;
	obj_t *tmp;
	obj_t *cur;
	obj_t *args;
	obj_t *defunc;

	if (IS_ATOM(exp)) {

		return query_env(env, exp);

	} else if (IS_LIST(exp)) {

		/* QUOTE is special, we don't eval() the args */
		if (strcmp(ATOM(CAR(exp)), "QUOTE") == 0) {
			if (!IS_LIST(CDR(exp)) || !IS_NIL(CDR(CDR(exp)))) {
				fprintf(stdout, "QUOTE: expected 1 argument\n");
				return alloc_fail();
			}
			return clone_obj(CADR(exp));

		/* Same with DEFUN, we don't eval() the args (yet) */
		} else if (strcmp(ATOM(CAR(exp)), "DEFUN") == 0) {

			obj_t *cur;
			obj_t *key;
			obj_t *item;
			obj_t *value;
			obj_t *name;
			obj_t *args;
			obj_t *body;

			int is_duplicate;
			obj_t *query_target;
			obj_t *query_result;
			obj_t *compare_result;
			is_duplicate = 0;

			cur = CDR(exp);

			if (!IS_LIST(cur) || !IS_ATOM(CAR(cur))) {
				fprintf(stdout, "DEFUN: 2nd argument should be function name\n");
				return alloc_fail();
			}
			name = CAR(cur);
			cur = CDR(cur);

			if (!IS_LIST(cur) || !(IS_LIST(CAR(cur)) || IS_NIL(CAR(cur)))) {
				fprintf(stdout, "DEFUN: 3rd argument should be parameter list or NIL\n");
				return alloc_fail();
			}
			args = CAR(cur);
			cur = CDR(cur);

			if (!IS_LIST(cur) || !IS_LIST(CAR(cur))) {
				fprintf(stdout, "DEFUN: 4th argument should be function body (list)\n");
				return alloc_fail();
			}
			body = CAR(cur);

			query_target = alloc_atom(strdup(ATOM(name)));
			query_result = query_env(env, query_target);
			
			compare_result = compare_obj(query_target, query_result);

			if (!IS_T(compare_result)) {
				is_duplicate = 1;
			}

			free_obj(query_target);
			free_obj(query_result);
			free_obj(compare_result);

			if (is_duplicate) {
				fprintf(stdout, "Definition for %s already exists\n", ATOM(name));
				return alloc_fail();
			}

			key = clone_obj(name);
			value = alloc_defunc(strdup(ATOM(name)), clone_obj(args), clone_obj(body));
			item = alloc_list(key, alloc_list(value, alloc_nil()));
			insert_env(env, item);
			free_obj(item);

			return alloc_t();
		} else if (strcmp(ATOM(CAR(exp)), "COND") == 0) {
			return func_cond(CDR(exp), env);
		}

		/* Check if expression is a DEFUNC */
		defunc = eval(CAR(exp), env);
		if (IS_LIST(defunc) && IS_DEFUNC(CAR(defunc))) {
			obj_t *l;
			l = alloc_list(defunc, clone_obj(CDR(exp)));
			r = apply(l, env);
			free_obj(l);
			return r;
		} else {
			free_obj(defunc);
		}

		/* recursive eval() and build args list for func  */
		tmp = eval(CAR(exp), env);
		args = alloc_list(tmp, alloc_nil());
		cur = CDR(exp);

		while (!IS_NIL(cur) && IS_LIST(cur)) {
			tmp = eval(CAR(cur), env);
			append_obj(args, tmp);
			free_obj(tmp);
			cur = CDR(cur);
		}

		r = apply(args, env);

		free_obj(args);

		return r;
	} else {
		return clone_obj(exp);
	}
}
