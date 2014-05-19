/*
 * edgar - a small LISP Interpreter written in C
 * Copyright (c) 2013, 2014 Thomas Cort <linuxgeek@gmail.com>
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

#ifndef __BUILTIN_H
#define __BUILTIN_H

#include "obj.h"

obj_t * func_atom(obj_t *args, obj_t *env);
obj_t * func_car(obj_t *args, obj_t *env);
obj_t * func_cdr(obj_t *args, obj_t *env);
obj_t * func_cond(obj_t *args, obj_t *env);
obj_t * func_cons(obj_t *args, obj_t *env);
obj_t * func_equal(obj_t *args, obj_t *env);
obj_t * func_greater(obj_t *args, obj_t *env);
obj_t * func_int(obj_t *args, obj_t *env);
obj_t * func_less(obj_t *args, obj_t *env);
obj_t * func_minus(obj_t *args, obj_t *env);
obj_t * func_plus(obj_t *args, obj_t *env);
obj_t * func_quotient(obj_t *args, obj_t *env);
obj_t * func_remainder(obj_t *args, obj_t *env);
obj_t * func_setq(obj_t *args, obj_t *env);
obj_t * func_times(obj_t *args, obj_t *env);
obj_t * func_if(obj_t *args, obj_t *env);
obj_t * func_print(obj_t *args, obj_t *env);

#endif /* __BUILTIN_H */
