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
#include "board.h"
#include "board_utils.h"
#include "hashkeys.h"
#include "pieces.h"


void overlay_boards(board_container_t * board_container);
void reset_board_to_init_position(board_container_t * board_to_reset);
void clear_board(board_container_t * board_to_clear);



const U8 BitTable[64] = {
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
board_container_t * init_board(){
	board_container_t * brd = get_clean_board();
	
	reset_board_to_init_position(brd);
	
	
	brd->king_squares[WHITE] = e5;
	brd->king_squares[BLACK] = e8;
	
	brd->side_to_move = WHITE;
	
	brd->en_passant = NO_SQUARE;
	
	brd->fifty_move_counter = 0;
	brd->ply = 0;
	brd->history_ply = 0;
	
	brd->pce_num[W_PAWN] 	= 8;
	brd->pce_num[W_KNIGHT] 	= 2;
	brd->pce_num[W_BISHOP] 	= 2;
	brd->pce_num[W_ROOK] 	= 2;
	brd->pce_num[W_QUEEN] 	= 1;
	brd->pce_num[W_KING] 	= 1;
	
	brd->pce_num[B_PAWN] 	= 8;
	brd->pce_num[B_KNIGHT] 	= 2;
	brd->pce_num[B_BISHOP] 	= 2;
	brd->pce_num[B_ROOK] 	= 2;
	brd->pce_num[B_QUEEN] 	= 1;
	brd->pce_num[B_KING] 	= 1;

	brd->big_pieces[WHITE] 	= 8;
	brd->big_pieces[BLACK] 	= 8;

	brd->major_pieces[WHITE] = 3;
	brd->major_pieces[BLACK] = 3;
	
	brd->minor_pieces[WHITE] = 4;
	brd->minor_pieces[BLACK] = 4;
	
	brd->castle_perm = (WQCA | WKCA | BQCA | BKCA);

	brd->position_key = get_position_hashkey(brd);
	//printf("******* 0x%016llx\n", brd->position_key);

	for(int i = 0; i < MAX_GAME_MOVES;i++){
		brd->history[i].castle_perm = 0;
		brd->history[i].en_passant = 0;
		brd->history[i].fifty_move_counter = 0;
		brd->history[i].position_key = 0;
	}


	return brd;
}



void reset_board_to_init_position(board_container_t * board_to_reset){

	assert(board_to_reset != 0);
	
    clear_board(board_to_reset);
    
    add_piece_to_board(board_to_reset, 	B_PAWN, 	a7);
    add_piece_to_board(board_to_reset, 	B_PAWN, 	b7);
    add_piece_to_board(board_to_reset, 	B_PAWN, 	c7);
    add_piece_to_board(board_to_reset, 	B_PAWN, 	d7);
    add_piece_to_board(board_to_reset, 	B_PAWN, 	e7);
    add_piece_to_board(board_to_reset, 	B_PAWN, 	f7);
    add_piece_to_board(board_to_reset, 	B_PAWN, 	g7);
    add_piece_to_board(board_to_reset, 	B_PAWN, 	h7);
       
    add_piece_to_board(board_to_reset, 	B_ROOK, 	a8);
    add_piece_to_board(board_to_reset, 	B_ROOK, 	h8);
    add_piece_to_board(board_to_reset, 	B_KNIGHT, 	b8);
    add_piece_to_board(board_to_reset, 	B_KNIGHT, 	g8);
    add_piece_to_board(board_to_reset, 	B_BISHOP, 	c8);
    add_piece_to_board(board_to_reset, 	B_BISHOP, 	f8);
    add_piece_to_board(board_to_reset, 	B_QUEEN, 	d8);
    add_piece_to_board(board_to_reset, 	B_KING, 	e8);
    
    
    add_piece_to_board(board_to_reset, 	W_PAWN, 	a2);
    add_piece_to_board(board_to_reset, 	W_PAWN, 	b2);
    add_piece_to_board(board_to_reset, 	W_PAWN, 	c2);
    add_piece_to_board(board_to_reset, 	W_PAWN, 	d2);
    add_piece_to_board(board_to_reset, 	W_PAWN, 	e2);
    add_piece_to_board(board_to_reset, 	W_PAWN, 	f2);
    add_piece_to_board(board_to_reset, 	W_PAWN, 	g2);
    add_piece_to_board(board_to_reset, 	W_PAWN, 	h2);
    
    add_piece_to_board(board_to_reset, 	W_ROOK, 	a1);
    add_piece_to_board(board_to_reset, 	W_ROOK, 	h1);
    add_piece_to_board(board_to_reset, 	W_KNIGHT, 	b1);
    add_piece_to_board(board_to_reset, 	W_KNIGHT, 	g1);
    add_piece_to_board(board_to_reset, 	W_BISHOP, 	c1);
    add_piece_to_board(board_to_reset, 	W_BISHOP, 	f1);
    add_piece_to_board(board_to_reset, 	W_QUEEN, 	d1);
    add_piece_to_board(board_to_reset, 	W_KING, 	e1);
    
    
}

void update_piece_material(board_container_t * brd) {	
	
	for(square_t sq; sq < NUM_SQUARES; sq++) {
		piece_id_t pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE){
			colour_t colour = get_colour(pce);
			
			if (IS_BIG_PIECE(pce))
				brd->big_pieces[colour]++;
			if (IS_MINOR_PIECE(pce))
				brd->minor_pieces[colour]++;
			if (IS_MAJOR_PIECE(pce))
				brd->major_pieces[colour]++;

			brd->material[colour] += piece_values[pce];
			brd->pce_num[pce]++;
			
			if(pce == W_KING) brd->king_squares[WHITE] = sq;
			if(pce == B_KING) brd->king_squares[BLACK] = sq;
			
		}
	}

}





