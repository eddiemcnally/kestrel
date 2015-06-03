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


typedef unsigned long board_t;


enum squares {
    A1 = 0, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NUM_SQUARES
};



typedef enum board_array_offset {
    BLK_P = 0,
    BLK_R,
    BLK_B,
    BLK_N,
    BLK_Q,
    BLK_K,
    WHT_P,
    WHT_R,
    WHT_B,
    WHT_N,
    WHT_Q,
    WHT_K,
    NUM_PIECE_TYPES
} board_array_offsets_t;




/*************************************************************
 * Initial Board Configuration
 *************************************************************/
/* initial board - black */
#define BRD_B_P		(board_t)(0x1ul << A7 | 0x1ul << B7 | 0x1ul << C7 | 0x1ul << D7 | 0x1ul << E7 | 0x1ul << F7 |0x1ul << G7 |0x1ul << H7)
#define BRD_B_R 	(board_t)(0x1ul << A8 | 0x1ul << H8)
#define BRD_B_N 	(board_t)(0x1ul << B8 | 0x1ul << G8)
#define BRD_B_B 	(board_t)(0x1ul << C8 | 0x1ul << F8)
#define BRD_B_Q 	(board_t)(0x1ul << D8)
#define BRD_B_K 	(board_t)(0x1ul << E8)

#define BRD_B 		(board_t)(BRD_B_P | BRD_B_R | BRD_B_B | BRD_B_N | BRD_B_Q | BRD_B_K)


/* initial board - white*/
#define BRD_W_P 	(board_t)(0x1ul << A2 | 0x1ul << B2 | 0x1ul << C2 | 0x1ul << D2 | 0x1ul << E2 | 0x1ul << F2 | 0x1ul << G2 | 0x1ul << H2)
#define BRD_W_R 	(board_t)(0x1ul << A1 | 0x1ul << H1)
#define BRD_W_N 	(board_t)(0x1ul << B1 | 0x1ul << G1)
#define BRD_W_B 	(board_t)(0x1ul << C1 | 0x1ul << F1)
#define BRD_W_Q 	(board_t)(0x1ul << D1)
#define BRD_W_K 	(board_t)(0x1ul << E1)

#define BRD_W 		(board_t)(BRD_W_P | BRD_W_R | BRD_W_B | BRD_W_N | BRD_W_Q | BRD_W_K)



/**
 * A container for holding the bitboards
 */
typedef struct board_container {
    // an entry for each piece type, offset by the BRD_ offsets
    board_t piece_boards[NUM_PIECE_TYPES];
    // the above array piece arrays overlayed into a single bitboard
    board_t board;
}board_container_t;


void reset_board(board_container_t *board_to_reset);
void print_board(board_container_t *the_board);



#endif
