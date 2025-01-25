/*
 * edgar - an interpreter for the edgar programming language
 * Copyright (c) 2013, 2014, 2015, 2020, 2025 Thomas Cort <linuxgeek@gmail.com>
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
#include <errno.h>
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
#include "prelude.h"
#include "repl.h"

static void splash(obj_t * env);
static void print_help(char *progname);
static void print_version (void);

static void splash(obj_t * env) {
	fprintf(stdout, "            _                    \n");
	fprintf(stdout, "    ___  __| | __ _  __ _ _ __   \n");
	fprintf(stdout, "   / _ \\/ _` |/ _` |/ _` | '__|  \n");
	fprintf(stdout, "  |  __/ (_| | (_| | (_| | |     \n");
	fprintf(stdout, "   \\___|\\__,_|\\__, |\\__,_|_|     \n");
	fprintf(stdout, "              |___/              \n");
	fprintf(stdout, "\n");
	fprintf(stdout, "           Version %s\n", PROJECT_VERSION);
	fprintf(stdout,"\n");
	print_func_names(env);
	fprintf(stdout, "\n");
}

void print_help(char *progname) {

	fprintf(stdout, "%s -  an interpreter for the edgar programming language\n", PROJECT_NAME);
	fprintf(stdout, "\n");
	fprintf(stdout, "Usage: %s [OPTION] [filename]\n", progname);
	fprintf(stdout, "\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, " -h, -?            --help                  Print a helpful message and exit\n");
	fprintf(stdout, " -l file.edgar     --lib file.edgar        Load a library\n");
	fprintf(stdout, " -v                --version               Print version information and exit\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Report bugs to %s\n", PROJECT_BUG_TRACKER);

	exit(0);
}

static void print_version(void) {

	fprintf(stdout, "%s\n", PROJECT_STRING);
	fprintf(stdout, "\n");
	fprintf(stdout, "Copyright (C) 2013, 2014, 2015, 2020, 2025 Thomas Cort <linuxgeek@gmail.com>\n");
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
	char *lib;
	obj_t *env;

	const char* const short_options = "h?l:v";
	static const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"lib", required_argument, NULL, 'l'},
		{"version", no_argument, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};

	lib = NULL;

	while ((optc = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (optc) {
			case 'v':
				print_version();
				break;
			case 'l':
				lib = optarg;
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
			fprintf(stdout, "File Error: %s: %s\n", argv[optind], strerror(errno));
			free_env(env);
			return 1;
		}
	} else {
		f = stdin;
		if (is_interactive(f)) {
			splash(env);
		}
	}

	if (1) {
		int rc = load_prelude(env);
		if (rc == -1) {
			fprintf(stderr, "Prelude Error: %s: %s\n", lib, strerror(errno));
			free_env(env);
			fclose(f);
			return 1;
		}
	}

	if (lib != NULL) {

		int rc = load_file(lib, env);
		if (rc == -1) {
			fprintf(stderr, "File Error: %s: %s\n", lib, strerror(errno));
			free_env(env);
			fclose(f);
			return 1;
		}

	}

	if (is_interactive(f)) {
		print_defunc_names(env);
		fprintf(stdout, "\n");
	}

	repl(f, env, !is_interactive(f));

	fclose(f);

	free_env(env);

	return 0;
}
