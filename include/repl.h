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

#ifndef __REPL_H
#define __REPL_H

#include <stdio.h>

int is_interactive(FILE *f);
void repl(FILE *f, obj_t * env, int silent);

#endif /* __REPL_H */
