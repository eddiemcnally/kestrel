
/*
 * bitboard.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for manipulating the bitboards
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
#include "types.h"

struct bitboards{
	// bitboard entry for each piece
    uint64_t pieces[NUM_PIECES];

    // The above array piece arrays overlayed into a single bitboard.
    // In effect, an bitwise OR of all elements in pieces[]
    uint64_t board;

	// a bitboard per colour
	uint64_t colour_bb[NUM_COLOURS];
};

void add_piece_to_bitboards(struct bitboards *bb, enum piece pce, enum square sq);
void remove_piece_from_bitboards(struct bitboards *bb, enum piece pce, enum square sq);
uint64_t get_bitboard(const struct bitboards *bb, enum piece pce);
uint64_t get_bitboard_all_pieces(const struct bitboards *bb);
uint64_t get_bitboard_combined(const struct bitboards *bb, enum piece pce_1, enum piece pce_2);
uint64_t get_bitboard_for_colour(const struct bitboards *bb, enum colour col);
uint64_t get_bitboard_for_king(const struct bitboards *bb, enum colour col);
uint64_t square_to_bitboard(enum square sq);

bool bitboard_stucts_are_same(const struct bitboards *bb1, const struct bitboards *bb2);

void clear_bit(uint64_t *bitboard, enum square sq);
void set_bit(uint64_t *bitboard, enum square sq);
uint8_t count_bits(const uint64_t bitboard);









