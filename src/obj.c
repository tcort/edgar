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
#include <ctype.h>
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"

static void print_atom(obj_t *atom);
static void print_func(obj_t *func);
static void print_defunc(obj_t *defunc);

static obj_t *clone_atom(obj_t *o);
static obj_t *clone_list(obj_t *o);
static obj_t *clone_func(obj_t *o);
static obj_t *clone_defunc(obj_t *o);

static obj_t * compare_atom(obj_t *o1, obj_t *o2);
static obj_t * compare_list(obj_t *o1, obj_t *o2);
static obj_t * compare_func(obj_t *o1, obj_t *o2);
static obj_t * compare_defunc(obj_t *o1, obj_t *o2);

static void free_atom(obj_t *o);
static void free_list(obj_t *o);
static void free_func(obj_t *o);
static void free_defunc(obj_t *o);

obj_t * alloc_t(void) {

	return alloc_atom(strdup("T"));
}

obj_t * alloc_nil(void) {

	return alloc_atom(strdup("NIL"));
}

obj_t * alloc_fail(void) {

	return alloc_atom(strdup("#FAIL"));
}

obj_t * alloc_atom(char *val) {

	obj_t *new_atom;

	new_atom = (obj_t *) malloc(sizeof(obj_t));
	assert(new_atom != NULL);

	new_atom->type = ATOM;
	ATOM(new_atom) = val;

	return new_atom;
}

obj_t * alloc_list(obj_t *car, obj_t *cdr) {

	obj_t *new_list;

	new_list = (obj_t *) malloc(sizeof(obj_t));
	assert(new_list != NULL);

	new_list->type = LIST;
	CAR(new_list) = car;
	CDR(new_list) = cdr;

	return new_list;
}

obj_t * alloc_func(obj_t * (*func)(obj_t *, obj_t *)) {

	obj_t *new_func;

	new_func = (obj_t *) malloc(sizeof(obj_t));
	assert(new_func != NULL);

	new_func->type = FUNC;
	FUNC(new_func) = func;

	return new_func;
}

obj_t * alloc_defunc(char *name, obj_t *args, obj_t *body) {

	obj_t *new_defunc;

	new_defunc = (obj_t *) malloc(sizeof(obj_t));
	assert(new_defunc != NULL);

	new_defunc->type = DEFUNC;
	NAME(new_defunc) = name;
	ARGS(new_defunc) = args;
	BODY(new_defunc) = body;

	return new_defunc;
}

void replace_obj(obj_t *replace, obj_t *with, obj_t *exp) {

	obj_t * r;
	
	if (IS_LIST(exp)) {

		r = compare_obj(replace, CAR(exp));
		if (IS_T(r)) {
			free_obj(CAR(exp));
			CAR(exp) = clone_obj(with);
		} else {
			replace_obj(replace, with, CAR(exp));
		}
		free_obj(r);

		r = compare_obj(replace, CDR(exp));
		if (IS_T(r)) {
			free_obj(CDR(exp));
			CDR(exp) = clone_obj(with);
		} else {
			replace_obj(replace, with, CDR(exp));
		}
		free_obj(r);
	}
}

static void print_atom(obj_t *atom) {

	assert(IS_ATOM(atom));

	fprintf(stdout, "%s", ATOM(atom));
}

static void print_func(obj_t *func) {

	assert(IS_FUNC(func));

	fprintf(stdout, "<FUNC>");
}

static void print_defunc(obj_t *defunc) {

	assert(IS_DEFUNC(defunc));

	fprintf(stdout, "<DEFUNC:");
	fprintf(stdout, "[name=%s][args=(", NAME(defunc));
	print_list(ARGS(defunc));
	fprintf(stdout, ")]");
	fprintf(stdout, "[body=(");
	print_list(BODY(defunc));
	fprintf(stdout, ")]>");
}

void print_list(obj_t *list) {

	assert(IS_LIST(list));

	if (IS_LIST(CAR(list))) {
		fprintf(stdout, "(");
		print_list(CAR(list));
		fprintf(stdout, ")");
	} else if (IS_ATOM(CAR(list))) {
		print_atom(CAR(list));
	} else if (IS_FUNC(CAR(list))) {
		print_func(CAR(list));
	} else if (IS_DEFUNC(CAR(list))) {
		print_defunc(CAR(list));
	}

	if (!IS_NIL(CDR(list))) {
		fprintf(stdout, " ");
		if (IS_ATOM(CDR(list))) {
			print_obj(CDR(list));
		} else if (IS_LIST(CDR(list))) {
			print_list(CDR(list));
		} else if (IS_FUNC(CDR(list))) {
			print_func(CDR(list));
		} else if (IS_DEFUNC(CDR(list))) {
			print_defunc(CDR(list));
		}
	}
}

void print_obj(obj_t *o) {

	assert(IS_ATOM(o) || IS_LIST(o) || IS_FUNC(o) || IS_DEFUNC(o));

	if (IS_ATOM(o)) {
		print_atom(o);
	} else if (IS_LIST(o)) {
		fprintf(stdout, "(");
		print_list(o);
		fprintf(stdout, ")");
	} else if (IS_FUNC(o)) {
		print_func(o);
	} else if (IS_DEFUNC(o)) {
		print_defunc(o);
	}

	fprintf(stdout, "\n");
}

static obj_t *clone_atom(obj_t *o) {

	obj_t *c;

	assert(IS_ATOM(o));

	c = alloc_atom(strdup(ATOM(o)));

	return c;
}

static obj_t *clone_func(obj_t *o) {

	obj_t *c;

	assert(IS_FUNC(o));

	c = alloc_func(FUNC(o));

	return c;
}

