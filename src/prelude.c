/*
 * edgar - a small LISP Interpreter written in C
 * Copyright (c) 2013, 2014, 2015, 2020 Thomas Cort <linuxgeek@gmail.com>
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

#include <stdio.h>

#include "obj.h"
#include "repl.h"

struct prelude {
	char *sexp;
};

struct prelude defs[] = {

	{ "(DEFUN 1+ (X) (PLUS X 1))" },
	{ "(DEFUN 1- (X) (MINUS X 1))" },
	{ "(DEFUN ABS (X) (COND ((LESS X 0) (MINUS 0 X)) (T X)))" },
	{ "(DEFUN GCD (X Y) (COND ((EQUAL Y 0) X) (T (GCD Y (REMAINDER X Y)))))" },
	{ "(DEFUN LCM (X Y) (QUOTIENT (ABS (TIMES X Y)) (GCD X Y)))" },
	{ "(DEFUN LENGTH (X) (COND ((NULL X) 0) ((ATOM X) 1) (T (PLUS (LENGTH (CDR X)) 1))))" },
	{ "(DEFUN NULL (X) (EQUAL X NIL))" },
	{ "(DEFUN ZEROP (X) (EQUAL X 0))" }
};

#define NDEFS (sizeof(defs)/sizeof(defs[0]))

int load_prelude(obj_t * env) {

	int i;

	for (i = 0; i < NDEFS; i++) {
		FILE *f;

		f = fmemopen(defs[i].sexp, strlen(defs[i].sexp), "r");
		if (f == NULL) {
			return -1;
		}
		repl(f, env, 1);
		fclose(f);
	}

	return 0;

}
