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


typedef unsigned long long board_t;


#define NUM_RANKS 	8
#define NUM_FILES	8


#define GET_PIECE_MASK(square)	((board_t)(0x01ull << (int)square))


typedef enum squares {
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

// square numbers in the board
// 56 57 58 59 60 61 62 63
// 48 49 50 51 52 53 54 55
// 40 41 42 43 44 45 46 47 
// 32 33 34 35 36 37 38 39
// 24 25 26 27 28 29 30 31
// 16 17 18 19 20 21 22 23
// 08 09 10 11 12 13 14 15
// 00 01 02 03 40 05 06 07


#define GET_RANK_ZERO_BASED(square)		(square / 8)
#define GET_FILE_ZERO_BASED(square)		(square % 8)


typedef enum piece_id {
	W_PAWN 			= 	0,
	W_ROOK			= 	1,
	W_KNIGHT		=	2,
	W_BISHOP		=	3,
	W_QUEEN			=	4,
	W_KING			=	5,
	B_PAWN			=	6,
	B_ROOK			=	7,
	B_KNIGHT		=	8,
	B_BISHOP		=	9,
	B_QUEEN			=	10,
	B_KING			=	11,
} piece_id_t;

#define	NUM_PIECE_TYPES	12

#define IS_WHITE(piece_id_t)	(piece_id_t <= piece_id_t.W_KING)
#define IS_BLACK(piece_id_t)	(IS_WHITE(piece_id_t) == FALSE)

static const char pieceToChar[12] = {
	'P','R', 'N','B', 'Q', 'K', 'p','r', 'n','b', 'q','k'
};



/*************************************************************
 * Initial Board Configuration
 *************************************************************/
/* initial board - black */
#define INIT_BRD_B_P	(board_t)(0x1ull << a7 \
								| 0x1ull << b7 \
								| 0x1ull << c7 \
								| 0x1ull << d7 \
								| 0x1ull << e7 \
								| 0x1ull << f7 \
								| 0x1ull << g7 \
								| 0x1ull << h7)
#define INIT_BRD_B_R 	(board_t)(0x1ull << a8 | 0x1ull << h8)
#define INIT_BRD_B_N 	(board_t)(0x1ull << b8 | 0x1ull << g8)
#define INIT_BRD_B_B 	(board_t)(0x1ull << c8 | 0x1ull << f8)
#define INIT_BRD_B_Q 	(board_t)(0x1ull << d8)
#define INIT_BRD_B_K 	(board_t)(0x1ull << e8)
#define INIT_BRD_B 		(board_t)(INIT_BRD_B_P | INIT_BRD_B_R | INIT_BRD_B_B | INIT_BRD_B_N | INIT_BRD_B_Q | INIT_BRD_B_K)


/* initial board - white */
#define INIT_BRD_W_P 	(board_t)(0x1ull << a2 \
								| 0x1ull << b2 \
								| 0x1ull << c2 \
								| 0x1ull << d2 \
								| 0x1ull << e2 \
								| 0x1ull << f2 \
								| 0x1ull << g2 \
								| 0x1ull << h2)
#define INIT_BRD_W_R 	(board_t)(0x1ull << a1 | 0x1ull << h1)
#define INIT_BRD_W_N 	(board_t)(0x1ull << b1 | 0x1ull << g1)
#define INIT_BRD_W_B 	(board_t)(0x1ull << c1 | 0x1ull << f1)
#define INIT_BRD_W_Q 	(board_t)(0x1ull << d1)
#define INIT_BRD_W_K 	(board_t)(0x1ull << e1)

#define INIT_BRD_W 		(board_t)(INIT_BRD_W_P | INIT_BRD_W_R | INIT_BRD_W_B | INIT_BRD_W_N | INIT_BRD_W_Q | INIT_BRD_W_K)



/**
 * A container for holding the bitboards
 */
typedef struct board_container {
    // an entry for each piece type
    board_t piece_boards[NUM_PIECE_TYPES];

    // the above array piece arrays overlayed into a single bitboard
    board_t board;
}board_container_t;


void reset_board(board_container_t *board_to_reset);
void clear_board(board_container_t *board_to_clear);
void print_board(board_container_t *the_board);
void overlay_boards(board_container_t *board_container);
piece_id_t get_piece_at_square(board_container_t *the_board, square_t square);
int add_piece_to_board(board_container_t *board, piece_id_t piece, square_t square);
board_t get_black_piece_map(board_container_t *the_board);
board_t get_white_piece_map(board_container_t *the_board);
void set_bit(board_t *brd, square_t sq);
void clear_bit(board_t *brd, square_t sq);
int check_bit(board_t *brd, square_t sq);


#endif
