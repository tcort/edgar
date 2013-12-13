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

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "const.h"
#include "scanner.h"

char *scanner_next_token(FILE *f) {

	int c;
	int pos;
	char buf[MAX_ATOM_LEN];

	memset(buf, '\0', MAX_ATOM_LEN);
	while ((c = getc(f)) != EOF) {

		switch (c) {
			case ';':
				do {
					/* skip comments */
					c = getc(f);
				} while (!feof(f) && c != '\n');
				continue;
			case ' ':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
			case '\v':
				/* skip whitespace */
				continue;
			case '(':
				return strdup("(");
			case ')':
				return strdup(")");
			case '"':
				/* read quoted string */
				for (pos = 0; pos < MAX_ATOM_LEN; pos++) {
					c = getc(f);
					if (c == '"') {
						buf[pos] = '\0';
						return strdup(buf);
					}
					buf[pos] = c;
				}
				buf[MAX_ATOM_LEN - 1] = '\0';
				return strdup(buf);

			default:
				ungetc(c, f);
				for (pos = 0; pos < MAX_ATOM_LEN; pos++) {

					buf[pos] = getc(f);

					if (feof(f)) {
						break;
					} else if (buf[pos] == ' '  ||
							buf[pos] == '\f' ||
							buf[pos] == '\n' ||
							buf[pos] == '\r' ||
							buf[pos] == '\t' ||
							buf[pos] == '\v' ||
							buf[pos] == ';'  ||
							buf[pos] == '('  ||
							buf[pos] ==')') {

						ungetc(buf[pos], f);
						break;
					}

					buf[pos] = toupper(buf[pos]);
				}

				/* remove delimiter from buffer, ensure EOS */
				buf[pos] = '\0';
				return strdup(buf);
		}
	}

	return NULL;
}

