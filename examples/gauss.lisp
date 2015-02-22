; edgar - a small LISP Interpreter written in C
; Copyright (c) 2013, 2014, 2015 Thomas Cort <linuxgeek@gmail.com>
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



; Sum the integers from 1 to N.
; Use a recursive function which adds N to the sum of 0 to N-1.
(DEFUN SUM-TO-N (N)
       (COND
	 	((LESS N 0)
	  		0)
		(T
		 	(PLUS N (SUM-TO-N (MINUS N 1))))))

; Sum the integers from 1 to N.
; Use Gauss' method for summing consecutive integers.
(DEFUN GAUSS (N)
       (QUOTIENT (TIMES N (PLUS N 1)) 2))

(SUM-TO-N 100)
(GAUSS 100)
