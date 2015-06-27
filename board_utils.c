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





void print_board(board_container_t * the_board)
{

    char brd[8][8];

    for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			brd[i][j] = '-';
		}
    }


    for (int rank = 0; rank < 8; rank++) {
		for (int file = 0; file < 8; file++) {

			square_t square = (rank * 8) + file;

			piece_t piece = get_piece_at_square(the_board, square);
			if (piece >= 0) {
				char pce = get_piece_label(piece);
				brd[rank][file] = pce;
			}
		}
    }

	printf("\n\r");
    for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file <= 7; file++) {
			printf("%c", brd[rank][file]);
		}
		printf("\n\r");
    }
    printf("\n\r");

}



