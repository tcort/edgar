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

#ifndef __OBJ_H
#define __OBJ_H

#include <string.h>

typedef struct obj {
	enum obj_type {ATOM=0x01, LIST=0x02, FUNC=0x04, DEFUNC=0x08} type;
	union val {
		struct pair {
			struct obj *car;
			struct obj *cdr;
		} pair;
		char *atom;
		struct obj * (*func)(struct obj *, struct obj *);
		struct defun {
			char *name;
			struct obj *args;
			struct obj *body;
		} defun;
	} val;
} obj_t;

/* Simple accessors for various fields */
#define CAR(x) (x->val.pair.car)
#define CDR(x) (x->val.pair.cdr)
#define CADR(x) CAR(CDR(x))
#define ATOM(x) (x->val.atom)
#define FUNC(x) (x->val.func)
#define NAME(x) (x->val.defun.name)
#define ARGS(x) (x->val.defun.args)
#define BODY(x) (x->val.defun.body)

#include "number.h"

/* Simple type checking and utility functions */
#define IS_LIST(x) (x != NULL && x->type == LIST)
#define IS_ATOM(x) (x != NULL && x->type == ATOM)
#define IS_FUNC(x) (x != NULL && x->type == FUNC)
#define IS_DEFUNC(x) (x != NULL && x->type == DEFUNC)
#define IS_INT(x) (x != NULL && IS_ATOM(x) && is_int_obj(x))
#define ATOM_EQ(a,s) (IS_ATOM(a) && strlen(ATOM(a)) == strlen(s) && strcmp(ATOM(a), s) == 0)
#define IS_T(x) ATOM_EQ(x,"T")
#define IS_NIL(x) ATOM_EQ(x,"NIL")
#define IS_FAIL(x) ATOM_EQ(x, "#FAIL")

obj_t * alloc_t(void);
obj_t * alloc_nil(void);
obj_t * alloc_fail(void);
obj_t * alloc_atom(char *val);
obj_t * alloc_list(obj_t *car, obj_t *cdr);
obj_t * alloc_func(obj_t * (*func)(obj_t *, obj_t *));
obj_t * alloc_defunc(char * name, obj_t *args, obj_t *body);
obj_t * clone_obj(obj_t *o);
obj_t * compare_obj(obj_t *o1, obj_t *o2);
void replace_obj(obj_t *replace, obj_t *with, obj_t *exp);
void append_obj(obj_t *list, obj_t *o);
void free_obj(obj_t *o);
void print_obj(obj_t *o);
unsigned long list_length(obj_t *o);

#endif /* __OBJ_H */
