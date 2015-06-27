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
void verify_initial_board_placement(board_container_t * the_board);

/**
 * Verifies the initial board setup plus some supporting code
 */ 
void test_initial_board_placement() 
{
    	board_container_t * the_board = get_clean_board();
    
	// set up initial board
	reset_board(the_board);
    	assert_true(W_ROOK == get_piece_at_square(the_board, a1));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, b1));
	assert_true(W_BISHOP == get_piece_at_square(the_board, c1));
	assert_true(W_QUEEN == get_piece_at_square(the_board, d1));
	assert_true(W_KING == get_piece_at_square(the_board, e1));
	assert_true(W_BISHOP == get_piece_at_square(the_board, f1));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, g1));
	assert_true(W_ROOK == get_piece_at_square(the_board, h1));
		assert_true(W_PAWN == get_piece_at_square(the_board, a2));
	assert_true(W_PAWN == get_piece_at_square(the_board, b2));
	assert_true(W_PAWN == get_piece_at_square(the_board, c2));
	assert_true(W_PAWN == get_piece_at_square(the_board, d2));
	assert_true(W_PAWN == get_piece_at_square(the_board, e2));
	assert_true(W_PAWN == get_piece_at_square(the_board, f2));
	assert_true(W_PAWN == get_piece_at_square(the_board, g2));
	assert_true(W_PAWN == get_piece_at_square(the_board, h2));
		assert_true(B_PAWN == get_piece_at_square(the_board, a7));
	assert_true(B_PAWN == get_piece_at_square(the_board, b7));
	assert_true(B_PAWN == get_piece_at_square(the_board, c7));
	assert_true(B_PAWN == get_piece_at_square(the_board, d7));
	assert_true(B_PAWN == get_piece_at_square(the_board, e7));
	assert_true(B_PAWN == get_piece_at_square(the_board, f7));
	assert_true(B_PAWN == get_piece_at_square(the_board, g7));
	assert_true(B_PAWN == get_piece_at_square(the_board, h7));
			assert_true(B_ROOK == get_piece_at_square(the_board, a8));
	assert_true(B_KNIGHT == get_piece_at_square(the_board, b8));
	assert_true(B_BISHOP == get_piece_at_square(the_board, c8));
	assert_true(B_QUEEN == get_piece_at_square(the_board, d8));
	assert_true(B_KING == get_piece_at_square(the_board, e8));
	assert_true(B_BISHOP == get_piece_at_square(the_board, f8));
	assert_true(B_KNIGHT == get_piece_at_square(the_board, g8));
	assert_true(B_ROOK == get_piece_at_square(the_board, h8));
    
	// no piece present
	for (int i = a3; i < h6; i++) {
		assert_true(-1 == get_piece_at_square(the_board, i));
	}} void test_clean_board()
{
    	board_container_t * the_board = get_clean_board();
    
	// check all squares are zero
	assert_true(the_board->board == (board_t) 0);
    	for (int i = 0; i < NUM_PIECE_TYPES; i++) {
		assert_true(the_board->piece_boards[i] == (board_t) 0);
	} } void test_add_to_board()
{
	board_container_t * the_board = get_clean_board();
    	add_piece_to_board(the_board, W_PAWN, a8);
	add_piece_to_board(the_board, B_QUEEN, g3);
	add_piece_to_board(the_board, W_PAWN, b6);
	add_piece_to_board(the_board, W_KING, a4);
	add_piece_to_board(the_board, W_ROOK, h8);
		
	// add again, to check error handling
	assert_true(add_piece_to_board(the_board, W_PAWN, a8) == false);
	assert_true(get_piece_at_square(the_board, a8) == (piece_id_t) W_PAWN);
	assert_true(get_piece_at_square(the_board, g3) == (piece_id_t) B_QUEEN);
	assert_true(get_piece_at_square(the_board, b6) == (piece_id_t) W_PAWN);
	assert_true(get_piece_at_square(the_board, a4) == (piece_id_t) W_KING);
	assert_true(get_piece_at_square(the_board, h8) == (piece_id_t) W_ROOK);
} void test_fen_parsing_initial_board_layout()
{
    	board_container_t * the_board = get_clean_board();
    
	// this is the initial bpard setup
    char *test_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    	consume_fen_notation(test_fen, the_board);
    
	//print_board(the_board);
	
	// verify the board
	// NOTE: This code is cut/paste from above, to make it easier tp
	// find out what tests are failing
	assert_true(W_ROOK == get_piece_at_square(the_board, a1));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, b1));
	assert_true(W_BISHOP == get_piece_at_square(the_board, c1));
	assert_true(W_QUEEN == get_piece_at_square(the_board, d1));
	assert_true(W_KING == get_piece_at_square(the_board, e1));
	assert_true(W_BISHOP == get_piece_at_square(the_board, f1));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, g1));
	assert_true(W_ROOK == get_piece_at_square(the_board, h1));
			assert_true(W_PAWN == get_piece_at_square(the_board, a2));
	assert_true(W_PAWN == get_piece_at_square(the_board, b2));
	assert_true(W_PAWN == get_piece_at_square(the_board, c2));
	assert_true(W_PAWN == get_piece_at_square(the_board, d2));
	assert_true(W_PAWN == get_piece_at_square(the_board, e2));
	assert_true(W_PAWN == get_piece_at_square(the_board, f2));
	assert_true(W_PAWN == get_piece_at_square(the_board, g2));
	assert_true(W_PAWN == get_piece_at_square(the_board, h2));
    assert_true(B_PAWN == get_piece_at_square(the_board, a7));
