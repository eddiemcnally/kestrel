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
#include <stdint.h>
#include <stdlib.h>
#include "seatest.h"
#include "types.h"
#include "hashkeys.h"
#include "board_utils.h"
#include "board.h"
#include "fen.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "pieces.h"
#include "utils.h"

void verify_initial_board_placement(struct board *brd);
void test_initial_board_placement(void);
void test_clean_board(void);
void test_fen_parsing_initial_board_layout(void);
void test_add_to_board(void);
void test_fen_parsing_general_layout_1(void);
void test_fen_parsing_general_layout_2(void);
void test_fen_parsing_general_layout_3(void);
void test_position_1(void);
void test_setting_bits_in_a_board(void);
void test_clearing_bits_in_a_board(void);
void test_checking_bits_in_a_board(void);
void test_pawn_control(void);
void test_bit_counting(void);
void test_LSB_clear(void);
void board_test_fixture(void);
void test_king_bitboard(void);
void test_get_king_square(void);
void test_get_set_side_to_move(void);
void test_is_pawn_controlling_square(void);

/**
 * Verifies the initial board setup plus some supporting code
 */
void test_initial_board_placement()
{
    struct board *brd = allocate_board();
    consume_fen_notation(STARTING_FEN, brd);

    
   	ASSERT_BOARD_OK(brd);

    assert_true(W_ROOK == get_piece_on_square(brd, a1));
    assert_true(W_KNIGHT == get_piece_on_square(brd, b1));
    assert_true(W_BISHOP == get_piece_on_square(brd, c1));
    assert_true(W_QUEEN == get_piece_on_square(brd, d1));
    assert_true(W_KING == get_piece_on_square(brd, e1));
    assert_true(W_BISHOP == get_piece_on_square(brd, f1));
    assert_true(W_KNIGHT == get_piece_on_square(brd, g1));
    assert_true(W_ROOK == get_piece_on_square(brd, h1));

    assert_true(W_PAWN == get_piece_on_square(brd, a2));
    assert_true(W_PAWN == get_piece_on_square(brd, b2));
    assert_true(W_PAWN == get_piece_on_square(brd, c2));
    assert_true(W_PAWN == get_piece_on_square(brd, d2));
    assert_true(W_PAWN == get_piece_on_square(brd, e2));
    assert_true(W_PAWN == get_piece_on_square(brd, f2));
    assert_true(W_PAWN == get_piece_on_square(brd, g2));
    assert_true(W_PAWN == get_piece_on_square(brd, h2));

    assert_true(B_PAWN == get_piece_on_square(brd, a7));
    assert_true(B_PAWN == get_piece_on_square(brd, b7));
    assert_true(B_PAWN == get_piece_on_square(brd, c7));
    assert_true(B_PAWN == get_piece_on_square(brd, d7));
    assert_true(B_PAWN == get_piece_on_square(brd, e7));
    assert_true(B_PAWN == get_piece_on_square(brd, f7));
    assert_true(B_PAWN == get_piece_on_square(brd, g7));
    assert_true(B_PAWN == get_piece_on_square(brd, h7));

    assert_true(B_ROOK == get_piece_on_square(brd, a8));
    assert_true(B_KNIGHT == get_piece_on_square(brd, b8));
    assert_true(B_BISHOP == get_piece_on_square(brd, c8));
    assert_true(B_QUEEN == get_piece_on_square(brd, d8));
    assert_true(B_KING == get_piece_on_square(brd, e8));
    assert_true(B_BISHOP == get_piece_on_square(brd, f8));
    assert_true(B_KNIGHT == get_piece_on_square(brd, g8));
    assert_true(B_ROOK == get_piece_on_square(brd, h8));

    // no piece present
    for (int i = a3; i < h6; i++) {
        assert_true(get_piece_on_square(brd, (enum square)i) == NO_PIECE);
    }

    // now verify remainder of struct is populated
    assert_true(get_side_to_move(brd) == WHITE);

    assert_true(get_en_passant_sq(brd) == NO_SQUARE);

    assert_true(get_fifty_move_counter(brd) == 0);
    assert_true(get_ply(brd) == 0);
    assert_true(get_history_ply(brd) == 0);

    assert_true(get_castle_permissions(brd) == (WQCA | WKCA | BQCA | BKCA));

    assert_true(get_board_hash(brd) != 0);

	free_board(brd);
}

