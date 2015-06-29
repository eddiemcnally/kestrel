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
#include "types.h"
#include "board.h"
#include "pieces.h"




/* Pretty-prints the board
 * 
 * name: print_board
 * @param: a board
 * @return : void
 * 
 */

// char arrays to suport printing
const char ranks[] 	= "12345678";
const char files[] 	= "abcdefgh";

/**
 * Thanks again to Bluefever Software for this code
 */
void print_board(board_container_t * the_board)
{

	//int sq,piece;
	
	printf("\nGame Board:\n\n");
	
	for(int rank = RANK8; rank >= RANK1; rank--) {
		printf("%d  ",rank+1);	// enum is zero-based
		for(int file = FILE_A; file <= FILE_H; file++) {
			square_t sq = GET_SQUARE(rank, file);
			piece_t pce = get_piece_at_square(the_board, sq);
			if (pce != NO_PIECE){
				char c = get_piece_label(pce);
				printf("%3c", c);
			} else {
				printf("  -");
			}
		}
		printf("\n");
	}
	
	printf("\n   ");
	for(int file = FILE_A; file <= FILE_H; file++) {
		printf("%3c",'a'+file);	
	}
	printf("\n\n");
	char side;
	if (the_board->side_to_move == WHITE){
		side = 'w';
	} else {
		side = 'b';
	}
	printf("side:\t%c\n", side);
	
	if (the_board->en_passant == -1){
		printf("enPas:\t-\n");
	}
	else{
		int rank = GET_RANK(the_board->en_passant);
		int file = GET_FILE(the_board->en_passant);
		printf("enPas:\t%c%c\n", files[file], ranks[rank]);
	}

	printf("castle:\t%c%c%c%c\n",
			(the_board->castle_perm & WKCA) ? 'K' : '-',
			(the_board->castle_perm & WQCA) ? 'Q' : '-',
			(the_board->castle_perm & BKCA) ? 'k' : '-',
			(the_board->castle_perm & BQCA) ? 'q' : '-'	
			);
	printf("PosKey:\t0x%016llx\n",the_board->position_key);

	printf("\n\n");
	
}



