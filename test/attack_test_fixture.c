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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "seatest.h"
#include "kestrel.h"
#include "attack.h"
#include "fen/fen.h"
#include "board.h"
#include "bitboard.h"
#include "pieces.h"
#include "board_utils.h"
#include "move_gen.h"
#include "occupancy_mask.h"
#include "move_gen_utils.h"

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
void test_inbetween_bits(void);
void debug_move(void);
bool TEST_is_bishop_attacking_square(const struct position *pos,
                                     enum square sq,
                                     enum colour attacking_side);
bool TEST_is_knight_attacking_square(const struct position *pos,
                                     enum square sq,
                                     enum colour attacking_side);
bool TEST_is_pawn_attacking_square(const struct position *pos, enum square sq,
                                   enum colour attacking_side);
bool TEST_is_king_attacking_square(const struct position *pos, enum square sq,
                                   enum colour attacking_side);
bool TEST_is_rook_attacking_square(const struct position *pos, enum square sq,
                                   enum colour attacking_side);
bool TEST_is_queen_attacking_square(const struct position *pos,
                                    enum square sq, enum colour attacking_side);
bool TEST_is_attacked_horizontally_or_vertically(const struct position *pos,
        enum square sq_one,
        enum square sq_two);
bool TEST_is_attacked_diagonally(const struct position *pos,
                                 enum square sq_one, enum square sq_two);





