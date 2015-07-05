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
#include "hashkeys.h"
#include "pieces.h"


void overlay_boards(board_container_t * board_container);
int count_minor_pieces(board_container_t * brd);
int count_big_pieces(board_container_t * brd);
int count_major_pieces(board_container_t * brd);


static const U8 BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
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
board_container_t * init_board(void){
	board_container_t * brd = get_clean_board();
		
	consume_fen_notation(STARTING_FEN, brd);
	
	return brd;
}


void update_piece_material(board_container_t * brd) {	
	
	for(square_t sq; sq < NUM_SQUARES; sq++) {
		piece_id_t pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE){
			colour_t colour = get_colour(pce);
			
			if (is_big_piece(pce))
				brd->big_pieces[colour]++;
			if (is_minor_piece(pce))
				brd->minor_pieces[colour]++;
			if (is_major_piece(pce))
				brd->major_pieces[colour]++;

			brd->material[colour] += piece_values[pce];
			brd->pce_num[pce]++;
			
			if(pce == W_KING) brd->king_squares[WHITE] = sq;
			if(pce == B_KING) brd->king_squares[BLACK] = sq;
			
		}
	}

}





/*
 * Creates an empty board struct 
 * name: get_clean_board
 * @param			
 * @return	ptr to a created board struct
 * 
 */
board_container_t *get_clean_board(void)
{
    board_container_t *brd = malloc(sizeof(board_container_t));

    memset(brd, 0, sizeof(board_container_t));
    
    brd->king_squares[WHITE] = NO_SQUARE;
    brd->king_squares[BLACK] = NO_SQUARE;

	for(square_t sq = 0; sq < NUM_SQUARES; sq++){
		brd->pieces[sq] = NO_PIECE;
	}

    return brd;
}

/*
 * Adds a piece to a board.
 * name: add_piece_to_board
 * @param : the board, the piece, and the square
 * @return : true if OK, false if piece already on that square
 *
 */
bool add_piece_to_board(board_container_t * board, piece_id_t piece,
			square_t square)
{
	assert((square >= a1) && (square <= h8));
	assert((piece >= W_PAWN) && (piece <= B_KING));
	
	
    if (check_bit(&board->board, square) != 0) {
		// square already occupied
		assert(check_bit(&board->board, square) != 0);
		return false;
    } else {
		// set bit in relevant piece board
		set_bit(&(board->bitboards[piece]), square);

		// regen flat board
		overlay_boards(board);

		//printf("adding %d to square %d\n", piece, square);
		board->pieces[square] = piece;
		return true;
    }
}


inline void overlay_boards(board_container_t * the_board)
{
    int i = 0;
    board_t flat_board = BOARD_EMPTY;
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

inline piece_id_t get_piece_at_square(board_container_t * the_board,
			       square_t square)
{
	assert((square >= a1) && (square <= h8));
	
	return the_board->pieces[square];	
	
}



/*
 *
 * name: set_bit
 * @param : board, square
 * @return : void
 *
 */
inline void set_bit(board_t * brd, square_t sq)
{
	assert((sq >= a1) && (sq <= h8));

    *brd = *brd | (board_t) (0x01ull << sq);
}


/*
 *
 * name: clear_bit
 * @param : board, square
 * @return : void
 *
 */
inline void clear_bit(board_t * brd, square_t sq)
{
	assert((sq >= a1) && (sq <= h8));

    *brd = *brd & (board_t) (~(0x01ull << sq));
}

/*
 *
 * name: check_bit
 * @param : board, square
 * @return : bool false if unset, bool true otherwise
 *
 */
inline bool check_bit(board_t * brd, square_t sq)
{
	assert((sq >= a1) && (sq <= h8));
	
    if (((*brd >> sq) & 0x01ull) != 0) {
		return true;
	}
    return false;
}

/*
 * Counts set bits in a board_t 
 * name: count_bits
 * @param 	the board
 * @return	the number of set bits
 * 
 */
inline U8 count_bits(board_t bb){
	U8 cntr;
	for (cntr = 0; bb; cntr++)	{
		bb &= bb - 1; // clear the least significant bit set
	}
	return cntr; 
}



/*
 * Clears the LSB of the board, and returns the bit # that was cleared. 
 * name: pop_1st_bit
 * @param	ptr to board_t
 * @return	index of bit cleared.
 * 
 */
inline U8 pop_1st_bit(board_t *bb) {
	board_t b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	return BitTable[(fold * 0x783a9b23) >> 26];
}




inline bool is_square_occupied(board_t board, square_t square)
{
	assert((square >= a1) && (square <= h8));

    if (check_bit(&board, square) != 0) {
		return true;
	}
    return false;
}