static obj_t *clone_defunc(obj_t *o) {

	obj_t *c;

	assert(IS_DEFUNC(o));

	c = alloc_defunc(strdup(NAME(o)), clone_obj(ARGS(o)), clone_obj(BODY(o)));

	return c;
}


static obj_t *clone_list(obj_t *o) {

	obj_t *c;

	assert(IS_LIST(o));

	c = alloc_list(clone_obj(CAR(o)), clone_obj(CDR(o)));

	return c;
}

obj_t *clone_obj(obj_t *o) {

	obj_t *c;

	c = NULL;

	assert(IS_ATOM(o) || IS_LIST(o) || IS_FUNC(o) || IS_DEFUNC(o));

	if (IS_ATOM(o)) {
		c = clone_atom(o);
	} else if (IS_LIST(o)) {
		c = clone_list(o);
	} else if (IS_FUNC(o)) {
		c = clone_func(o);
	} else if (IS_DEFUNC(o)) {
		c = clone_defunc(o);
	}

	assert(c != NULL);

	return c;
}

void append_obj(obj_t *list, obj_t *o) {

	obj_t *cur;

	assert(IS_LIST(list));
	assert(o != NULL);

	if (IS_FAIL(CAR(list))) {
		/* warning: first item should never be #FAIL */
		free_obj(CAR(list));
		CAR(list) = clone_obj(o);
		return;
	}

	for (cur = list; !(IS_NIL(CDR(cur))); cur = CDR(cur)) {
		/* find end of list */;
	}

	free_obj(CDR(cur));
	CDR(cur) = alloc_list(clone_obj(o), alloc_nil());
}

static obj_t * compare_atom(obj_t *o1, obj_t *o2) {

	int r;
	mpz_t o1z;
	mpz_t o2z;

	assert(IS_ATOM(o1) && IS_ATOM(o2));

	if (strlen(ATOM(o1)) == strlen(ATOM(o2)) &&
			strcmp(ATOM(o1), ATOM(o2)) == 0) {

		return alloc_t();
	} else {
		return alloc_nil();
	}
}

static obj_t * compare_list(obj_t *o1, obj_t *o2) {

	int eq;
	obj_t *car_eq;
	obj_t *cdr_eq;

	assert(IS_LIST(o1) && IS_LIST(o2));

	car_eq = compare_obj(CAR(o1), CAR(o2));
	cdr_eq = compare_obj(CDR(o1), CDR(o2));

	eq = (IS_T(car_eq) && IS_T(cdr_eq));

	free_obj(car_eq);
	free_obj(cdr_eq);

	if (eq) {
		return alloc_t();
	} else {
		return alloc_nil();
	}
}

static obj_t * compare_func(obj_t *o1, obj_t *o2) {

	assert(IS_FUNC(o1) && IS_FUNC(o2));

	if (FUNC(o1) == FUNC(o2)) {
		return alloc_t();
	} else {
		return alloc_nil();
	}
}

static obj_t * compare_defunc(obj_t *o1, obj_t *o2) {

	int eq;
	int name_eq;
	obj_t *args_eq;
	obj_t *body_eq;

	assert(IS_DEFUNC(o1) && IS_DEFUNC(o2));

	name_eq = (strcmp(NAME(o1), NAME(o2)) == 0);
	args_eq = compare_obj(ARGS(o1), ARGS(o2));
	body_eq = compare_obj(BODY(o1), BODY(o2));

	eq = (name_eq && IS_T(args_eq) && IS_T(body_eq));

	free_obj(args_eq);
	free_obj(body_eq);

	if (eq) {
		return alloc_t();
	} else {
		return alloc_nil();
	}


}

obj_t * compare_obj(obj_t *o1, obj_t *o2) {

	assert(o1 != NULL && o2 != NULL);

	/* simple equality, o1 *is* o2 */
	if (o1 == o2) {
		return alloc_t();
	} else if (IS_ATOM(o1) && IS_ATOM(o2)) {
		return compare_atom(o1, o2);
	} else if (IS_LIST(o1) && IS_LIST(o2)) {
		return compare_list(o1, o2);
	} else if (IS_FUNC(o1) && IS_FUNC(o2)) {
		return compare_func(o1, o2);
	} else if (IS_DEFUNC(o1) && IS_DEFUNC(o2)) {
		return compare_defunc(o1, o2);
	} else {
		/* not even the same types of objects */
		return alloc_nil();
	}

}

static void free_atom(obj_t *o) {

	assert(IS_ATOM(o));

	free(ATOM(o));
	free(o);
}

static void free_list(obj_t *o) {

	assert(IS_LIST(o));

	free_obj(CAR(o));
	free_obj(CDR(o));
	free(o);
}

static void free_func(obj_t *o) {

	assert(IS_FUNC(o));

	free(o);
}

static void free_defunc(obj_t *o) {

	assert(IS_DEFUNC(o));

	free(NAME(o));
	free_obj(ARGS(o));
	free_obj(BODY(o));
	free(o);
}

void free_obj(obj_t *o) {

	assert(IS_ATOM(o) || IS_LIST(o) || IS_FUNC(o) || IS_DEFUNC(o));

	if (IS_ATOM(o)) {
		free_atom(o);
	} else if (IS_LIST(o)) {
		free_list(o);
	} else if (IS_FUNC(o)) {
		free_func(o);
	} else if (IS_DEFUNC(o)) {
		free_defunc(o);
	}
}

unsigned long list_length(obj_t *o) {

	obj_t *cur;
	unsigned long length;

	length = 0;
	if (!IS_LIST(o)) {
		return length;
	}

	for (cur = o; IS_LIST(cur); cur = CDR(cur)) {
		length++;
	}

	return length;
}