void test_fen_parsing_initial_board_layout(void)
{

    // this is the initial bpard setup
    char *test_fen =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    struct board *brd = allocate_board();
    consume_fen_notation(test_fen, brd);

	ASSERT_BOARD_OK(brd);
	
    //print_board(brd);

    // verify the board
    assert_true(W_ROOK == get_piece_on_square(brd, a1));
    assert_true(W_KNIGHT == get_piece_on_square(brd, b1));
    assert_true(W_BISHOP == get_piece_on_square(brd, c1));
    assert_true(W_QUEEN == get_piece_on_square(brd, d1));
    assert_true(W_KING == get_piece_on_square(brd, e1));
    assert_true(W_BISHOP == get_piece_on_square(brd, f1));
    assert_true(W_KNIGHT == get_piece_on_square(brd, g1));
    assert_true(W_ROOK == get_piece_on_square(brd, h1));

    assert_true(W_PAWN == get_piece_on_square(brd, a2));
    assert_true(W_PAWN == get_piece_on_square(brd, b2));
    assert_true(W_PAWN == get_piece_on_square(brd, c2));
    assert_true(W_PAWN == get_piece_on_square(brd, d2));
    assert_true(W_PAWN == get_piece_on_square(brd, e2));
    assert_true(W_PAWN == get_piece_on_square(brd, f2));
    assert_true(W_PAWN == get_piece_on_square(brd, g2));
    assert_true(W_PAWN == get_piece_on_square(brd, h2));

    assert_true(B_PAWN == get_piece_on_square(brd, a7));
    assert_true(B_PAWN == get_piece_on_square(brd, b7));
    assert_true(B_PAWN == get_piece_on_square(brd, c7));
    assert_true(B_PAWN == get_piece_on_square(brd, d7));
    assert_true(B_PAWN == get_piece_on_square(brd, e7));
    assert_true(B_PAWN == get_piece_on_square(brd, f7));
    assert_true(B_PAWN == get_piece_on_square(brd, g7));
    assert_true(B_PAWN == get_piece_on_square(brd, h7));

    assert_true(B_ROOK == get_piece_on_square(brd, a8));
    assert_true(B_KNIGHT == get_piece_on_square(brd, b8));
    assert_true(B_BISHOP == get_piece_on_square(brd, c8));
    assert_true(B_QUEEN == get_piece_on_square(brd, d8));
    assert_true(B_KING == get_piece_on_square(brd, e8));
    assert_true(B_BISHOP == get_piece_on_square(brd, f8));
    assert_true(B_KNIGHT == get_piece_on_square(brd, g8));
    assert_true(B_ROOK == get_piece_on_square(brd, h8));

    // no piece present
    for (int i = a3; i < h6; i++) {
        assert_true(NO_PIECE == get_piece_on_square(brd, (enum square)i));
    }

    assert_true(get_side_to_move(brd) == WHITE);

    assert_true(get_castle_permissions(brd) == (WKCA | WQCA | BKCA | BQCA));

    assert_true(get_en_passant_sq(brd) == NO_SQUARE);

	free_board(brd);
}

void test_fen_parsing_general_layout_1()
{
    char *test_fen = "k7/8/8/4N3/8/8/8/3K4 b - - 13 56";

    struct board *brd = allocate_board();
    consume_fen_notation(test_fen, brd);

	ASSERT_BOARD_OK(brd);
	
    //print_board(brd);

    // verify the board
    assert_true(B_KING == get_piece_on_square(brd, a8));
    assert_true(W_KNIGHT == get_piece_on_square(brd, e5));
    assert_true(W_KING == get_piece_on_square(brd, d1));

    assert_true(get_side_to_move(brd) == BLACK);

    assert_true(get_castle_permissions(brd) == 0);

    assert_true(get_en_passant_sq(brd) == NO_SQUARE);

	free_board(brd);
}

void test_king_bitboard(){
	char *pos = "2rq1b1r/p3kppp/2np1n2/1pp1pb2/PP1P1B2/2N1PP2/2P1N1PP/RQK2B1R b - - 0 1\n";
	
    struct board *brd = allocate_board();
    consume_fen_notation(pos, brd);
	
	uint64_t blk_bb = get_bitboard_for_king(brd, BLACK);
	enum square black_king_sq = pop_1st_bit(&blk_bb);
	assert_true(black_king_sq == e7);

	uint64_t wht_bb = get_bitboard_for_king(brd, WHITE);
	enum square white_king_sq = pop_1st_bit(&wht_bb);
	assert_true(white_king_sq == c1);
	
}


