/*
 * attack.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: Contains code associated with checking if a square is
 * being attacked.
 * ---------------------------------------------------------------------
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

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "kestrel.h"
#include "board.h"
#include "bitboard.h"
#include "board_utils.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "attack.h"
#include "pieces.h"
#include "utils.h"
#include "occupancy_mask.h"

static bool is_rook_or_queen_attacking_square(const struct board *brd, enum square sq, uint64_t rq_bb);
static bool is_bishop_or_queen_attacking_square(const struct board *brd, enum square sq, uint64_t bq_bb);
static uint64_t in_between(enum square sq1, enum square sq2);
static void populate_intervening_squares_array(void);




// a lookup array of bitmasks for squares between the "from" and "to"
// squares.
// since there is a commutative property associated with to/from squares
// when identifying intervening squares, it's irrelevent whether you index using
// [from][to] or [to][from]
uint64_t intervening_squares_lookup[NUM_SQUARES][NUM_SQUARES];



void init_attack_framework(void)
{
    populate_intervening_squares_array();
}




/*
 * Checks to see if a given square is being attacked by
 * any piece on the board
 * name: is_sq_attacked
 * @param
 * @return
 *
 */

bool is_sq_attacked(const struct board *brd, enum square sq,
                    enum colour attacking_side)
{
	const struct bitboards *bb = get_bitboard_struct(brd);

    // create a bitboard for the square being considered
    uint64_t sq_bb = 0;
    set_bit(&sq_bb, sq);

    if (attacking_side == WHITE) {
        if (is_knight_attacking_square(brd, sq_bb, W_KNIGHT)) {
            return true;
        }
        // white pawn controls this square?
        if (is_pawn_controlling_sq(brd, WHITE, sq)) {
            return true;
        }

        // get the bitboard for rook and queen
        uint64_t rq_bb = get_bitboard_combined_rook_queen(bb, WHITE);
        if (is_rook_or_queen_attacking_square(brd, sq, rq_bb)) {
            return true;
        }

        // get the bitboard for bishop and queen
        uint64_t bq_bb = get_bitboard_combined_bishop_queen(bb, WHITE);
        if (is_bishop_or_queen_attacking_square(brd, sq, bq_bb)) {
            return true;
        }

        if (is_king_attacking_square(brd, sq_bb, WHITE)) {
            return true;
        }

    } else {

        if (is_knight_attacking_square(brd, sq_bb, B_KNIGHT)) {
            return true;
        }
        // Black pawn controls this square?
        if (is_pawn_controlling_sq(brd, BLACK, sq)) {
            return true;
        }

        // get the bitboard for rook and queen
        uint64_t rq_bb = get_bitboard_combined_rook_queen(bb, BLACK);
        if (is_rook_or_queen_attacking_square(brd, sq, rq_bb)) {
            return true;
        }
        // get the bitboard for bishop and queen
        uint64_t rq_bq = get_bitboard_combined_bishop_queen(bb, BLACK);
        if (is_bishop_or_queen_attacking_square(brd, sq, rq_bq)) {
            return true;
        }

        if (is_king_attacking_square(brd, sq_bb, BLACK)) {
            return true;
        }
    }
    return false;
}




static void populate_intervening_squares_array(void)
{
    for(int fr_sq = a1; fr_sq <= h8; fr_sq++) {
        for(int to_sq = a1; to_sq <= h8; to_sq++) {

            uint64_t bits = in_between((enum square)fr_sq, (enum square)to_sq);
            intervening_squares_lookup[fr_sq][to_sq] = bits;
        }
    }
}


// checks vertical and horizontal for both rook and queen
static inline bool is_rook_or_queen_attacking_square(const struct board
        *brd, enum square sq,
        uint64_t rq_bb)
{
    while (rq_bb != 0) {
        enum square att_pce_sq = pop_1st_bit(&rq_bb);

        // exclude our target square
        if (att_pce_sq != sq) {
            if (is_attacked_horizontally_or_vertically(brd, att_pce_sq, sq)) {
                return true;
            }
        }
    }

    return false;
}

