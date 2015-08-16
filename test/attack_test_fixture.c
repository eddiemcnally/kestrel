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
#include "init.h"
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

void test_is_square_being_attacked_by_knight(void)
{
    // knight attacks e5
    char *test_fen =
	"rnbqkbnr/pppp1ppp/8/4p3/8/5N2/PPPPPPPP/RNBQKB1R w KQkq e6 0 2";

    struct board *brd = init_game(test_fen);

    //print_board(brd);
    assert_true(TEST_is_knight_attacking_square(brd, e5, WHITE));

    // - white Knight attacks d5
    // - black knight attacks d4
    // - white knight attacks e5
    // - white knight attacks g5
    // - black knight attacks e4
    // - black knight attacks g4
    test_fen =
	"r2qkb1r/pppb1p1p/2n2n2/3pp1p1/3PP1Q1/BPN2N2/P1P2PPP/R3KB1R b KQkq - 4 7";
    brd = init_game(test_fen);

    //print_board(brd);
    assert_true(TEST_is_knight_attacking_square(brd, d5, WHITE));
    assert_true(TEST_is_knight_attacking_square(brd, d4, BLACK));
    assert_true(TEST_is_knight_attacking_square(brd, e5, WHITE));
    assert_true(TEST_is_knight_attacking_square(brd, g5, WHITE));
    assert_true(TEST_is_knight_attacking_square(brd, e4, BLACK));
    assert_true(TEST_is_knight_attacking_square(brd, g4, BLACK));

    // do some negative tests
    assert_false(TEST_is_knight_attacking_square(brd, a3, WHITE));
    assert_false(TEST_is_knight_attacking_square(brd, h4, BLACK));
    assert_false(TEST_is_knight_attacking_square(brd, g7, BLACK));
    assert_false(TEST_is_knight_attacking_square(brd, b2, BLACK));
    assert_false(TEST_is_knight_attacking_square(brd, a1, BLACK));
    assert_false(TEST_is_knight_attacking_square(brd, c2, BLACK));

    // - white Knight attacks c6
    // - white knight attacks d6
    test_fen = "8/NPK2NPP/P1pppP2/3P4/k5p1/2p5/6n1/8 w - - 0 1";
    brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_knight_attacking_square(brd, c6, WHITE));
    assert_true(TEST_is_knight_attacking_square(brd, d6, WHITE));

    // WN attacks f6, g7, f4
    // BN attacks f7, b5, e4
    test_fen =
	"B1kn3Q/R1p2Rpq/2p1pqN1/1PK2p1N/1p2PpP1/1Pn1Pr1b/1pr2PP1/4BQ2 w - - 0 1";
    brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_knight_attacking_square(brd, f6, WHITE));
    assert_true(TEST_is_knight_attacking_square(brd, g7, WHITE));
    assert_true(TEST_is_knight_attacking_square(brd, f4, WHITE));

    assert_true(TEST_is_knight_attacking_square(brd, f7, BLACK));
    assert_true(TEST_is_knight_attacking_square(brd, b5, BLACK));
    assert_true(TEST_is_knight_attacking_square(brd, e4, BLACK));

}

