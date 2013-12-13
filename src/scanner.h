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

#ifndef __SCANNER_H
#define __SCANNER_H

#include <stdio.h>

typedef struct token {
	enum token_types { OPAREN_T, CPAREN_T, STRING_T, ATOM_T} token_type;
	char *text;
} token_t;

token_t *alloc_token(enum token_types token_type, char *text);
void free_token(token_t *token);
token_t *scanner_next_token(FILE *f);

#endif /* __SCANNER_H */
