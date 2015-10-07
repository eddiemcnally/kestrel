/*
 * attack.c
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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "board.h"
#include "board_utils.h"
#include "move.h"
#include "attack.h"
#include "pieces.h"
#include "utils.h"
#include "occupancy_mask.h"

static bool are_intervening_squares_empty(const struct board *brd,
					  U64 occ_mask,
					  enum square attacking_sq,
					  enum square target_sq);
static bool is_blocked_horizontally(const struct board *brd,
				    enum square sq1, enum square sq2);
static bool is_blocked_vertically(const struct board *brd, enum square sq1,
				  enum square sq2);
static bool is_attacked_horizontally_or_vertically(const struct board *brd,
						   enum square sq_one,
						   enum square sq_two);
static bool is_attacked_diagonally(const struct board *brd,
				   enum square attacking_sq,
				   enum square target_sq);
static bool is_knight_attacking_square(const struct board *brd, U64 sqBB,
				       enum piece attacking_piece);
static bool is_king_attacking_square(const struct board *brd, U64 sqBB,
				     enum piece attacking_piece);
static bool is_rook_or_queen_attacking_square(const struct board *brd,
					      enum square sq, U64 rq_bb);
static bool is_bishop_or_queen_attacking_square(const struct board *brd,
						enum square sq, U64 bq_bb);
static bool is_WHITE_pawn_attacking_square(const struct board *brd, U64 sqBB);
static bool is_BLACK_pawn_attacking_square(const struct board *brd, U64 sqBB);

/*
 * Checks to see if a given square is being attacked by
 * any piece on the board
 * name: is_sq_attacked
 * @param
 * @return
 *
 */

bool
is_sq_attacked(const struct board *brd, enum square sq,
	       enum colour attacking_side)
{
	// create a bitboard for the square being considered
	U64 sq_bb = 0;
	set_bit(&sq_bb, sq);

	// trading code bloat for efficiency and duplicate the calls
	// based on colour....
	if (attacking_side == WHITE) {
		// get the bitbard for rook and queen
		U64 rq_bb = 0;
		rq_bb = brd->bitboards[W_ROOK];
		rq_bb |= brd->bitboards[W_QUEEN];
		if (is_rook_or_queen_attacking_square(brd, sq, rq_bb)) {
			return true;
		}
		// get the bitboard for bishop and queen
		U64 rq_bq = 0;
		rq_bq = brd->bitboards[W_BISHOP];
		rq_bq |= brd->bitboards[W_QUEEN];
		if (is_bishop_or_queen_attacking_square(brd, sq, rq_bq)) {
			return true;
		}

		if (is_knight_attacking_square(brd, sq_bb, W_KNIGHT)) {
			return true;
		}
		if (is_WHITE_pawn_attacking_square(brd, sq_bb)) {
			return true;
		}

		if (is_king_attacking_square(brd, sq_bb, W_KING)) {
			return true;
		}

	} else {
		// get the bitboard for rook and queen
		U64 rq_bb = 0;
		rq_bb = brd->bitboards[B_ROOK];
		rq_bb |= brd->bitboards[B_QUEEN];
		if (is_rook_or_queen_attacking_square(brd, sq, rq_bb)) {
			return true;
		}
		// get the bitboard for bishop and queen
		U64 rq_bq = 0;
		rq_bq = brd->bitboards[B_BISHOP];
		rq_bq |= brd->bitboards[B_QUEEN];
		if (is_bishop_or_queen_attacking_square(brd, sq, rq_bq)) {
			return true;
		}

		if (is_knight_attacking_square(brd, sq_bb, B_KNIGHT)) {
			return true;
		}
		if (is_BLACK_pawn_attacking_square(brd, sq_bb)) {
			return true;
		}

		if (is_king_attacking_square(brd, sq_bb, B_KING)) {
			return true;
		}
	}
	return false;
}

// checks vertical and horizontal for both rook and queen
static inline bool is_rook_or_queen_attacking_square(const struct board
						     *brd, enum square sq,
						     U64 rq_bb)
{
	while (rq_bb != 0) {
		enum square att_pce_sq = pop_1st_bit(&rq_bb);

		// exclude our target square
		if (att_pce_sq != sq) {
			if (is_attacked_horizontally_or_vertically
			    (brd, att_pce_sq, sq)) {
				return true;
			}
		}
	}

	return false;
}

// checks diagonal and anti-diagonal for quuen and bishop
static inline bool is_bishop_or_queen_attacking_square(const struct board
						       *brd, enum square sq,
						       U64 bq_bb)
{
	while (bq_bb != 0) {
		enum square att_pce_sq = pop_1st_bit(&bq_bb);
		// exclude target square
		if (att_pce_sq != sq) {
			if (is_attacked_diagonally(brd, att_pce_sq, sq)) {
				return true;
			}
		}
	}
	return false;
}

