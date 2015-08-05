/*
 * move_test_fixture.c
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
#include "makemove.h"
#include "move.h"

void test_generation_white_pawn_moves(void);
void test_generation_black_pawn_moves(void);
void test_generation_white_knight_pawn_moves(void);
void test_generation_black_knight_pawn_moves(void);
void test_generation_sliding_horizontal_and_vertical_moves(void);
void test_generation_sliding_diagonal_moves(void);
void test_generation_queen_moves(void);
void test_generation_king_moves(void);
void test_king_castling_moves(void);
void test_sample_board_position(void);
void test_clear_piece(void);



void test_generation_white_pawn_moves(void)
{

/*Move:1 > a2a3 (score:0)
Move:2 > a2a4 (score:0)
Move:3 > c2c3 (score:0)
Move:4 > c2c4 (score:0)
Move:5 > e2e3 (score:0)
Move:6 > e2e4 (score:0)
Move:7 > h3h4 (score:0)
Move:8 > b4b5 (score:0)
Move:9 > b4c5 (score:0)
Move:10 > d4d5 (score:0)
Move:11 > d4c5 (score:0)
Move:12 > d4e5 (score:0)
Move:13 > f5f6 (score:0)
Move:14 > f5e6 (score:0)
Move:15 > g7g8q (score:0)
Move:16 > g7g8r (score:0)
Move:17 > g7g8b (score:0)
Move:18 > g7g8n (score:0)
Move:19 > g7f8q (score:0)
Move:20 > g7f8r (score:0)
Move:21 > g7f8b (score:0)
Move:22 > g7f8n (score:0)
Move:23 > g7h8q (score:0)
Move:24 > g7h8r (score:0)
Move:25 > g7h8b (score:0)
Move:26 > g7h8n (score:0)
MoveList Total 26 Moves:
 */

	// setup
    char *test_fen =
	"rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = TEST_get_empty_move_list();
	TEST_generate_white_pawn_moves(brd, mvl);


	// test
	assert_true(mvl->move_count == 26);

	mv_bitmap mv = MOVE(a2, a3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(a2, a4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	//print_move_details(mv);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(c2, c3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(e2, e3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(e2, e4, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(h3, h4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(b4, b5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(b4, c5, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(d4, d5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(d4, c5, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(d4, e5, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(f5, f6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	mv = MOVE(g7, g8, NO_PIECE, W_QUEEN, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, g8, NO_PIECE, W_KNIGHT, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, g8, NO_PIECE, W_BISHOP, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, g8, NO_PIECE, W_ROOK, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, f8, B_BISHOP, W_ROOK, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, f8, B_BISHOP, W_QUEEN, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, f8, B_BISHOP, W_BISHOP, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, f8, B_BISHOP, W_KNIGHT, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


/*

Move:23 > g7h8q (score:0)
Move:24 > g7h8r (score:0)
Move:25 > g7h8b (score:0)
Move:26 > g7h8n (score:0)

 */

	mv = MOVE(g7, h8, B_ROOK, W_KNIGHT, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, h8, B_ROOK, W_BISHOP, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, h8, B_ROOK, W_QUEEN, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g7, h8, B_ROOK, W_ROOK, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


}


void test_generation_black_pawn_moves(void)
{
	// setup
    char *test_fen =
	"rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = TEST_get_empty_move_list();
	TEST_generate_black_pawn_moves(brd, mvl);

	// test
	assert_true(mvl->move_count == 26);


	mv_bitmap mv = MOVE(g2, g1, NO_PIECE, B_ROOK, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_QUEEN, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_BISHOP, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_KNIGHT, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	mv = MOVE(g2, h1, W_ROOK, B_KNIGHT, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, h1, W_ROOK, B_BISHOP, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, h1, W_ROOK, B_ROOK, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, h1, W_ROOK, B_QUEEN, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	mv = MOVE(g2, g1, NO_PIECE, B_KNIGHT, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_BISHOP, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_ROOK, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g2, g1, NO_PIECE, B_QUEEN, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(f4, f3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(b5, b4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(d5, d4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

}


void test_generation_white_knight_pawn_moves(void)
{
	// setup
    char *test_fen = "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = TEST_get_empty_move_list();
	TEST_generate_knight_piece_moves(brd, mvl, WHITE);


	//printf("move count %d\n", mvl->move_count);
	//print_move_list_details(mvl);

	// test
	assert_true(mvl->move_count == 14);


	// check moves from d3
	mv_bitmap mv = MOVE(d3, f2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, e1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, c1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, b2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, b4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, c5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, e5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, f4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	// check moves from g5
	mv = MOVE(g5, h3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g5, f3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g5, e4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g5, f7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g5, h7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(g5, e6, B_KNIGHT, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

}

void test_generation_black_knight_pawn_moves(void)
{
	// setup
    char *test_fen = "r1bqkb1r/pppppppp/2N5/3n1N2/8/1n6/PPPPPPPP/R1BQKB1R b KQkq - 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = TEST_get_empty_move_list();
	TEST_generate_knight_piece_moves(brd, mvl, BLACK);


	//printf("move count %d\n", mvl->move_count);
	//print_move_list_details(mvl);

	// test
	assert_true(mvl->move_count == 12);

	// start on b3
	mv_bitmap mv = MOVE(b3, d2, W_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b3, c1, W_BISHOP, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b3, a1, W_ROOK, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(b3, d2, W_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b3, c1, W_BISHOP, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b3, a1, W_ROOK, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	// start on d5

	mv = MOVE(d5, f4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d5, e3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d5, c3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d5, b4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d5, b6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d5, f6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));



}

void test_generation_king_moves(void){
	// setup
    char *test_fen = "8/p1P4p/R3p2P/P1pP1q2/PRP1p1P1/3Qprr1/kp1p3P/7K w - - 0 1";

    struct board *brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	struct move_list * mvl = TEST_get_empty_move_list();
	TEST_generate_king_moves(brd, mvl, BLACK);

	assert_true(mvl->move_count == 4);

	// black king on a2
	mv_bitmap mv = MOVE(a2, a3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(a2, b3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a2, b1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a2, a1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	// reset things and check for white king
	brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	mvl = TEST_get_empty_move_list();
	TEST_generate_king_moves(brd, mvl, WHITE);

	assert_true(mvl->move_count == 2);

	// black king on h1
	mv = MOVE(h1, g2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h1, g1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	//=======================
	// reset things and do another test
	brd = get_clean_board();
	test_fen = "r5r1/2KP2R1/R1PP2p1/1pPp2P1/pp1Pp3/3kp1P1/3P2pQ/6q1 w - - 0 1";
    consume_fen_notation(test_fen, brd);

	mvl = TEST_get_empty_move_list();
	TEST_generate_king_moves(brd, mvl, BLACK);

	assert_true(mvl->move_count == 6);

	// black king on d3
	mv = MOVE(d3, c4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, c3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, c2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, d2, W_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, d4, W_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d3, e2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	// reset things and check for white king
	brd = get_clean_board();

    consume_fen_notation(test_fen, brd);

	mvl = TEST_get_empty_move_list();
	TEST_generate_king_moves(brd, mvl, WHITE);

	assert_true(mvl->move_count == 5);

	// black king on c7
	mv = MOVE(c7, b6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c7, b7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c7, b8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c7, c8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c7, d8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


}


void test_generation_sliding_diagonal_moves(void){

	char * sliding_test = "8/2B1p2P/4PppK/p2pP1P1/1P4pp/Pb2p3/3P1Pk1/2bB4 w - - 0 1";
    struct board *brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    struct move_list *mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));



	// black bishops
	TEST_generate_sliding_diagonal_moves(brd, mvl, B_BISHOP);

	assert_true(mvl->move_count == 8);

	mv_bitmap mv = MOVE(b3, a4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b3, c2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b3, d1, W_BISHOP, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b3, a2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b3, c4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(c1, b2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c1, a3, W_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c1, d2, W_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	// now look at white bishops
    brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_sliding_diagonal_moves(brd, mvl, W_BISHOP);
	assert_true(mvl->move_count == 10);

	mv = MOVE(d1, c2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d1, b3, B_BISHOP, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d1, e2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d1, f3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(d1, g4, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(c7, b6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c7, a5, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c7, d6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c7, b8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c7, d8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));



}


void test_generation_sliding_horizontal_and_vertical_moves(void){

	char * sliding_test = "K7/1rp5/5R1P/6p1/7P/1k3p1P/1P1p2r1/4R3 w - - 0 1";
    struct board *brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    struct move_list *mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_sliding_horizontal_vertical_moves(brd, mvl, B_ROOK);

	assert_true(mvl->move_count == 11);

	mv_bitmap mv = MOVE(g2, e2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g2, f2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g2, h2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g2, g1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g2, g3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(g2, g4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(b7, b8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b7, a7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b7, b6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b7, b5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(b7, b4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	// check the white rooks
    brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_sliding_horizontal_vertical_moves(brd, mvl, W_ROOK);

	assert_true(mvl->move_count == 25);

	mv = MOVE(e1, d1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, c1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, b1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, a1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, f1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, g1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, h1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, e2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, e3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, e4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, e5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, e6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, e7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, e8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(f6, f7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, f8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, g6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, a6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, b6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, c6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, d6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, e6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, f5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, f4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(f6, f3, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

}


void test_king_castling_moves(void){
	char * sliding_test = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";
    struct board *brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    struct move_list *mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_castle_moves(brd, mvl, WHITE);


	assert_true(mvl->move_count == 2);
	mv_bitmap mv = MOVE(e1, g1, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e1, c1, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
	assert_true(TEST_is_move_in_list(mvl, mv));

	brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_castle_moves(brd, mvl, BLACK);

	assert_true(mvl->move_count == 2);
	mv = MOVE(e8, g8, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, c8, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
	assert_true(TEST_is_move_in_list(mvl, mv));



	sliding_test = "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1";
    brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_castle_moves(brd, mvl, WHITE);

	assert_true(mvl->move_count == 0);

    brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);
    mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_castle_moves(brd, mvl, BLACK);
	assert_true(mvl->move_count == 1);

	mv = MOVE(e8, g8, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
	assert_true(TEST_is_move_in_list(mvl, mv));


}

void test_generation_queen_moves(void){

	char * sliding_test = "4q3/4p2p/q4p1Q/1k2p2P/1p4P1/p1Pp1KPP/PpQ2PP1/8 w - - 0 1";
    struct board *brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    struct move_list *mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_queen_moves(brd, mvl, W_QUEEN);

	assert_true(mvl->move_count == 19);

	// check W_QUEEN's
	mv_bitmap mv = MOVE(h6, h7, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, f6, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, g6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, g7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, f8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, g5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, f4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, e3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, d2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(h6, c1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(c2, b3, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c2, a4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c2, b2, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c2, b1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c2, c1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c2, d1, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c2, d2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(c2, e2, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(c2, d3, B_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));


	// now look at the black queens
	brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    mvl = malloc(sizeof(struct move_list));
	memset(mvl, 0, sizeof(struct move_list));

	TEST_generate_queen_moves(brd, mvl, B_QUEEN);

	assert_true(mvl->move_count == 22);

	mv = MOVE(a6, a7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, a8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, b6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, c6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, d6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, e6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, b7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, c8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, a5, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(a6, a4, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));

	mv = MOVE(e8, d8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, c8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, b8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, a8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, d7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, c6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, f8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, g8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, h8, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, f7, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, g6, NO_PIECE, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));
	mv = MOVE(e8, h5, W_PAWN, NO_PIECE, 0);
	assert_true(TEST_is_move_in_list(mvl, mv));



}


void test_sample_board_position(){

	char * sample_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    struct board *brd= get_clean_board();

    consume_fen_notation(sample_position, brd);

    struct move_list *list = malloc(sizeof(struct move_list));
	memset(list, 0, sizeof(struct move_list));

	generate_all_moves(brd, list);

	assert_true(list->move_count == 48);
}


void test_clear_piece(){


	//
	// remove the knight from c3
	//
	char * sample_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    struct board *brd= get_clean_board();

    consume_fen_notation(sample_position, brd);

	assert_true(check_bit(&brd->board, c3) == true);
	assert_true(check_bit(&brd->bitboards[W_KNIGHT], c3) == true);


	// save some info before the move for comparison
	U64 old_hash = brd->board_hash;
	U8 old_num_pces_on_brd = brd->pce_num[W_KNIGHT];
	U8 old_num_big_pces = brd->big_pieces[WHITE];
	U8 old_num_minor_pces = brd->minor_pieces[WHITE];
	U32 old_material = brd->material[WHITE];
	enum piece old_pce = brd->pieces[c3];


	// remove the knight from c3
	clear_piece(brd, c3);

	assert_true(old_hash != brd->board_hash);

	assert_true(old_num_pces_on_brd == 2);
	assert_true(brd->pce_num[W_KNIGHT] == 1);

	assert_true(old_num_big_pces == 8);
	assert_true(brd->big_pieces[WHITE] == 7);

	assert_true(old_num_minor_pces = 4);
	assert_true(brd->minor_pieces[WHITE] == 3);

	U32 new_material = brd->material[WHITE] + piece_values[W_KNIGHT];
	assert_true(new_material == old_material);

	assert_true(old_pce == W_KNIGHT);
	assert_true(brd->pieces[c3] == NO_PIECE);

	assert_true(check_bit(&brd->board, c3) == false);
	assert_true(check_bit(&brd->bitboards[W_KNIGHT], c3) == false);

}





void move_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    run_test(test_generation_white_pawn_moves);
    run_test(test_generation_black_pawn_moves);
	run_test(test_generation_white_knight_pawn_moves);
	run_test(test_generation_black_knight_pawn_moves);
	run_test(test_generation_king_moves);
	run_test(test_king_castling_moves);
	run_test(test_generation_sliding_horizontal_and_vertical_moves);
	run_test(test_generation_sliding_diagonal_moves);
	run_test(test_generation_queen_moves);
	run_test(test_sample_board_position);
	run_test(test_clear_piece);


    test_fixture_end();		// ends a fixture
}
