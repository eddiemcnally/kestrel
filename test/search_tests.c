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
#include "init.h"
#include "types.h"
#include "search.h"
#include "move_gen_utils.h"


#define MATE_IN_TWO			"1r3rk1/1pnnq1bR/p1pp2B1/P2P1p2/1PP1pP2/2B3P1/5PK1/2Q4R w - - 0 1"

void test_mate_in_two(void);
void test_move_sort_1(void);


void test_move_sort_1(void)
{

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };

    uint32_t start_score = 3;
    uint32_t score_incr = 75;

    // create some dummy moves and scores
    for(int32_t i = 0; i < 20; i++) {
        // interested more in the score than squares or pieces

        uint32_t score = start_score + (score_incr * i);
        mv_bitmap mv = Test_MOVE(e5, e6, NO_PIECE, NO_PIECE, MFLAG_NONE);
        add_to_score(&mv, score);

        // add to move list
        mvl.moves[i] = mv;
        mvl.move_count++;
    }

    // sort to bring best score to top
    bring_best_move_to_top(0, &mvl);


    //print_move_list_details(&mvl);
    mv_bitmap top_mv = mvl.moves[0];
    assert_true(get_score(top_mv) == 1428);

}

void test_mate_in_two()
{
    struct board *brd = init_game(MATE_IN_TWO);

    struct search_info si = {0};
    si.depth = 4;
    search_positions(brd, &si, 64000000);

    // *** exclude the score ***
    mv_bitmap h7h8 = get_move(Test_MOVE(h7, h8, NO_PIECE, NO_PIECE, 0));
    mv_bitmap g7h8 = get_move(Test_MOVE(g7, h8, W_ROOK, NO_PIECE, MFLAG_CAPTURE));
    mv_bitmap h1h8 = get_move(Test_MOVE(h1, h8, B_BISHOP, NO_PIECE, MFLAG_CAPTURE));

    mv_bitmap pv_line_h7h8 = get_move(brd->pv_line[0]);
    mv_bitmap pv_line_g7h8 = get_move(brd->pv_line[1]);
    mv_bitmap pv_line_h1h8 = get_move(brd->pv_line[2]);

    //printf("PV g7h8 ->%s\n", print_move(pv_line_g7h8));
    //printf("PV h1h8 ->%s\n", print_move(pv_line_h1h8));

    assert_true(h7h8 == pv_line_h7h8);
    assert_true(g7h8 == pv_line_g7h8);
    assert_true(h1h8 == pv_line_h1h8);

}



void search_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    run_test(test_move_sort_1);
    run_test(test_mate_in_two);


    test_fixture_end();	// ends a fixture
}