static inline bool
is_knight_attacking_square(const struct board *brd,
			   U64 sqBB, enum piece attacking_piece)
{
	// get the bitboard representing all knights on the board of
	// this colour
	U64 bbKnight = brd->bitboards[attacking_piece];

	while (bbKnight != 0) {
		enum square att_pce_sq = pop_1st_bit(&bbKnight);

		// get occupancy mask for this piece and square
		U64 mask = GET_KNIGHT_OCC_MASK(att_pce_sq);

		if ((mask & sqBB) != 0) {
			// a Knight is attacking this square
			return true;
		}
	}
	return false;
}

static inline bool
is_BLACK_pawn_attacking_square(const struct board *brd, U64 sqBB)
{
	U64 bbPawn = brd->bitboards[B_PAWN];
	U64 mask = 0;

	// overlay all masks for all the pieces
	while (bbPawn != 0) {
		enum square att_pce_sq = pop_1st_bit(&bbPawn);
		mask |= GET_BLACK_PAWN_OCC_MASK(att_pce_sq);
	}
	return ((mask & sqBB) != 0);
}

static inline bool
is_WHITE_pawn_attacking_square(const struct board *brd, U64 sqBB)
{
	U64 bbPawn = brd->bitboards[W_PAWN];
	U64 mask = 0;

	// overlay all masks for all the pieces
	while (bbPawn != 0) {
		enum square att_pce_sq = pop_1st_bit(&bbPawn);
		mask |= GET_WHITE_PAWN_OCC_MASK(att_pce_sq);
	}
	return ((mask & sqBB) != 0);
}

static inline bool
is_king_attacking_square(const struct board *brd,
			 U64 sqBB, enum piece attacking_piece)
{
	// get the bitboard representing the king on the board of
	// this colour
	U64 bbKing = brd->bitboards[attacking_piece];
	enum square att_pce_sq = pop_1st_bit(&bbKing);

	// get occupancy mask for this square
	U64 mask = GET_KING_OCC_MASK(att_pce_sq);
	return ((mask & sqBB) != 0);
}

static inline bool is_attacked_horizontally_or_vertically(const struct board
							  *brd,
							  enum square
							  sq_one,
							  enum square sq_two)
{

	int sq_one_rank = GET_RANK(sq_one);
	int sq_two_rank = GET_RANK(sq_two);

	if (sq_one_rank == sq_two_rank) {
		bool blocked = is_blocked_horizontally(brd, sq_one, sq_two);
		return !blocked;
	}

	int sq_two_file = GET_FILE(sq_two);
	int sq_one_file = GET_FILE(sq_one);
	if (sq_one_file == sq_two_file) {
		bool blocked = is_blocked_vertically(brd, sq_one, sq_two);
		return !blocked;
	}

	return false;
}

/*
 * Searches along the rank between the squares to see if the path is
 * blocked by a piece
 * name: search_horizontally
 * @param
 * @return : true if blocking piece between start_sq and end_sq, false otherwise
 *
 */
static inline bool
is_blocked_horizontally(const struct board *brd,
			enum square start_sq, enum square end_sq)
{
	enum square start = start_sq;
	enum square end = end_sq;
	if (start > end) {
		// swap for convenience
		start = end_sq;
		end = start_sq;
	}
	// TODO : replace this loop with a lookup mask for all
	// possible horizontal intervening squares (could then do a simple
	// bit mask check instead of a loop) 
	for (enum square s = start + 1; s < end; s++) {
		if (IS_SQUARE_OCCUPIED(brd->board, s)) {
			return true;
		}
	}
	return false;
}

/*
 * Searches along the file between the squares to see if the path is
 * blocked by a piece
 * name: search_vertically
 * @param
 * @return : true if blocking piece between start_sq and end_sq, false otherwise
 *
 */
static inline bool
is_blocked_vertically(const struct board *brd,
		      enum square start_sq, enum square end_sq)
{
	enum square start = start_sq;
	enum square end = end_sq;
	if (start > end) {
		// swap for convenience
		start = end_sq;
		end = start_sq;
	}
	// TODO : replace this loop with a lookup mask for all
	// possible vertical intervening squares (could then so a simple
	// bit mask check instead of a loop) 
	for (enum square s = start + 8; s < end; s = s + 8) {
		if (IS_SQUARE_OCCUPIED(brd->board, s)) {
			return true;
		}
	}
	return false;
}

