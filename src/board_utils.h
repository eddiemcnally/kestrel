/*
 * board_utils.h
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * kestrel is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kestrel is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _BOARD_UTILS_H_
#define _BOARD_UTILS_H_

#include <stdbool.h>
#include "types.h"

void print_board(const struct board *brd);
bool ASSERT_BOARD_OK(const struct board *brd);
char *print_square(enum square sq);
void assert_boards_are_equal(const struct board * brd1, const struct board * brd2);
struct board * clone_board(const struct board * board_to_clone);
	
#endif
