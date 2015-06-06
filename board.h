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
    A1 = 0, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NUM_SQUARES
} square_t;



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


/*************************************************************
 * Initial Board Configuration
 *************************************************************/
/* initial board - black */
#define INIT_BRD_B_P	(board_t)(0x1ull << A7 \
								| 0x1ull << B7 \
								| 0x1ull << C7 \
								| 0x1ull << D7 \
								| 0x1ull << E7 \
								| 0x1ull << F7 \
								| 0x1ull << G7 \
								| 0x1ull << H7)
#define INIT_BRD_B_R 	(board_t)(0x1ull << A8 | 0x1ull << H8)
#define INIT_BRD_B_N 	(board_t)(0x1ull << B8 | 0x1ull << G8)
#define INIT_BRD_B_B 	(board_t)(0x1ull << C8 | 0x1ull << F8)
#define INIT_BRD_B_Q 	(board_t)(0x1ull << D8)
#define INIT_BRD_B_K 	(board_t)(0x1ull << E8)
#define INIT_BRD_B 		(board_t)(INIT_BRD_B_P | INIT_BRD_B_R | INIT_BRD_B_B | INIT_BRD_B_N | INIT_BRD_B_Q | INIT_BRD_B_K)


/* initial board - white */
#define INIT_BRD_W_P 	(board_t)(0x1ull << A2 \
								| 0x1ull << B2 \
								| 0x1ull << C2 \
								| 0x1ull << D2 \
								| 0x1ull << E2 \
								| 0x1ull << F2 \
								| 0x1ull << G2 \
								| 0x1ull << H2)
#define INIT_BRD_W_R 	(board_t)(0x1ull << A1 | 0x1ull << H1)
#define INIT_BRD_W_N 	(board_t)(0x1ull << B1 | 0x1ull << G1)
#define INIT_BRD_W_B 	(board_t)(0x1ull << C1 | 0x1ull << F1)
#define INIT_BRD_W_Q 	(board_t)(0x1ull << D1)
#define INIT_BRD_W_K 	(board_t)(0x1ull << E1)

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
void print_board(board_container_t *the_board);
piece_id_t get_piece_at_square(board_container_t *the_board, square_t square);
board_t get_black_piece_map(board_container_t *the_board);
board_t get_white_piece_map(board_container_t *the_board);



#endif