/*
 * name: clear_board
 * Cleans a board of all pieces
 * @param : board_container_t *	: the board to clear
 * @return : void
 *
 */
inline void clear_board(board_container_t * board_to_clear)
{
    board_to_clear->board = BOARD_EMPTY;

    for (int i = 0; i < NUM_PIECE_TYPES; i++) {
		board_to_clear->piece_boards[i] = BOARD_EMPTY;
    }
}

/*
 * Creates an empty board struct 
 * name: get_clean_board
 * @param			
 * @return	ptr to a created board struct
 * 
 */
board_container_t *get_clean_board()
{
    board_container_t *the_board = malloc(sizeof(board_container_t));

    memset(the_board, 0, sizeof(board_container_t));

    return the_board;
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
		return false;
    } else {
		// set bit in relevant piece board
		set_bit(&(board->piece_boards[piece]), square);

		// regen flat board
		overlay_boards(board);

		return true;
    }
}


inline void overlay_boards(board_container_t * the_board)
{
    int i = 0;
    board_t flat_board = BOARD_EMPTY;
    for (i = 0; i < NUM_PIECE_TYPES; i++) {
		flat_board = flat_board | the_board->piece_boards[i];
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
	
	// quick check to see if square occupied
	if (check_bit(&the_board->board, square)){
		// it is, so find the piece type
		board_t piece_mask = GET_PIECE_MASK(square);

		for (int pce = 0; pce < NUM_PIECE_TYPES; pce++) {
			board_t brd = the_board->piece_boards[pce];

			if ((brd & piece_mask) != 0) {
				// found it
				return pce;
			}
		}
	} 
	// no piece on that square
	return NO_PIECE;
	
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

    *brd = *brd & (board_t) (~0x01ull << sq);
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

// returns a board with all white pieces
inline board_t get_white_occupancy_map(board_container_t * the_board)
{
    return the_board->piece_boards[W_PAWN]
				| the_board->piece_boards[W_ROOK]
				| the_board->piece_boards[W_KNIGHT]
				| the_board->piece_boards[W_BISHOP]
				| the_board->piece_boards[W_QUEEN]
				| the_board->piece_boards[W_KING];
}

// returns a board with all black pieces
inline board_t get_black_occupancy_map(board_container_t * the_board)
{
    return the_board->piece_boards[B_PAWN]
				| the_board->piece_boards[B_ROOK]
				| the_board->piece_boards[B_KNIGHT]
				| the_board->piece_boards[B_BISHOP]
				| the_board->piece_boards[B_QUEEN]
				| the_board->piece_boards[B_KING];
}

