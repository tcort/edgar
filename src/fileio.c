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

#include "fileio.h"
#include "obj.h"
#include "repl.h"

int load_file(char *filename, obj_t * env) {

	FILE *f;

	f = fopen(filename, "r");
	if (f == NULL) {
		return -1;
	}

	repl(f, env, 1);

	fclose(f);
	return 0;
}