void test_is_square_being_attacked_by_pawn(void)
{
    // black pawn attacks c4
    // black pawn attacks d4
    // black pawn attacks e4
    // black pawn attacks g4
    // black pawn attacks h4
    // white pawn attacks e5
    // white pawn attacks d5
    // white pawn attacks g5
    char *test_fen_1 =
	"r2qkb1r/p1pb1p2/2n2n2/1p1pp1pp/2BPP1QP/BPN2N2/P1P2PP1/R3K2R b KQkq - 1 9";

    struct board *brd = init_game(test_fen_1);

    //print_board(brd);
    assert_true(TEST_is_pawn_attacking_square(brd, c4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(brd, d4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(brd, e4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(brd, g4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(brd, h4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(brd, e5, WHITE));
    assert_true(TEST_is_pawn_attacking_square(brd, d5, WHITE));
    assert_true(TEST_is_pawn_attacking_square(brd, g5, WHITE));

    test_fen_1 =
	"1nr3n1/ppqNkp2/1R3p2/bQ4PK/1PpRp1PP/P2bBPqp/4P1r1/1B1Q1N2 w - - 0 1";

    brd = init_game(test_fen_1);
    //print_board(brd);
    assert_true(TEST_is_pawn_attacking_square(brd, b6, BLACK));
    assert_true(TEST_is_pawn_attacking_square(brd, f3, BLACK));
    assert_true(TEST_is_pawn_attacking_square(brd, a5, WHITE));
    assert_true(TEST_is_pawn_attacking_square(brd, d3, WHITE));
    assert_true(TEST_is_pawn_attacking_square(brd, f6, WHITE));
    assert_true(TEST_is_pawn_attacking_square(brd, d3, WHITE));
    assert_true(TEST_is_pawn_attacking_square(brd, e4, WHITE));

}

void test_is_square_being_attacked_by_king(void)
{
    // black king attacks c5
    // white king attacks f2
    char *test_fen = "8/p3p1P1/2p5/pkP5/8/1p1pP1P1/pP1P1pK1/8 w - - 0 1";

    struct board *brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_king_attacking_square(brd, c5, BLACK));
    assert_true(TEST_is_king_attacking_square(brd, f2, WHITE));

    // black king attacks h6
    // white king attacks b7
    test_fen = "8/1p2pp1k/1KP4P/3PP3/4p3/2pp2P1/P2p2P1/8 w - - 0 1";

	brd = init_game(test_fen);
	//print_board(brd);
    assert_true(TEST_is_king_attacking_square(brd, h6, BLACK));
    assert_true(TEST_is_king_attacking_square(brd, b7, WHITE));

    // black king attacks g2
    // white king attacks h7
    test_fen = "8/p5Pp/PP5K/p4p2/P4ppP/3p4/3p2Pk/8 w - - 0 1";

    brd = init_game(test_fen);
	//print_board(brd);
    assert_true(TEST_is_king_attacking_square(brd, g2, BLACK));
    assert_true(TEST_is_king_attacking_square(brd, h7, WHITE));

    // black king attacks g7
    // white king attacks c7
    test_fen = "3K2k1/2p3P1/Pp5p/P3P2P/P7/4p1p1/P1p1P3/8 w - - 0 1";

    brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_king_attacking_square(brd, g7, BLACK));
    assert_true(TEST_is_king_attacking_square(brd, c7, WHITE));

}

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

void test_is_square_attacked_by_queen(void)
{

    // c6 attacked by BQ
    // d3 attacked by WQ
    // e7 attacked by WQ
    // d3 attacked by WQ (2nd WQ)
    // f4 attacked by WQ
    // h5 attacked by WQ
    // f7 attacked by BQ
    char *test_fen =
	"1n6/2pqpPq1/2Pp2pk/2NP3p/2K2p1p/P1pn1Q2/2PPQPP1/7N w - - 0 1";

    struct board *brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_queen_attacking_square(brd, c6, BLACK));
    assert_true(TEST_is_queen_attacking_square(brd, d3, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, e7, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, d3, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, f4, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, h5, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, f7, BLACK));

    test_fen =
	"R1q2B2/KpNp1P1P/3p2pP/NPk1pQ2/PpP3np/2b3R1/3n2BP/rb1qQ1r1 w - - 0 1";

    brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_queen_attacking_square(brd, a8, BLACK));
    assert_true(TEST_is_queen_attacking_square(brd, f8, BLACK));
    assert_true(TEST_is_queen_attacking_square(brd, c7, BLACK));
    assert_true(TEST_is_queen_attacking_square(brd, e1, BLACK));
    assert_true(TEST_is_queen_attacking_square(brd, a4, BLACK));

    assert_true(TEST_is_queen_attacking_square(brd, d1, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, d2, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, g1, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, e5, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, g4, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, g6, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, e5, WHITE));
    assert_true(TEST_is_queen_attacking_square(brd, d7, WHITE));

}

void test_is_square_being_attacked_by_rook(void)
{

    // c8 attacked by WR
    // a6 attacked by BR
    // g6 attacked by BR
    // d2 attacked by BR
    // c2 attacked by WR
    char *test_fen =
	"R1qq4/P1k2p1P/Qr4QP/2p2PP1/2prp1p1/2p4p/1PpPPK2/2R5 w - - 0 1";

    struct board *brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_rook_attacking_square(brd, c8, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, a6, BLACK));
    assert_true(TEST_is_rook_attacking_square(brd, g6, BLACK));
    assert_true(TEST_is_rook_attacking_square(brd, d2, BLACK));
    assert_true(TEST_is_rook_attacking_square(brd, c2, WHITE));

    // d8 attacked by WR
    // f7 attacked by WR
    // a6 attacked by BR
    // e6 attacked by WR
    // e2 attacked by WR
    // e3 attacked by BR
    test_fen =
	"1Q1b1R2/1P1P1p2/Q2ppP1K/r1pp1b2/pP1P1Pp1/NkqPR3/N2pr2P/4q3 w - - 0 1";

    brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_rook_attacking_square(brd, d8, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, f7, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, a6, BLACK));
    assert_true(TEST_is_rook_attacking_square(brd, e6, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, e2, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, e3, BLACK));

    // a8 attacked by WR
    // d7 attacked by WR
    // b6 attacked by WR
    // d4 attacked by WR
    // f5 attacked by BR
    // h6 attacked by BR
    // f2 attacked by BR
    // h1 attacked by BR
    test_fen =
	"b1qkN3/R2pb1P1/1pp1pPpP/4QP1r/1R1p2P1/P2q1P2/p1pP1N1r/6KQ w - - 0 1";

    brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_rook_attacking_square(brd, a8, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, d7, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, b6, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, d4, WHITE));
    assert_true(TEST_is_rook_attacking_square(brd, f5, BLACK));
    assert_true(TEST_is_rook_attacking_square(brd, h6, BLACK));
    assert_true(TEST_is_rook_attacking_square(brd, f2, BLACK));
    assert_true(TEST_is_rook_attacking_square(brd, h1, BLACK));

}