// checks diagonal and anti-diagonal for quuen and bishop
static inline bool is_bishop_or_queen_attacking_square(const struct board
        *brd,
        enum square sq,
        uint64_t bq_bb)
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

inline bool is_knight_attacking_square(const struct board *brd,
        uint64_t sq_bb,
        enum piece attacking_piece)
{
	const struct bitboards *bb = get_bitboard_struct(brd);

    // get the bitboard representing all knights on the board of
    // this colour
    uint64_t bb_knight = get_bitboard_for_piece(bb, attacking_piece);
    while (bb_knight != 0) {
        enum square knight_sq = pop_1st_bit(&bb_knight);
        uint64_t msk = get_knight_occ_mask(knight_sq);
        if ((msk & sq_bb) != 0) {
            // knight is attacking square
            return true;
        }
    }
    return false;
}

inline bool is_king_attacking_square(const struct board *brd,
        uint64_t sq_bb,
        enum colour col)
{
    enum square king_sq = get_king_square(brd, col);

    // get occupancy mask for this square
    uint64_t mask = get_king_occ_mask(king_sq);
    return ((mask & sq_bb) != 0);
}

inline bool is_attacked_horizontally_or_vertically(
    const struct board *brd,
    enum square sq_one,
    enum square sq_two)
{

	const struct bitboards *bb = get_bitboard_struct(brd);

    uint64_t rook_mask = get_rook_occ_mask(sq_one);

    if (is_square_occupied(rook_mask, sq_two)) {
        // the piece on sq_two shares a rank or file with the rook, so
        // check if there are blocking pieces
        uint64_t interim_squares = intervening_squares_lookup[sq_one][sq_two];
        uint64_t pieces_on_board = get_bitboard_all_pieces(bb);

        if ((interim_squares & pieces_on_board) == 0) {
            // no intervening/blocking pieces
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
inline bool is_attacked_diagonally(const struct board *brd,
        enum square attacking_sq,
        enum square target_sq)
{
    uint64_t bishop_occ_mask = get_bishop_occ_mask(attacking_sq);
	const struct bitboards *bb = get_bitboard_struct(brd);

    if(is_square_occupied(bishop_occ_mask, target_sq)) {
        // the piece on target_sq shares a diagonal with the bishop so
        // check intervening squares
        uint64_t interim_squares = intervening_squares_lookup[attacking_sq][target_sq];
        uint64_t pieces_on_board = get_bitboard_all_pieces(bb);

        if ((interim_squares & pieces_on_board) == 0) {
            // no intervening/blocking pieces
            return true;
        }
    }
    return false;
}







// This code returns a bitboard with bits set representing squares between
// the given 2 squares.
//
// The code is taken from :
// http://chessprogramming.wikispaces.com/Square+Attacked+By#Legality%20Test-In%20Between
//
static uint64_t in_between(enum square sq1, enum square sq2)
{
    const uint64_t m1   = 0xffffffffffffffff;
    const uint64_t a2a7 = 0x0001010101010100;
    const uint64_t b2g7 = 0x0040201008040200;
    const uint64_t h1b7 = 0x0002040810204080;
    uint64_t btwn, line, rank, file;

    btwn  = (m1 << sq1) ^ (m1 << sq2);
    file  =   (sq2 & 7) - (sq1   & 7);
    rank  =  ((sq2 | 7) -  sq1) >> 3 ;
    line  =      (   (file  &  7) - 1) & a2a7; /* a2a7 if same file */
    line += 2 * ((   (rank  &  7) - 1) >> 58); /* b1g1 if same rank */
    line += (((rank - file) & 15) - 1) & b2g7; /* b2g7 if same diagonal */
    line += (((rank + file) & 15) - 1) & h1b7; /* h1b7 if same antidiag */
    line *= btwn & -btwn; /* mul acts like shift by smaller square */

    return line & btwn;   /* return the bits on that line in-between */
}
