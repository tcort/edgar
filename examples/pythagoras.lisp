; edgar - a small LISP Interpreter written in C
; Copyright (c) 2013 Thomas Cort <linuxgeek@gmail.com>
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


; Implements a right triangle solver. Given the lengths of the two shortest
; sides, calculate the length of the hypotenuse.

; Find the Square of a number X (x^2 = x*x)
(DEFUN SQR (X) (TIMES X X))

; Implements the pythagorean theorem (a^2 + b^2 = c^2)
(DEFUN PYTHAG (A B)
       (SQRT (PLUS (SQR A) (SQR B))))

(PYTHAG 3 4)
