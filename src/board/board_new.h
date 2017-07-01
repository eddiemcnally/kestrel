/*
 * board_new.h
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for manipulating pieces and the board
 * ---------------------------------------------------------------------
 *
 *
 *
 *
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

#pragma once

#include <stdbool.h>
#include "kestrel.h"


void clear_board(struct new_board *brd);
void add_to_board(struct new_board *brd, enum piece pce, enum square sq);
void remove_from_board(struct new_board *brd, enum piece pce, enum square sq);

int32_t get_material_value(const struct new_board *brd, enum colour col);
enum square get_king_square(const struct new_board *brd, enum colour col);





struct new_board;
