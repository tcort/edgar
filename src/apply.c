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

#include <assert.h>
#include <stdio.h>

#include "apply.h"
#include "env.h"
#include "eval.h"
#include "obj.h"

obj_t * apply(obj_t *args, obj_t *env) {

	assert(IS_LIST(args));

	if (IS_LIST(args) && IS_LIST(CAR(args)) && IS_FUNC(CAR(CAR(args)))) {

		return (FUNC(CAR(CAR(args))))(CDR(args), env);
	} else if (IS_LIST(args) && IS_LIST(CAR(args)) && IS_DEFUNC(CAR(CAR(args)))) {

		obj_t * func_arg;
		obj_t * func_args;
		obj_t * call_arg;
		obj_t * call_args;

		obj_t * body;
		obj_t * result;

		body = clone_obj(BODY(CAR(CAR(args))));
		func_args = ARGS(CAR(CAR(args)));
		call_args = CDR(args);

		/* ((<DEFUNC:[args=(X)][body=(TIMES X X)]>) 3) */

		while (IS_LIST(func_args) && IS_LIST(call_args)) {
			func_arg = CAR(func_args);
			call_arg = CAR(call_args);

			replace_obj(func_arg, call_arg, body);

			func_args = CDR(func_args);
			call_args = CDR(call_args);
		}

		if ((IS_LIST(func_args) && !IS_LIST(call_args)) ||
				(!IS_LIST(func_args) && IS_LIST(call_args))) {

			free_obj(body); /* clean up */

			fprintf(stdout, "Unexpected number of arguments\n");
			return alloc_fail();
		}

		result = eval(body, env);

		free_obj(body);

		return result;

	} else {

		return clone_obj(args);
	}
}
