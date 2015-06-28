/*
 * fen.c
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
#include <assert.h>
#include "types.h"
#include "pieces.h"
#include "board.h"

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
			 board_container_t * board_to_setup){

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

    for (int rank = 7; rank >= 0; rank--) {
		bool move_to_next_rank = false;
		for (int file = 0; file <= 7 && move_to_next_rank == false; file++) {
			int sq = GET_SQUARE(rank, file);
			char c = *(fen_string++);

			piece_t to_add = NO_PIECE;
			
			switch (c) {
				case 'p': to_add = B_PAWN; 	break;
				case 'r': to_add = B_ROOK; 	break;
				case 'n': to_add = B_KNIGHT;break;
				case 'b': to_add = B_BISHOP;break;
				case 'q': to_add = B_QUEEN; break;
				case 'k': to_add = B_KING; 	break;
				case 'P': to_add = W_PAWN; 	break;
				case 'R': to_add = W_ROOK; 	break;
				case 'N': to_add = W_KNIGHT;break;
				case 'B': to_add = W_BISHOP;break;
				case 'Q': to_add = W_QUEEN; break;
				case 'K': to_add = W_KING; 	break;
				case '/':
					move_to_next_rank = true;
					rank++;
					break;
				case '1': 
				case '2': 
				case '3': 
				case '4': 
				case '5': 
				case '6': 
				case '7': 
				case '8': 
					file += ((c - '0') - 1);
					break;
				default:
					return -1;
			}

			if (to_add != NO_PIECE){
				bool added = add_piece_to_board(board_to_setup, to_add, sq);
				assert(added == true);
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
