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



; This file contains a library of functions for use in other programs.
; Include it in the environment by running edgar with the -l option like this:
; 	edgar -l /usr/share/edgar/edgar.lisp
; 
; Functions: 1+, 1-, ABS, GCD, LCM, LENGTH, NULL, ZEROP

; 1+ - Add 1 to a value
(DEFUN 1+ (X) (PLUS X 1))


; 1- - Subtract 1 from a value
(DEFUN 1- (X) (MINUS X 1))


; ABS - Absolute Value Function
(DEFUN ABS (X)
       (COND
	 	((LESS X 0) (MINUS X))
		(T X)))


; GCD - Find the Greatest Common Divisor of 2 numbers
(DEFUN GCD (X Y)
       (COND
	 	((EQUAL Y 0) X)
		(T (GCD Y (REMAINDER X Y)))))


; LCM - Find the Least Common Multiple of 2 numbers
(DEFUN LCM (X Y)
       (QUOTIENT (ABS (TIMES X Y)) (GCD X Y)))


; LENGTH - Find the length of the list
(DEFUN LENGTH (X)
	(COND
		((NULL X) 0)
		((ATOM X) 1)
		(T (PLUS (LENGTH (CDR X)) 1))))


; NULL - Test if the argument is NIL.
(DEFUN NULL (X) (EQUAL X NIL))


; ZEROP - Test if the argument is 0.
(DEFUN ZEROP (X) (EQUAL X 0))
