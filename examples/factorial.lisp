; edgar - a small LISP Interpreter written in C
; Copyright (c) 2013, 2014, 2015, 2020 Thomas Cort <linuxgeek@gmail.com>
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.



; The factorial function is the product of N * N-1 * N-2 * ... * 1.
; Thus, (FACTORIAL 5) = 5*4*3*2*1 = 120
(DEFUN FACTORIAL (N)
       (COND ((EQUAL N 1)
	      	1)
	     (T
	      	(TIMES N (FACTORIAL (MINUS N 1))))))


(FACTORIAL 5)
