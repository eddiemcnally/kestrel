/*
 * board.c
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chess is distributed in the hope that it will be useful, but
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
#include "board.h"
#include "pieces.h"


void overlay_boards(board_container_t *board_container);


void reset_board(board_container_t *board_to_reset) {

	clear_board(board_to_reset);

    board_to_reset->piece_boards[B_PAWN] 	= 	INIT_BRD_B_P;
    board_to_reset->piece_boards[B_ROOK] 	= 	INIT_BRD_B_R;
    board_to_reset->piece_boards[B_BISHOP] 	= 	INIT_BRD_B_B;
    board_to_reset->piece_boards[B_KNIGHT] 	= 	INIT_BRD_B_N;
    board_to_reset->piece_boards[B_QUEEN] 	= 	INIT_BRD_B_Q;
    board_to_reset->piece_boards[B_KING] 	= 	INIT_BRD_B_K;
    board_to_reset->piece_boards[W_PAWN] 	= 	INIT_BRD_W_P;
    board_to_reset->piece_boards[W_ROOK] 	= 	INIT_BRD_W_R;
    board_to_reset->piece_boards[W_BISHOP] 	= 	INIT_BRD_W_B;
    board_to_reset->piece_boards[W_KNIGHT] 	= 	INIT_BRD_W_N;
    board_to_reset->piece_boards[W_QUEEN] 	= 	INIT_BRD_W_Q;
    board_to_reset->piece_boards[W_KING] 	= 	INIT_BRD_W_K;

    board_to_reset->board = INIT_BRD_B | INIT_BRD_W;

}

/*
 *
 * name: clear_board
 * Cleans a board of all pieces
 * @param : board_container_t *	: the board to clear
 * @return : void
 *
 */
inline void clear_board(board_container_t *board_to_clear){

	board_to_clear->board = BOARD_EMPTY;

	for(int i = 0; i < NUM_PIECE_TYPES; i++){
		board_to_clear->piece_boards[i] = BOARD_EMPTY;
	}
}

/*
 *
 * name: set_bit
 * @param : board, square
 * @return : void
 *
 */
inline void set_bit(board_t *brd, square_t sq){
	*brd = *brd | (board_t)(0x01ull << sq);
}

/*
 *
 * name: clear_bit
 * @param : board, square
 * @return : void
 *
 */
inline void clear_bit(board_t *brd, square_t sq){
	*brd = *brd & (board_t)(~0x01ull << sq);
}

/*
 *
 * name: check_bit
 * @param : board, square
 * @return : bool false if unset, bool true otherwise
 *
 */
inline bool check_bit(board_t *brd, square_t sq){
	if (((*brd >> sq) & 0x01ull) !=0){
		return true;
	}
	return false;
}


board_container_t * get_clean_board(){
	
	board_container_t *the_board = malloc(sizeof (struct board_container));

	memset(the_board, 0, sizeof (struct board_container));
	 
	return the_board;
}

/*
 * Adds a piece to a board.
 * name: add_piece_to_board
 * @param : the board, the piece, and the square
 * @return : true if OK, false if piece already on that square
 *
 */
bool add_piece_to_board(board_container_t *board, piece_id_t piece, square_t square){

	if (check_bit(&board->board, square) != 0){
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


inline void overlay_boards(board_container_t *the_board) {
    int i = 0;
    board_t flat_board = BOARD_EMPTY;
    for (i = 0; i < NUM_PIECE_TYPES; i++) {
        flat_board = flat_board | the_board->piece_boards[i];
    }
    the_board->board = flat_board;
}


piece_id_t get_piece_at_square(board_container_t *the_board, square_t square){
	board_t the_piece = GET_PIECE_MASK(square);

	for(int i = 0; i < NUM_PIECE_TYPES; i++){

		board_t brd = the_board->piece_boards[i];

		piece_id_t piece = (piece_id_t)i;
		if ((brd & the_piece) != 0){
			// found it
			return piece;
		}
	}

	// no piece on that square
	return -1;
}


inline bool is_square_occupied(board_t board, square_t square){
	if ( check_bit(&board, square) != 0){
		return true;
	}
	return false;
}


inline board_t get_white_occupancy_map(board_container_t *the_board){
	return the_board->piece_boards[W_PAWN]
			| the_board->piece_boards[W_ROOK]
			| the_board->piece_boards[W_KNIGHT]
			| the_board->piece_boards[W_BISHOP]
			| the_board->piece_boards[W_QUEEN]
			| the_board->piece_boards[W_KING];
}


inline board_t get_black_occupancy_map(board_container_t *the_board){
	return the_board->piece_boards[B_PAWN]
			| the_board->piece_boards[B_ROOK]
			| the_board->piece_boards[B_KNIGHT]
			| the_board->piece_boards[B_BISHOP]
			| the_board->piece_boards[B_QUEEN]
			| the_board->piece_boards[B_KING];
}



void print_board(board_container_t *the_board) {
	
	char brd[8][8];
			
	for(int i =0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			brd[i][j] = ' ';
		}
	}
			
			
	for(int rank = 0; rank < 8; rank++){
		for(int file = 0; file < 8; file++){
			
			square_t square = (rank * 8) + file;
			
			piece_t piece = get_piece_at_square(the_board, square);
			if (piece >= 0){
				char pce = get_piece_label(piece);
				brd[rank][file] = pce;	
			}			
		}
	}
	
	for(int rank = 7; rank >= 0; rank--){
		for(int file = 0; file <= 7; file++){
			printf("%c", brd[rank][file]);
		}
		printf("\n\r");
	}
	
}