void test_fen_parsing_general_layout_2()
{

    // this is the initial board setup
    char *test_fen =
        "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";

    struct board *brd = allocate_board();
    consume_fen_notation(test_fen, brd);

	ASSERT_BOARD_OK(brd);

    //print_board(brd);

	uint64_t white_bb = get_bitboard_for_colour(brd, WHITE);

    // verify the board
    assert_true(W_ROOK == get_piece_on_square(brd, a1));
    assert_true(get_piece_on_square(brd, a1) == W_ROOK);
	assert_true(is_square_occupied(white_bb, a1));
	assert_true(is_piece_on_square(brd, W_ROOK, a1));
    
    assert_true(W_KNIGHT == get_piece_on_square(brd, b1));
    assert_true(get_piece_on_square(brd, b1) == W_KNIGHT);
	assert_true(is_square_occupied(white_bb, b1));
	assert_true(is_piece_on_square(brd, W_KNIGHT, b1));
    
    assert_true(W_BISHOP == get_piece_on_square(brd, c1));
    assert_true(get_piece_on_square(brd, c1) == W_BISHOP);
   	assert_true(is_square_occupied(white_bb, c1));
	assert_true(is_piece_on_square(brd, W_BISHOP, c1));

    assert_true(W_QUEEN == get_piece_on_square(brd, d1));
    assert_true(get_piece_on_square(brd, d1) == W_QUEEN);
	assert_true(is_square_occupied(white_bb, d1));
	assert_true(is_piece_on_square(brd, W_QUEEN, d1));

    assert_true(W_KING == get_piece_on_square(brd, e1));
    assert_true(get_piece_on_square(brd, e1) == W_KING);
	assert_true(is_square_occupied(white_bb, e1));
	assert_true(is_piece_on_square(brd, W_KING, e1));

    assert_true(W_BISHOP == get_piece_on_square(brd, f1));
    assert_true(get_piece_on_square(brd, f1) == W_BISHOP);
	assert_true(is_square_occupied(white_bb, f1));
	assert_true(is_piece_on_square(brd, W_BISHOP, f1));

    assert_true(W_ROOK == get_piece_on_square(brd, h1));
    assert_true(get_piece_on_square(brd, h1) == W_ROOK);
	assert_true(is_square_occupied(white_bb, h1));
	assert_true(is_piece_on_square(brd, W_ROOK, h1));

    assert_true(W_PAWN == get_piece_on_square(brd, a2));
    assert_true(get_piece_on_square(brd, a2) == W_PAWN);
	assert_true(is_square_occupied(white_bb, a2));
	assert_true(is_piece_on_square(brd, W_PAWN, a2));

    assert_true(W_PAWN == get_piece_on_square(brd, b2));
    assert_true(get_piece_on_square(brd, b2) == W_PAWN);
	assert_true(is_square_occupied(white_bb, b2));
	assert_true(is_piece_on_square(brd, W_PAWN, b2));

    assert_true(W_PAWN == get_piece_on_square(brd, d2));
    assert_true(get_piece_on_square(brd, d2) == W_PAWN);
	assert_true(is_square_occupied(white_bb, d2));
	assert_true(is_piece_on_square(brd, W_PAWN, d2));

    assert_true(W_PAWN == get_piece_on_square(brd, f2));
    assert_true(get_piece_on_square(brd, f2) == W_PAWN);
	assert_true(is_square_occupied(white_bb, f2));
	assert_true(is_piece_on_square(brd, W_PAWN, f2));

    assert_true(W_PAWN == get_piece_on_square(brd, g2));
    assert_true(get_piece_on_square(brd, g2) == W_PAWN);
	assert_true(is_square_occupied(white_bb, g2));
	assert_true(is_piece_on_square(brd, W_PAWN, g2));

    assert_true(W_PAWN == get_piece_on_square(brd, h2));
    assert_true(get_piece_on_square(brd, h2) == W_PAWN);
	assert_true(is_square_occupied(white_bb, h2));
	assert_true(is_piece_on_square(brd, W_PAWN, h2));


	uint64_t black_bb = get_bitboard_for_colour(brd, BLACK);

    assert_true(B_PAWN == get_piece_on_square(brd, a7));
    assert_true(get_piece_on_square(brd, a7) == B_PAWN);
	assert_true(is_square_occupied(black_bb, a7));
	assert_true(is_piece_on_square(brd, B_PAWN, a7));



    assert_true(B_PAWN == get_piece_on_square(brd, b7));
    assert_true(get_piece_on_square(brd, b7) == B_PAWN);
	assert_true(is_square_occupied(black_bb, b7));
	assert_true(is_piece_on_square(brd, B_PAWN, b7));


    assert_true(B_PAWN == get_piece_on_square(brd, d7));
    assert_true(get_piece_on_square(brd, d7) == B_PAWN);
	assert_true(is_square_occupied(black_bb, d7));
	assert_true(is_piece_on_square(brd, B_PAWN, d7));



    assert_true(B_PAWN == get_piece_on_square(brd, f7));
    assert_true(get_piece_on_square(brd, f7) == B_PAWN);
	assert_true(is_square_occupied(black_bb, f7));
	assert_true(is_piece_on_square(brd, B_PAWN, f7));

    assert_true(B_PAWN == get_piece_on_square(brd, g7));
    assert_true(get_piece_on_square(brd, g7) == B_PAWN);
	assert_true(is_square_occupied(black_bb, g7));
	assert_true(is_piece_on_square(brd, B_PAWN, g7));


    assert_true(B_PAWN == get_piece_on_square(brd, h7));
    assert_true(get_piece_on_square(brd, h7) == B_PAWN);
	assert_true(is_square_occupied(black_bb, h7));
	assert_true(is_piece_on_square(brd, B_PAWN, h7));


    assert_true(B_ROOK == get_piece_on_square(brd, a8));
    assert_true(get_piece_on_square(brd, a8) == B_ROOK);
	assert_true(is_square_occupied(black_bb, a8));
	assert_true(is_piece_on_square(brd, B_ROOK, a8));

    assert_true(B_KNIGHT == get_piece_on_square(brd, b8));
    assert_true(get_piece_on_square(brd, b8) == B_KNIGHT);
   	assert_true(is_square_occupied(black_bb, b8));
	assert_true(is_piece_on_square(brd, B_KNIGHT, b8));

    assert_true(B_BISHOP == get_piece_on_square(brd, c8));
    assert_true(get_piece_on_square(brd, c8) == B_BISHOP);
	assert_true(is_square_occupied(black_bb, c8));
	assert_true(is_piece_on_square(brd, B_BISHOP, c8));

    assert_true(B_QUEEN == get_piece_on_square(brd, d8));
    assert_true(get_piece_on_square(brd, d8) == B_QUEEN);
	assert_true(is_square_occupied(black_bb, d8));
	assert_true(is_piece_on_square(brd, B_QUEEN, d8));

    assert_true(B_KING == get_piece_on_square(brd, e8));
    assert_true(get_piece_on_square(brd, b8) == B_KNIGHT);
	assert_true(is_square_occupied(black_bb, b8));
	assert_true(is_piece_on_square(brd, B_KNIGHT, b8));

    assert_true(B_BISHOP == get_piece_on_square(brd, f8));
    assert_true(get_piece_on_square(brd, f8) == B_BISHOP);
	assert_true(is_square_occupied(black_bb, f8));
	assert_true(is_piece_on_square(brd, B_BISHOP, f8));

    assert_true(B_ROOK == get_piece_on_square(brd, h8));
    assert_true(get_piece_on_square(brd, h8) == B_ROOK);
	assert_true(is_square_occupied(black_bb, h8));
	assert_true(is_piece_on_square(brd, B_ROOK, h8));

    assert_true(get_side_to_move(brd) == BLACK);

    assert_true(get_castle_permissions(brd) == (WKCA | WQCA | BKCA | BQCA));

    assert_true(get_en_passant_sq(brd) == NO_SQUARE);


    // test pawns on files
    for(enum file f = FILE_A; f <= FILE_H; f++) {
        assert_true(get_num_pawns_on_file(brd, WHITE, f) == 1);
        assert_true(get_num_pawns_on_file(brd, BLACK, f) == 1);
    }
    
    // test pawns on files
    assert_true(get_num_pawns_on_rank(brd, WHITE, RANK_1) == 0);
    assert_true(get_num_pawns_on_rank(brd, WHITE, RANK_2) == 7);
    assert_true(get_num_pawns_on_rank(brd, WHITE, RANK_3) == 0);
    assert_true(get_num_pawns_on_rank(brd, WHITE, RANK_4) == 1);
    assert_true(get_num_pawns_on_rank(brd, WHITE, RANK_5) == 0);
    assert_true(get_num_pawns_on_rank(brd, WHITE, RANK_6) == 0);
    assert_true(get_num_pawns_on_rank(brd, WHITE, RANK_7) == 0);
    assert_true(get_num_pawns_on_rank(brd, WHITE, RANK_8) == 0);

    assert_true(get_num_pawns_on_rank(brd, BLACK, RANK_1) == 0);
    assert_true(get_num_pawns_on_rank(brd, BLACK, RANK_2) == 0);
    assert_true(get_num_pawns_on_rank(brd, BLACK, RANK_3) == 0);
    assert_true(get_num_pawns_on_rank(brd, BLACK, RANK_4) == 0);
    assert_true(get_num_pawns_on_rank(brd, BLACK, RANK_5) == 1);
    assert_true(get_num_pawns_on_rank(brd, BLACK, RANK_6) == 0);
    assert_true(get_num_pawns_on_rank(brd, BLACK, RANK_7) == 7);
    assert_true(get_num_pawns_on_rank(brd, BLACK, RANK_8) == 0);
        
    
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,a3) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,b3) == 2);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,c3) == 2);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,d3) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,e3) == 2);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,f3) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,g3) == 2);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,h3) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,d5) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,WHITE,f5) == 1);

    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,a6) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,b6) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,c6) == 2);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,d6) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,e6) == 2);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,f6) == 2);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,g6) == 2);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,h6) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,b4) == 1);
    assert_true(get_num_squares_under_pawn_ctl(brd,BLACK,d4) == 1);

	free_board(brd);

}


