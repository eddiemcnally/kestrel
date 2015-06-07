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
#include "board.h"
#include "pieces.h"


/*
 *
 * name: reset_board
 * @param: board_container_t *	:	the board to reset
 * @return: void
 *
 */
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
void clear_board(board_container_t *board_to_clear){

	board_to_clear->board = (board_t)0;

	for(int i = 0; i < NUM_PIECE_TYPES; i++){
		board_to_clear->piece_boards[i] = (board_t)0;
	}
}

/*
 *
 * name: set_bit
 * @param : board, square
 * @return : void
 *
 */
void set_bit(board_t *brd, square_t sq){
	*brd = *brd | (board_t)(0x01ull << sq);
}

/*
 *
 * name: clear_bit
 * @param : board, square
 * @return : void
 *
 */
void clear_bit(board_t *brd, square_t sq){
	*brd = *brd & (board_t)(~0x01ull << sq);
}

/*
 *
 * name: check_bit
 * @param : board, square
 * @return : 0 if unset, 1 otherwise
 *
 */
int check_bit(board_t *brd, square_t sq){
	return (*brd >> sq) & 0x01ull;
}





int add_piece_to_board(board_container_t *board, piece_id_t piece, square_t square){

	if (check_bit(&board->board, square) != 0){
		// square already occupied
		return -1;
	} else{
		// set bit in relevant piece board
		set_bit(&(board->piece_boards[piece]), square);

		// regen flat board
		overlay_boards(board);

		return 0;
	}
}


void overlay_boards(board_container_t *board_container) {
    int i = 0;
    board_t flat_board = 0;
    for (i = 0; i < NUM_PIECE_TYPES; i++) {
        flat_board = flat_board | board_container->piece_boards[i];
    }
    board_container->board = flat_board;
}


piece_id_t get_piece_at_square(board_container_t *the_board, square_t square){

	board_t the_piece = GET_PIECE_MASK(square);
	//printf("piece mask for square %d= 0x%llx\n\r", (int)square, (unsigned long long)the_piece);

	for(int i = 0; i < NUM_PIECE_TYPES; i++){

		board_t brd = the_board->piece_boards[i];

		piece_id_t piece = (piece_id_t)i;
		//char c = get_piece_label(piece);

		//printf("piece %c board = 0x%llx\n\r", c, (unsigned long long)brd);

		if ((brd & the_piece) != 0){
			// found it
			return piece;
		}
	}


	// no piece on that square
	return -1;

}


board_t get_white_piece_map(board_container_t *the_board){
	return the_board->piece_boards[W_PAWN]
			| the_board->piece_boards[W_ROOK]
			| the_board->piece_boards[W_KNIGHT]
			| the_board->piece_boards[W_BISHOP]
			| the_board->piece_boards[W_QUEEN]
			| the_board->piece_boards[W_KING];
}


board_t get_black_piece_map(board_container_t *the_board){
	return the_board->piece_boards[B_PAWN]
			| the_board->piece_boards[B_ROOK]
			| the_board->piece_boards[B_KNIGHT]
			| the_board->piece_boards[B_BISHOP]
			| the_board->piece_boards[B_QUEEN]
			| the_board->piece_boards[B_KING];
}




void print_board(board_container_t *the_board) {

    int i = 0;

	// board[0] = A1, board[1] = B1, etc
    char board[NUM_SQUARES] = {' '};

	for(int i = 0; i < NUM_PIECE_TYPES; i++){
		board_t brd = the_board->piece_boards[i];

		piece_id_t piece = (piece_id_t)i;
		char c = get_piece_label(piece);

		for(int j = 0; j < NUM_SQUARES; j++){
			if (((brd >> j) & 0x01) != 0){
				board[j] = c;
			}
		}
	}

	// char array is filled in according to char[0]= A1
	// so swap for display

    for (i = NUM_SQUARES; i >=0; i--) {
        printf("%c", board[i]);
		if ((i % 8 == 0)) {
            printf("\n\r");
        }

    }
}
