/*
 * board.h
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

#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>
#include "types.h"
#include "board.h"


#define	BOARD_EMPTY	((board_t)0x0ull)

#define NUM_RANKS 	8
#define NUM_FILES	8

enum {
	RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8
};

enum {
	FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};



#define	NO_SQUARE	(-1)

#define GET_PIECE_MASK(square)		((board_t)(0x01ull << (int)(square)))


// zero-based
#define GET_RANK(square)			((square) / 8)
#define GET_FILE(square)			((square) % 8)
#define GET_SQUARE(RANK, FILE)		((RANK * 8) + FILE)

// castling permissions
enum {
	WKCA = 0x01,	// white, king-side
	WQCA = 0x02,	// white, queen-side
	BKCA = 0x04,	// black, king-side
	BQCA = 0x08		// black, queen-side
};


//#define	NUM_PIECE_TYPES	12


#define IS_WHITE(piece)			(piece <= W_KING)
#define IS_BLACK(piece)			(IS_WHITE(piece) == false)

// offsets into this array are by type piece_id_t 
static const char pieceToChar[NUM_PIECE_TYPES] = {
    'P', 'R', 'N', 'B', 'Q', 'K', 'p', 'r', 'n', 'b', 'q', 'k'
};


// some shortcut macros
#define POP(bb)			pop_1st_bit(bb)
#define	CNT(bb)			count_bits(bb)


board_container_t * init_board();
board_container_t *get_clean_board();
piece_id_t get_piece_at_square(board_container_t * the_board, square_t square);
bool add_piece_to_board(board_container_t * board, piece_id_t piece, square_t square);
board_t get_black_occupancy_map(board_container_t * the_board);
board_t get_white_occupancy_map(board_container_t * the_board);
bool is_square_occupied(board_t board, square_t square);
void set_bit(board_t * brd, square_t sq);
void clear_bit(board_t * brd, square_t sq);
bool check_bit(board_t * brd, square_t sq);
U8 count_bits(board_t bb);
U8 pop_1st_bit(board_t *bb);



#endif
