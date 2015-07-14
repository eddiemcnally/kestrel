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
void test_is_square_being_attacked_by_rook(void);
void test_is_square_being_attacked_by_bishop(void);
void test_is_square_attacked_by_queen(void);
void test_is_square_under_attack(void);
void test_is_blocked_up_or_down(void);
void test_is_blocked_diagonally(void);



void test_is_square_being_attacked_by_knight(void){
	// knight attacks e5
	char * test_fen = "rnbqkbnr/pppp1ppp/8/4p3/8/5N2/PPPPPPPP/RNBQKB1R w KQkq e6 0 2";
	
	BOARD * brd = get_clean_board();
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
	
	// do some negative tests
	assert_false(is_knight_attacking_square(a3, WHITE, brd)); 
	assert_false(is_knight_attacking_square(h4, BLACK, brd)); 
	assert_false(is_knight_attacking_square(g7, BLACK, brd)); 
	assert_false(is_knight_attacking_square(b2, BLACK, brd)); 
	assert_false(is_knight_attacking_square(a1, BLACK, brd)); 
	assert_false(is_knight_attacking_square(c2, BLACK, brd)); 


	// - white Knight attacks c6
	// - white knight attacks d6
	test_fen = "8/NPK2NPP/P1pppP2/3P4/k5p1/2p5/6n1/8 w - - 0 1";
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_knight_attacking_square(c6, WHITE, brd)); 
	assert_true(is_knight_attacking_square(d6, WHITE, brd)); 


	// WN attacks f6, g7, f4
	// BN attacks f7, b5, e4
	test_fen = "B1kn3Q/R1p2Rpq/2p1pqN1/1PK2p1N/1p2PpP1/1Pn1Pr1b/1pr2PP1/4BQ2 w - - 0 1";
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_knight_attacking_square(f6, WHITE, brd)); 
	assert_true(is_knight_attacking_square(g7, WHITE, brd));
	assert_true(is_knight_attacking_square(f4, WHITE, brd)); 

	assert_true(is_knight_attacking_square(f7, BLACK, brd)); 
	assert_true(is_knight_attacking_square(b5, BLACK, brd)); 
	assert_true(is_knight_attacking_square(e4, BLACK, brd)); 


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
	
	BOARD * brd = get_clean_board();
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
	
	
	
	test_fen_1 = "1nr3n1/ppqNkp2/1R3p2/bQ4PK/1PpRp1PP/P2bBPqp/4P1r1/1B1Q1N2 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen_1, brd);
	//print_board(brd);
	assert_true(is_pawn_attacking_square(b6, BLACK, brd)); 
	assert_true(is_pawn_attacking_square(f3, BLACK, brd)); 
	assert_true(is_pawn_attacking_square(a5, WHITE, brd)); 
	assert_true(is_pawn_attacking_square(d3, WHITE, brd)); 
	assert_true(is_pawn_attacking_square(f6, WHITE, brd)); 
	assert_true(is_pawn_attacking_square(d3, WHITE, brd)); 
	assert_true(is_pawn_attacking_square(e4, WHITE, brd)); 
	
}


void test_is_square_being_attacked_by_king(void){
	// black king attacks c5
	// white king attacks f2
	char * test_fen = "8/p3p1P1/2p5/pkP5/8/1p1pP1P1/pP1P1pK1/8 w - - 0 1";
	
	BOARD * brd = get_clean_board();
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

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

void test_is_square_attacked_by_queen(void){
	
	// c6 attacked by BQ
	// d3 attacked by WQ
	// e7 attacked by WQ
	// d3 attacked by WQ (2nd WQ)
	// f4 attacked by WQ
	// h5 attacked by WQ
	// f7 attacked by BQ
	char * test_fen = "1n6/2pqpPq1/2Pp2pk/2NP3p/2K2p1p/P1pn1Q2/2PPQPP1/7N w - - 0 1";
	
	BOARD * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_queen_attacking_square(c6, BLACK, brd)); 
	assert_true(is_queen_attacking_square(d3, WHITE, brd)); 
	assert_true(is_queen_attacking_square(e7, WHITE, brd)); 
	assert_true(is_queen_attacking_square(d3, WHITE, brd)); 
	assert_true(is_queen_attacking_square(f4, WHITE, brd)); 
	assert_true(is_queen_attacking_square(h5, WHITE, brd)); 
	assert_true(is_queen_attacking_square(f7, BLACK, brd)); 
	


	test_fen = "R1q2B2/KpNp1P1P/3p2pP/NPk1pQ2/PpP3np/2b3R1/3n2BP/rb1qQ1r1 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_queen_attacking_square(a8, BLACK, brd));
	assert_true(is_queen_attacking_square(f8, BLACK, brd));
	assert_true(is_queen_attacking_square(c7, BLACK, brd));
	assert_true(is_queen_attacking_square(e1, BLACK, brd));
	assert_true(is_queen_attacking_square(a4, BLACK, brd));
	
	assert_true(is_queen_attacking_square(d1, WHITE, brd));
	assert_true(is_queen_attacking_square(d2, WHITE, brd));
	assert_true(is_queen_attacking_square(g1, WHITE, brd));
	assert_true(is_queen_attacking_square(e5, WHITE, brd));
	assert_true(is_queen_attacking_square(g4, WHITE, brd));
	assert_true(is_queen_attacking_square(g6, WHITE, brd));
	assert_true(is_queen_attacking_square(e5, WHITE, brd));
	assert_true(is_queen_attacking_square(d7, WHITE, brd));

	
}

