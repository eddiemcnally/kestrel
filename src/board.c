/*
 * board.c
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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "types.h"
#include "fen.h"
#include "board.h"
#include "board_utils.h"
#include "makemove.h"
#include "hashkeys.h"
#include "pieces.h"


static const U8 BitTable[64] = {
    63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29,
    2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5,
    52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38,
    28,
    58, 20, 37, 17, 36, 8
};



static struct board *get_clean_board(void);

/*
 * Creates and initialises a new board. The default starting piece
 * positions are populated.
 * name: init_board
 * @param fen : the FEN string
 * @return	a new board
 *
 */
struct board *init_board(char * fen)
{
    struct board *brd = get_clean_board();

    consume_fen_notation(fen, brd);

    return brd;
}



void remove_piece_from_board(struct board *brd, enum piece pce, enum square sq){


	if ((pce == W_KING) || (pce == B_KING)){
		print_board(brd);
		printf("pce = %c on square %s\n", get_piece_label(pce), print_square(sq));
	}
	assert((sq >= a1) && (sq <= h8));
	assert(pce != W_KING);
	assert(pce != B_KING);

	enum colour col = GET_COLOUR(pce);

	update_piece_hash(brd, pce, sq);

	brd->pieces[sq] = NO_PIECE;

	brd->material[col] -= get_piece_value(pce);

	// remove piece from bitboards
	clear_bit(&brd->bitboards[pce], sq);
	clear_bit(&brd->board, sq);
}


void add_piece_to_board(struct board *brd, enum piece pce, enum square sq){

	enum colour col = GET_COLOUR(pce);

	update_piece_hash(brd, pce, sq);

	brd->pieces[sq] = pce;

	brd->material[col] += get_piece_value(pce);

	// set piece on bitboards
	set_bit(&brd->bitboards[pce], sq);
	set_bit(&brd->board, sq);
}



/*
 * Creates an empty board struct
 * name: get_clean_board
 * @param
 * @return	ptr to a created board struct
 *
 */
static struct board *get_clean_board(void)
{
    struct board *brd = malloc(sizeof(struct board));

    memset(brd, 0, sizeof(struct board));

    for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		brd->pieces[sq] = NO_PIECE;
    }

    return brd;
}



U64 overlay_colours(const struct board *brd, enum colour col){
	if (col == WHITE){
		return brd->bitboards[W_PAWN] | brd->bitboards[W_BISHOP]
				| brd->bitboards[W_KNIGHT] | brd->bitboards[W_ROOK]
				| brd->bitboards[W_QUEEN] | brd->bitboards[W_KING];
	} else{
		return brd->bitboards[B_PAWN] | brd->bitboards[B_BISHOP]
				| brd->bitboards[B_KNIGHT] | brd->bitboards[B_ROOK]
				| brd->bitboards[B_QUEEN] | brd->bitboards[B_KING];
	}
}


void overlay_boards(struct board *the_board)
{
    int i = 0;
    U64 flat_board = BOARD_EMPTY;
    for (i = 0; i < NUM_PIECES; i++) {
		flat_board |= the_board->bitboards[i];
    }
    the_board->board = flat_board;
}

/*
 * Returns the piece type os the given square
 *
 * name: 	get_piece_at_square
 * @param	the board container and the square to test
 * @return	the piece or NO_PIECE
 *
 * TODO - possibly replace this with a cache lookup on a new board array
 */

inline enum piece get_piece_at_square(const struct board *the_board, enum square sq)
{
    assert((sq >= a1) && (sq <= h8));

    return the_board->pieces[sq];
}

/*
 *
 * name: set_bit
 * @param : board, square
 * @return : void
 *
 */
inline void set_bit(U64 * brd, enum square sq)
{

    assert((sq >= a1) && (sq <= h8));

    *brd = *brd | (U64) (0x01ull << sq);
}

/*
 *
 * name: clear_bit
 * @param : board, square
 * @return : void
 *
 */
inline void clear_bit(U64 * brd, enum square sq)
{

    assert((sq >= a1) && (sq <= h8));

    *brd = *brd & (U64) (~(0x01ull << sq));
}

/*
 *
 * name: check_bit
 * @param : board, square
 * @return : bool false if unset, bool true otherwise
 *
 */
inline bool check_bit(const U64 * brd, enum square sq)
{
    if (((*brd >> sq) & 0x01ull) != 0) {
		return true;
    }
    return false;
}

/*
 * Counts set bits in a U64
 * name: count_bits
 * @param 	the board
 * @return	the number of set bits
 *
 */
inline U8 count_bits(U64 bb)
{
	return (U8)__builtin_popcountll(bb);
}

/*
 * Clears the LSB of the board, and returns the bit # that was cleared.
 * name: pop_1st_bit
 * @param	ptr to U64
 * @return	index of bit cleared.
 *
 */
inline U8 pop_1st_bit(U64 * bb)
{
    U64 b = *bb ^ (*bb - 1);
    unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
    *bb &= (*bb - 1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}



void clear_MSB_to_inclusive_bit(U64 * bb, U8 bit){
	if (*bb == 0){
		return;
	}

	U8 msb = get_MSB_index(*bb);
	while ((msb >= bit) && (*bb != 0)) {
		clear_bit(bb, msb);
		//printf("cleared MSB %d, bb_bit = %d\n", msb, bit);
		msb = get_MSB_index(*bb);
	}

}


void clear_LSB_to_inclusive_bit(U64 * bb, U8 bit){

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
 * int __builtin_ctz (unsigned int x)
 *
 * Returns the number of trailing 0-bits in x, starting at the least
 * significant bit position. If x is 0, the result is undefined
 */
U8 get_LSB_index(U64 bb){
	// gcc built-in function (see https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)
	return (U8)__builtin_ctzll(bb);
}


U8 get_MSB_index(U64 bb){
// gcc built-in function (see https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)
	U8 b = (U8)__builtin_clzll(bb);

	// the above is number of leading zeros.
	// the MSB index is (63-b)
	return (U8)(63 - b);
}


inline bool is_square_occupied(U64 board, enum square square)
{
    if (check_bit(&board, square) != 0) {
		return true;
    }
    return false;
}