assert_true(B_PAWN == get_piece_at_square(the_board, b7));
assert_true(B_PAWN == get_piece_at_square(the_board, c7));
assert_true(B_PAWN == get_piece_at_square(the_board, d7));
assert_true(B_PAWN == get_piece_at_square(the_board, e7));
assert_true(B_PAWN == get_piece_at_square(the_board, f7));
assert_true(B_PAWN == get_piece_at_square(the_board, g7));
assert_true(B_PAWN == get_piece_at_square(the_board, h7));
assert_true(B_ROOK == get_piece_at_square(the_board, a8));
assert_true(B_KNIGHT == get_piece_at_square(the_board, b8));
assert_true(B_BISHOP == get_piece_at_square(the_board, c8));
assert_true(B_QUEEN == get_piece_at_square(the_board, d8));
assert_true(B_KING == get_piece_at_square(the_board, e8));
assert_true(B_BISHOP == get_piece_at_square(the_board, f8));
assert_true(B_KNIGHT == get_piece_at_square(the_board, g8));
assert_true(B_ROOK == get_piece_at_square(the_board, h8));
    
	// no piece present
	for (int i = a3; i < h6; i++) {
		assert_true(-1 == get_piece_at_square(the_board, i));
	} } void test_fen_parsing_general_layout_1()
{
    	board_container_t * the_board = get_clean_board();
    
	// this is the initial board setup
    char *test_fen = "k7/8/8/4N3/8/8/8/3K4 b - - 13 56";
    	consume_fen_notation(test_fen, the_board);
    
	//print_board(the_board);
	
	// verify the board
	assert_true(B_KING == get_piece_at_square(the_board, a8));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, e5));
	assert_true(W_KING == get_piece_at_square(the_board, d1));
} void test_fen_parsing_general_layout_2()
{
    	board_container_t * the_board = get_clean_board();
    
	// this is the initial board setup
    char *test_fen ="r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R";
    	consume_fen_notation(test_fen, the_board);

	//print_board(the_board);
	assert_true(W_ROOK == get_piece_at_square(the_board, a1));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, b1));
	assert_true(W_BISHOP == get_piece_at_square(the_board, c1));
	assert_true(W_QUEEN == get_piece_at_square(the_board, d1));
	assert_true(W_KING == get_piece_at_square(the_board, e1));
	assert_true(W_ROOK == get_piece_at_square(the_board, h1));
	assert_true(W_PAWN == get_piece_at_square(the_board, a2));
	assert_true(W_PAWN == get_piece_at_square(the_board, b2));
	assert_true(W_PAWN == get_piece_at_square(the_board, c2));
	assert_true(W_PAWN == get_piece_at_square(the_board, d2));
	assert_true(W_PAWN == get_piece_at_square(the_board, f2));
	assert_true(W_PAWN == get_piece_at_square(the_board, g2));
	assert_true(W_PAWN == get_piece_at_square(the_board, h2));
			assert_true(B_PAWN == get_piece_at_square(the_board, a7));
	assert_true(B_PAWN == get_piece_at_square(the_board, b7));
	assert_true(B_PAWN == get_piece_at_square(the_board, c7));
	assert_true(B_PAWN == get_piece_at_square(the_board, d7));
	assert_true(B_PAWN == get_piece_at_square(the_board, f7));
	assert_true(B_PAWN == get_piece_at_square(the_board, g7));
	assert_true(B_PAWN == get_piece_at_square(the_board, h7));
	assert_true(B_ROOK == get_piece_at_square(the_board, a8));
	assert_true(B_BISHOP == get_piece_at_square(the_board, c8));
	assert_true(B_QUEEN == get_piece_at_square(the_board, d8));
	assert_true(B_KING == get_piece_at_square(the_board, e8));
	assert_true(B_BISHOP == get_piece_at_square(the_board, f8));
	assert_true(B_KNIGHT == get_piece_at_square(the_board, g8));
	assert_true(B_ROOK == get_piece_at_square(the_board, h8));
} void test_fen_parsing_general_layout_3()
{
    	board_container_t * the_board = get_clean_board();
    
	// this is the initial board setup
    char *test_fen = "r1bq1rk1/pp3ppp/3n4/2p1N3/2B5/7P/PPP2PP1/R1BQR1K1";
    	consume_fen_notation(test_fen, the_board);
    
	//print_board(the_board);
		assert_true(W_ROOK == get_piece_at_square(the_board, a1));
	assert_true(W_BISHOP == get_piece_at_square(the_board, c1));
	assert_true(W_QUEEN == get_piece_at_square(the_board, d1));
	assert_true(W_ROOK == get_piece_at_square(the_board, e1));
	assert_true(W_KING == get_piece_at_square(the_board, g1));
	assert_true(W_PAWN == get_piece_at_square(the_board, a2));
	assert_true(W_PAWN == get_piece_at_square(the_board, b2));
	assert_true(W_PAWN == get_piece_at_square(the_board, c2));
	assert_true(W_PAWN == get_piece_at_square(the_board, f2));
	assert_true(W_PAWN == get_piece_at_square(the_board, g2));
	assert_true(W_BISHOP == get_piece_at_square(the_board, c4));
	assert_true(B_PAWN == get_piece_at_square(the_board, c5));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, e5));
	assert_true(B_KNIGHT == get_piece_at_square(the_board, d6));
			assert_true(B_PAWN == get_piece_at_square(the_board, a7));
	assert_true(B_PAWN == get_piece_at_square(the_board, b7));
	assert_true(B_PAWN == get_piece_at_square(the_board, f7));
	assert_true(B_PAWN == get_piece_at_square(the_board, g7));
	assert_true(B_PAWN == get_piece_at_square(the_board, h7));
	assert_true(B_ROOK == get_piece_at_square(the_board, a8));
	assert_true(B_BISHOP == get_piece_at_square(the_board, c8));
	assert_true(B_QUEEN == get_piece_at_square(the_board, d8));
	assert_true(B_ROOK == get_piece_at_square(the_board, f8));
	assert_true(B_KING == get_piece_at_square(the_board, g8));
} void test_white_occupancy_map()
{
    board_container_t *board = get_clean_board();
    	add_piece_to_board(board, W_PAWN, a3);
	add_piece_to_board(board, W_PAWN, b8);
	add_piece_to_board(board, W_PAWN, a4);
	add_piece_to_board(board, W_PAWN, d7);
	add_piece_to_board(board, W_PAWN, g8);
	add_piece_to_board(board, W_BISHOP, a8);
	add_piece_to_board(board, W_ROOK, h8);
	add_piece_to_board(board, W_QUEEN, a5);
	add_piece_to_board(board, W_KNIGHT, c6);
	add_piece_to_board(board, W_KNIGHT, b6);
			board_t a_board = get_white_occupancy_map(board);
	assert_true(is_square_occupied(a_board, a3));
	assert_true(is_square_occupied(a_board, b8));
	assert_true(is_square_occupied(a_board, a4));
	assert_true(is_square_occupied(a_board, d7));
	assert_true(is_square_occupied(a_board, g8));
	assert_true(is_square_occupied(a_board, a8));
	assert_true(is_square_occupied(a_board, h8));
	assert_true(is_square_occupied(a_board, a5));
	assert_true(is_square_occupied(a_board, c6));
	assert_true(is_square_occupied(a_board, b6));
} void test_black_occupancy_map()
{
    board_container_t *board = get_clean_board();
    	add_piece_to_board(board, B_PAWN, a3);
	add_piece_to_board(board, B_PAWN, a1);
	add_piece_to_board(board, B_PAWN, d7);
	add_piece_to_board(board, B_PAWN, e8);
	add_piece_to_board(board, B_PAWN, g8);
	add_piece_to_board(board, B_BISHOP, f4);
	add_piece_to_board(board, B_ROOK, h6);
	add_piece_to_board(board, B_QUEEN, a5);
	add_piece_to_board(board, B_KNIGHT, g2);
	add_piece_to_board(board, B_KNIGHT, g8);
			board_t a_board = get_black_occupancy_map(board);
	assert_true(is_square_occupied(a_board, a3));
	assert_true(is_square_occupied(a_board, a1));
	assert_true(is_square_occupied(a_board, d7));
	assert_true(is_square_occupied(a_board, e8));
	assert_true(is_square_occupied(a_board, g8));
	assert_true(is_square_occupied(a_board, f4));
	assert_true(is_square_occupied(a_board, h6));
	assert_true(is_square_occupied(a_board, a5));
	assert_true(is_square_occupied(a_board, g2));
	assert_true(is_square_occupied(a_board, g8));
} void test_setting_bits_in_a_board()
{    	board_t test_brd = 0;
    
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
    } void test_clearing_bits_in_a_board()
{
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
} void test_checking_bits_in_a_board()
{			board_t test_brd = 0;

	for (int i = 0; i < NUM_SQUARES; i++){
		set_bit(&test_brd, i);
		assert_true(check_bit(&test_brd, i));
		
		clear_bit(&test_brd, i);
		assert_false(check_bit(&test_brd, i));		
	}
} 


