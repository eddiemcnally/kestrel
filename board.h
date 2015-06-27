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
#include "pieces.h"


typedef unsigned long long board_t;

#define	BOARD_EMPTY	((board_t)0x0ull)

typedef unsigned char 			U8;
typedef unsigned long long 		U64;

// half moves
#define MAX_GAME_MOVES 2048





#define NUM_RANKS 	8
#define NUM_FILES	8


#define	NO_SQUARE	-1


#define GET_PIECE_MASK(square)		((board_t)(0x01ull << (int)square))


typedef enum {
    a1 = 0, b1, c1, d1, e1, f1, g1, h1,
    a2, 	b2, c2, d2, e2, f2, g2, h2,
    a3, 	b3, c3, d3, e3, f3, g3, h3,
    a4, 	b4, c4, d4, e4, f4, g4, h4,
    a5, 	b5, c5, d5, e5, f5, g5, h5,
    a6, 	b6, c6, d6, e6, f6, g6, h6,
    a7, 	b7, c7, d7, e7, f7, g7, h7,
    a8, 	b8, c8, d8, e8, f8, g8, h8,
    NUM_SQUARES
} square_t;


// zero-based
#define GET_RANK(square)			(square / 8)
#define GET_FILE(square)			(square % 8)
#define GET_SQUARE(RANK, FILE)		((RANK * 8) + FILE)

// castling permissions
enum {
	WKCA = 1,	// white, king-side
	WQCA = 2,	// white, queen-side
	BKCA = 4,	// black, king-side
	BQCA = 8	// black, queen-side
};


#define	NUM_PIECE_TYPES	12


typedef enum {
	BLACK,
	WHITE
} colour_t;

#define IS_WHITE(piece_id_t)		(piece_id_t <= piece_id_t.W_KING)
#define IS_BLACK(piece_id_t)		(IS_WHITE(piece_id_t) == false)

// offsets into this array are by type piece_id_t 
static const char pieceToChar[NUM_PIECE_TYPES] = {
    'P', 'R', 'N', 'B', 'Q', 'K', 'p', 'r', 'n', 'b', 'q', 'k'
};


// some shortcut macros
#define POP(bb)			pop_1st_bit(bb)
#define	CNT(bb)			count_bits(bb)


// contains information before the current
// move was made
typedef struct {
	int move;
	U8 castle_perm;
	U8 en_passant;
	U8 fifty_move_counter;
	U64 position_key;
} undo_t;


/**
 * A container for holding the bitboards
 */
typedef struct {
    // an entry for each piece type
    board_t piece_boards[NUM_PIECE_TYPES];

    // The above array piece arrays overlayed into a single bitboard.
    // In effect, an OR of all elements in piece_boards[]
    board_t board;


	// squares where the kings are
	square_t king_squares[2];

	// the next side to move
	colour_t side_to_move;
	
	// any square where en passent is active
	// TO DO
	square_t en_passant;

	// fifty move ounter
	U8 fifty_move_counter;
	
	// keeping track of ply
	U8 ply;
	U8 history_ply;
	
	// indexed by piece_id_t, contains the number of pieces of that type on the board
	U8 pce_num[NUM_PIECE_TYPES];

	// indexed by colour_t, contains number of pieces != PAWN
	U8 big_pieces[2]; 
	
	// indexed by colour_t, contains number of ROOKs and QUEENs
	U8 major_pieces[2];
	
	// indexed by colour_t, contains number of BISHOPs and KNIGHTs
	U8 minor_pieces[2];

	// castling permissions
	U8 castle_perm;

	// move history
	undo_t history[MAX_GAME_MOVES];
	
	U64 position_key;
	
} board_container_t;


board_container_t * init_board();
board_container_t *get_clean_board();
piece_id_t get_piece_at_square(board_container_t * the_board,
			       square_t square);
bool add_piece_to_board(board_container_t * board, piece_id_t piece,
			square_t square);
board_t get_black_occupancy_map(board_container_t * the_board);
board_t get_white_occupancy_map(board_container_t * the_board);
bool is_square_occupied(board_t board, square_t square);
void set_bit(board_t * brd, square_t sq);
void clear_bit(board_t * brd, square_t sq);
bool check_bit(board_t * brd, square_t sq);
U8 count_bits(board_t bb);
U8 pop_1st_bit(board_t *bb);



#endif
