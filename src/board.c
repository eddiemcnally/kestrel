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
#include <stdlib.h>
#include "board.h"
#include "pieces.h"

static piece_t convert_piece_map_to_piece(board_array_offsets_t board_offset);

/*
 * Creates a new board layer
 * 
 */

void reset_board(board_container_t *board_to_reset) {

    board_to_reset->piece_boards[BLK_P] = BRD_B_P;
    board_to_reset->piece_boards[BLK_R] = BRD_B_R;
    board_to_reset->piece_boards[BLK_B] = BRD_B_B;
    board_to_reset->piece_boards[BLK_N] = BRD_B_N;
    board_to_reset->piece_boards[BLK_Q] = BRD_B_Q;
    board_to_reset->piece_boards[BLK_K] = BRD_B_K;
    board_to_reset->piece_boards[WHT_P] = BRD_W_P;
    board_to_reset->piece_boards[WHT_R] = BRD_W_R;
    board_to_reset->piece_boards[WHT_B] = BRD_W_B;
    board_to_reset->piece_boards[WHT_N] = BRD_W_N;
    board_to_reset->piece_boards[WHT_Q] = BRD_W_Q;
    board_to_reset->piece_boards[WHT_K] = BRD_W_K;

    board_to_reset->board = BRD_B | BRD_W;

}

board_t overlay_boards(board_container_t *board_container) {
    int i = 0;
    board_t flat_board = 0;
    for (i = 0; i < NUM_PIECE_TYPES; i++) {
        flat_board = flat_board | board_container->piece_boards[i];
    }
    return flat_board;
}

void print_board(board_container_t *the_board) {

    int i = 0;
    int j = 0;

    char board[NUM_SQUARES] = {' '};

    for (i = 0; i < NUM_PIECE_TYPES; i++) {
        board_t piece_map = the_board->piece_boards[i];

        for (j = 0; j < NUM_SQUARES; j++) {
            if ((piece_map & 0x1) != 0) {
                piece_t piece = convert_piece_map_to_piece((board_array_offsets_t)i);
                char p = get_piece_label(piece);
                board[j] = p;
            }
            piece_map = piece_map >> 1;
            if (piece_map == 0){
            	// no more bits set, so no pieces to consider
            	break;
            }
        }
    }

    for (i = 0; i < NUM_SQUARES; i++) {
        if ((i > 0) && (i % 8 == 0)) {
            printf("\n\r");
        }
        printf("%c", board[i]);
    }
}

static piece_t convert_piece_map_to_piece(board_array_offsets_t board_offset) {
    switch (board_offset) {
        case (BLK_P):
            return B_PAWN;
        case (BLK_R):
            return B_ROOK;
        case (BLK_B):
            return B_BISHOP;
        case (BLK_N):
            return B_KNIGHT;
        case (BLK_Q):
            return B_QUEEN;
        case(BLK_K):
            return B_KING;
        case (WHT_P):
            return W_PAWN;
        case (WHT_R):
            return W_ROOK;
        case (WHT_B):
            return W_BISHOP;
        case (WHT_N):
            return W_KNIGHT;
        case (WHT_Q):
            return W_QUEEN;
        case(WHT_K):
            return W_KING;
        default:
            return -1;
    }
}