void test_is_square_being_attacked_by_bishop(void)
{

    // e7 is attacked by WB
    // d4 is attacked by WB
    // f5 is attacked by WB
    // e2 is attacked by WB
    // b6 is attacked by WB
    // d5 is attacked by BB
    char *test_fen =
	"6rR/P3b2P/1p2bP2/p1BPkp1p/1P1r1pPp/3B4/PK1ppR1P/8 w - - 0 1";

    struct board *brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_bishop_attacking_square(brd, e7, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, d4, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, f5, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, e2, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, b6, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, d5, BLACK));

    // f6 attacked by WB
    // g4 attacked by WB
    // e4 attacked by WB
    // f5 attacked by BB
    // c3 attacked by BB
    test_fen = "8/2p1B1P1/p4p2/P1KpPBp1/2Ppp1bP/1PP5/kb3Pp1/8 w - - 0 1";

    brd = init_game(test_fen);
    //print_board(brd);
    assert_true(TEST_is_bishop_attacking_square(brd, f6, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, g4, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, e4, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, f5, BLACK));
    assert_true(TEST_is_bishop_attacking_square(brd, c3, BLACK));

    test_fen =
	"3RN1n1/pp1bQrQP/1pPb1NPq/p1P1p2q/2R1pBPk/5p1P/2K1p2P/1n1B4 w - - 0 1";

    brd = init_game(test_fen);
	//print_board(brd);
    assert_true(TEST_is_bishop_attacking_square(brd, e2, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, h6, WHITE));
    assert_true(TEST_is_bishop_attacking_square(brd, e5, WHITE));

    assert_true(TEST_is_bishop_attacking_square(brd, c5, BLACK));
    assert_true(TEST_is_bishop_attacking_square(brd, e7, BLACK));
    assert_true(TEST_is_bishop_attacking_square(brd, c5, BLACK));
    assert_true(TEST_is_bishop_attacking_square(brd, e8, BLACK));
    assert_true(TEST_is_bishop_attacking_square(brd, g4, BLACK));

}

