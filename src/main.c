/*
 * main.c
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
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "types.h"
#include "move.h"
#include "board.h"
#include "makemove.h"
#include "board_utils.h"
#include "hashkeys.h"
#include "init.h"
#include "occupancy_mask.h"
#include "fen.h"
#include "utils.h"
#include "search.h"
#include "move.h"

// sample game position
#define SAMPLE_POSITION 	"2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"

int main(int argc, char **argv)
{
	if (argc > 0) {
		printf("%d", **argv);
	}

	// set process pri and cpu affinity for max performance
	set_priority_and_affinity();

	struct board *brd = init_game(STARTING_FEN);
	print_board(brd);

	struct search_info si[1];

	char input[6];
	mv_bitmap move = NO_MOVE;
	while (true) {
		print_board(brd);
		printf("Enter a move > ");
		fgets(input, 6, stdin);

		if (input[0] == 'q') {
			// quit
			break;
		} else if (input[0] == 't') {
			take_move(brd);
		} else if (input[0] == 's'){
			si->depth = 6;
			search_positions(brd, si);

		} else {
			move = parse_move(input, brd);
			if (move != NO_MOVE) {
				make_move(brd, move);
				if (is_repetition(brd)) {
					printf("*** repetition ***\n");
				}
			} else {
				printf("Move Not Parsed:%s\n", input);
			}
		}

		fflush(stdin);
	}

	return 0;
}
