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

/*
 * Creates and initialises a new board. The default starting piece
 * positions are populated.
 * name: init_board
 * @param
 * @return	a new board
 *
 */
struct board *init_board(void)
{
    struct board *brd = get_clean_board();

    consume_fen_notation(STARTING_FEN, brd);

    return brd;
}



void remove_piece_from_board(struct board *brd, enum piece pce, enum square sq){

	assert((sq >= a1) && (sq <= h8));

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
struct board *get_clean_board(void)
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

	if ((sq < a1) || (sq > h8)){
		printf("uibiupiuv");
	}

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
	if ((sq < a1) || (sq > h8)){
		printf("uibiupiuv");
	}

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
	if ((sq < a1) || (sq > h8)){
		printf("uibiupiuv");
	}

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
    U8 cntr;
    for (cntr = 0; bb; cntr++) {
		bb &= bb - 1;		// clear the least significant bit set
    }
    return cntr;
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

inline bool is_square_occupied(U64 board, enum square square)
{
    if (check_bit(&board, square) != 0) {
		return true;
    }
    return false;
}
