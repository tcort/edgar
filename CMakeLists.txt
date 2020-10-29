# edgar - an interpreter for the edgar programming language
# Copyright (c) 2013, 2014, 2015, 2020 Thomas Cort <linuxgeek@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(edgar VERSION 2.1.0 LANGUAGES C)

set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(gmp)

configure_file (
    "${PROJECT_SOURCE_DIR}/include/config.h.in"
    "${PROJECT_BINARY_DIR}/include/config.h"
)

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}/include"
    "${GMP_INCLUDE_DIR}"
)

file(GLOB SOURCES "src/*.c")
add_executable(edgar ${SOURCES})
target_link_libraries(edgar gmp)

install(TARGETS edgar DESTINATION bin)
install(FILES ${PROJECT_SOURCE_DIR}/man/edgar.1 DESTINATION man/man1)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples DESTINATION share/${PROJECT_NAME})

include(CTest)
enable_testing()

set (functional_tests "atom" "car" "cdr" "cond" "cons" "defun" "equal" "greater" "if" "int" "less" "micromanual" "minus" "misc" "plus" "prelude" "print" "quote" "quotient" "regression-apply-leak" "regression-setq-str" "remainder" "setq" "times")

foreach(test ${functional_tests})
    add_test(NAME ${test}_test COMMAND sh -c "${PROJECT_BINARY_DIR}/${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/tests/${test}.in > ${PROJECT_BINARY_DIR}/tests_${test}.out")
    add_test(NAME ${test}_chck COMMAND ${CMAKE_COMMAND} -E compare_files "${PROJECT_SOURCE_DIR}/tests/${test}.ex" "${PROJECT_BINARY_DIR}/tests_${test}.out")
endforeach()

