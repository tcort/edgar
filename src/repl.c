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

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "const.h"
#include "env.h"
#include "eval.h"
#include "fileio.h"
#include "obj.h"
#include "parser.h"
#include "repl.h"

/*
 * Determine if the session is interactive (i.e. user at a tty) or if
 * we're reading from a file or pipe. This is used to determine if a
 * prompt should be displayed.
 */
int is_interactive(FILE *f) {
	int fd;

	fd = fileno(f);
	if (fd == -1) {
		return 0;
	}

	return isatty(fd);
}

void repl(FILE *f, obj_t * env, int silent) {

	do {
		obj_t * in;
		obj_t * out;

		if (is_interactive(f)) {
			fprintf(stdout, "%s", REPL_PROMPT);
			fflush(stdout);
		}

		/* read */
		in = parse(f);
		if (feof(f) != 0) {
			free_obj(in);
			break;
		}

		/* eval */
		out = eval(in, env);
		free_obj(in);

		if (!silent) {
			/* print */
			print_obj(out);
		}
		free_obj(out);

		/* loop */
	} while (feof(f) == 0);
}
