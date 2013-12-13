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

#include <string.h>
#include <stdlib.h>

#include "obj.h"
#include "number.h"
#include "parser.h"
#include "scanner.h"

static obj_t *parse_one(FILE *f, char *peek);
static obj_t *parse_list(FILE *f);

static obj_t *parse_one(FILE *f, char *peeked) {

	char *next;

	if (peeked == NULL) {
		next = scanner_next_token(f);
	} else {
		next = peeked;
	}

	if (next == NULL) {
		return alloc_nil();
	} else if (*next == '(' && strlen(next) == 1) {
		free(next);
		return parse_list(f);
	} else {
		return number_filter(alloc_atom(next));
	}
}

static obj_t *parse_list(FILE *f) {

	char *next;

	next = scanner_next_token(f);

	if (next == NULL || (*next == ')' && strlen(next) == 1)) {
		free(next);
		return alloc_nil();
	} else {
		obj_t *car;
		obj_t *cdr;

		car = parse_one(f, next);
		cdr = parse_list(f);

		return alloc_list(car, cdr);
	}	
}

obj_t *parse(FILE *f) {

	return parse_one(f, NULL);
}
