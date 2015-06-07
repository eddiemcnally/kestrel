/*
 * board_test_fixture.c
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
#include "seatest.h"
#include "board.h"
#include "pieces.h"

/**
 * Verifies the initial board setup plus some supporting code
 */
void test_initial_board_placement()
{
	struct board_container *the_board = malloc(sizeof (struct board_container));

    // set up initial board
    reset_board(the_board);

    assert_true(W_ROOK 		== get_piece_at_square(the_board, A1));
	assert_true(W_KNIGHT 	== get_piece_at_square(the_board, B1));
	assert_true(W_BISHOP 	== get_piece_at_square(the_board, C1));
	assert_true(W_QUEEN 	== get_piece_at_square(the_board, D1));
	assert_true(W_KING 		== get_piece_at_square(the_board, E1));
	assert_true(W_BISHOP 	== get_piece_at_square(the_board, F1));
	assert_true(W_KNIGHT 	== get_piece_at_square(the_board, G1));
	assert_true(W_ROOK 		== get_piece_at_square(the_board, H1));

	assert_true(W_PAWN 		== get_piece_at_square(the_board, A2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, B2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, C2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, D2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, E2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, F2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, G2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, H2));


	assert_true(B_PAWN 		== get_piece_at_square(the_board, A7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, B7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, C7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, D7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, E7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, F7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, G7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, H7));

    assert_true(B_ROOK 		== get_piece_at_square(the_board, A8));
	assert_true(B_KNIGHT 	== get_piece_at_square(the_board, B8));
	assert_true(B_BISHOP 	== get_piece_at_square(the_board, C8));
	assert_true(B_QUEEN 	== get_piece_at_square(the_board, D8));
	assert_true(B_KING 		== get_piece_at_square(the_board, E8));
	assert_true(B_BISHOP 	== get_piece_at_square(the_board, F8));
	assert_true(B_KNIGHT 	== get_piece_at_square(the_board, G8));
	assert_true(B_ROOK 		== get_piece_at_square(the_board, H8));

	// no piece present
	for(int i = A3; i < H6; i++){
		assert_true(-1 == get_piece_at_square(the_board, i));
	}

}


void test_clean_board(){
	struct board_container *the_board = malloc(sizeof (struct board_container));

    // set up initial board
    reset_board(the_board);

	// now clean it
	clear_board(the_board);

	assert_true(the_board->board == (board_t)0);
	for(int i = 0; i < NUM_PIECE_TYPES; i++){
		assert_true(the_board->piece_boards[i] == (board_t)0);
	}
}



void test_arrays_equal()
{
	unsigned char expected_bytes[] = { 1, 2, 3};
	unsigned char buffer[5];
	int i;

	// put 5 bytes in
	for(i=0; i<5; i++) buffer[i]=i+1;

	// only check the first 3
	assert_n_array_equal(expected_bytes, buffer, 3);
}

void test_bits()
{
	assert_bit_set(0, 0x01);
	assert_bit_set(2, 0x04);
	assert_bit_not_set(3, 0x02);
}

void test_strings()
{
	char *s = "hello";
	assert_string_equal("hello", s);
	assert_string_contains("blah", "why say blah?");
	assert_string_doesnt_contain("blah", "why say hello?");
	assert_string_ends_with("h?", "why say blah?");
	assert_string_starts_with("why", "why say blah?");
}



void board_test_fixture( void )
{
	test_fixture_start();               // starts a fixture
	run_test(test_initial_board_placement);
	run_test(test_clean_board);
	//run_test(test_strings);   // run tests
	//run_test(test_arrays_equal);
	//run_test(test_bits);
	test_fixture_end();                 // ends a fixture
}
