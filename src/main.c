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

#define _XOPEN_SOURCE
#define _POSIX_SOURCE

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "const.h"
#include "env.h"
#include "eval.h"
#include "obj.h"
#include "parser.h"

static int is_interfactive(FILE *f);
static void splash(obj_t * env);
static void print_help(char *progname);
static void print_version (void);

/*
 * Determine if the session is interactive (i.e. user at a tty) or if
 * we're reading from a file or pipe. This is used to determine if a
 * prompt should be displayed.
 */
static int is_interfactive(FILE *f) {
	int fd;

	fd = fileno(f);
	if (fd == -1) {
		return 0;
	}

	return isatty(fd);
}

static void splash(obj_t * env) {
	fprintf(stdout, "            _                    \n");
	fprintf(stdout, "    ___  __| | __ _  __ _ _ __   \n");
	fprintf(stdout, "   / _ \\/ _` |/ _` |/ _` | '__|  \n");
	fprintf(stdout, "  |  __/ (_| | (_| | (_| | |     \n");
	fprintf(stdout, "   \\___|\\__,_|\\__, |\\__,_|_|     \n");
	fprintf(stdout, "              |___/              \n");
	fprintf(stdout, "\n");
	fprintf(stdout, "           Version %s\n", VERSION);
	fprintf(stdout,"\n");
	print_func_names(env);
	fprintf(stdout, "\n");
}

void print_help(char *progname) {

	fprintf(stdout, "%s - a small LISP Interpreter written in C\n", PACKAGE_NAME);
	fprintf(stdout, "\n");
	fprintf(stdout, "Usage: %s [OPTION] [filename]\n", progname);
	fprintf(stdout, "\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, " -h, -?            --help                  Print a helpful message and exit\n");
	fprintf(stdout, " -v                --version               Print version information and exit\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Report bugs to %s\n", PACKAGE_BUGREPORT);

	exit(0);
}

static void print_version(void) {

	fprintf(stdout, "%s\n", PACKAGE_STRING);
	fprintf(stdout, "\n");
	fprintf(stdout, "Copyright (C) 2013  Thomas Cort <linuxgeek@gmail.com>\n");
	fprintf(stdout, "This is free software; see the source for copying conditions.  There is NO\n");
	fprintf(stdout, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Written by Thomas Cort <linuxgeek@gmail.com>\n");

	exit(0);
}

/*
 * Main Read-Eval-Print-Loop
 */
int main(int argc, char *argv[]) {

	FILE *f;
	int optc;
	obj_t *in, *out, *env;

	const char* const short_options = "h?v";
#if HAVE_GETOPT_LONG
	static const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};
#endif

#if HAVE_GETOPT_LONG
	while ((optc = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
#else
	while ((optc = getopt(argc, argv, short_options)) != -1) {
#endif
		switch (optc) {
			case 'v':
				print_version();
				break;
			case 'h':
			case '?':
				print_help(argv[0]);
				break;
		}
	}

	if (optind != argc && optind + 1 != argc) {
		print_help(argv[0]);
	}

	env = alloc_env();

	if (optind + 1 == argc) {
		f = fopen(argv[optind], "r");
		if (f == NULL) {
			fprintf(stdout, "File Error: %s\n", argv[optind]);
			free_env(env);
			return 1;
		}
	} else {
		f = stdin;
		if (is_interfactive(f)) {
			splash(env);
		}
	}


	do {
		if (is_interfactive(f)) {
			fprintf(stdout, "%s", REPL_PROMPT);
			fflush(stdout);
		}

		/* read */
		in = parse(f);
		if (feof(f)) {
			free_obj(in);
			break;
		}

		/* eval */
		out = eval(in, env);
		free_obj(in);

		/* print */
		print_obj(out);
		free_obj(out);

		/* loop */
	} while (!feof(f));

	fclose(f);

	free_env(env);

	return 0;
}
