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
#include <string.h>

#include "builtin.h"
#include "env.h"
#include "obj.h"

int add_to_env(obj_t *key, obj_t *value, obj_t *env) {

	int r;
	obj_t *item;

	item = alloc_list(clone_obj(key), clone_obj(value));
	r = insert_env(env, item);
	free_obj(item);

	return r;
}

void add_func_to_env(char *name, obj_t * (*func)(obj_t *, obj_t *), obj_t *env) {

	obj_t *key;
	obj_t *value;

	key = alloc_atom(strdup(name));
	value = alloc_list(alloc_func(func), alloc_nil());
	add_to_env(key, value, env);
	free_obj(key);
	free_obj(value);

	return;
}

void add_def_to_env(char *name, obj_t *exp, obj_t *env) {

	obj_t *item;
	obj_t *key;
	obj_t *value;

	key = alloc_atom(strdup(name));
	value = alloc_list(clone_obj(exp), alloc_nil());
	add_to_env(key, value, env);
	free_obj(key);
	free_obj(value);

	return;
}

obj_t * alloc_env(void) {

	obj_t *env;

	env = alloc_list(alloc_fail(), alloc_nil());

	add_func_to_env("SETQ", func_setq, env);
	add_func_to_env("CAR", func_car, env);
	add_func_to_env("CDR", func_cdr, env);
	add_func_to_env("CONS", func_cons, env);
	add_func_to_env("EQUAL", func_equal, env);
	add_func_to_env("LESS", func_less, env);
	add_func_to_env("GREATER", func_greater, env);
	add_func_to_env("ATOM", func_atom, env);
	add_func_to_env("INT", func_int, env);
	add_func_to_env("COND", func_cond, env);
	add_func_to_env("PLUS", func_plus, env);
	add_func_to_env("MINUS", func_minus, env);
	add_func_to_env("TIMES", func_times, env);
	add_func_to_env("QUOTIENT", func_quotient, env);
	add_func_to_env("REMAINDER", func_remainder, env);
	add_func_to_env("IF", func_if, env);
	add_func_to_env("PRINT", func_print, env);

	return env;
}

obj_t * clone_env(obj_t *env) {
	return clone_obj(env);
}

int insert_env(obj_t *env, obj_t *o) {

	int is_duplicate;
	obj_t * query_target;
	obj_t * query_result;
	obj_t * compare_result;

	is_duplicate = 0;
	query_target = clone_obj(CAR(o));
	query_result = query_env(env, query_target);
	compare_result = compare_obj(query_target, query_result);
	if (!IS_T(compare_result)) {
		is_duplicate = 1;
	}
	free_obj(query_target);
	free_obj(query_result);
	free_obj(compare_result);

	if (is_duplicate) {
		fprintf(stdout, "Definition for %s already exists\n", ATOM(CAR(o)));
		return -1;
	}

	append_obj(env, o);

	return 0;
}

obj_t * query_env(obj_t *env, obj_t *o) {

	obj_t *cur;

	obj_t *item;
	obj_t *key;
	obj_t *value;

	assert(IS_LIST(env));
	assert(IS_ATOM(o));

	cur = env;
	while (IS_LIST(cur) && IS_LIST(CAR(cur))) {

		item = CAR(cur);
		key = CAR(item);
		value = CDR(item);

		if (strcmp(ATOM(key), ATOM(o)) == 0 && strlen(ATOM(key)) ==
				strlen(ATOM(o))) {

			/* match found */
			return clone_obj(value);
		}

		cur = CDR(cur);
	}

	return clone_obj(o);
}

void print_func_names(obj_t *env) {

	int i;
	obj_t *cur;

	obj_t *item;
	obj_t *key;
	obj_t *value;

	assert(IS_LIST(env));

	cur = env;

	i = strlen("QUOTE") + strlen("DEFUN") + 1;
	fprintf(stdout, "Built-in Functions:\nDEFUN QUOTE");
	while (IS_LIST(cur) && IS_LIST(CAR(cur))) {

		item = CAR(cur);
		key = CAR(item);
		value = CDR(item);

		if (IS_ATOM(key) && IS_FUNC(CAR(value))) {
			if (i > 32) {
				fprintf(stdout, "\n");
				i = 0;
			} else {
				fprintf(stdout, " ");
				i += 1;
			}
			fprintf(stdout, "%s", ATOM(key));
			i += strlen(ATOM(key));
		}

		cur = CDR(cur);
	}
	fprintf(stdout, "\n");
}

void print_defunc_names(obj_t *env) {

	int i;
	int first;
	obj_t *cur;

	obj_t *item;
	obj_t *key;
	obj_t *value;

	assert(IS_LIST(env));

	cur = env;

	i = 0; first = 1;
	fprintf(stdout, "Library Functions:\n");
	while (IS_LIST(cur) && IS_LIST(CAR(cur))) {

		item = CAR(cur);
		key = CAR(item);
		value = CDR(item);

		if (IS_ATOM(key) && IS_DEFUNC(CAR(value))) {
			if (i > 32) {
				fprintf(stdout, "\n");
				i = 0;
			} else if (!first) {
				fprintf(stdout, " ");
				i += 1;
			} else {
				first = 0;
			}
			fprintf(stdout, "%s", ATOM(key));
			i += strlen(ATOM(key));
		}

		cur = CDR(cur);
	}
	fprintf(stdout, "\n");
}

void free_env(obj_t *env) {
	free_obj(env);
}

/*
 * Show the environment (mostly for debug purposes)
 */
void print_env(obj_t *env) {
	fprintf(stdout, "\n");
	fprintf(stdout, "=-=-=-=-=-=-= ENVIRONMENT =-=-=-=-=-=-=\n");
	print_obj(env);
	fprintf(stdout, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

