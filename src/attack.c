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
#include "utils.h"
#include "occupancy_mask.h"

static bool are_intervening_squares_empty(const struct board *brd, U64 occ_mask, enum square attacking_sq, enum square target_sq);
static bool is_blocked_horizontally(const struct board *brd, enum square sq1, enum square sq2);
static bool is_blocked_vertically(const struct board *brd, enum square sq1, enum square sq2);
static bool is_knight_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_pawn_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_king_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two);
static bool is_attacked_diagonally(const struct board *brd, enum square attacking_sq, enum square target_sq);
static bool is_rook_or_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_bishop_or_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);




/*
 * Checks to see if a given square is being attacked by
 * any piece on the board
 * name: is_sq_attacked
 * @param
 * @return
 *
 */

bool is_sq_attacked(const struct board *brd, enum square sq, enum colour attacking_side)
{
    if (is_knight_attacking_square(brd, sq, attacking_side)){
		return true;
	}
    if (is_pawn_attacking_square(brd, sq, attacking_side)){
		return true;
	}

    if (is_king_attacking_square(brd, sq, attacking_side)){
		return true;
	}
	
	if (is_rook_or_queen_attacking_square(brd, sq, attacking_side)){
		return true;
	}
	if (is_bishop_or_queen_attacking_square(brd, sq, attacking_side)){
		return true;
	}

    return false;
}


// checks vertical and horizontal for both rook and queen
static inline bool is_rook_or_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

	// Logical OR the rook and queen bitboards for the relevant colour
    U64 rq_bb = 0;
    if (attacking_side == BLACK){
		rq_bb = brd->bitboards[B_ROOK];
		rq_bb |= brd->bitboards[B_QUEEN];
	} else {
		rq_bb = brd->bitboards[W_ROOK];
		rq_bb |= brd->bitboards[W_QUEEN];
	}
    
    while (rq_bb!= 0) {
		enum square att_pce_sq = pop_1st_bit(&rq_bb);

		bool is_attacked = is_attacked_horizontally_or_vertically(brd, att_pce_sq, sq);
		if (is_attacked){
			return true;
		}
    }

    return false;	
}


// checks diagonal and anti-diagonal for quuen and bishop
static inline bool is_bishop_or_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

	// Logical OR the bishop and queen bitboards for the relevant colour
    U64 bq_bb = 0;
    if (attacking_side == BLACK){
		bq_bb = brd->bitboards[B_BISHOP];
		bq_bb |= brd->bitboards[B_QUEEN];
	} else {
		bq_bb = brd->bitboards[W_BISHOP];
		bq_bb |= brd->bitboards[W_QUEEN];
	}
    
    while (bq_bb!= 0) {
		enum square att_pce_sq = pop_1st_bit(&bq_bb);

		bool is_attacked = is_attacked_diagonally(brd, att_pce_sq, sq);
		if (is_attacked){
			return true;
		}
    }

    return false;	
}



