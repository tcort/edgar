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

#ifndef __NUMBER_H
#define __NUMBER_H

#include "obj.h"

obj_t * number_filter(obj_t * o);

int is_int_obj(obj_t *o);

obj_t * edgar_plus(obj_t *op1, obj_t *op2);
obj_t * edgar_minus(obj_t *op1, obj_t *op2);
obj_t * edgar_times(obj_t *op1, obj_t *op2);
obj_t * edgar_quotient(obj_t *op1, obj_t *op2);
obj_t * edgar_remainder(obj_t *op1, obj_t *op2);
int edgar_cmp(obj_t *op1, obj_t *op2, char *op_name);

#endif /* __NUMBER_H */
