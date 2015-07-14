/*
 * board_test_fixture.c
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
#include "seatest.h"
#include "types.h"
#include "hashkeys.h"
#include "board_utils.h"
#include "board.h"
#include "fen.h"
#include "pieces.h"

void verify_initial_board_placement(struct board * the_board);
void test_initial_board_placement(void);
void test_clean_board(void);
void test_fen_parsing_initial_board_layout(void);
void test_add_to_board(void);
void test_fen_parsing_general_layout_1(void);
void test_fen_parsing_general_layout_2(void);
void test_setting_bits_in_a_board(void);
void test_clearing_bits_in_a_board(void);
void test_checking_bits_in_a_board(void);
void test_bit_counting(void);
void test_LSB_clear(void);
void board_test_fixture(void);
/**
 * Verifies the initial board setup plus some supporting code
 */ 
void test_initial_board_placement() 
{
	init_hash_keys();	struct board * the_board = init_board();
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
		assert_true(NO_PIECE == get_piece_at_square(the_board, i));
	}
	
	
	// now verify remainder of struct is populated
	assert_true(the_board->king_squares[WHITE] == e1);
	assert_true(the_board->king_squares[BLACK] == e8);
	
	assert_true(the_board->side_to_move == WHITE);
	
	assert_true(the_board->en_passant == NO_SQUARE);
	
	assert_true(the_board->fifty_move_counter == 0);
	assert_true(the_board->ply == 0);
	assert_true(the_board->history_ply == 0);
	
	assert_true(the_board->pce_num[W_PAWN] == 8);
	assert_true(the_board->pce_num[W_KNIGHT] == 2);
	assert_true(the_board->pce_num[W_BISHOP] == 2);
	assert_true(the_board->pce_num[W_ROOK] 	== 2);
	assert_true(the_board->pce_num[W_QUEEN] == 1);
	assert_true(the_board->pce_num[W_KING] 	== 1);
	
	assert_true(the_board->pce_num[B_PAWN] == 8);
	assert_true(the_board->pce_num[B_KNIGHT] == 2);
	assert_true(the_board->pce_num[B_BISHOP] == 2);
	assert_true(the_board->pce_num[B_ROOK] == 2);
	assert_true(the_board->pce_num[B_QUEEN] == 1);
	assert_true(the_board->pce_num[B_KING] == 1);

	assert_true(the_board->big_pieces[WHITE] == 8);
	assert_true(the_board->big_pieces[BLACK] == 8);

	assert_true(the_board->major_pieces[WHITE] == 3);
	assert_true(the_board->major_pieces[BLACK] == 3);
	
	assert_true(the_board->minor_pieces[WHITE] == 4);
	assert_true(the_board->minor_pieces[BLACK] == 4);
	
	assert_true(the_board->castle_perm == (WQCA | WKCA | BQCA | BKCA));

	assert_true(the_board->board_hash != 0);

	
	} void test_clean_board()
{
    	struct board * the_board = get_clean_board();
    
	// check all squares are zero
	assert_true(the_board->board == (U64) 0);
    	for (int i = 0; i < NUM_PIECES; i++) {
		assert_true(the_board->bitboards[i] == (U64) 0);
	} } void test_add_to_board()
{
	struct board * the_board = get_clean_board();
    	add_piece_to_board(the_board, W_PAWN, a8);
	add_piece_to_board(the_board, B_QUEEN, g3);
	add_piece_to_board(the_board, W_PAWN, b6);
	add_piece_to_board(the_board, W_KING, a4);
	add_piece_to_board(the_board, W_ROOK, h8);
		
	// add again, to check error handling
	assert_true(add_piece_to_board(the_board, W_PAWN, a8) == false);
	assert_true(get_piece_at_square(the_board, a8) == (enum piece) W_PAWN);
	assert_true(get_piece_at_square(the_board, g3) == (enum piece) B_QUEEN);
	assert_true(get_piece_at_square(the_board, b6) == (enum piece) W_PAWN);
	assert_true(get_piece_at_square(the_board, a4) == (enum piece) W_KING);
	assert_true(get_piece_at_square(the_board, h8) == (enum piece) W_ROOK);
} void test_fen_parsing_initial_board_layout()
{
    	struct board * the_board = get_clean_board();
    
	// this is the initial bpard setup
	char *test_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    	consume_fen_notation(test_fen, the_board);
    
	//print_board(the_board);
	
	// verify the board
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
		assert_true(NO_PIECE == get_piece_at_square(the_board, i));
	} 
	
	assert_true(the_board->side_to_move == WHITE);
	
	assert_true(the_board->castle_perm == (WKCA | WQCA | BKCA | BQCA));
	
	assert_true(the_board->en_passant == NO_SQUARE);
	} void test_fen_parsing_general_layout_1()
{   	struct board * the_board = get_clean_board();
    
	// this is the initial board setup
	char *test_fen = "k7/8/8/4N3/8/8/8/3K4 b - - 13 56";
    	consume_fen_notation(test_fen, the_board);
    
	//print_board(the_board);
	
	// verify the board
	assert_true(B_KING == get_piece_at_square(the_board, a8));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, e5));
	assert_true(W_KING == get_piece_at_square(the_board, d1));

	assert_true(the_board->side_to_move == BLACK);
	
	assert_true(the_board->castle_perm == 0);
	
	assert_true(the_board->en_passant == NO_SQUARE);
} 
void test_fen_parsing_general_layout_2()
{
	struct board * the_board = get_clean_board();
    

	// this is the initial board setup
	char *test_fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
    
	consume_fen_notation(test_fen, the_board);
    

	//print_board(the_board);
	
	// verify the board
	assert_true(W_ROOK == get_piece_at_square(the_board, a1));
	assert_true(W_KNIGHT == get_piece_at_square(the_board, b1));
	assert_true(W_BISHOP == get_piece_at_square(the_board, c1));
	assert_true(W_QUEEN == get_piece_at_square(the_board, d1));
	assert_true(W_KING == get_piece_at_square(the_board, e1));
	assert_true(W_BISHOP == get_piece_at_square(the_board, f1));

	assert_true(W_ROOK == get_piece_at_square(the_board, h1));
		

	assert_true(W_PAWN == get_piece_at_square(the_board, a2));
	assert_true(W_PAWN == get_piece_at_square(the_board, b2));
	assert_true(W_PAWN == get_piece_at_square(the_board, d2));
	assert_true(W_PAWN == get_piece_at_square(the_board, f2));
	assert_true(W_PAWN == get_piece_at_square(the_board, g2));
	assert_true(W_PAWN == get_piece_at_square(the_board, h2));
    
	assert_true(B_PAWN == get_piece_at_square(the_board, a7));
	assert_true(B_PAWN == get_piece_at_square(the_board, b7));
	assert_true(B_PAWN == get_piece_at_square(the_board, d7));
	assert_true(B_PAWN == get_piece_at_square(the_board, f7));
	assert_true(B_PAWN == get_piece_at_square(the_board, g7));
	assert_true(B_PAWN == get_piece_at_square(the_board, h7));
	
	assert_true(B_ROOK == get_piece_at_square(the_board, a8));
	assert_true(B_KNIGHT == get_piece_at_square(the_board, b8));
	assert_true(B_BISHOP == get_piece_at_square(the_board, c8));
	assert_true(B_QUEEN == get_piece_at_square(the_board, d8));
	assert_true(B_KING == get_piece_at_square(the_board, e8));
	assert_true(B_BISHOP == get_piece_at_square(the_board, f8));
	assert_true(B_ROOK == get_piece_at_square(the_board, h8));

	assert_true(the_board->side_to_move == BLACK);
	
	assert_true(the_board->castle_perm == (WKCA | WQCA | BKCA | BQCA));
	
	assert_true(the_board->en_passant == NO_SQUARE);
} 
void test_setting_bits_in_a_board()
{    	U64 test_brd = 0;
    
	//
	// Test setting bits
	//
	set_bit(&test_brd, 22);
	assert_true(test_brd == 4194304);
	assert_true(1 == CNT(test_brd));
    
    	test_brd = 0;
	set_bit(&test_brd, 0);
	assert_true(test_brd == 1);
	assert_true(1 == CNT(test_brd));

    	test_brd = 0;
	set_bit(&test_brd, 63);
	assert_true(test_brd == 0x8000000000000000);
	assert_true(1 == CNT(test_brd));	
	
	test_brd = 0;
	set_bit(&test_brd, 31);
	assert_true(test_brd == 2147483648);
	assert_true(1 == CNT(test_brd));
    
    
	test_brd = 0;
	set_bit(&test_brd, 31);
	set_bit(&test_brd, 1);
	set_bit(&test_brd, 21);
	set_bit(&test_brd, 55);
	set_bit(&test_brd, 8);
	assert_true(test_brd == 0x80000080200102);
	assert_true(5 == CNT(test_brd));
    
    } void test_clearing_bits_in_a_board()
{
    	U64 test_brd = 0;
    
	//
	// Test clearing bits
	//
	set_bit(&test_brd, 22);
	set_bit(&test_brd, 23);	
	assert_true(2 == CNT(test_brd));
	assert_true(check_bit(&test_brd, 22));
	assert_true(check_bit(&test_brd, 23));
	clear_bit(&test_brd, 22);
	assert_true(check_bit(&test_brd, 23));
	assert_true(1 == CNT(test_brd));
    
    
    
	test_brd = 0;	set_bit(&test_brd, 1);
	clear_bit(&test_brd, 1);
	assert_true(test_brd == 0);
    	test_brd = 0;
	set_bit(&test_brd, 31);
	clear_bit(&test_brd, 31);
	assert_true(test_brd == 0);
   	set_bit(&test_brd, 32);
	clear_bit(&test_brd, 32);
	assert_true(test_brd == 0);
    	set_bit(&test_brd, 63);
	clear_bit(&test_brd, 63);
	assert_true(test_brd == 0);
} void test_checking_bits_in_a_board(void)
{			U64 test_brd = 0;

	for (int i = 0; i < NUM_SQUARES; i++){
		set_bit(&test_brd, i);
		assert_true(check_bit(&test_brd, i));
		
		clear_bit(&test_brd, i);
		assert_false(check_bit(&test_brd, i));		
	}
} 


void test_bit_counting(void){
	U64 brd = 0;
	
	set_bit(&brd, d3);
	set_bit(&brd, a7);
	set_bit(&brd, b3);
	set_bit(&brd, g6);
	set_bit(&brd, d2);
	
	assert_true(count_bits(brd) == 5);
		
}



void test_LSB_clear(void){
	U64 brd = 0;
	
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

	run_test(test_bit_counting);
	run_test(test_LSB_clear);
	
		test_fixture_end();	// ends a fixture
} 
