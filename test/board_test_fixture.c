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
#include "board_utils.h"


void verify_initial_board_placement(board_container_t *the_board);


/**
 * Verifies the initial board setup plus some supporting code
 */
void test_initial_board_placement()
{
	board_container_t *the_board = malloc(sizeof (struct board_container));

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
	board_container_t *the_board = malloc(sizeof (struct board_container));

    // set up initial board
    reset_board(the_board);

	// now clean it
	clear_board(the_board);

	// check all squares are zero
	assert_true(the_board->board == (board_t)0);
	for(int i = 0; i < NUM_PIECE_TYPES; i++){
		assert_true(the_board->piece_boards[i] == (board_t)0);
	}
}


void test_add_to_board(){

	board_container_t *the_board = malloc(sizeof (struct board_container));

	// now clean it
	clear_board(the_board);

	add_piece_to_board(the_board, W_PAWN, A8);
	add_piece_to_board(the_board, B_QUEEN, G3);
	add_piece_to_board(the_board, W_PAWN, B6);
	add_piece_to_board(the_board, W_KING, A4);
	add_piece_to_board(the_board, W_ROOK, H8);

	// add again, to check error handling
	assert_true(add_piece_to_board(the_board, W_PAWN, A8) == -1);

	assert_true(get_piece_at_square(the_board, A8) == (piece_id_t)W_PAWN);
	assert_true(get_piece_at_square(the_board, G3) == (piece_id_t)B_QUEEN);
	assert_true(get_piece_at_square(the_board, B6) == (piece_id_t)W_PAWN);
	assert_true(get_piece_at_square(the_board, A4) == (piece_id_t)W_KING);
	assert_true(get_piece_at_square(the_board, H8) == (piece_id_t)W_ROOK);

}


void test_fen_parsing_initial_board_layout(){

	board_container_t *the_board = malloc(sizeof (struct board_container));

	// now clean it
	clear_board(the_board);

	// this is the initial bpard setup
	char *test_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	consume_fen_notation(test_fen, the_board);

	//print_board(the_board);

	// verify the board
	// NOTE: This code is cut/paste from above, to make it easier tp
	// find out whattests are failing
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


void test_fen_parsing_general_layout(){

	board_container_t *the_board = malloc(sizeof (struct board_container));


	/////////////////////////////////////////////////////////
	// Test FEN #1
	/////////////////////////////////////////////////////////

	// now clean it
	clear_board(the_board);

	// this is the initial board setup
	char *test_fen = "3b4/8/1R6/8/8/5R2/7P/4BK1k w - -";

	consume_fen_notation(test_fen, the_board);

	//print_board(the_board);

	// verify the board
	assert_true(B_BISHOP 	== get_piece_at_square(the_board, D8));
	assert_true(W_ROOK 		== get_piece_at_square(the_board, B6));
	assert_true(W_ROOK 		== get_piece_at_square(the_board, F3));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, H2));
	assert_true(B_KING 		== get_piece_at_square(the_board, H1));
	assert_true(W_KING 		== get_piece_at_square(the_board, F1));
	assert_true(W_BISHOP 	== get_piece_at_square(the_board, E1));




	/////////////////////////////////////////////////////////
	// Test FEN #2
	/////////////////////////////////////////////////////////

	// now clean it
	clear_board(the_board);

	// this is the initial board setup
	test_fen = "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R";

	consume_fen_notation(test_fen, the_board);

	//print_board(the_board);


    assert_true(W_ROOK 		== get_piece_at_square(the_board, A1));
	assert_true(W_KNIGHT 	== get_piece_at_square(the_board, B1));
	assert_true(W_BISHOP 	== get_piece_at_square(the_board, C1));
	assert_true(W_QUEEN 	== get_piece_at_square(the_board, D1));
	assert_true(W_KING 		== get_piece_at_square(the_board, E1));
	assert_true(W_ROOK 		== get_piece_at_square(the_board, H1));

	assert_true(W_PAWN 		== get_piece_at_square(the_board, A2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, B2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, C2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, D2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, F2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, G2));
	assert_true(W_PAWN 		== get_piece_at_square(the_board, H2));


	assert_true(B_PAWN 		== get_piece_at_square(the_board, A7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, B7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, C7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, D7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, F7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, G7));
	assert_true(B_PAWN 		== get_piece_at_square(the_board, H7));

    assert_true(B_ROOK 		== get_piece_at_square(the_board, A8));
	assert_true(B_BISHOP 	== get_piece_at_square(the_board, C8));
	assert_true(B_QUEEN 	== get_piece_at_square(the_board, D8));
	assert_true(B_KING 		== get_piece_at_square(the_board, E8));
	assert_true(B_BISHOP 	== get_piece_at_square(the_board, F8));
	assert_true(B_KNIGHT 	== get_piece_at_square(the_board, G8));
	assert_true(B_ROOK 		== get_piece_at_square(the_board, H8));


}





