/*
 * performance_tests.c
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
#include <string.h>
#include "seatest.h"
#include "types.h"
#include "attack.h"
#include "fen.h"
#include "board.h"
#include "pieces.h"
#include "board_utils.h"
#include "makemove.h"
#include "move.h"



void perf_test(int depth, struct board *brd);
void test_move_gen_depth(void);
void perft(int depth, struct board *brd);



long leafNodes = 0;

void test_move_gen_depth(){


	//1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1 ;D1 26 ;D2 583 ;D3 14252 ;D4 334705 ;D5 8198901 ;D6 198328929
	char * sample_position = "1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1";
    struct board *brd= init_board(sample_position);

	perf_test(4, brd);

	assert_true(leafNodes == 334705);


/////////////////////////////////////////////////////

	leafNodes = 0;
	// r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ;D1 48 ;D2 2039 ;D3 97862 ;D4 4085603 ;D5 193690690 ;D6 8031647685
	//
	sample_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    brd= init_board(sample_position);

	perf_test(3, brd);

	assert_true(leafNodes == 97862);

////////////////////////////////////////////


	leafNodes = 0;
	// r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1 ;D1 25 ;D2 547 ;D3 13579 ;D4 316214 ;D5 7878456 ;D6 189224276

	sample_position = "r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1";
    brd= init_board(sample_position);

	perf_test(4, brd);

	assert_true(leafNodes == 316214);



}


void perf_test(int depth, struct board *brd) {

    ASSERT_BOARD_OK(brd);

	//print_board(brd);
	//printf("\nStarting Test To Depth:%d\n",depth);


    struct move_list *mv_list = malloc(sizeof(struct move_list));
	memset(mv_list, 0, sizeof(struct move_list));

	generate_all_moves(brd, mv_list);

	//print_move_list_details(mv_list);

    mv_bitmap mv;
    for(U32 mv_num = 0; mv_num < mv_list->move_count; ++mv_num) {
        mv = mv_list->moves[mv_num].move_bitmap;
        if ( !make_move(brd, mv))  {
            continue;
        }
		//print_board(brd);
        //long total = leafNodes;
        perft(depth - 1, brd);
        take_move(brd);
        //long oldnodes = leafNodes - total;
        //printf("move %d : %s : %ld\n", mv_num+1, print_move(mv),oldnodes);
    }

	//printf("\nTest Complete : %ld nodes visited\n",leafNodes);

    return;
}



void perft(int depth, struct board *brd) {

    ASSERT_BOARD_OK(brd);

	if(depth == 0) {
        leafNodes++;
        return;
    }

    struct move_list *mv_list = malloc(sizeof(struct move_list));
	memset(mv_list, 0, sizeof(struct move_list));

	generate_all_moves(brd, mv_list);

	//printf("# moves generated = %d\n", mv_list->move_count);

    mv_bitmap mv;
    for(U32 mv_num = 0; mv_num < mv_list->move_count; mv_num++) {
        mv = mv_list->moves[mv_num].move_bitmap;
        if ( !make_move(brd, mv))  {
            continue;
        }
        perft(depth - 1, brd);
        take_move(brd);
    }

    return;
}





void perf_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    run_test(test_move_gen_depth);




    test_fixture_end();		// ends a fixture
}