void test_is_square_being_attacked_by_rook(void){


	// c8 attacked by WR
	// a6 attacked by BR
	// g6 attacked by BR
	// d2 attacked by BR
	// c2 attacked by WR
	char * test_fen = "R1qq4/P1k2p1P/Qr4QP/2p2PP1/2prp1p1/2p4p/1PpPPK2/2R5 w - - 0 1";
	
	BOARD * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_rook_attacking_square(c8, WHITE, brd)); 
	assert_true(is_rook_attacking_square(a6, BLACK, brd)); 
	assert_true(is_rook_attacking_square(g6, BLACK, brd)); 
	assert_true(is_rook_attacking_square(d2, BLACK, brd)); 
	assert_true(is_rook_attacking_square(c2, WHITE, brd)); 
	
	
	
	
	// d8 attacked by WR
	// f7 attacked by WR
	// a6 attacked by BR
	// e6 attacked by WR
	// e2 attacked by WR
	// e3 attacked by BR
	test_fen = "1Q1b1R2/1P1P1p2/Q2ppP1K/r1pp1b2/pP1P1Pp1/NkqPR3/N2pr2P/4q3 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_rook_attacking_square(d8, WHITE, brd)); 
	assert_true(is_rook_attacking_square(f7, WHITE, brd)); 
	assert_true(is_rook_attacking_square(a6, BLACK, brd)); 
	assert_true(is_rook_attacking_square(e6, WHITE, brd)); 
	assert_true(is_rook_attacking_square(e2, WHITE, brd));
	assert_true(is_rook_attacking_square(e3, BLACK, brd));
	 
	
	
	
	// a8 attacked by WR
	// d7 attacked by WR
	// b6 attacked by WR
	// d4 attacked by WR
	// f5 attacked by BR
	// h6 attacked by BR
	// f2 attacked by BR
	// h1 attacked by BR
	test_fen = "b1qkN3/R2pb1P1/1pp1pPpP/4QP1r/1R1p2P1/P2q1P2/p1pP1N1r/6KQ w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_rook_attacking_square(a8, WHITE, brd)); 
	assert_true(is_rook_attacking_square(d7, WHITE, brd));
	assert_true(is_rook_attacking_square(b6, WHITE, brd));
	assert_true(is_rook_attacking_square(d4, WHITE, brd));
	assert_true(is_rook_attacking_square(f5, BLACK, brd));
	assert_true(is_rook_attacking_square(h6, BLACK, brd));
	assert_true(is_rook_attacking_square(f2, BLACK, brd));
	assert_true(is_rook_attacking_square(h1, BLACK, brd));
	
	
	
}