void test_setting_bits_in_a_board(){
	board_t test_brd = 0;

	//
	// Test setting bits
	//
	set_bit(&test_brd, 22);
	assert_true(test_brd == 4194304);
	test_brd = 0;

	set_bit(&test_brd, 0);
	assert_true(test_brd == 1);
	test_brd = 0;


	set_bit(&test_brd, 63);
	assert_true(test_brd == 0x8000000000000000);
	test_brd = 0;


	set_bit(&test_brd, 31);
	assert_true(test_brd == 2147483648);
	test_brd = 0;
}


void test_clearing_bits_in_a_board(){
	board_t test_brd = 0;

	//
	// Test clearing bits
	//
	set_bit(&test_brd, 22);
	clear_bit(&test_brd, 22);
	assert_true(test_brd == 0);

	set_bit(&test_brd, 1);
	clear_bit(&test_brd, 1);
	assert_true(test_brd == 0);

	set_bit(&test_brd, 31);
	clear_bit(&test_brd, 31);
	assert_true(test_brd == 0);

	set_bit(&test_brd, 32);
	clear_bit(&test_brd, 32);
	assert_true(test_brd == 0);

	set_bit(&test_brd, 63);
	clear_bit(&test_brd, 63);
	assert_true(test_brd == 0);


}







void test_checking_bits_in_a_board(){
	board_t test_brd = 0;

	//
	// Test checking bits
	//
	set_bit(&test_brd, 22);
	assert_true(check_bit(&test_brd, 22) == 1);
	test_brd = 0; // clean up

	set_bit(&test_brd, 1);
	assert_true(check_bit(&test_brd, 1) == 1);
	test_brd = 0; // clean up

	set_bit(&test_brd, 31);
	assert_true(check_bit(&test_brd, 31) == 1);
	test_brd = 0; // clean up

	set_bit(&test_brd, 32);
	assert_true(check_bit(&test_brd, 32) == 1);
	test_brd = 0; // clean up

	set_bit(&test_brd, 63);
	assert_true(check_bit(&test_brd, 63) == 1);
	test_brd = 0; // clean up


	// do some negative testing
	assert_true(check_bit(&test_brd, 22) == 0);
	assert_true(check_bit(&test_brd, 1) == 0);
	assert_true(check_bit(&test_brd, 31) == 0);
	assert_true(check_bit(&test_brd, 8) == 0);
	assert_true(check_bit(&test_brd, 48) == 0);
	assert_true(check_bit(&test_brd, 63) == 0);
	assert_true(check_bit(&test_brd, 58) == 0);
	assert_true(check_bit(&test_brd, 61) == 0);

}






void board_test_fixture( void )
{
	test_fixture_start();               // starts a fixture
	run_test(test_initial_board_placement);
	run_test(test_clean_board);
	run_test(test_add_to_board);
	run_test(test_setting_bits_in_a_board);
	run_test(test_checking_bits_in_a_board);
	run_test(test_clearing_bits_in_a_board);
	run_test(test_fen_parsing_initial_board_layout);
	run_test(test_fen_parsing_general_layout);

	test_fixture_end();                 // ends a fixture
}
