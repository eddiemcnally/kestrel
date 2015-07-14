/*
 * piece_test_fixture.c
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
#include "seatest.h"
#include "types.h"
#include "board.h"
#include "pieces.h"

void test_rank_file_macros(void);
void test_piece_colour_macro(void);
void test_piece_classification_IS_BIG_PIECE(void);
void test_piece_classification_IS_MAJOR_PIECE(void);
void test_piece_classification_IS_MINOR_PIECE(void);
void test_piece_to_char_conversion(void);
void piece_test_fixture(void);



void test_rank_file_macros(void){
	
	// offsets into ranks enum
	assert_true(0 == GET_RANK(a1));
	assert_true(1 == GET_RANK(a2));
	assert_true(7 == GET_RANK(a8));
	assert_true(7 == GET_RANK(h8));
	
	
	// offsets into files enum
	assert_true(0 == GET_FILE(a1));
	assert_true(0 == GET_FILE(a2));
	assert_true(2 == GET_FILE(c3));
	assert_true(7 == GET_FILE(h8));
	

	// check on square derived from rank/file
	assert_true(a1 == GET_SQUARE(RANK_1, FILE_A));
	assert_true(a8 == GET_SQUARE(RANK_8, FILE_A));
	assert_true(h1 == GET_SQUARE(RANK_1, FILE_H));
	assert_true(h8 == GET_SQUARE(RANK_8, FILE_H));
}



void test_piece_colour_macro(void){
	assert_true(true == IS_WHITE(W_PAWN));
	assert_true(true == IS_WHITE(W_ROOK));
	assert_true(true == IS_WHITE(W_BISHOP));
	assert_true(true == IS_WHITE(W_KNIGHT));
	assert_true(true == IS_WHITE(W_KING));
	assert_true(true == IS_WHITE(W_QUEEN));

	assert_true(false == IS_WHITE(B_PAWN));
	assert_true(false == IS_WHITE(B_ROOK));
	assert_true(false == IS_WHITE(B_BISHOP));
	assert_true(false == IS_WHITE(B_KNIGHT));
	assert_true(false == IS_WHITE(B_KING));
	assert_true(false == IS_WHITE(B_QUEEN));


	assert_true(false == IS_BLACK(W_PAWN));
	assert_true(false == IS_BLACK(W_ROOK));
	assert_true(false == IS_BLACK(W_BISHOP));
	assert_true(false == IS_BLACK(W_KNIGHT));
	assert_true(false == IS_BLACK(W_KING));
	assert_true(false == IS_BLACK(W_QUEEN));

	assert_true(true == IS_BLACK(B_PAWN));
	assert_true(true == IS_BLACK(B_ROOK));
	assert_true(true == IS_BLACK(B_BISHOP));
	assert_true(true == IS_BLACK(B_KNIGHT));
	assert_true(true == IS_BLACK(B_KING));
	assert_true(true == IS_BLACK(B_QUEEN));	
}



void test_piece_classification_IS_BIG_PIECE(void){
	
	printf("is_big_piece(W_PAWN) %d\n", (int)is_big_piece(W_PAWN));
	
	assert_false(is_big_piece(W_PAWN));
	assert_false(is_big_piece(B_PAWN));
	
	assert_true(is_big_piece(W_ROOK) 	== true);
	assert_true(is_big_piece(W_BISHOP) 	== true);
	assert_true(is_big_piece(W_KNIGHT) 	== true);
	assert_true(is_big_piece(W_QUEEN) 	== true);
	assert_true(is_big_piece(W_KING) 	== true);

	assert_true(is_big_piece(B_ROOK) 	== true);
	assert_true(is_big_piece(B_BISHOP) 	== true);
	assert_true(is_big_piece(B_KNIGHT) 	== true);
	assert_true(is_big_piece(B_QUEEN) 	== true);
	assert_true(is_big_piece(B_KING) 	== true);

}

void test_piece_classification_IS_MAJOR_PIECE(void){
	
	assert_true(is_major_piece(W_PAWN)		== false);
	assert_true(is_major_piece(B_PAWN)		== false);
	assert_true(is_major_piece(W_BISHOP)	== false);
	assert_true(is_major_piece(B_BISHOP)	== false);
	assert_true(is_major_piece(W_KNIGHT)	== false);
	assert_true(is_major_piece(B_KNIGHT)	== false);
	
	assert_true(is_big_piece(B_ROOK)	== true);
	assert_true(is_big_piece(B_QUEEN)	== true);
	assert_true(is_big_piece(W_QUEEN)	== true);
	assert_true(is_big_piece(W_ROOK)	== true);


}



void test_piece_classification_IS_MINOR_PIECE(void){
	
	assert_true(is_minor_piece(W_PAWN)		== false);
	assert_true(is_minor_piece(B_PAWN)		== false);
	assert_true(is_minor_piece(W_ROOK)		== false);
	assert_true(is_minor_piece(B_ROOK)		== false);
	assert_true(is_minor_piece(W_QUEEN)		== false);
	assert_true(is_minor_piece(B_QUEEN)		== false);
	
	assert_true(is_minor_piece(B_BISHOP)	== true);
	assert_true(is_minor_piece(B_KNIGHT)	== true);
	assert_true(is_minor_piece(W_BISHOP)	== true);
	assert_true(is_minor_piece(W_KNIGHT)	== true);


}




void test_piece_to_char_conversion(void)
{
			assert_true('P' == get_piece_label(W_PAWN));
	assert_true('p' == get_piece_label(B_PAWN));
	assert_true('R' == get_piece_label(W_ROOK));
	assert_true('r' == get_piece_label(B_ROOK));
	assert_true('B' == get_piece_label(W_BISHOP));
	assert_true('b' == get_piece_label(B_BISHOP));
	assert_true('N' == get_piece_label(W_KNIGHT));
	assert_true('n' == get_piece_label(B_KNIGHT));
	assert_true('Q' == get_piece_label(W_QUEEN));
	assert_true('q' == get_piece_label(B_QUEEN));
	assert_true('K' == get_piece_label(W_KING));
	assert_true('k' == get_piece_label(B_KING));
} void piece_test_fixture(void) 
{
    	test_fixture_start();
    	run_test(test_piece_to_char_conversion);
    run_test(test_rank_file_macros);
    run_test(test_piece_colour_macro);
    run_test(test_piece_classification_IS_BIG_PIECE);
    run_test(test_piece_classification_IS_MAJOR_PIECE);
    run_test(test_piece_classification_IS_MINOR_PIECE);
    	test_fixture_end();
} 
