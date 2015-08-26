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
#include "occupancy_mask.h"

static bool are_intervening_squares_empty(const struct board *brd, U64 occ_mask, enum square attacking_sq, enum square target_sq);
static bool is_blocked_horizontally(const struct board *brd, enum square sq1, enum square sq2);
static bool is_blocked_vertically(const struct board *brd, enum square sq1, enum square sq2);
static bool is_bishop_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_knight_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_pawn_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_king_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_rook_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side);
static bool is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two);
static bool is_attacked_diagonally(const struct board *brd, enum square sq_one, enum square sq_two);


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
    ASSERT_BOARD_OK(brd);

	//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	//printf("checking sq %s being attacked\n", print_square(sq));

	// TODO:
	// combine rook/queen and bishop/queen to reduce number of lookups

    if (is_knight_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W Knight attacking %s\n", print_square(sq));
		//} else{
			//printf("B Knight attacking %s\n", print_square(sq));
		//}
		return true;
	}
    if (is_pawn_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W PAWN attacking %s\n", print_square(sq));
		//} else{
			//printf("B PAWN attacking %s\n", print_square(sq));
		//}

		return true;
	}

    if (is_king_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W KING attacking %s\n", print_square(sq));
		//} else{
			//printf("B KING attacking %s\n", print_square(sq));
		//}

		return true;
	}
    if (is_queen_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W QUEEN attacking %s\n", print_square(sq));
		//} else{
			//printf("B QUEEN attacking %s\n", print_square(sq));
		//}
		return true;
	}
    if (is_bishop_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W BISHOP attacking %s\n", print_square(sq));
		//} else{
			//printf("B BISHOP attacking %s\n", print_square(sq));
		//}
		return true;
	}
    if (is_rook_attacking_square(brd, sq, attacking_side)){
		//if(attacking_side == WHITE){
			//printf("W ROOK attacking %s\n", print_square(sq));
		//} else{
			//printf("B ROOK attacking %s\n", print_square(sq));
		//}
		return true;
	}

    return false;
}



//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

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
		enum square att_pce_sq = POP(&bbKnight);

		// get occupancy mask for this piece and square
		U64 mask = GET_KNIGHT_OCC_MASK(att_pce_sq);

		if ((mask & sqBB) != 0) {
			// a Knight is attacking this square
			return true;
		}
    }
    return false;
}

static bool is_bishop_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{
    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    if (attacking_side == WHITE)
		attacking_piece = W_BISHOP;
    else
		attacking_piece = B_BISHOP;

    // get the bitboard representing all bishops on the board of
    // this colour
    U64 bbBishop = brd->bitboards[attacking_piece];

    while (bbBishop != 0) {
		enum square att_pce_sq = POP(&bbBishop);

		// get occupancy mask for this piece and square
		U64 mask = GET_BISHOP_OCC_MASK(att_pce_sq);
		if (mask & sqBB) {
			// a bishop is possibly attacking this square
			// search for any blocking pieces
			bool attacked = is_attacked_diagonally(brd, att_pce_sq, sq);
			if (attacked){
				//printf("not blocked between %s and %s\n", print_square(sq), print_square(att_pce_sq));
				return true;
			}
		}
    }

	//printf("not attacking %s\n", print_square(sq));
    return false;
}