static inline bool is_knight_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{
    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    if (attacking_side == WHITE)
		attacking_piece = W_KNIGHT;
    else
		attacking_piece = B_KNIGHT;

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

static inline bool is_pawn_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{
    enum piece attacking_piece;

    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    // ------------------------
    // check which pawn
    // ------------------------
    if (attacking_side == WHITE)
		attacking_piece = W_PAWN;
    else
		attacking_piece = B_PAWN;

    // get the bitboard representing all pawns on the board of
    // this colour
    U64 bbPawn = brd->bitboards[attacking_piece];
        
    while (bbPawn != 0) {
		enum square att_pce_sq = pop_1st_bit(&bbPawn);

		// get occupancy mask for this piece
		U64 mask = 0;
		if (attacking_side == WHITE)
			mask = GET_WHITE_PAWN_OCC_MASK(att_pce_sq);
		else
			mask = GET_BLACK_PAWN_OCC_MASK(att_pce_sq);

		if (mask & sqBB) {
			// a pawn is attacking this square
			return true;
		}
	}
    return false;
}


static inline bool is_king_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{

    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    // ------------------------
    // check king
    // ------------------------
    if (attacking_side == WHITE)
		attacking_piece = W_KING;
    else
		attacking_piece = B_KING;

    // get the bitboard representing the king on the board of
    // this colour
    U64 bbPawn = brd->bitboards[attacking_piece];
    while (bbPawn != 0) {
		enum square att_pce_sq = pop_1st_bit(&bbPawn);

		// get occupancy mask for this square
		U64 mask = GET_KING_OCC_MASK(att_pce_sq);
		if (mask & sqBB) {
			// a king is attaching this square
			return true;
		}
    }
    return false;
}

static inline bool is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two)
{

    int sq_one_rank = GET_RANK(sq_one);
    int sq_one_file = GET_FILE(sq_one);

    int sq_two_rank = GET_RANK(sq_two);
    int sq_two_file = GET_FILE(sq_two);

    if (sq_one_rank == sq_two_rank) {
		bool blocked = is_blocked_horizontally(brd, sq_one, sq_two);
		return !blocked;
    } else if (sq_one_file == sq_two_file) {
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
static inline bool is_blocked_horizontally(const struct board *brd, enum square start_sq, enum square end_sq)
{

	enum square start = start_sq;
	enum square end = end_sq;
	if (start > end){
		// swap for convenience
		start = end_sq;
		end = start_sq;
	}


	// TODO : replace this loop with a lookup mask for all
	// possible horizontal intervening squares (could then so a simple
	// bit mask check instead of a loop) 
	for(enum square s = start + 1; s < end; s++){
		if (is_square_occupied(brd->board, s)){
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
static inline bool is_blocked_vertically(const struct board *brd, enum square start_sq, enum square end_sq)
{
	enum square start = start_sq;
	enum square end = end_sq;
	if (start > end){
		// swap for convenience
		start = end_sq;
		end = start_sq;
	}

	// TODO : replace this loop with a lookup mask for all
	// possible vertical intervening squares (could then so a simple
	// bit mask check instead of a loop) 
	for(enum square s = start + 8; s < end; s = s + 8){
		if (is_square_occupied(brd->board, s)){
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
static inline bool is_attacked_diagonally(const struct board *brd, enum square attacking_sq, enum square target_sq)
{

	U64 diag_occ_mask = GET_DIAGONAL_OCC_MASK(attacking_sq);
	if (check_bit(&diag_occ_mask, target_sq)){
		// target sq is on diagnoal....check to see if vector between
		// attacking square and target is blocked
		bool is_attack_possible = are_intervening_squares_empty(brd, diag_occ_mask, attacking_sq, target_sq);
		return is_attack_possible;
	}

	U64 anti_diag_occ_mask = GET_ANTI_DIAGONAL_OCC_MASK(attacking_sq);
	if (check_bit(&anti_diag_occ_mask, target_sq)){
		// target sq is on diagnoal....check to see if vector between
		// attacking square and target is blocked
		bool is_attack_possible = are_intervening_squares_empty(brd, anti_diag_occ_mask, attacking_sq, target_sq);
		return is_attack_possible;
	}

	return false;

}


static inline bool are_intervening_squares_empty(const struct board *brd, U64 occ_mask, enum square attacking_sq, enum square target_sq){

	//// clear all bits outside the range between the 2 squares.
	if (attacking_sq < target_sq){

		clear_LSB_to_inclusive_bit(&occ_mask, attacking_sq);
		clear_MSB_to_inclusive_bit(&occ_mask, target_sq);

	} else {

		clear_LSB_to_inclusive_bit(&occ_mask, target_sq);
		clear_MSB_to_inclusive_bit(&occ_mask, attacking_sq);
	}

	if ((brd->board & occ_mask) != 0){
		return false;
	}

	// squares empty
	return true;
}



inline void clear_LSB_to_inclusive_bit(U64 * bb, U8 bit){

	if (*bb == 0){
		return;
	}

	U8 lsb = get_LSB_index(*bb);

	while ((lsb <= bit) && (*bb != 0)) {
		clear_bit(bb, lsb);
		//printf("cleared LSB %d bb_bit %d, bb  \t0x%016llx\n", lsb, bit, *bb);
		lsb = get_LSB_index(*bb);
	}
}


/**
 * Test wrapper functions.
 * These wrapper functions provide access for the unit test framework while
 * allowing the functions themselves to be static to this file and inline-able
 */
bool TEST_is_knight_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_knight_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_pawn_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_pawn_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_king_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_king_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two){
	return is_attacked_horizontally_or_vertically(brd, sq_one, sq_two);
}
bool TEST_is_attacked_diagonally(const struct board *brd, enum square sq_one, enum square sq_two){
	return is_attacked_diagonally(brd, sq_one, sq_two);
}
