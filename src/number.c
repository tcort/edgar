/*
 * edgar - a small LISP Interpreter written in C
 * Copyright (c) 2013, 2014, 2015 Thomas Cort <linuxgeek@gmail.com>
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include "number.h"
#include "obj.h"

/*
 * Put numbers in a form GMP can parse.
 */
obj_t * number_filter(obj_t * o) {

	if (IS_INT(o)) {

		char *s;

		s = ATOM(o);

		/* gmp doesn't like leading '+' */
		if (s[0] == '+') {
			char *tmp = strdup(s+1);
			memcpy(s, tmp, strlen(tmp) + 1);
			free(tmp);
		}

		/* Ensure 0 == +0 == -0 by rewriting all to "0" */
		if (strcmp(s, "-0") == 0 && strlen("-0") == strlen(s)) {
			s[0] = '0';
			s[1] = '\0';
		}
	}

	return o;
}

int is_int_obj(obj_t *o) {

	size_t i;
	size_t len;
	char *s;

	if (o == NULL || !IS_ATOM(o)) {
		return 0;
	}

	s = ATOM(o);
	if (s[0] == '\0') {
		return 0;
	}

	/* first byte can be sign or digit */
	if (s[0] != '+' && s[0] != '-' && !isdigit(s[0])) {
		return 0;
	}

	/* cannot have a lonely sign -- signs must be followed by a digit */
	if ((s[0] == '+' || s[0] == '-') && !isdigit(s[1])) {
		return 0;
	}

	/* rest of bytes must be digits */
	for (i = 1, len = strlen(s); i < len; i++) {
		if (!isdigit(s[i])) {
			return 0;
		}
	}

	return 1;
}

static obj_t * int_op(obj_t *op1, obj_t *op2, void (*op)(mpz_t rop, const mpz_t op1, const mpz_t op2), char *op_name) {

	mpz_t op1z;
	mpz_t op2z;
	mpz_t rz;
	char *s1;
	char *s2;
	char *sr;
	obj_t *result;

	if (!IS_INT(op1) || !IS_INT(op2)) {
		fprintf(stdout, "%s: expecting INT arguments.\n", op_name);
		return alloc_fail();
	}
	
	s1 = strdup(ATOM(op1));
	s2 = strdup(ATOM(op2));

	mpz_init_set_str(op1z, s1, 10);
	mpz_init_set_str(op2z, s2, 10);
	mpz_init_set_str(rz, "0", 10);

	(*op)(rz, op1z, op2z);
	sr = mpz_get_str(NULL, 10, rz);

	result = number_filter(alloc_atom(sr));

	mpz_clear(op1z);
	mpz_clear(op2z);
	mpz_clear(rz);

	free(s1);
	free(s2);

	return result;
}

obj_t * edgar_plus(obj_t *op1, obj_t *op2) {
	return int_op(op1, op2, mpz_add, "PLUS");
}

obj_t * edgar_minus(obj_t *op1, obj_t *op2) {
	return int_op(op1, op2, mpz_sub, "MINUS");
}

obj_t * edgar_times(obj_t *op1, obj_t *op2) {
	return int_op(op1, op2, mpz_mul, "TIMES");
}

obj_t * edgar_quotient(obj_t *op1, obj_t *op2) {
	if (IS_ATOM(op2) && strcmp(ATOM(op2), "0") == 0) {
		fprintf(stdout, "QUOTIENT: Divide by 0 Error.\n");
		return alloc_fail();
	}
	return int_op(op1, op2, mpz_cdiv_q, "QUOTIENT");
}

obj_t * edgar_remainder(obj_t *op1, obj_t *op2) {
	if (IS_ATOM(op2) && strcmp(ATOM(op2), "0") == 0) {
		fprintf(stdout, "REMAINDER: Divide by 0 Error.\n");
		return alloc_fail();
	}
	return int_op(op1, op2, mpz_mod, "REMAINDER");
}

int edgar_cmp(obj_t *op1, obj_t *op2, char *op_name) {
	mpz_t op1z;
	mpz_t op2z;
	char *s1;
	char *s2;
	int r;

	if (!IS_INT(op1) || !IS_INT(op2)) {
		fprintf(stdout, "%s: expecting INT arguments.\n", op_name);
		return 77;
	}
	
	s1 = strdup(ATOM(op1));
	s2 = strdup(ATOM(op2));

	mpz_init_set_str(op1z, s1, 10);
	mpz_init_set_str(op2z, s2, 10);

	r = mpz_cmp(op1z, op2z);

	mpz_clear(op1z);
	mpz_clear(op2z);

	free(s1);
	free(s2);

	return r;
}
