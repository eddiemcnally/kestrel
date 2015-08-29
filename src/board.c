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



const int debruijn_index64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};


static struct board *get_clean_board(void);
static inline U8 bit_scan_reverse(U64 bb);
static inline U8 get_LSB_index(U64 bb);

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



void clear_MSB_to_inclusive_bit(U64 * bb, U8 bit){
	U8 msb = bit_scan_reverse(*bb);
	while (msb > bit){
		msb = bit_scan_reverse(*bb);
		clear_bit(bb, msb);
	}
}


void clear_LSB_to_inclusive_bit(U64 * bb, U8 bit){

	U8 lsb_offset = get_LSB_index(*bb);

	if (lsb_offset > bit){
		// nothing to do
		return;
	} else {
		do {
			lsb_offset = POP(bb);
		} while(lsb_offset < bit);
	}
}


/**
 * Taken from https://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightLinear
 *
 * counts trailing 0's on the right of the U64
 */
static inline U8 get_LSB_index(U64 bb){
	U8 c;  // output: c will count v's trailing zero bits,
        // so if v is 1101000 (base 2), then c will be 3
	if (bb) {
		bb = (bb ^ (bb - 1)) >> 1;  // Set bb's trailing 0s to 1s and zero rest
		for (c = 0; bb; c++) {
			bb >>= 1;
		}
		return c;
	}
	return 0;
}


/**
 * bitScanReverse
 * @authors Kim Walisch, Mark Dickinson
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of most significant one bit
 */
static inline U8 bit_scan_reverse(U64 bb) {
	const U64 debruijn64 = 0x03f79d71b4cb0a89;
	bb |= bb >> 1;
	bb |= bb >> 2;
	bb |= bb >> 4;
	bb |= bb >> 8;
	bb |= bb >> 16;
	bb |= bb >> 32;
	return debruijn_index64[(bb * debruijn64) >> 58];
}



inline bool is_square_occupied(U64 board, enum square square)
{
    if (check_bit(&board, square) != 0) {
		return true;
    }
    return false;
}