void test_is_square_being_attacked_by_bishop(void){

	// e7 is attacked by WB
	// d4 is attacked by WB
	// f5 is attacked by WB
	// e2 is attacked by WB
	// b6 is attacked by WB
	// d5 is attacked by BB
	char * test_fen = "6rR/P3b2P/1p2bP2/p1BPkp1p/1P1r1pPp/3B4/PK1ppR1P/8 w - - 0 1";
	
	BOARD * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_bishop_attacking_square(e7, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(d4, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(f5, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(e2, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(b6, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(d5, BLACK, brd)); 
	
	
	
	// f6 attacked by WB
	// g4 attacked by WB
	// e4 attacked by WB
	// f5 attacked by BB
	// c3 attacked by BB
	test_fen = "8/2p1B1P1/p4p2/P1KpPBp1/2Ppp1bP/1PP5/kb3Pp1/8 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_bishop_attacking_square(f6, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(g4, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(e4, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(f5, BLACK, brd)); 
	assert_true(is_bishop_attacking_square(c3, BLACK, brd));

	test_fen = "3RN1n1/pp1bQrQP/1pPb1NPq/p1P1p2q/2R1pBPk/5p1P/2K1p2P/1n1B4 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_bishop_attacking_square(e2, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(h6, WHITE, brd)); 
	assert_true(is_bishop_attacking_square(e5, WHITE, brd)); 
	
	assert_true(is_bishop_attacking_square(c5, BLACK, brd)); 
	assert_true(is_bishop_attacking_square(e7, BLACK, brd)); 
	assert_true(is_bishop_attacking_square(c5, BLACK, brd)); 
	assert_true(is_bishop_attacking_square(e8, BLACK, brd)); 
	assert_true(is_bishop_attacking_square(g4, BLACK, brd)); 
	

		
}


void test_is_blocked_up_or_down(void){
	
	// Rd2 blocked on d3 for attack on d5
	// Qf1 blocked on f4 for attack on f6
	// Qg3 blocked on d3 for attack on b3
	char * test_fen = "1b4b1/7k/1Q3R2/1RQ4B/K4B2/Nn1n2qN/1r4r1/5q2 w - - 0 1";
	
	BOARD * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_horizontal_or_vertical_blocked(g3, b3, brd)); 
	assert_true(is_horizontal_or_vertical_blocked(f1, f6, brd)); 
	assert_true(is_horizontal_or_vertical_blocked(d2, d5, brd)); 
	

	// Qh1 blocked for attack on d1
	// Qd6 blocked for attack on d3
	test_fen = "2k5/5qb1/3qn1rN/3B3R/8/3RK2Q/1r5N/2nbB2Q w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_horizontal_or_vertical_blocked(h1, d1, brd)); 
	assert_true(is_horizontal_or_vertical_blocked(d6, d3, brd)); 

	
}


void test_is_blocked_diagonally(void){
		
	// Qf7 blocked for attack on d5
	// Qh3 blocked for attack on c8
	// Bd5 blocked for attack on f7
	char * test_fen = "2k5/5qb1/3qn1rN/3B3R/8/3RK2Q/1r5N/2nbB2Q w - - 0 1";
	
	BOARD * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_diagonally_blocked(f7, d5, brd)); 
	assert_true(is_diagonally_blocked(h3, c8, brd)); 
	assert_true(is_diagonally_blocked(d5, f7, brd)); 


	// Qh3 blocked for attacks on e6, d7, c8
	// Bb2 blocked for attacks on d4, e5, f6, g7, h8
	test_fen = "3q4/1R5N/3k3q/4Rb2/1r6/2B3KQ/Qb5n/1Nn1r2B w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_diagonally_blocked(h3, e6, brd)); 
	assert_true(is_diagonally_blocked(h3, d7, brd)); 
	assert_true(is_diagonally_blocked(h3, c8, brd)); 
	assert_true(is_diagonally_blocked(b2, d4, brd)); 
	assert_true(is_diagonally_blocked(b2, e5, brd)); 
	assert_true(is_diagonally_blocked(b2, f6, brd)); 
	assert_true(is_diagonally_blocked(b2, g7, brd)); 
	assert_true(is_diagonally_blocked(b2, h8, brd)); 


	// Qb1 blocked for attacks on f5, g6, h7
	// Bb3 blocked for attacks on d5, e6, f7, g8
	// Bc4 blocked for attacks on a2
	// Qe2 blocked for attacks on b5, a6
	test_fen = "7K/2Bn4/2Q1nQ2/8/r1B1RR2/bb5k/4q1N1/Nq4r1 w - - 0 1";
	
	brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_diagonally_blocked(b1, f5, brd));	
	assert_true(is_diagonally_blocked(b1, g6, brd)); 
	assert_true(is_diagonally_blocked(b1, h7, brd)); 
	assert_true(is_diagonally_blocked(b3, d5, brd));	
 	assert_true(is_diagonally_blocked(b3, e6, brd)); 
	assert_true(is_diagonally_blocked(b3, f7, brd)); 
	assert_true(is_diagonally_blocked(b3, g8, brd)); 
	assert_true(is_diagonally_blocked(c4, a2, brd)); 
	assert_true(is_diagonally_blocked(e2, b5, brd)); 
	assert_true(is_diagonally_blocked(e2, a6, brd)); 

}


void test_is_square_under_attack(void){
	char * test_fen = "2b5/PP2K3/pP1n2R1/2P3P1/2r1p1N1/2pPn3/B1pp1Pkq/4QqB1 w - - 0 1";
	
	BOARD * brd = get_clean_board();
	consume_fen_notation(test_fen, brd);
	//print_board(brd);
	assert_true(is_sq_attacked(b7, BLACK, brd)); 
	assert_true(is_sq_attacked(c5, BLACK, brd)); 
	assert_true(is_sq_attacked(d3, BLACK, brd)); 
	assert_true(is_sq_attacked(e1, BLACK, brd)); 
	assert_true(is_sq_attacked(g1, BLACK, brd)); 
	assert_true(is_sq_attacked(f2, BLACK, brd)); 
	assert_true(is_sq_attacked(g4, BLACK, brd)); 

	assert_true(is_sq_attacked(c8, WHITE, brd)); 
	assert_true(is_sq_attacked(d6, WHITE, brd)); 
	assert_true(is_sq_attacked(e4, WHITE, brd)); 
	assert_true(is_sq_attacked(c4, WHITE, brd)); 
	assert_true(is_sq_attacked(e3, WHITE, brd)); 
	assert_true(is_sq_attacked(h2, WHITE, brd)); 

	
}


void attack_test_fixture(void)
{
	test_fixture_start();	// starts a fixture

	run_test(test_is_square_being_attacked_by_knight);
	run_test(test_is_square_being_attacked_by_pawn);
	run_test(test_is_square_being_attacked_by_king);
	run_test(test_is_square_being_attacked_by_rook);
	run_test(test_is_square_being_attacked_by_bishop);
	run_test(test_is_square_attacked_by_queen);
    
	run_test(test_is_blocked_up_or_down);
	run_test(test_is_blocked_diagonally);
    
	run_test(test_is_square_under_attack);
       
    
	test_fixture_end();		// ends a fixture
}

