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
 * Copyright (C) 2017 Eddie McNally <emcn@gmx.com>
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

#include <stdbool.h>
#include <stdint.h>
#include "bitboard.h"
#include "pieces.h"
#include "types.h"


/** @brief adds a new piece to the bitboard framework
 *
 * @param bb address of bitboard struct
 * @param pce the piece to add
 * @param sq the square to add it to
 * @return
 */
void add_piece_to_bitboards(struct bitboards *bb, enum piece pce, enum square sq)
{
    // set pieces[] bb
    uint64_t pce_bb = bb->pieces[pce];
    set_bit(&pce_bb, sq);
    bb->pieces[pce] = pce_bb;

    // set overall bb
    set_bit(&bb->board, sq);

    // set the colour bb
    if (IS_BLACK(pce)) {
        set_bit(&bb->colour_bb[BLACK], sq);
    } else {
        set_bit(&bb->colour_bb[WHITE], sq);
    }
}

/** @brief removes a piece from the bitboard framework
 *
 * @param bb ptr to bitboard struct
 * @param pce the piece to remove
 * @param sq the square to remove it from
 * @return
 */
void remove_piece_from_bitboards(struct bitboards *bb, enum piece pce, enum square sq)
{
    // clear pieces[] bb
    uint64_t pce_bb = bb->pieces[pce];
    clear_bit(&pce_bb, sq);
    bb->pieces[pce] = pce_bb;

    // set overall bb
    clear_bit(&bb->board, sq);

    // set the colour bb
    if (IS_BLACK(pce)) {
        clear_bit(&bb->colour_bb[BLACK], sq);
    } else {
        clear_bit(&bb->colour_bb[WHITE], sq);
    }
}



/** @brief converts the given square to a bitboard
 *
 * @param the square to convert
 * @return
 */
uint64_t square_to_bitboard(enum square sq)
{
    uint64_t retval = 0;
    set_bit(&retval, sq);
    return retval;
}


/** @brief returns the bitboard for the given piece
 *
 * @param bb ptr to bitboard struct
 * @param the piece
 * @return a bitboard with bits set corresponding to squares occupied by the piece
 *
 */

uint64_t get_bitboard_for_piece(const struct bitboards *bb, enum piece pce)
{
    return bb->pieces[pce];
}


/** @brief returns the bitboard representing the given king
 *
 * @param bb ptr to the bitboard struct
 * @param piece_col the colour of the king
 * @return a bitboard with bit set corresponding to square occupied by the king
 *
 */

uint64_t get_bitboard_for_king(const struct bitboards *bb, enum colour piece_col)
{
    if (piece_col == WHITE) {
        return bb->pieces[W_KING];
    } else {
        return bb->pieces[B_KING];
    }
}

/** @brief combines the Bishop and Queen bitboards for a given colour
 *
 * @param bb ptr to bitboard struct
 * @param col the colour of the pieces
 * @return
 *
 */

uint64_t get_bitboard_combined_bishop_queen(const struct bitboards *bb, enum colour col)
{
    if (col == BLACK) {
        return bb->pieces[B_BISHOP] | bb->pieces[B_QUEEN];
    } else {
        return bb->pieces[W_BISHOP] | bb->pieces[W_QUEEN];
    }
}

/** @brief combines the Rook and Queen bitboards for a given colour
 *
 * @param bb ptr to bitboard struct
 * @param col the colour of the pieces
 * @return
 *
 */

uint64_t get_bitboard_combined_rook_queen(const struct bitboards *bb, enum colour col)
{
    if (col == BLACK) {
        return bb->pieces[B_ROOK] | bb->pieces[B_QUEEN];
    } else {
        return bb->pieces[W_ROOK] | bb->pieces[W_QUEEN];
    }
}





/** @brief clears the bit in the bitboard which represents the given square
 *
 * @param bb address of the bitboard to adjust
 * @param sq the square to clear
 * @return the adjusted bitboard
 *
 */

void clear_bit(uint64_t * bb, enum square sq)
{
    *bb = *bb & (uint64_t) (~(0x01ull << sq));
}

/** @brief sets the bit in the bitboard which represents the given square
 *
 * @param bb address of the bitboard to adjust
 * @param sq the square to set
 * @return the adjusted bitboard
 *
 */

void set_bit(uint64_t * bb, enum square sq)
{
    *bb = *bb | (uint64_t) (0x01ull << sq);
}


/** @brief counts the bits in the given bitboard
 *
 * @param bb the bitboard to check
 * @return number of set bits
 *
 */

uint8_t count_bits(const uint64_t bb)
{
    return (uint8_t) __builtin_popcountll(bb);
}



/** @brief gets the combined bitboard representing all pieces on the board
 *
 * @param bb ptr to the bitboard struct
 * @return a bitboard representing all occupied squares on the board
 *
 */
uint64_t get_bitboard_all_pieces(const struct bitboards *bb)
{
    return bb->board;
}


/** @brief gets a bitboard representing all occupied squares for the given piece colour
 *
 * @param col the piece colour
 * @return a bitboard representing all pieces of the given colour
 *
 */
uint64_t get_bitboard_for_colour(const struct bitboards *bb, enum colour col)
{
    return bb->colour_bb[col];
}

/** @brief conpares 2 bitboard structs to see if they are equal
 *
 * @param bb1 bitboard struct #1
 * @param bb2 bitboard struct #2
 * @return true if the structs are the same, false otherwise
 *
 */
bool bitboard_stucts_are_same(const struct bitboards *bb1, const struct bitboards *bb2)
{
    for(int i = 0; i < NUM_PIECES; i++) {
        if(bb1->pieces[i] != bb2->pieces[i]) {
            return false;
        }
    }
    if (bb1->board != bb2->board) {
        return false;
    }
    for(int i = 0; i < NUM_COLOURS; i++) {
        if(bb1->colour_bb[i] != bb2->colour_bb[i]) {
            return false;
        }
    }
    return true;
}