void test_bit_counting(){
	board_t brd = 0;
	
	set_bit(&brd, d3);
	set_bit(&brd, a7);
	set_bit(&brd, b3);
	set_bit(&brd, g6);
	set_bit(&brd, d2);
	
	assert_true(count_bits(brd) == 5);
		
}



void test_LSB_clear(){
	board_t brd = 0;
	
	set_bit(&brd, d3);
	set_bit(&brd, a7);
	set_bit(&brd, b3);
	set_bit(&brd, g6);
	set_bit(&brd, a1);
	
	
	U8 cleared_bit = pop_1st_bit(&brd);	
	
	assert_true(cleared_bit == a1);
	assert_false(check_bit(&brd, a1));
}
void board_test_fixture(void) 
{
    	test_fixture_start();	// starts a fixture
			run_test(test_initial_board_placement);
	run_test(test_clean_board);
	run_test(test_add_to_board);
			run_test(test_setting_bits_in_a_board);
	run_test(test_checking_bits_in_a_board);
	run_test(test_clearing_bits_in_a_board);
			run_test(test_fen_parsing_initial_board_layout);
	run_test(test_fen_parsing_general_layout_1);
	run_test(test_fen_parsing_general_layout_2);
	run_test(test_fen_parsing_general_layout_3);
	run_test(test_white_occupancy_map);
	run_test(test_black_occupancy_map);

	run_test(test_bit_counting);
	run_test(test_LSB_clear);
	
		test_fixture_end();	// ends a fixture
} 
