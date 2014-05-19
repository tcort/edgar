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

#ifndef __ENV_H
#define __ENV_H

#include "obj.h"

obj_t * alloc_env(void);
int insert_env(obj_t *env, obj_t *o);
obj_t * query_env(obj_t *env, obj_t *o);
void free_env(obj_t *env);
void print_func_names(obj_t *env);
void print_defunc_names(obj_t *env);

int add_to_env(obj_t *key, obj_t *value, obj_t *env);

#endif /* __ENV_H */