/*
 * Searches diagonally down left and right, seeing of the path is blocked
 * between the two squares.
 *
 * name: is_attacked_diagonally
 * @param
 * @return : true if attack is possible, false otherwise
 *
 */
static inline bool
is_attacked_diagonally(const struct board *brd,
		       enum square attacking_sq, enum square target_sq)
{
	U64 diag_occ_mask = GET_DIAGONAL_OCC_MASK(attacking_sq);
	if (CHECK_BIT(diag_occ_mask, target_sq)) {
		// target sq is on diagonal....check to see if vector between
		// attacking square and target is blocked
		return are_intervening_squares_empty(brd, diag_occ_mask,
						     attacking_sq, target_sq);
	}

	U64 anti_diag_occ_mask = GET_ANTI_DIAGONAL_OCC_MASK(attacking_sq);
	if (CHECK_BIT(anti_diag_occ_mask, target_sq)) {
		// target sq is on diagonal....check to see if vector between
		// attacking square and target is blocked
		return are_intervening_squares_empty(brd, anti_diag_occ_mask,
						     attacking_sq, target_sq);
	}
	return false;
}

static inline bool
are_intervening_squares_empty(const struct board *brd,
			      U64 occ_mask,
			      enum square attacking_sq, enum square target_sq)
{
	//// clear all bits outside the range between the 2 squares.
	if (attacking_sq < target_sq) {
		clear_LSB_to_inclusive_bit(&occ_mask, attacking_sq);
		clear_MSB_to_inclusive_bit(&occ_mask, target_sq);
	} else {
		clear_LSB_to_inclusive_bit(&occ_mask, target_sq);
		clear_MSB_to_inclusive_bit(&occ_mask, attacking_sq);
	}

	if ((brd->board & occ_mask) != 0) {
		return false;
	}
	// squares empty
	return true;
}

inline void clear_LSB_to_inclusive_bit(U64 * bb, U8 bit)
{
	if (*bb == 0) {
		return;
	}

	U8 lsb = get_LSB_index(*bb);

	while ((lsb <= bit) && (*bb != 0)) {
		clear_bit(bb, lsb);
		lsb = get_LSB_index(*bb);
	}
}

void clear_MSB_to_inclusive_bit(U64 * bb, U8 bit)
{
	if (*bb == 0) {
		return;
	}

	U8 msb = get_MSB_index(*bb);
	while ((msb >= bit) && (*bb != 0)) {
		clear_bit(bb, msb);
		msb = get_MSB_index(*bb);
	}
}

/**
 * int __builtin_ctz (unsigned int x)
 *
 * Returns the number of trailing 0-bits in x, starting at the least
 * significant bit position. If x is 0, the result is undefined
 */
inline U8 get_LSB_index(U64 bb)
{
	// gcc built-in function (see https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)
	return (U8) __builtin_ctzll(bb);
}

inline U8 get_MSB_index(U64 bb)
{
	// gcc built-in function (see https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)
	U8 b = (U8) __builtin_clzll(bb);

	// the above is number of leading zeros.
	// the MSB index is (63-b)
	return (U8) (63 - b);
}

/**
 * Test wrapper functions.
 * These wrapper functions provide access for the unit test framework while
 * allowing the functions themselves to be static to this file and inline-able
 */
bool
TEST_is_knight_attacking_square(const struct board * brd,
				enum square sq, enum colour attacking_side)
{
	enum piece pce;
	if (attacking_side == WHITE) {
		pce = W_KNIGHT;
	} else {
		pce = B_KNIGHT;
	}
	return is_knight_attacking_square(brd, square_to_bitboard(sq), pce);
}

bool
TEST_is_pawn_attacking_square(const struct board * brd,
			      enum square sq, enum colour attacking_side)
{
	if (attacking_side == WHITE) {
		return is_WHITE_pawn_attacking_square(brd,
						      square_to_bitboard(sq));
	} else {
		return is_BLACK_pawn_attacking_square(brd,
						      square_to_bitboard(sq));
	}
}

bool
TEST_is_king_attacking_square(const struct board * brd,
			      enum square sq, enum colour attacking_side)
{
	enum piece pce;
	if (attacking_side == WHITE) {
		pce = W_KING;
	} else {
		pce = B_KING;
	}
	return is_king_attacking_square(brd, square_to_bitboard(sq), pce);
}

bool
TEST_is_attacked_horizontally_or_vertically(const struct board * brd,
					    enum square sq_one,
					    enum square sq_two)
{
	return is_attacked_horizontally_or_vertically(brd, sq_one, sq_two);
}

bool
TEST_is_attacked_diagonally(const struct board * brd,
			    enum square sq_one, enum square sq_two)
{
	return is_attacked_diagonally(brd, sq_one, sq_two);
}