void test_is_square_under_attack(void)
{
    char *test_fen =
	"2Q3qb/pN3P1p/P4qr1/1KP1BnP1/1p2pPNp/2rkP3/pP3npP/4QbRB w - - 0 1";

    struct board *brd = init_game(test_fen);
    //print_board(brd);

	assert_true(is_sq_attacked(brd, a1, WHITE) == true);
	assert_true(is_sq_attacked(brd, a1, BLACK) == false);
	assert_true(is_sq_attacked(brd, a2, WHITE) == false);
	assert_true(is_sq_attacked(brd, a2, BLACK) == false);
	assert_true(is_sq_attacked(brd, a3, WHITE) == true);
	assert_true(is_sq_attacked(brd, a3, BLACK) == true);
	assert_true(is_sq_attacked(brd, a4, WHITE) == true);
	assert_true(is_sq_attacked(brd, a4, BLACK) == false);
	assert_true(is_sq_attacked(brd, a5, WHITE) == true);
	assert_true(is_sq_attacked(brd, a5, BLACK) == false);
	assert_true(is_sq_attacked(brd, a6, WHITE) == true);
	assert_true(is_sq_attacked(brd, a6, BLACK) == true);
	assert_true(is_sq_attacked(brd, a7, WHITE) == false);
	assert_true(is_sq_attacked(brd, a7, BLACK) == false);
	assert_true(is_sq_attacked(brd, a8, WHITE) == true);
	assert_true(is_sq_attacked(brd, a8, BLACK) == false);


	assert_true(is_sq_attacked(brd, b1, WHITE) == true);
	assert_true(is_sq_attacked(brd, b1, BLACK) == true);
	assert_true(is_sq_attacked(brd, b2, WHITE) == false);
	assert_true(is_sq_attacked(brd, b2, BLACK) == false);
	assert_true(is_sq_attacked(brd, b3, WHITE) == false);
	assert_true(is_sq_attacked(brd, b3, BLACK) == true);
	assert_true(is_sq_attacked(brd, b4, WHITE) == true);
	assert_true(is_sq_attacked(brd, b4, BLACK) == false);
	assert_true(is_sq_attacked(brd, b5, WHITE) == false);
	assert_true(is_sq_attacked(brd, b5, BLACK) == false);
	assert_true(is_sq_attacked(brd, b6, WHITE) == true);
	assert_true(is_sq_attacked(brd, b6, BLACK) == true);
	assert_true(is_sq_attacked(brd, b7, WHITE) == true);
	assert_true(is_sq_attacked(brd, b7, BLACK) == false);
	assert_true(is_sq_attacked(brd, b8, WHITE) == true);
	assert_true(is_sq_attacked(brd, b8, BLACK) == false);


	assert_true(is_sq_attacked(brd, c1, WHITE) == true);
	assert_true(is_sq_attacked(brd, c1, BLACK) == true);
	assert_true(is_sq_attacked(brd, c2, WHITE) == false);
	assert_true(is_sq_attacked(brd, c2, BLACK) == true);
	assert_true(is_sq_attacked(brd, c3, WHITE) == true);
	assert_true(is_sq_attacked(brd, c3, BLACK) == true);
	assert_true(is_sq_attacked(brd, c4, WHITE) == true);
	assert_true(is_sq_attacked(brd, c4, BLACK) == true);
	assert_true(is_sq_attacked(brd, c5, WHITE) == true);
	assert_true(is_sq_attacked(brd, c5, BLACK) == true);
	assert_true(is_sq_attacked(brd, c6, WHITE) == true);
	assert_true(is_sq_attacked(brd, c6, BLACK) == true);
	assert_true(is_sq_attacked(brd, c7, WHITE) == true);
	assert_true(is_sq_attacked(brd, c7, BLACK) == false);
	assert_true(is_sq_attacked(brd, c8, WHITE) == false);
	assert_true(is_sq_attacked(brd, c8, BLACK) == true);


	assert_true(is_sq_attacked(brd, d1, WHITE) == true);
	assert_true(is_sq_attacked(brd, d1, BLACK) == true);
	assert_true(is_sq_attacked(brd, d2, WHITE) == true);
	assert_true(is_sq_attacked(brd, d2, BLACK) == true);
	assert_true(is_sq_attacked(brd, d3, WHITE) == false);
	assert_true(is_sq_attacked(brd, d3, BLACK) == true);
	assert_true(is_sq_attacked(brd, d4, WHITE) == true);
	assert_true(is_sq_attacked(brd, d4, BLACK) == true);
	assert_true(is_sq_attacked(brd, d5, WHITE) == false);
	assert_true(is_sq_attacked(brd, d5, BLACK) == false);
	assert_true(is_sq_attacked(brd, d6, WHITE) == true);
	assert_true(is_sq_attacked(brd, d6, BLACK) == true);
	assert_true(is_sq_attacked(brd, d7, WHITE) == true);
	assert_true(is_sq_attacked(brd, d7, BLACK) == false);
	assert_true(is_sq_attacked(brd, d8, WHITE) == true);
	assert_true(is_sq_attacked(brd, d8, BLACK) == true);

	assert_true(is_sq_attacked(brd, e1, WHITE) == false);
	assert_true(is_sq_attacked(brd, e1, BLACK) == false);
	assert_true(is_sq_attacked(brd, e2, WHITE) == true);
	assert_true(is_sq_attacked(brd, e2, BLACK) == true);
	assert_true(is_sq_attacked(brd, e3, WHITE) == true);
	assert_true(is_sq_attacked(brd, e3, BLACK) == true);
	assert_true(is_sq_attacked(brd, e4, WHITE) == false);
	assert_true(is_sq_attacked(brd, e4, BLACK) == true);
	assert_true(is_sq_attacked(brd, e5, WHITE) == true);
	assert_true(is_sq_attacked(brd, e5, BLACK) == true);
	assert_true(is_sq_attacked(brd, e6, WHITE) == true);
	assert_true(is_sq_attacked(brd, e6, BLACK) == true);
	assert_true(is_sq_attacked(brd, e7, WHITE) == false);
	assert_true(is_sq_attacked(brd, e7, BLACK) == true);
	assert_true(is_sq_attacked(brd, e8, WHITE) == true);
	assert_true(is_sq_attacked(brd, e8, BLACK) == true);



	assert_true(is_sq_attacked(brd, f1, WHITE) == true);
	assert_true(is_sq_attacked(brd, f1, BLACK) == true);
	assert_true(is_sq_attacked(brd, f2, WHITE) == true);
	assert_true(is_sq_attacked(brd, f2, BLACK) == false);
	assert_true(is_sq_attacked(brd, f3, WHITE) == false);
	assert_true(is_sq_attacked(brd, f3, BLACK) == true);
	assert_true(is_sq_attacked(brd, f4, WHITE) == true);
	assert_true(is_sq_attacked(brd, f4, BLACK) == false);
	assert_true(is_sq_attacked(brd, f5, WHITE) == true);
	assert_true(is_sq_attacked(brd, f5, BLACK) == true);
	assert_true(is_sq_attacked(brd, f6, WHITE) == true);
	assert_true(is_sq_attacked(brd, f6, BLACK) == true);
	assert_true(is_sq_attacked(brd, f7, WHITE) == false);
	assert_true(is_sq_attacked(brd, f7, BLACK) == true);
	assert_true(is_sq_attacked(brd, f8, WHITE) == true);
	assert_true(is_sq_attacked(brd, f8, BLACK) == true);



	assert_true(is_sq_attacked(brd, g1, WHITE) == false);
	assert_true(is_sq_attacked(brd, g1, BLACK) == false);
	assert_true(is_sq_attacked(brd, g2, WHITE) == true);
	assert_true(is_sq_attacked(brd, g2, BLACK) == true);
	assert_true(is_sq_attacked(brd, g3, WHITE) == true);
	assert_true(is_sq_attacked(brd, g3, BLACK) == true);
	assert_true(is_sq_attacked(brd, g4, WHITE) == false);
	assert_true(is_sq_attacked(brd, g4, BLACK) == true);
	assert_true(is_sq_attacked(brd, g5, WHITE) == true);
	assert_true(is_sq_attacked(brd, g5, BLACK) == true);
	assert_true(is_sq_attacked(brd, g6, WHITE) == false);
	assert_true(is_sq_attacked(brd, g6, BLACK) == true);
	assert_true(is_sq_attacked(brd, g7, WHITE) == false);
	assert_true(is_sq_attacked(brd, g7, BLACK) == true);
	assert_true(is_sq_attacked(brd, g8, WHITE) == true);
	assert_true(is_sq_attacked(brd, g8, BLACK) == true);




	assert_true(is_sq_attacked(brd, h1, WHITE) == true);
	assert_true(is_sq_attacked(brd, h1, BLACK) == true);
	assert_true(is_sq_attacked(brd, h2, WHITE) == true);
	assert_true(is_sq_attacked(brd, h2, BLACK) == false);
	assert_true(is_sq_attacked(brd, h3, WHITE) == false);
	assert_true(is_sq_attacked(brd, h3, BLACK) == true);
	assert_true(is_sq_attacked(brd, h4, WHITE) == false);
	assert_true(is_sq_attacked(brd, h4, BLACK) == true);
	assert_true(is_sq_attacked(brd, h5, WHITE) == false);
	assert_true(is_sq_attacked(brd, h5, BLACK) == false);
	assert_true(is_sq_attacked(brd, h6, WHITE) == true);
	assert_true(is_sq_attacked(brd, h6, BLACK) == true);
	assert_true(is_sq_attacked(brd, h7, WHITE) == false);
	assert_true(is_sq_attacked(brd, h7, BLACK) == true);
	assert_true(is_sq_attacked(brd, h8, WHITE) == false);
	assert_true(is_sq_attacked(brd, h8, BLACK) == true);



///////////////////////////////////
    test_fen = "8/1pppp2k/6p1/5p2/8/3B4/8/4K3 w - - 0 1";

    brd = init_game(test_fen);
	assert_false(is_sq_attacked(brd, h7, WHITE));


///////////////////////////////////////
    test_fen = "rnbq1bnr/ppppp1pp/6k1/5p2/4P3/3B4/PPPP2PP/RNBQK1NR w KQ - 0 1";

    brd = init_game(test_fen);
	//print_board(brd);
	assert_false(is_sq_attacked(brd, g6, WHITE));


///////////////////////////////////////////////
    test_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN2Q/1p2P3/2N4p/PPPBBPPP/R3K2R w KQkq - 0 1";


//8    r  -  -  -  k  -  -  r
//7    p  -  p  p  q  p  b  -
//6    b  n  -  -  p  n  p  -
//5    -  -  -  P  N  -  -  Q
//4    -  p  -  -  P  -  -  -
//3    -  -  N  -  -  -  -  p
//2    P  P  P  B  B  P  P  P
//1    R  -  -  -  K  -  -  R

     //a  b  c  d  e  f  g  h

    brd = init_game(test_fen);
	//print_board(brd);
	assert_false(is_sq_attacked(brd, e8, WHITE));
	assert_false(is_sq_attacked(brd, a1, WHITE));

	assert_true(is_sq_attacked(brd, h8, WHITE));
	assert_true(is_sq_attacked(brd, g6, WHITE));

}

void attack_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    //run_test(test_is_square_being_attacked_by_knight);
    //run_test(test_is_square_being_attacked_by_pawn);
    //run_test(test_is_square_being_attacked_by_king);
    //run_test(test_is_square_being_attacked_by_rook);
    //run_test(test_is_square_being_attacked_by_bishop);
    //run_test(test_is_square_attacked_by_queen);

    run_test(test_is_square_under_attack);

    test_fixture_end();		// ends a fixture
}