void test_get_king_square(){
	char *test_pos = "rnbq3r/ppp2kpp/4pn2/1N6/1b1NP3/3B4/PP1B1PPP/R2Q2KR w KQkq -\n";
	
    struct board *brd = allocate_board();
    consume_fen_notation(test_pos, brd);

	assert_true(get_king_square(brd, BLACK) == f7);
	assert_true(get_king_square(brd, WHITE) == g1);
	free_board(brd);

	test_pos = "rnb4r/pp4pp/1q2pn2/1Np3k1/1b1NP3/2BB4/PP3PPP/RK1Q3R w KQkq -\n";
	
    brd = allocate_board();
    consume_fen_notation(test_pos, brd);

	assert_true(get_king_square(brd, BLACK) == g5);
	assert_true(get_king_square(brd, WHITE) == b1);
	free_board(brd);


}


void test_position_1(){
	char *test_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n";
	
    struct board *brd = allocate_board();
    consume_fen_notation(test_pos, brd);

	ASSERT_BOARD_OK(brd);

	free_board(brd);
}


void test_is_pawn_controlling_square(){
	char *test_pos = "r1b1k2r/pp1n1ppp/3bpn2/q1pp4/1PPBB3/P2N1P2/3PP1PP/R1BQK3 b Qkq - 0 1\n";
	
    struct board *brd = allocate_board();
    consume_fen_notation(test_pos, brd);

	assert_true(is_pawn_controlling_sq(brd, WHITE, b4));
	assert_true(is_pawn_controlling_sq(brd, WHITE, c3));
	assert_true(is_pawn_controlling_sq(brd, WHITE, d5));
	assert_true(is_pawn_controlling_sq(brd, WHITE, e4));
	assert_true(is_pawn_controlling_sq(brd, WHITE, g4));
	assert_true(is_pawn_controlling_sq(brd, WHITE, g3));
	assert_true(is_pawn_controlling_sq(brd, WHITE, h3));

	assert_true(is_pawn_controlling_sq(brd, BLACK, a6));
	assert_true(is_pawn_controlling_sq(brd, BLACK, b6));
	assert_true(is_pawn_controlling_sq(brd, BLACK, b4));
	assert_true(is_pawn_controlling_sq(brd, BLACK, c6));
	assert_true(is_pawn_controlling_sq(brd, BLACK, c4));
	assert_true(is_pawn_controlling_sq(brd, BLACK, d4));
	assert_true(is_pawn_controlling_sq(brd, BLACK, d5));
	assert_true(is_pawn_controlling_sq(brd, BLACK, e4));
	assert_true(is_pawn_controlling_sq(brd, BLACK, e6));
	assert_true(is_pawn_controlling_sq(brd, BLACK, f5));
	assert_true(is_pawn_controlling_sq(brd, BLACK, g6));
	assert_true(is_pawn_controlling_sq(brd, BLACK, h6));

	free_board(brd);
}


