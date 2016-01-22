/*
 * search_tests.c
 * Copyright (C) 2016 Eddie McNally <emcn@gmx.com>
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "seatest.h"
#include "types.h"
#include "search.h"
#include "move_gen_utils.h"



void test_move_sort_1(void){

	struct move_list mvl = {
		.moves = {0},
		.move_count = 0
	};
	
	int32_t start_score = -500;
	int32_t score_incr = 75;
	
	// create some dummy moves and scores
	for(int32_t i = 0; i < 20; i++){
		// interested more in the score than squares or pieces
		
		int32_t score = start_score + (score_incr * i);
		mv_bitmap mv = MOVE(e5, e6, NO_PIECE, NO_PIECE, MFLAG_NONE, score);
	
		// add to move list
		mvl.moves[i] = mv;
		mvl.move_count++;	
	}
	
	// sort to bring best score to top
	bring_best_move_to_top(0, &mvl);
	
	
	//print_move_list_details(&mvl);
	mv_bitmap top_mv = mvl.moves[0];
	assert_true(get_score(top_mv) == 925);
		
}



void search_test_fixture(void)
{
	test_fixture_start();	// starts a fixture

	run_test(test_move_sort_1);
	
	test_fixture_end();	// ends a fixture
}






