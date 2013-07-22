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

#include <stdlib.h>
#include <string.h>

#include "number.h"

/*
 * Put numbers in a form GMP can parse.
 */
obj_t * number_filter(obj_t * o) {

	if (IS_INT(o)) {

		char *s;
		char *tmp;

		s = ATOM(o);

		/* gmp doesn't like leading '+' */
		if (s[0] == '+') {
			tmp = strdup(s+1);
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

	int i;
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
	for (i = 1; i < strlen(s); i++) {
		if (!isdigit(s[i])) {
			return 0;
		}
	}

	return 1;
}