void test_pawn_control(){
	char *test_pos = "r1b1k2r/pp1n1ppp/3bpn2/q1pp4/1PPBB3/P2N1P2/3PP1PP/R1BQK3 b Qkq - 0 1\n";
	
    struct board *brd = allocate_board();
    consume_fen_notation(test_pos, brd);

	// check white pawn control
	for(enum square sq = a1; sq <= h2; sq++){
		assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, sq) == 0);		
	}
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, a3) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, b3) == 0);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, c3) == 1);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, d3) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, e3) == 1);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, f3) == 2);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, g3) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, h3) == 1);	

	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, a4) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, b4) == 1);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, c4) == 0);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, d4) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, e4) == 1);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, f4) == 0);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, g4) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, h4) == 0);	

	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, a5) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, b5) == 1);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, c5) == 1);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, d5) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, e5) == 0);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, f5) == 0);	
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, g5) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, h5) == 0);	

	for(enum square sq = a6; sq <= h8; sq++){
		assert_true(get_num_squares_under_pawn_ctl(brd, WHITE, sq) == 0);		
	}
	
	// check black pawn control
	for(enum square sq = a7; sq <= h8; sq++){
		assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, sq) == 0);		
	}
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, a6) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, b6) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, c6) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, d6) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, e6) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, f6) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, g6) == 2);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, h6) == 1);
	
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, a5) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, b5) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, c5) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, d5) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, e5) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, f5) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, g5) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, h5) == 0);
	
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, a4) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, b4) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, c4) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, d4) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, e4) == 1);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, f4) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, g4) == 0);
	assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, h4) == 0);

	for(enum square sq = a1; sq <=h3; sq++){
		assert_true(get_num_squares_under_pawn_ctl(brd, BLACK, sq) == 0);		
	}
	
	free_board(brd);
}



