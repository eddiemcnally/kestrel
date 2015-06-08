/*
 * board_utils.c
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




/*
 *
 * name: Takes a char array as FEN notation, and returns a populated
 * board
 * @param 	char * representing the FEN string
 * 			board * representing the bpard tp be populated
 * @return 	0 = OK, -1 Error
 *
 */
int consume_fen_notation(char *fen_string, board_container_t *board_to_setup){

	//example of starting poition:
	//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

	// step 1: ignore moves, castling, etc, just return the board
	int sq = 0;

	// ensure board is initialised
	clear_board(board_to_setup);

	while (sq < NUM_SQUARES) {
		char c = *(fen_string + sq);
		switch (c) {
			case 'p' :
				add_piece_to_board(board_to_setup, B_PAWN, sq);
				break;
			case 'r' :
				add_piece_to_board(board_to_setup, B_ROOK, sq);
				break;
			case 'n' :
				add_piece_to_board(board_to_setup, B_KNIGHT, sq);
				break;
			case 'b' :
				add_piece_to_board(board_to_setup, B_BISHOP, sq);
				break;
			case 'q' :
				add_piece_to_board(board_to_setup, B_QUEEN, sq);
				break;
			case 'k' :
				add_piece_to_board(board_to_setup, B_KING, sq);
				break;
			case 'P' :
				add_piece_to_board(board_to_setup, W_PAWN, sq);
				break;
			case 'R' :
				add_piece_to_board(board_to_setup, W_ROOK, sq);
				break;
			case 'N' :
				add_piece_to_board(board_to_setup, W_KNIGHT, sq);
				break;
			case 'B' :
				add_piece_to_board(board_to_setup, W_BISHOP, sq);
				break;
			case 'Q' :
				add_piece_to_board(board_to_setup, W_QUEEN, sq);
				break;
			case 'K' :
				add_piece_to_board(board_to_setup, W_KING, sq);
				break;
			case '/' :
				break;
			case '1' :
				sq++;
				break;
			case '2' :
				sq += 2;
				break;
			case '3' :
				sq+=3;
				break;
			case '4' :
				sq+=4;
				break;
			case '5' :
				sq+=5;
				break;
			case '6' :
				sq+=6;
				break;
			case '7' :
				sq+=7;
				break;
			case '8' :
				sq+=8;
				break;
			default:
				return -1;
			}
	}
	return 0;
}
