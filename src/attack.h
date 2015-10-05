/*
 * attack.h
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

#ifndef _ATTACK_H_
#define _ATTACK_H_

#include <stdbool.h>
#include "types.h"

U8 get_LSB_index(U64 bb);
U8 get_MSB_index(U64 bb);
void clear_LSB_to_inclusive_bit(U64 * bb, U8 bit);
void clear_MSB_to_inclusive_bit(U64 * bb, U8 bit);
bool is_sq_attacked(const struct board *brd, enum square sq,
		    enum colour attacking_side);

bool TEST_is_bishop_attacking_square(const struct board *brd,
				     enum square sq,
				     enum colour attacking_side);
bool TEST_is_knight_attacking_square(const struct board *brd,
				     enum square sq,
				     enum colour attacking_side);
bool TEST_is_pawn_attacking_square(const struct board *brd, enum square sq,
				   enum colour attacking_side);
bool TEST_is_king_attacking_square(const struct board *brd, enum square sq,
				   enum colour attacking_side);
bool TEST_is_rook_attacking_square(const struct board *brd, enum square sq,
				   enum colour attacking_side);
bool TEST_is_queen_attacking_square(const struct board *brd,
				    enum square sq, enum colour attacking_side);
bool TEST_is_attacked_horizontally_or_vertically(const struct board *brd,
						 enum square sq_one,
						 enum square sq_two);
bool TEST_is_attacked_diagonally(const struct board *brd,
				 enum square sq_one, enum square sq_two);

#endif
