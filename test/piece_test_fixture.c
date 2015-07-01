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


void test_rank_file_macros(){
	
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



#define IS_WHITE(piece)			(piece <= W_KING)
#define IS_BLACK(piece)			(IS_WHITE(piece) == false)

void test_piece_colour_macro(){
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



void test_piece_classification_IS_BIG_PIECE(){
	assert_true(IS_BIG_PIECE(W_PAWN) 	== false);
	assert_true(IS_BIG_PIECE(B_PAWN) 	== false);
	
	assert_true(IS_BIG_PIECE(W_ROOK) 	== true);
	assert_true(IS_BIG_PIECE(W_BISHOP) 	== true);
	assert_true(IS_BIG_PIECE(W_KNIGHT) 	== true);
	assert_true(IS_BIG_PIECE(W_QUEEN) 	== true);
	assert_true(IS_BIG_PIECE(W_KING) 	== true);

	assert_true(IS_BIG_PIECE(B_ROOK) 	== true);
	assert_true(IS_BIG_PIECE(B_BISHOP) 	== true);
	assert_true(IS_BIG_PIECE(B_KNIGHT) 	== true);
	assert_true(IS_BIG_PIECE(B_QUEEN) 	== true);
	assert_true(IS_BIG_PIECE(B_KING) 	== true);

}

void test_piece_classification_IS_MAJOR_PIECE(){
	
	assert_true(IS_MAJOR_PIECE(W_PAWN)		== false);
	assert_true(IS_MAJOR_PIECE(B_PAWN)		== false);
	assert_true(IS_MAJOR_PIECE(W_BISHOP)	== false);
	assert_true(IS_MAJOR_PIECE(B_BISHOP)	== false);
	assert_true(IS_MAJOR_PIECE(W_KNIGHT)	== false);
	assert_true(IS_MAJOR_PIECE(B_KNIGHT)	== false);
	
	assert_true(IS_BIG_PIECE(B_ROOK)	== true);
	assert_true(IS_BIG_PIECE(B_QUEEN)	== true);
	assert_true(IS_BIG_PIECE(W_QUEEN)	== true);
	assert_true(IS_BIG_PIECE(W_ROOK)	== true);


}



void test_piece_classification_IS_MINOR_PIECE(){
	
	assert_true(IS_MINOR_PIECE(W_PAWN)		== false);
	assert_true(IS_MINOR_PIECE(B_PAWN)		== false);
	assert_true(IS_MINOR_PIECE(W_ROOK)		== false);
	assert_true(IS_MINOR_PIECE(B_ROOK)		== false);
	assert_true(IS_MINOR_PIECE(W_QUEEN)		== false);
	assert_true(IS_MINOR_PIECE(B_QUEEN)		== false);
	
	assert_true(IS_MINOR_PIECE(B_BISHOP)	== true);
	assert_true(IS_MINOR_PIECE(B_KNIGHT)	== true);
	assert_true(IS_MINOR_PIECE(W_BISHOP)	== true);
	assert_true(IS_MINOR_PIECE(W_KNIGHT)	== true);


}




void test_piece_to_char_conversion()
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
