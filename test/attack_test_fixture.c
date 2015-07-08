/*
 * attack_test_fixture.c
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
#include "move.h"


void attack_test_fixture(void);
void test_is_square_being_attacked_by_knight(void);
void test_is_square_being_attacked_by_pawn(void);
void test_is_square_being_attacked_by_king(void);
void test_is_square_being_attacked_by_rook_or_queen(void);



void test_is_square_being_attacked_by_knight(void){
	// knight attacks e5
	char * test_fen = "rnbqkbnr/pppp1ppp/8/4p3/8/5N2/PPPPPPPP/RNBQKB1R w KQkq e6 0 2";
	
	board_container_t * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_knight_attacking_square(e5, WHITE, brd)); 


	// - white Knight attacks d5
	// - black knight attacks d4
	// - white knight attacks e5
	// - white knight attacks g5
	// - black knight attacks e4
	// - black knight attacks g4
	test_fen = "r2qkb1r/pppb1p1p/2n2n2/3pp1p1/3PP1Q1/BPN2N2/P1P2PPP/R3KB1R b KQkq - 4 7";
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_knight_attacking_square(d5, WHITE, brd)); 
	assert_true(is_knight_attacking_square(d4, BLACK, brd)); 
	assert_true(is_knight_attacking_square(e5, WHITE, brd)); 
	assert_true(is_knight_attacking_square(g5, WHITE, brd)); 
	assert_true(is_knight_attacking_square(e4, BLACK, brd)); 
	assert_true(is_knight_attacking_square(g4, BLACK, brd)); 



	// - white Knight attacks c6
	// - white knight attacks d6
	test_fen = "8/NPK2NPP/P1pppP2/3P4/k5p1/2p5/6n1/8 w - - 0 1";
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_knight_attacking_square(c6, WHITE, brd)); 
	assert_true(is_knight_attacking_square(d6, WHITE, brd)); 

}



void test_is_square_being_attacked_by_pawn(void){
	// black pawn attacks c4
	// black pawn attacks d4
	// black pawn attacks e4
	// black pawn attacks g4
	// black pawn attacks h4
	// white pawn attacks e5
	// white pawn attacks d5
	// white pawn attacks g5
	char * test_fen_1 = "r2qkb1r/p1pb1p2/2n2n2/1p1pp1pp/2BPP1QP/BPN2N2/P1P2PP1/R3K2R b KQkq - 1 9";
	
	board_container_t * brd = get_clean_board();
	consume_fen_notation(test_fen_1, brd);
	//print_board(brd);
	assert_true(is_pawn_attacking_square(c4, BLACK, brd)); 
	assert_true(is_pawn_attacking_square(d4, BLACK, brd)); 
	assert_true(is_pawn_attacking_square(e4, BLACK, brd)); 
	assert_true(is_pawn_attacking_square(g4, BLACK, brd)); 
	assert_true(is_pawn_attacking_square(h4, BLACK, brd)); 
	assert_true(is_pawn_attacking_square(e5, WHITE, brd)); 
	assert_true(is_pawn_attacking_square(d5, WHITE, brd)); 
	assert_true(is_pawn_attacking_square(g5, WHITE, brd)); 
}


void test_is_square_being_attacked_by_king(void){
	// black king attacks c5
	// white king attacks f2
	char * test_fen = "8/p3p1P1/2p5/pkP5/8/1p1pP1P1/pP1P1pK1/8 w - - 0 1";
	
	board_container_t * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_king_attacking_square(c5, BLACK, brd)); 
	assert_true(is_king_attacking_square(f2, WHITE, brd));



	// black king attacks h6
	// white king attacks b7
	test_fen = "8/1p2pp1k/1KP4P/3PP3/4p3/2pp2P1/P2p2P1/8 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_king_attacking_square(h6, BLACK, brd)); 
	assert_true(is_king_attacking_square(b7, WHITE, brd));
	
	
	
	// black king attacks g2
	// white king attacks h7
	test_fen = "8/p5Pp/PP5K/p4p2/P4ppP/3p4/3p2Pk/8 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_king_attacking_square(g2, BLACK, brd)); 
	assert_true(is_king_attacking_square(h7, WHITE, brd));
	
	
	
	// black king attacks g7
	// white king attacks c7
	test_fen = "3K2k1/2p3P1/Pp5p/P3P2P/P7/4p1p1/P1p1P3/8 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_king_attacking_square(g7, BLACK, brd)); 
	assert_true(is_king_attacking_square(c7, WHITE, brd));
	
	
	
}


void test_is_square_being_attacked_by_rook_or_queen(void){
	
	// black queen attacking e2
	char * test_fen = "88/1R1PpR2/5P2/1K3p2/3r1p2/k5Pp/4Pq1p/1Q6 w - - 0 1";
	
	board_container_t * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_rook_or_queen_attacking_square(e2, B_QUEEN, brd));	
	
	
	
	
	
	// c7 attacked by WQ
	// c7 attacked by WQ (2nd queen)
	// b5 attacked by WQ
	// b3 attacked by BR
	// b3 attacked by BQ
	// d2 attacked by BQ
	// b7 attacked by BR	
	test_fen = "3R4/1Qp2QP1/p7/rrp1K2P/2p2P1p/pPP1kPp1/P1pRP3/1q1q4 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_rook_or_queen_attacking_square(c7, W_QUEEN, brd));	
	assert_true(is_rook_or_queen_attacking_square(c7, W_QUEEN, brd));	
	assert_true(is_rook_or_queen_attacking_square(b5, W_QUEEN, brd));	
	assert_true(is_rook_or_queen_attacking_square(b3, B_ROOK, brd));	
	assert_true(is_rook_or_queen_attacking_square(b3, B_QUEEN, brd));	
	assert_true(is_rook_or_queen_attacking_square(d2, B_QUEEN, brd));
	assert_true(is_rook_or_queen_attacking_square(b7, B_ROOK, brd));






	// b7 attacked by BR
	// a5 attacked by WR
	// b5 attacked by WR
	// h5 attacked by WR
	// e6 attacked by BQ
	// g7 attacked by BQ
	test_fen = "r7/1Pr2p2/2P1PqP1/pp1R3p/3P1p2/RKP2pP1/2P2pkp/4Q3 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_rook_or_queen_attacking_square(b7, B_ROOK, brd));	
	assert_true(is_rook_or_queen_attacking_square(a5, W_ROOK, brd));	
	assert_true(is_rook_or_queen_attacking_square(b5, W_ROOK, brd));	
	assert_true(is_rook_or_queen_attacking_square(h5, W_ROOK, brd));	
	assert_true(is_rook_or_queen_attacking_square(e6, B_QUEEN, brd));	
	assert_true(is_rook_or_queen_attacking_square(g6, B_QUEEN, brd));
		
	
}





void attack_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

	run_test(test_is_square_being_attacked_by_knight);
	run_test(test_is_square_being_attacked_by_pawn);
	run_test(test_is_square_being_attacked_by_king);
	run_test(test_is_square_being_attacked_by_rook_or_queen);
    
    test_fixture_end();		// ends a fixture
}

