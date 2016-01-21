/*
 * kestrel.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: main entry point for kestrel chess engine
 * ---------------------------------------------------------------------
 *
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
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "types.h"
#include "board.h"
#include "makemove.h"
#include "board_utils.h"
#include "hashkeys.h"
#include "init.h"
#include "occupancy_mask.h"
#include "fen.h"
#include "utils.h"
#include "pv_table.h"
#include "search.h"
#include "move_gen.h"
#include "move_gen_utils.h"


// sample game positions
#define MATE_IN_TWO			"1r3rk1/1pnnq1bR/p1pp2B1/P2P1p2/1PP1pP2/2B3P1/5PK1/2Q4R w - - 0 1"
#define WAC1 				"r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"


// Bratko-Kopec Tests
#define BK_01	"1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -"



int main(int argc, char **argv)
{
	if (argc > 0) {
		printf("%d", **argv);
	}

	// set process pri and cpu affinity for max performance
	set_priority_and_affinity();

	struct board * brd = init_game(WAC1);
	print_board(brd);


////////////// debug /////////////////
//	struct move_list mv_list = {
//		.moves = {{0, 0}},
//		.move_count = 0
//	};

//	generate_all_moves(brd, &mv_list);

//	for (uint16_t i = 0; i < mv_list.move_count; i++) {
//		printf("move %s\n", print_move(mv_list.moves[i].move_bitmap));
//	}
//	return 0;


/////////////////////////////////////////////

	char input[6];
	mv_bitmap move = NO_MOVE;
	while (true) {
		print_board(brd);
		printf("Enter a move > ");
		
		if (fgets(input, 6, stdin)){
			if (input[0] == 'q') {
				// quit
				dispose_board(brd);
				break;
			} else if (input[0] == 't') {
				take_move(brd);
			} else if (input[0] == 's') {
				search_positions(brd, 6);
				dump_pv_table_stats(brd->pvtable);
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
		}
		fflush(stdin);
	}

	return 0;
}
