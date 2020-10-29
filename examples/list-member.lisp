; edgar - an interpreter for the edgar programming language
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



; Search for an element in a list.
; Return T if NEEDLE is in HAYSTACK, otherwise return NIL
(DEFUN LIST-MEMBER (NEEDLE HAYSTACK)
       (COND
	 	((EQUAL NIL HAYSTACK)
	  		NIL)
		((EQUAL NEEDLE (CAR HAYSTACK))
		 	T)
		(T
		 	(LIST-MEMBER NEEDLE (CDR HAYSTACK)))))


(LIST-MEMBER 5 (1 2 3 4 5 6))
(LIST-MEMBER 42 (1 2 3 4 5 6))