static bool is_rook_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{
    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    if (attacking_side == WHITE)
		attacking_piece = W_ROOK;
    else
		attacking_piece = B_ROOK;

    // get the bitboard representing all rooks on the board of
    // this colour
    U64 bbRook = brd->bitboards[attacking_piece];

    // check all rooks
    while (bbRook != 0) {
		enum square att_pce_sq = POP(&bbRook);

		// get occupancy mask for this piece and square
		U64 mask = GET_ROOK_OCC_MASK(att_pce_sq);
		if (mask & sqBB) {
			// a Rook is possibly attacking this square
			// search for any blocking pieces
			bool attacked = is_attacked_horizontally_or_vertically(brd, sq, att_pce_sq);
			if (attacked){
				return true;
			} else {
				// keep looking
				continue;
			}
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
		enum square att_pce_sq = POP(&bbPawn);

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

static inline bool is_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side)
{

    // create bitboard for square under attack
    U64 sqBB = 0;
    set_bit(&sqBB, sq);

    enum piece attacking_piece;

    if (attacking_side == WHITE)
		attacking_piece = W_QUEEN;
    else
		attacking_piece = B_QUEEN;

    // get the bitboard representing all queens on the board of
    // this colour
    U64 bbQueen = brd->bitboards[attacking_piece];

    // check *all* queens to see if blocked.
    while (bbQueen != 0) {
		enum square att_pce_sq = POP(&bbQueen);

		//if (attacking_side == WHITE)
			//printf("checking W_QUEEN on %s can attack square %s\n", print_square(att_pce_sq), print_square(sq));
		//else
			//printf("checking B_QUEEN on %s can attack square %s\n", print_square(att_pce_sq), print_square(sq));


		// get occupancy mask for this square
		U64 mask = GET_QUEEN_OCC_MASK(att_pce_sq);

		if (mask & sqBB) {
			// the queen is potentially attacking this square.
			// Need to see if any blocking pieces

			if (is_attacked_diagonally(brd, sq, att_pce_sq)) {
				//printf("Queen on %s can DIAGONALLY attack square %s\n", print_square(att_pce_sq), print_square(sq));
				return true;
			}
			if (is_attacked_horizontally_or_vertically(brd, sq, att_pce_sq)) {
				//printf("Queen on %s can HORIZ or VERT attack square %s\n", print_square(att_pce_sq), print_square(sq));
				return true;
			}
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
		enum square att_pce_sq = POP(&bbPawn);

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
	U64 ne_occ_mask = GET_NORTH_EAST_OCC_MASK(attacking_sq);
	if (check_bit(&ne_occ_mask, target_sq)){
		// target sq is on diagnoal....check to see if vector between
		// attacking square and target is blocked
		bool is_attack_possible = are_intervening_squares_empty(brd, ne_occ_mask, attacking_sq, target_sq);
		return is_attack_possible;
	}

	U64 nw_occ_mask = GET_NORTH_WEST_OCC_MASK(attacking_sq);
	if (check_bit(&nw_occ_mask, target_sq)){
		// target sq is on diagnoal....check to see if vector between
		// attacking square and target is blocked
		bool is_attack_possible = are_intervening_squares_empty(brd, nw_occ_mask, attacking_sq, target_sq);
		return is_attack_possible;
	}

	U64 se_occ_mask = GET_SOUTH_EAST_OCC_MASK(attacking_sq);
	if (check_bit(&se_occ_mask, target_sq)){
		// target sq is on diagnoal....check to see if vector between
		// attacking square and target is blocked
		bool is_attack_possible = are_intervening_squares_empty(brd, se_occ_mask, attacking_sq, target_sq);
		return is_attack_possible;
	}


	U64 sw_occ_mask = GET_SOUTH_WEST_OCC_MASK(attacking_sq);
	if (check_bit(&sw_occ_mask, target_sq)){
		// target sq is on diagnoal....check to see if vector between
		// attacking square and target is blocked
		bool is_attack_possible = are_intervening_squares_empty(brd, sw_occ_mask, attacking_sq, target_sq);
		return is_attack_possible;
	}

	return false;

}

static inline bool are_intervening_squares_empty(const struct board *brd, U64 occ_mask, enum square attacking_sq, enum square target_sq){

	//printf("checking interim squares between %s and %s\n", print_square(attacking_sq), print_square(target_sq));

	// clear target bit and attacker bit from mask
	clear_bit(&occ_mask, target_sq);
	clear_bit(&occ_mask, attacking_sq);

	// now check all bits between attacker and target
	while (occ_mask != 0){
		enum square interim_sq = POP(&occ_mask);
		if (is_square_occupied(brd->board, interim_sq)){
			// squares not empty
			return false;
		}
	}
	// squares empty
	return true;
}



/**
 * Test wrapper functions.
 * These wrapper functions provide access for the unit test framework while
 * allowing the functions themselves to be static to this file and inline-able
 */
bool TEST_is_bishop_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_bishop_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_knight_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_knight_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_pawn_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_pawn_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_king_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_king_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_rook_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_rook_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_queen_attacking_square(const struct board *brd, enum square sq, enum colour attacking_side){
	return is_queen_attacking_square(brd, sq, attacking_side);
}
bool TEST_is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two){
	return is_attacked_horizontally_or_vertically(brd, sq_one, sq_two);
}
bool TEST_is_attacked_diagonally(const struct board *brd, enum square sq_one, enum square sq_two){
	return is_attacked_diagonally(brd, sq_one, sq_two);
}
