/*
 * move_test_fixture.c
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
#include "seatest.h"
#include "types.h"
#include "board.h"
#include "pieces.h"
#include "board_utils.h"
#include "move.h"


void test_king_occupancy_mask_generation_centre_squares()
{

    board_t *masks = malloc(sizeof(board_t) * NUM_SQUARES);
    memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    generate_king_occupancy_masks(masks);

    // TEST d4
    // =======
    board_t mask_d4 = masks[d4];

    assert_true(is_square_occupied(mask_d4, d3));
    assert_true(is_square_occupied(mask_d4, d5));

    assert_true(is_square_occupied(mask_d4, c3));
    assert_true(is_square_occupied(mask_d4, c4));
    assert_true(is_square_occupied(mask_d4, c5));

    assert_true(is_square_occupied(mask_d4, e3));
    assert_true(is_square_occupied(mask_d4, e4));
    assert_true(is_square_occupied(mask_d4, e5));

    // check remainder of bit mask is empty
    assert_false(is_square_occupied(mask_d4, a1));
    assert_false(is_square_occupied(mask_d4, a2));
    assert_false(is_square_occupied(mask_d4, a3));
    assert_false(is_square_occupied(mask_d4, a4));
    assert_false(is_square_occupied(mask_d4, a5));
    assert_false(is_square_occupied(mask_d4, a6));
    assert_false(is_square_occupied(mask_d4, a7));
    assert_false(is_square_occupied(mask_d4, a8));

    assert_false(is_square_occupied(mask_d4, b1));
    assert_false(is_square_occupied(mask_d4, b2));
    assert_false(is_square_occupied(mask_d4, b3));
    assert_false(is_square_occupied(mask_d4, b4));
    assert_false(is_square_occupied(mask_d4, b5));
    assert_false(is_square_occupied(mask_d4, b6));
    assert_false(is_square_occupied(mask_d4, b7));
    assert_false(is_square_occupied(mask_d4, b8));

    assert_false(is_square_occupied(mask_d4, c1));
    assert_false(is_square_occupied(mask_d4, c2));
    assert_false(is_square_occupied(mask_d4, c6));
    assert_false(is_square_occupied(mask_d4, c7));

    assert_false(is_square_occupied(mask_d4, d1));
    assert_false(is_square_occupied(mask_d4, d2));
    assert_false(is_square_occupied(mask_d4, d6));
    assert_false(is_square_occupied(mask_d4, d7));

    assert_false(is_square_occupied(mask_d4, e1));
    assert_false(is_square_occupied(mask_d4, e2));
    assert_false(is_square_occupied(mask_d4, e6));
    assert_false(is_square_occupied(mask_d4, e7));

    assert_false(is_square_occupied(mask_d4, f1));
    assert_false(is_square_occupied(mask_d4, f2));
    assert_false(is_square_occupied(mask_d4, f3));
    assert_false(is_square_occupied(mask_d4, f4));
    assert_false(is_square_occupied(mask_d4, f5));
    assert_false(is_square_occupied(mask_d4, f6));
    assert_false(is_square_occupied(mask_d4, f7));
    assert_false(is_square_occupied(mask_d4, f8));

    assert_false(is_square_occupied(mask_d4, g1));
    assert_false(is_square_occupied(mask_d4, g2));
    assert_false(is_square_occupied(mask_d4, g3));
    assert_false(is_square_occupied(mask_d4, g4));
    assert_false(is_square_occupied(mask_d4, g5));
    assert_false(is_square_occupied(mask_d4, g6));
    assert_false(is_square_occupied(mask_d4, g7));
    assert_false(is_square_occupied(mask_d4, g8));

    assert_false(is_square_occupied(mask_d4, h1));
    assert_false(is_square_occupied(mask_d4, h2));
    assert_false(is_square_occupied(mask_d4, h3));
    assert_false(is_square_occupied(mask_d4, h4));
    assert_false(is_square_occupied(mask_d4, h5));
    assert_false(is_square_occupied(mask_d4, h6));
    assert_false(is_square_occupied(mask_d4, h7));
    assert_false(is_square_occupied(mask_d4, h8));

    free(masks);
}



void test_king_occupancy_mask_edge_squares()
{
    board_t *masks = malloc(sizeof(board_t) * NUM_SQUARES);
    memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    generate_king_occupancy_masks(masks);

    // TEST a1
    // =======
    board_t mask_a1 = masks[a1];
    assert_true(is_square_occupied(mask_a1, a2));
    assert_true(is_square_occupied(mask_a1, b1));
    assert_true(is_square_occupied(mask_a1, b2));


    // TEST a2
    // =======
    board_t mask_a2 = masks[a2];
    assert_true(is_square_occupied(mask_a2, a3));
    assert_true(is_square_occupied(mask_a2, a1));
    assert_true(is_square_occupied(mask_a2, b1));
    assert_true(is_square_occupied(mask_a2, b2));
    assert_true(is_square_occupied(mask_a2, b3));


    // TEST h1
    // =======
    board_t mask_h1 = masks[h1];
    assert_true(is_square_occupied(mask_h1, g1));
    assert_true(is_square_occupied(mask_h1, h2));
    assert_true(is_square_occupied(mask_h1, g2));



    // TEST h8
    // =======
    board_t mask_h8 = masks[h8];
    assert_true(is_square_occupied(mask_h8, g8));
    assert_true(is_square_occupied(mask_h8, g7));
    assert_true(is_square_occupied(mask_h8, h7));



    // TEST b8
    // =======
    board_t mask_b8 = masks[b8];
    assert_true(is_square_occupied(mask_b8, a8));
    assert_true(is_square_occupied(mask_b8, a7));
    assert_true(is_square_occupied(mask_b8, b7));
    assert_true(is_square_occupied(mask_b8, c7));
    assert_true(is_square_occupied(mask_b8, c8));

    free(masks);
}



void test_rook_occupancy_mask()
{
    board_t *masks = malloc(sizeof(board_t) * NUM_SQUARES);
    memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

    generate_rook_occupancy_masks(masks);

    // TEST d2
    // =======
    board_t mask_d2 = masks[d2];
    assert_true(is_square_occupied(mask_d2, a2));
    assert_true(is_square_occupied(mask_d2, b2));
    assert_true(is_square_occupied(mask_d2, c2));
    assert_true(is_square_occupied(mask_d2, e2));
    assert_true(is_square_occupied(mask_d2, f2));
    assert_true(is_square_occupied(mask_d2, g2));
    assert_true(is_square_occupied(mask_d2, h2));

    assert_true(is_square_occupied(mask_d2, d1));
    assert_true(is_square_occupied(mask_d2, d3));
    assert_true(is_square_occupied(mask_d2, d4));
    assert_true(is_square_occupied(mask_d2, d5));
    assert_true(is_square_occupied(mask_d2, d6));
    assert_true(is_square_occupied(mask_d2, d7));
    assert_true(is_square_occupied(mask_d2, d8));

    free(masks);

}


void test_knight_occupancy_mask()
{
    board_t *masks = malloc(sizeof(board_t) * NUM_SQUARES);
    memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

    generate_knight_occupancy_masks(masks);


    // TEST a1
    // =======
    board_t mask_a1 = masks[a1];
    assert_true(is_square_occupied(mask_a1, c2));
    assert_true(is_square_occupied(mask_a1, b3));

    // TEST d5
    // =======
    board_t mask_d5 = masks[d5];
    assert_true(is_square_occupied(mask_d5, b4));
    assert_true(is_square_occupied(mask_d5, c3));
    assert_true(is_square_occupied(mask_d5, e3));
    assert_true(is_square_occupied(mask_d5, f4));
    assert_true(is_square_occupied(mask_d5, b6));
    assert_true(is_square_occupied(mask_d5, c7));
    assert_true(is_square_occupied(mask_d5, f6));
    assert_true(is_square_occupied(mask_d5, e7));


    free(masks);

}





void move_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    run_test(test_king_occupancy_mask_generation_centre_squares);
    run_test(test_king_occupancy_mask_edge_squares);

    run_test(test_knight_occupancy_mask);

    run_test(test_rook_occupancy_mask);

    test_fixture_end();		// ends a fixture
}