void test_is_square_being_attacked_by_knight(void)
{
    // knight attacks e5
    char *test_fen =
        "rnbqkbnr/pppp1ppp/8/4p3/8/5N2/PPPPPPPP/RNBQKB1R w KQkq e6 0 2";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_true(TEST_is_knight_attacking_square(pos, e5, WHITE));

    // - white Knight attacks d5
    // - black knight attacks d4
    // - white knight attacks e5
    // - white knight attacks g5
    // - black knight attacks e4
    // - black knight attacks g4
    free_board(pos);
    test_fen =
        "r2qkb1r/pppb1p1p/2n2n2/3pp1p1/3PP1Q1/BPN2N2/P1P2PPP/R3KB1R b KQkq - 4 7";

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_true(TEST_is_knight_attacking_square(pos, d5, WHITE));
    assert_true(TEST_is_knight_attacking_square(pos, d4, BLACK));
    assert_true(TEST_is_knight_attacking_square(pos, e5, WHITE));
    assert_true(TEST_is_knight_attacking_square(pos, g5, WHITE));
    assert_true(TEST_is_knight_attacking_square(pos, e4, BLACK));
    assert_true(TEST_is_knight_attacking_square(pos, g4, BLACK));

    // do some negative tests
    assert_false(TEST_is_knight_attacking_square(pos, a3, WHITE));
    assert_false(TEST_is_knight_attacking_square(pos, h4, BLACK));
    assert_false(TEST_is_knight_attacking_square(pos, g7, BLACK));
    assert_false(TEST_is_knight_attacking_square(pos, b2, BLACK));
    assert_false(TEST_is_knight_attacking_square(pos, a1, BLACK));
    assert_false(TEST_is_knight_attacking_square(pos, c2, BLACK));

	free_board(pos);

    // - white Knight attacks c6
    // - white knight attacks d6

    test_fen = "8/NPK2NPP/P1pppP2/3P4/k5p1/2p5/6n1/8 w - - 0 1";

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_true(TEST_is_knight_attacking_square(pos, c6, WHITE));
    assert_true(TEST_is_knight_attacking_square(pos, d6, WHITE));
	free_board(pos);

    // WN attacks f6, g7, f4
    // BN attacks f7, b5, e4
    test_fen =
        "B1kn3Q/R1p2Rpq/2p1pqN1/1PK2p1N/1p2PpP1/1Pn1Pr1b/1pr2PP1/4BQ2 w - - 0 1";

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_true(TEST_is_knight_attacking_square(pos, f6, WHITE));
    assert_true(TEST_is_knight_attacking_square(pos, g7, WHITE));
    assert_true(TEST_is_knight_attacking_square(pos, f4, WHITE));

    assert_true(TEST_is_knight_attacking_square(pos, f7, BLACK));
    assert_true(TEST_is_knight_attacking_square(pos, b5, BLACK));
    assert_true(TEST_is_knight_attacking_square(pos, e4, BLACK));
	free_board(pos);
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

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen_1, pos);

    //print_board(pos);
    assert_true(TEST_is_pawn_attacking_square(pos, c4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(pos, d4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(pos, e4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(pos, g4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(pos, h4, BLACK));
    assert_true(TEST_is_pawn_attacking_square(pos, e5, WHITE));
    assert_true(TEST_is_pawn_attacking_square(pos, d5, WHITE));
    assert_true(TEST_is_pawn_attacking_square(pos, g5, WHITE));
	free_board(pos);

    test_fen_1 =
        "1nr3n1/ppqNkp2/1R3p2/bQ4PK/1PpRp1PP/P2bBPqp/4P1r1/1B1Q1N2 w - - 0 1";
    pos = allocate_board();
    consume_fen_notation(test_fen_1, pos);

	//print_board(pos);
    assert_true(TEST_is_pawn_attacking_square(pos, b6, BLACK));
    assert_true(TEST_is_pawn_attacking_square(pos, f3, BLACK));
    assert_true(TEST_is_pawn_attacking_square(pos, a5, WHITE));
    assert_true(TEST_is_pawn_attacking_square(pos, d3, WHITE));
    assert_true(TEST_is_pawn_attacking_square(pos, f6, WHITE));
    assert_true(TEST_is_pawn_attacking_square(pos, d3, WHITE));
    assert_true(TEST_is_pawn_attacking_square(pos, e4, WHITE));
	free_board(pos);
}

void test_is_square_being_attacked_by_king(void)
{
    // black king attacks c5
    // white king attacks f2
    char *test_fen = "8/p3p1P1/2p5/pkP5/8/1p1pP1P1/pP1P1pK1/8 w - - 0 1";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_true(TEST_is_king_attacking_square(pos, c5, BLACK));
    assert_true(TEST_is_king_attacking_square(pos, f2, WHITE));
	free_board(pos);

    // black king attacks h6
    // white king attacks b7
    test_fen = "8/1p2pp1k/1KP4P/3PP3/4p3/2pp2P1/P2p2P1/8 w - - 0 1";

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_true(TEST_is_king_attacking_square(pos, h6, BLACK));
    assert_true(TEST_is_king_attacking_square(pos, b7, WHITE));
	free_board(pos);
    // black king attacks g2
    // white king attacks h7
    test_fen = "8/p5Pp/PP5K/p4p2/P4ppP/3p4/3p2Pk/8 w - - 0 1";

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_true(TEST_is_king_attacking_square(pos, g2, BLACK));
    assert_true(TEST_is_king_attacking_square(pos, h7, WHITE));

	free_board(pos);
    // black king attacks g7
    // white king attacks c7
    test_fen = "3K2k1/2p3P1/Pp5p/P3P2P/P7/4p1p1/P1p1P3/8 w - - 0 1";

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_true(TEST_is_king_attacking_square(pos, g7, BLACK));
    assert_true(TEST_is_king_attacking_square(pos, c7, WHITE));

	free_board(pos);
}

void test_is_square_under_attack(void)
{
    char *test_fen =
        "2Q3qb/pN3P1p/P4qr1/1KP1BnP1/1p2pPNp/2rkP3/pP3npP/4QbRB w - - 0 1";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);

    assert_true(is_sq_attacked(pos, a1, WHITE) == true);
    assert_true(is_sq_attacked(pos, a1, BLACK) == false);
    assert_true(is_sq_attacked(pos, a2, WHITE) == false);
    assert_true(is_sq_attacked(pos, a2, BLACK) == false);
    assert_true(is_sq_attacked(pos, a3, WHITE) == true);
    assert_true(is_sq_attacked(pos, a3, BLACK) == true);
    assert_true(is_sq_attacked(pos, a4, WHITE) == true);
    assert_true(is_sq_attacked(pos, a4, BLACK) == false);
    assert_true(is_sq_attacked(pos, a5, WHITE) == true);
    assert_true(is_sq_attacked(pos, a5, BLACK) == false);
    assert_true(is_sq_attacked(pos, a6, WHITE) == true);
    assert_true(is_sq_attacked(pos, a6, BLACK) == true);
    assert_true(is_sq_attacked(pos, a7, WHITE) == false);
    assert_true(is_sq_attacked(pos, a7, BLACK) == false);
    assert_true(is_sq_attacked(pos, a8, WHITE) == true);
    assert_true(is_sq_attacked(pos, a8, BLACK) == false);

    assert_true(is_sq_attacked(pos, b1, WHITE) == true);
    assert_true(is_sq_attacked(pos, b1, BLACK) == true);
    assert_true(is_sq_attacked(pos, b2, WHITE) == false);
    assert_true(is_sq_attacked(pos, b2, BLACK) == false);
    assert_true(is_sq_attacked(pos, b3, WHITE) == false);
    assert_true(is_sq_attacked(pos, b3, BLACK) == true);
    assert_true(is_sq_attacked(pos, b4, WHITE) == true);
    assert_true(is_sq_attacked(pos, b4, BLACK) == false);
    assert_true(is_sq_attacked(pos, b5, WHITE) == false);
    assert_true(is_sq_attacked(pos, b5, BLACK) == false);
    assert_true(is_sq_attacked(pos, b6, WHITE) == true);
    assert_true(is_sq_attacked(pos, b6, BLACK) == true);
    assert_true(is_sq_attacked(pos, b7, WHITE) == true);
    assert_true(is_sq_attacked(pos, b7, BLACK) == false);
    assert_true(is_sq_attacked(pos, b8, WHITE) == true);
    assert_true(is_sq_attacked(pos, b8, BLACK) == false);

    assert_true(is_sq_attacked(pos, c1, WHITE) == true);
    assert_true(is_sq_attacked(pos, c1, BLACK) == true);
    assert_true(is_sq_attacked(pos, c2, WHITE) == false);
    assert_true(is_sq_attacked(pos, c2, BLACK) == true);
    assert_true(is_sq_attacked(pos, c3, WHITE) == true);
    assert_true(is_sq_attacked(pos, c3, BLACK) == true);
    assert_true(is_sq_attacked(pos, c4, WHITE) == true);
    assert_true(is_sq_attacked(pos, c4, BLACK) == true);
    assert_true(is_sq_attacked(pos, c5, WHITE) == true);
    assert_true(is_sq_attacked(pos, c5, BLACK) == true);
    assert_true(is_sq_attacked(pos, c6, WHITE) == true);
    assert_true(is_sq_attacked(pos, c6, BLACK) == true);
    assert_true(is_sq_attacked(pos, c7, WHITE) == true);
    assert_true(is_sq_attacked(pos, c7, BLACK) == false);
    assert_true(is_sq_attacked(pos, c8, WHITE) == false);
    assert_true(is_sq_attacked(pos, c8, BLACK) == true);

    assert_true(is_sq_attacked(pos, d1, WHITE) == true);
    assert_true(is_sq_attacked(pos, d1, BLACK) == true);
    assert_true(is_sq_attacked(pos, d2, WHITE) == true);
    assert_true(is_sq_attacked(pos, d2, BLACK) == true);
    assert_true(is_sq_attacked(pos, d3, WHITE) == false);
    assert_true(is_sq_attacked(pos, d3, BLACK) == true);
    assert_true(is_sq_attacked(pos, d4, WHITE) == true);
    assert_true(is_sq_attacked(pos, d4, BLACK) == true);
    assert_true(is_sq_attacked(pos, d5, WHITE) == false);
    assert_true(is_sq_attacked(pos, d5, BLACK) == false);
    assert_true(is_sq_attacked(pos, d6, WHITE) == true);
    assert_true(is_sq_attacked(pos, d6, BLACK) == true);
    assert_true(is_sq_attacked(pos, d7, WHITE) == true);
    assert_true(is_sq_attacked(pos, d7, BLACK) == false);
    assert_true(is_sq_attacked(pos, d8, WHITE) == true);
    assert_true(is_sq_attacked(pos, d8, BLACK) == true);

    assert_true(is_sq_attacked(pos, e1, WHITE) == false);
    assert_true(is_sq_attacked(pos, e1, BLACK) == false);
    assert_true(is_sq_attacked(pos, e2, WHITE) == true);
    assert_true(is_sq_attacked(pos, e2, BLACK) == true);
    assert_true(is_sq_attacked(pos, e3, WHITE) == true);
    assert_true(is_sq_attacked(pos, e3, BLACK) == true);
    assert_true(is_sq_attacked(pos, e4, WHITE) == false);
    assert_true(is_sq_attacked(pos, e4, BLACK) == true);
    assert_true(is_sq_attacked(pos, e5, WHITE) == true);
    assert_true(is_sq_attacked(pos, e5, BLACK) == true);
    assert_true(is_sq_attacked(pos, e6, WHITE) == true);
    assert_true(is_sq_attacked(pos, e6, BLACK) == true);
    assert_true(is_sq_attacked(pos, e7, WHITE) == false);
    assert_true(is_sq_attacked(pos, e7, BLACK) == true);
    assert_true(is_sq_attacked(pos, e8, WHITE) == true);
    assert_true(is_sq_attacked(pos, e8, BLACK) == true);

    assert_true(is_sq_attacked(pos, f1, WHITE) == true);
    assert_true(is_sq_attacked(pos, f1, BLACK) == true);
    assert_true(is_sq_attacked(pos, f2, WHITE) == true);
    assert_true(is_sq_attacked(pos, f2, BLACK) == false);
    assert_true(is_sq_attacked(pos, f3, WHITE) == false);
    assert_true(is_sq_attacked(pos, f3, BLACK) == true);
    assert_true(is_sq_attacked(pos, f4, WHITE) == true);
    assert_true(is_sq_attacked(pos, f4, BLACK) == false);
    assert_true(is_sq_attacked(pos, f5, WHITE) == true);
    assert_true(is_sq_attacked(pos, f5, BLACK) == true);
    assert_true(is_sq_attacked(pos, f6, WHITE) == true);
    assert_true(is_sq_attacked(pos, f6, BLACK) == true);
    assert_true(is_sq_attacked(pos, f7, WHITE) == false);
    assert_true(is_sq_attacked(pos, f7, BLACK) == true);
    assert_true(is_sq_attacked(pos, f8, WHITE) == true);
    assert_true(is_sq_attacked(pos, f8, BLACK) == true);

    assert_true(is_sq_attacked(pos, g1, WHITE) == false);
    assert_true(is_sq_attacked(pos, g1, BLACK) == false);
    assert_true(is_sq_attacked(pos, g2, WHITE) == true);
    assert_true(is_sq_attacked(pos, g2, BLACK) == true);
    assert_true(is_sq_attacked(pos, g3, WHITE) == true);
    assert_true(is_sq_attacked(pos, g3, BLACK) == true);
    assert_true(is_sq_attacked(pos, g4, WHITE) == false);
    assert_true(is_sq_attacked(pos, g4, BLACK) == true);
    assert_true(is_sq_attacked(pos, g5, WHITE) == true);
    assert_true(is_sq_attacked(pos, g5, BLACK) == true);
    assert_true(is_sq_attacked(pos, g6, WHITE) == false);
    assert_true(is_sq_attacked(pos, g6, BLACK) == true);
    assert_true(is_sq_attacked(pos, g7, WHITE) == false);
    assert_true(is_sq_attacked(pos, g7, BLACK) == true);
    assert_true(is_sq_attacked(pos, g8, WHITE) == true);
    assert_true(is_sq_attacked(pos, g8, BLACK) == true);

    assert_true(is_sq_attacked(pos, h1, WHITE) == true);
    assert_true(is_sq_attacked(pos, h1, BLACK) == true);
    assert_true(is_sq_attacked(pos, h2, WHITE) == true);
    assert_true(is_sq_attacked(pos, h2, BLACK) == false);
    assert_true(is_sq_attacked(pos, h3, WHITE) == false);
    assert_true(is_sq_attacked(pos, h3, BLACK) == true);
    assert_true(is_sq_attacked(pos, h4, WHITE) == false);
    assert_true(is_sq_attacked(pos, h4, BLACK) == true);
    assert_true(is_sq_attacked(pos, h5, WHITE) == false);
    assert_true(is_sq_attacked(pos, h5, BLACK) == false);
    assert_true(is_sq_attacked(pos, h6, WHITE) == true);
    assert_true(is_sq_attacked(pos, h6, BLACK) == true);
    assert_true(is_sq_attacked(pos, h7, WHITE) == false);
    assert_true(is_sq_attacked(pos, h7, BLACK) == true);
    assert_true(is_sq_attacked(pos, h8, WHITE) == false);
    assert_true(is_sq_attacked(pos, h8, BLACK) == true);

	free_board(pos);
///////////////////////////////////
    test_fen = "8/1pppp2k/6p1/5p2/8/3B4/8/4K3 w - - 0 1";

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    assert_false(is_sq_attacked(pos, h7, WHITE));
	free_board(pos);
///////////////////////////////////////
    test_fen =
        "rnbq1bnr/ppppp1pp/6k1/5p2/4P3/3B4/PPPP2PP/RNBQK1NR w KQ - 0 1";

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_false(is_sq_attacked(pos, g6, WHITE));

	free_board(pos);

///////////////////////////////////////////////
    test_fen =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN2Q/1p2P3/2N4p/PPPBBPPP/R3K2R w KQkq - 0 1";

//8    r  -  -  -  k  -  -  r
//7    p  -  p  p  q  p  b  -
//6    b  n  -  -  p  n  p  -
//5    -  -  -  P  N  -  -  Q
//4    -  p  -  -  P  -  -  -
//3    -  -  N  -  -  -  -  p
//2    P  P  P  B  B  P  P  P
//1    R  -  -  -  K  -  -  R

    //a  b  c  d  e  f  g  h

    pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);
    assert_false(is_sq_attacked(pos, e8, WHITE));
    assert_false(is_sq_attacked(pos, a1, WHITE));

    assert_true(is_sq_attacked(pos, h8, WHITE));
    assert_true(is_sq_attacked(pos, g6, WHITE));

	free_board(pos);
}

void debug_move(void)
{
    char *test_fen =
        "2Q3qb/pN3P1p/P4qr1/1KP1BnP1/1p2pPNp/2rkP3/pP3npP/4QbRB w - - 0 1";

    struct position *pos = allocate_board();
    consume_fen_notation(test_fen, pos);

    //print_board(pos);

    assert_true(is_sq_attacked(pos, e6, WHITE) == true);
    assert_true(is_sq_attacked(pos, g7, WHITE) == false);

	free_board(pos);

}



/**
 * Test wrapper functions.
 * These wrapper functions provide access for the unit test framework while
 * allowing the functions themselves to be static to this file and inline-able
 */
bool TEST_is_knight_attacking_square(const struct position *pos,
                                     enum square sq, enum colour attacking_side)
{
    enum piece pce;
    if (attacking_side == WHITE) {
        pce = W_KNIGHT;
    } else {
        pce = B_KNIGHT;
    }
    return is_knight_attacking_square(pos, square_to_bitboard(sq), pce);
}

bool TEST_is_pawn_attacking_square(const struct position *pos,
                                   enum square sq, enum colour attacking_side)
{
    if (attacking_side == WHITE) {
        if (is_pawn_controlling_sq(pos, WHITE, sq)) {
            return true;
        }
    } else {
        if (is_pawn_controlling_sq(pos, BLACK, sq)) {
            return true;
        }
    }
    return false;
}

bool TEST_is_king_attacking_square(const struct position *pos,
                                   enum square sq, enum colour attacking_side)
{
    return is_king_attacking_square(pos, square_to_bitboard(sq), attacking_side);
}

bool TEST_is_attacked_horizontally_or_vertically(const struct position *pos,
        enum square sq_one,
        enum square sq_two)
{
    return is_attacked_horizontally_or_vertically(pos, sq_one, sq_two);
}

bool TEST_is_attacked_diagonally(const struct position *pos,
                                 enum square sq_one, enum square sq_two)
{
    return is_attacked_diagonally(pos, sq_one, sq_two);
}



void attack_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    run_test(test_is_square_being_attacked_by_knight);
    run_test(test_is_square_being_attacked_by_pawn);
    run_test(test_is_square_being_attacked_by_king);
    run_test(test_is_square_under_attack);

    //run_test(debug_move);

    test_fixture_end();	// ends a fixture
}