// en passant
void test_fen_parsing_general_layout_3()
{

    // this is the initial board setup
    char *test_fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 1";

    struct board *brd = allocate_board();
    consume_fen_notation(test_fen, brd);

	ASSERT_BOARD_OK(brd);
	
    //print_board(brd);

    assert_true(get_side_to_move(brd) == WHITE);
    assert_true(get_castle_permissions(brd) == (WKCA | WQCA | BKCA | BQCA));

    assert_true(get_en_passant_sq(brd) == c6);

	free_board(brd);
}


void test_setting_bits_in_a_board()
{

    uint64_t test_brd = 0;

    //
    // Test setting bits
    //
    set_bit(&test_brd, 22);
    assert_true(test_brd == 4194304);
    assert_true(1 == count_bits(test_brd));

    test_brd = 0;
    set_bit(&test_brd, 0);
    assert_true(test_brd == 1);
    assert_true(1 == count_bits(test_brd));

    test_brd = 0;
    set_bit(&test_brd, 63);
    assert_true(test_brd == 0x8000000000000000);
    assert_true(1 == count_bits(test_brd));

    test_brd = 0;
    set_bit(&test_brd, 31);
    assert_true(test_brd == 2147483648);
    assert_true(1 == count_bits(test_brd));

    test_brd = 0;
    set_bit(&test_brd, 31);
    set_bit(&test_brd, 1);
    set_bit(&test_brd, 21);
    set_bit(&test_brd, 55);
    set_bit(&test_brd, 8);
    assert_true(test_brd == 0x80000080200102);
    assert_true(5 == count_bits(test_brd));

}

void test_clearing_bits_in_a_board()
{

    uint64_t test_brd = 0;

    //
    // Test clearing bits
    //
    set_bit(&test_brd, 22);
    set_bit(&test_brd, 23);
    assert_true(2 == count_bits(test_brd));
    assert_true(is_square_occupied(test_brd, 22));
    assert_true(is_square_occupied(test_brd, 23));
    clear_bit(&test_brd, 22);
    assert_true(is_square_occupied(test_brd, 23));
    assert_true(1 == count_bits(test_brd));

    test_brd = 0;

    set_bit(&test_brd, 1);
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

}

