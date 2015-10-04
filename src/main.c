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
#include "move.h"



// sample game position
#define SAMPLE_POSITION 	"k7/8/2N5/1N6/8/8/8/K6n b - - 0 1"

int main(int argc, char **argv)
{
    if (argc > 0) {
		printf("%d", **argv);
    }

	// set process pri and cpu affinity for max performance
	set_priority_and_affinity();

	struct board * brd = init_game(SAMPLE_POSITION);
	print_board(brd);

	char input[6];
	mv_bitmap move = NO_MOVE;
	while(true) {
		print_board(brd);
		printf("Enter a move > ");
		fgets(input, 6, stdin);
		
		if(input[0]=='q') {
			// quit
			break;
		} else if(input[0]=='t') {
			take_move(brd);			
		} else {
			move = parse_move(input, brd);
			if(move != NO_MOVE) {
				make_move(brd, move);
			} else {
				printf("Move Not Parsed:%s\n",input);
			}
		}	
		
		fflush(stdin);
	}

    return 0;
}
