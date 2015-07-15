/*
 * fen.c
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * kestrel is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kestrel is distributed in the hope that it will be useful, but
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
#include "hashkeys.h"
#include "board_utils.h"
#include "fen.h"

/*
 *
 * name: Takes a char array as FEN notation, and returns a populated
 * board
 * @param 	char * representing the FEN string
 * 			board * representing the bpard tp be populated
 * @return 	0 = OK, -1 Error
 *
 * Thanks for BlueFever Software for his youtube videos and this code
 */

int consume_fen_notation(const char *fen_string, struct board *board_to_setup)
{

	int rank = RANK_8;
	int file = FILE_A;
	int count = 0;

	while ((rank >= RANK_1) && *fen_string) {
		enum piece piece_to_add = NO_PIECE;
		count = 1;

		switch (*fen_string) {
		case 'p':
			piece_to_add = B_PAWN;
			break;
		case 'r':
			piece_to_add = B_ROOK;
			break;
		case 'n':
			piece_to_add = B_KNIGHT;
			break;
		case 'b':
			piece_to_add = B_BISHOP;
			break;
		case 'q':
			piece_to_add = B_QUEEN;
			break;
		case 'k':
			piece_to_add = B_KING;
			break;
		case 'P':
			piece_to_add = W_PAWN;
			break;
		case 'R':
			piece_to_add = W_ROOK;
			break;
		case 'N':
			piece_to_add = W_KNIGHT;
			break;
		case 'B':
			piece_to_add = W_BISHOP;
			break;
		case 'Q':
			piece_to_add = W_QUEEN;
			break;
		case 'K':
			piece_to_add = W_KING;
			break;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			count = (*fen_string) - '0';
			break;

		case '/':
		case ' ':
			rank--;
			file = FILE_A;
			fen_string++;
			continue;

		default:
			printf("FEN error \n");
			return -1;
		}

		for (int i = 0; i < count; i++) {
			if (piece_to_add != NO_PIECE) {
				int sq = GET_SQUARE(rank, file);
				add_piece_to_board(board_to_setup, piece_to_add,
						   sq);
			}
			file++;
		}
		fen_string++;
	}

	if (*fen_string == 'w') {
		board_to_setup->side_to_move = WHITE;
	} else {
		board_to_setup->side_to_move = BLACK;
	}

	// skip 'w' or 'b', and the next space
	fen_string += 2;

	for (int i = 0; i < 4; i++) {
		if (*fen_string == ' ') {
			break;
		}

		switch (*fen_string) {
		case 'K':
			board_to_setup->castle_perm |= WKCA;
			break;
		case 'Q':
			board_to_setup->castle_perm |= WQCA;
			break;
		case 'k':
			board_to_setup->castle_perm |= BKCA;
			break;
		case 'q':
			board_to_setup->castle_perm |= BQCA;
			break;
		default:
			break;
		}
		fen_string++;
	}

	fen_string++;

	if (*fen_string != '-') {
		// en passant square present
		file = fen_string[0] - 'a';
		rank = fen_string[1] - '1';
		board_to_setup->en_passant = GET_SQUARE(rank, file);
	} else {
		board_to_setup->en_passant = NO_SQUARE;
	}

	update_piece_material(board_to_setup);

	board_to_setup->board_hash = get_position_hashkey(board_to_setup);

	return 0;
}

/*
 * 
 * name: generate_fen_notation
 * @param 	struct board *	the board to generate	
 * @return 	cnar *	string representing fen notation
 * 
 */

char *generate_fen_notation(const struct board *board_to_setup)
{
	printf("PosKey:\t0x%016llx\n", board_to_setup->board_hash);
	// TODO
	return malloc(80);
}