void test_checking_bits_in_a_board(void)
{
    uint64_t test_brd = 0;

    for (enum square sq = a1; sq <= h8; sq++) {
        set_bit(&test_brd, sq);
        assert_true(is_square_occupied(test_brd, sq));

        clear_bit(&test_brd, sq);
        assert_false(is_square_occupied(test_brd, sq));
    }
}

void test_bit_counting(void)
{
    uint64_t brd = 0;

    set_bit(&brd, d3);
    set_bit(&brd, a7);
    set_bit(&brd, b3);
    set_bit(&brd, g6);
    set_bit(&brd, d2);

    assert_true(count_bits(brd) == 5);

}

void test_LSB_clear(void)
{
    uint64_t brd = 0;

    set_bit(&brd, d3);
    set_bit(&brd, a7);
    set_bit(&brd, b3);
    set_bit(&brd, g6);
    set_bit(&brd, a1);

    uint8_t cleared_bit = pop_1st_bit(&brd);

    assert_true(cleared_bit == a1);
    assert_false(is_square_occupied(brd, a1));
}

void  test_get_set_side_to_move(){
	char *test_fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 1";

    struct board *brd = allocate_board();
    consume_fen_notation(test_fen, brd);

	set_side_to_move(brd, WHITE);
	assert_true(get_side_to_move(brd) == WHITE);

	set_side_to_move(brd, BLACK);
	assert_true(get_side_to_move(brd) == BLACK);

	free_board(brd);
}


void board_test_fixture(void)
{

    test_fixture_start();	// starts a fixture

    run_test(test_initial_board_placement);

	run_test(test_king_bitboard);
    run_test(test_setting_bits_in_a_board);
    run_test(test_checking_bits_in_a_board);
    run_test(test_clearing_bits_in_a_board);
    run_test(test_get_king_square);

    run_test(test_fen_parsing_initial_board_layout);
    run_test(test_position_1);
    run_test(test_fen_parsing_general_layout_1);
    run_test(test_fen_parsing_general_layout_2);
    run_test(test_fen_parsing_general_layout_3);
    
    run_test(test_bit_counting);
    run_test(test_LSB_clear);
    
    run_test(test_pawn_control);
    run_test(test_is_pawn_controlling_square);
    
    run_test(test_get_set_side_to_move);

    test_fixture_end();	// ends a fixture
}



/* Test to do:
 * 
 * 
void move_piece(struct board *brd, enum square from, enum square to);
void remove_piece_from_board(struct board *brd,  enum piece pce_to_remove, enum square sq);
void add_piece_to_board(struct board *brd, enum piece pce, enum square sq);

void set_castle_permission(struct board *brd, enum castle_perm perm);
enum castle_perm get_castle_permissions(const struct board *brd);

void set_en_passant_sq(struct board *brd, enum square sq);
enum square get_en_passant_sq(const struct board *brd);

void update_board_hash(struct board *brd);
uint64_t get_board_hash(const struct board *brd);

void push_history(struct board *brd, mv_bitmap move);
mv_bitmap pop_history(struct board *brd);

uint8_t get_ply(const struct board *brd);
void set_ply(struct board *brd, uint8_t ply);

uint8_t get_history_ply(const struct board *brd);
void set_history_ply(struct board *brd, uint8_t hist_ply);

void assert_boards_are_equal(const struct board *brd1, const struct board *brd2);

mv_bitmap get_best_pvline(const struct board *brd);
mv_bitmap get_pvline(const struct board *brd, uint8_t search_depth);
void set_pvline(struct board *brd, uint8_t search_depth, mv_bitmap move);
uint8_t populate_pv_line(struct board *brd, uint8_t depth);




uint8_t get_fifty_move_counter(const struct board *brd);

bool make_move(struct board *brd, mv_bitmap mv);
void take_move(struct board *brd);
void flip_sides(struct board *brd);
void set_bit(uint64_t * brd, enum square sq);
void clear_bit(uint64_t * brd, enum square sq);


void set_piece_material(struct board *brd);
uint64_t overlay_white_piece_bitboards(const struct board * brd);
uint64_t overlay_black_piece_bitboards(const struct board * brd);
void overlay_boards(struct board *the_board);
uint8_t count_bits(uint64_t bb);


void clone_board(const struct board *board_to_clone, struct board *cloned);
*/
