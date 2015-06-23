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
 *
 */
int consume_fen_notation(char *fen_string,
			 board_container_t * board_to_setup)
{

    //example of starting position:
    //              rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    // 
    // definition of FEN notation: 
    //              https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

    // TODO:
    // - load moves, state, etc
    // 
    // NOTE: ignore moves, castling, etc, just returns the board
    //



    // ensure board is initialised
    clear_board(board_to_setup);


    for (int rank = 7; rank >= 0; rank--) {
	bool move_to_next_rank = false;
	for (int file = 0; file <= 7 && move_to_next_rank == false; file++) {
	    int sq = GET_SQUARE(rank, file);
	    char c = *(fen_string++);

	    bool added_ok = true;
	    switch (c) {
	    case 'p':
		added_ok = add_piece_to_board(board_to_setup, B_PAWN, sq);
		break;
	    case 'r':
		added_ok = add_piece_to_board(board_to_setup, B_ROOK, sq);
		break;
	    case 'n':
		added_ok =
		    add_piece_to_board(board_to_setup, B_KNIGHT, sq);
		break;
	    case 'b':
		added_ok =
		    add_piece_to_board(board_to_setup, B_BISHOP, sq);
		break;
	    case 'q':
		added_ok = add_piece_to_board(board_to_setup, B_QUEEN, sq);
		break;
	    case 'k':
		added_ok = add_piece_to_board(board_to_setup, B_KING, sq);
		break;
	    case 'P':
		added_ok = add_piece_to_board(board_to_setup, W_PAWN, sq);
		break;
	    case 'R':
		added_ok = add_piece_to_board(board_to_setup, W_ROOK, sq);
		break;
	    case 'N':
		added_ok =
		    add_piece_to_board(board_to_setup, W_KNIGHT, sq);
		break;
	    case 'B':
		added_ok =
		    add_piece_to_board(board_to_setup, W_BISHOP, sq);
		break;
	    case 'Q':
		added_ok = add_piece_to_board(board_to_setup, W_QUEEN, sq);
		break;
	    case 'K':
		added_ok = add_piece_to_board(board_to_setup, W_KING, sq);
		break;
	    case '/':
		move_to_next_rank = true;
		rank++;
		break;
	    case '1':
		//file++;
		break;
	    case '2':
		file += 1;
		break;
	    case '3':
		file += 2;
		break;
	    case '4':
		file += 3;
		break;
	    case '5':
		file += 4;
		break;
	    case '6':
		file += 5;
		break;
	    case '7':
		file += 6;
		break;
	    case '8':
		file += 7;
		break;
	    default:
		return -1;
	    }

	    if (added_ok == false) {
		printf("Problem adding piece");
		return -1;
	    }

	}
    }
    return 0;
}

/*
 * 
 * name: generate_fen_notation
 * @param 	board_container_t *	the board to generate	
 * @return 	cnar *	string representing fen notation
 * 
 */

char *generate_fen_notation(board_container_t * board_to_setup)
{
    // TODO
    return malloc(80);
}
