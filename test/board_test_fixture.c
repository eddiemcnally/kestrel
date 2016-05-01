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
#include "init.h"
#include "hashkeys.h"
#include "board_utils.h"
#include "makemove.h"
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
    init_hash_keys();
    struct board brd = init_game(STARTING_FEN);

    assert_true(W_ROOK == brd.pieces[a1]);
    assert_true(W_KNIGHT == brd.pieces[b1]);
    assert_true(W_BISHOP == brd.pieces[c1]);
    assert_true(W_QUEEN == brd.pieces[d1]);
    assert_true(W_KING == brd.pieces[e1]);
    assert_true(W_BISHOP == brd.pieces[f1]);
    assert_true(W_KNIGHT == brd.pieces[g1]);
    assert_true(W_ROOK == brd.pieces[h1]);

    assert_true(W_PAWN == brd.pieces[a2]);
    assert_true(W_PAWN == brd.pieces[b2]);
    assert_true(W_PAWN == brd.pieces[c2]);
    assert_true(W_PAWN == brd.pieces[d2]);
    assert_true(W_PAWN == brd.pieces[e2]);
    assert_true(W_PAWN == brd.pieces[f2]);
    assert_true(W_PAWN == brd.pieces[g2]);
    assert_true(W_PAWN == brd.pieces[h2]);

    assert_true(B_PAWN == brd.pieces[a7]);
    assert_true(B_PAWN == brd.pieces[b7]);
    assert_true(B_PAWN == brd.pieces[c7]);
    assert_true(B_PAWN == brd.pieces[d7]);
    assert_true(B_PAWN == brd.pieces[e7]);
    assert_true(B_PAWN == brd.pieces[f7]);
    assert_true(B_PAWN == brd.pieces[g7]);
    assert_true(B_PAWN == brd.pieces[h7]);

    assert_true(B_ROOK == brd.pieces[a8]);
    assert_true(B_KNIGHT == brd.pieces[b8]);
    assert_true(B_BISHOP == brd.pieces[c8]);
    assert_true(B_QUEEN == brd.pieces[d8]);
    assert_true(B_KING == brd.pieces[e8]);
    assert_true(B_BISHOP == brd.pieces[f8]);
    assert_true(B_KNIGHT == brd.pieces[g8]);
    assert_true(B_ROOK == brd.pieces[h8]);

    // no piece present
    for (int i = a3; i < h6; i++) {
        assert_true(brd.pieces[i] == NO_PIECE);
    }

    // now verify remainder of struct is populated
    assert_true(brd.side_to_move == WHITE);

    assert_true(brd.en_passant == NO_SQUARE);

    assert_true(brd.fifty_move_counter == 0);
    assert_true(brd.ply == 0);
    assert_true(brd.history_ply == 0);

    assert_true(brd.castle_perm == (WQCA | WKCA | BQCA | BKCA));

    assert_true(brd.board_hash != 0);

}

void test_fen_parsing_initial_board_layout(void)
{

    // this is the initial bpard setup
    char *test_fen =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    struct board brd = init_game(test_fen);

    //print_board(brd);

    // verify the board
    assert_true(W_ROOK == brd.pieces[a1]);
    assert_true(W_KNIGHT == brd.pieces[b1]);
    assert_true(W_BISHOP == brd.pieces[c1]);
    assert_true(W_QUEEN == brd.pieces[d1]);
    assert_true(W_KING == brd.pieces[e1]);
    assert_true(W_BISHOP == brd.pieces[f1]);
    assert_true(W_KNIGHT == brd.pieces[g1]);
    assert_true(W_ROOK == brd.pieces[h1]);

    assert_true(W_PAWN == brd.pieces[a2]);
    assert_true(W_PAWN == brd.pieces[b2]);
    assert_true(W_PAWN == brd.pieces[c2]);
    assert_true(W_PAWN == brd.pieces[d2]);
    assert_true(W_PAWN == brd.pieces[e2]);
    assert_true(W_PAWN == brd.pieces[f2]);
    assert_true(W_PAWN == brd.pieces[g2]);
    assert_true(W_PAWN == brd.pieces[h2]);

    assert_true(B_PAWN == brd.pieces[a7]);
    assert_true(B_PAWN == brd.pieces[b7]);
    assert_true(B_PAWN == brd.pieces[c7]);
    assert_true(B_PAWN == brd.pieces[d7]);
    assert_true(B_PAWN == brd.pieces[e7]);
    assert_true(B_PAWN == brd.pieces[f7]);
    assert_true(B_PAWN == brd.pieces[g7]);
    assert_true(B_PAWN == brd.pieces[h7]);

    assert_true(B_ROOK == brd.pieces[a8]);
    assert_true(B_KNIGHT == brd.pieces[b8]);
    assert_true(B_BISHOP == brd.pieces[c8]);
    assert_true(B_QUEEN == brd.pieces[d8]);
    assert_true(B_KING == brd.pieces[e8]);
    assert_true(B_BISHOP == brd.pieces[f8]);
    assert_true(B_KNIGHT == brd.pieces[g8]);
    assert_true(B_ROOK == brd.pieces[h8]);

    // no piece present
    for (int i = a3; i < h6; i++) {
        assert_true(NO_PIECE == brd.pieces[i]);
    }

    assert_true(brd.side_to_move == WHITE);

    assert_true(brd.castle_perm == (WKCA | WQCA | BKCA | BQCA));

    assert_true(brd.en_passant == NO_SQUARE);

}

void test_fen_parsing_general_layout_1()
{

    // this is the initial board setup
    char *test_fen = "k7/8/8/4N3/8/8/8/3K4 b - - 13 56";

    struct board brd = init_game(test_fen);

    //print_board(brd);

    // verify the board
    assert_true(B_KING == brd.pieces[a8]);
    assert_true(W_KNIGHT == brd.pieces[e5]);
    assert_true(W_KING == brd.pieces[d1]);

    assert_true(brd.side_to_move == BLACK);

    assert_true(brd.castle_perm == 0);

    assert_true(brd.en_passant == NO_SQUARE);
}

void test_fen_parsing_general_layout_2()
{

    // this is the initial board setup
    char *test_fen =
        "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";

    struct board brd = init_game(test_fen);

    //print_board(brd);

    // verify the board
    assert_true(W_ROOK == brd.pieces[a1]);
    assert_true(W_KNIGHT == brd.pieces[b1]);
    assert_true(W_BISHOP == brd.pieces[c1]);
    assert_true(W_QUEEN == brd.pieces[d1]);
    assert_true(W_KING == brd.pieces[e1]);
    assert_true(W_BISHOP == brd.pieces[f1]);

    assert_true(W_ROOK == brd.pieces[h1]);

    assert_true(W_PAWN == brd.pieces[a2]);
    assert_true(W_PAWN == brd.pieces[b2]);
    assert_true(W_PAWN == brd.pieces[d2]);
    assert_true(W_PAWN == brd.pieces[f2]);
    assert_true(W_PAWN == brd.pieces[g2]);
    assert_true(W_PAWN == brd.pieces[h2]);

    assert_true(B_PAWN == brd.pieces[a7]);
    assert_true(B_PAWN == brd.pieces[b7]);
    assert_true(B_PAWN == brd.pieces[d7]);
    assert_true(B_PAWN == brd.pieces[f7]);
    assert_true(B_PAWN == brd.pieces[g7]);
    assert_true(B_PAWN == brd.pieces[h7]);

    assert_true(B_ROOK == brd.pieces[a8]);
    assert_true(B_KNIGHT == brd.pieces[b8]);
    assert_true(B_BISHOP == brd.pieces[c8]);
    assert_true(B_QUEEN == brd.pieces[d8]);
    assert_true(B_KING == brd.pieces[e8]);
    assert_true(B_BISHOP == brd.pieces[f8]);
    assert_true(B_ROOK == brd.pieces[h8]);

    assert_true(brd.side_to_move == BLACK);

    assert_true(brd.castle_perm == (WKCA | WQCA | BKCA | BQCA));

    assert_true(brd.en_passant == NO_SQUARE);


    // test pawn positions
    for(int f = FILE_A; f <= FILE_H; f++) {
        assert_true(brd.pawns_on_file[WHITE][f] == 1);
        assert_true(brd.pawns_on_file[BLACK][f] == 1);
    }
    assert_true(brd.pawns_on_rank[WHITE][RANK_2] == 7);
    assert_true(brd.pawns_on_rank[WHITE][RANK_4] == 1);

    assert_true(brd.pawns_on_rank[BLACK][RANK_7] == 7);
    assert_true(brd.pawns_on_rank[BLACK][RANK_5] == 1);

    assert_true(brd.pawn_control[WHITE][a3] == 1);
    assert_true(brd.pawn_control[WHITE][b3] == 2);
    assert_true(brd.pawn_control[WHITE][c3] == 2);
    assert_true(brd.pawn_control[WHITE][d3] == 1);
    assert_true(brd.pawn_control[WHITE][e3] == 2);
    assert_true(brd.pawn_control[WHITE][f3] == 1);
    assert_true(brd.pawn_control[WHITE][g3] == 2);
    assert_true(brd.pawn_control[WHITE][h3] == 1);
    assert_true(brd.pawn_control[WHITE][d5] == 1);
    assert_true(brd.pawn_control[WHITE][f5] == 1);

    assert_true(brd.pawn_control[BLACK][a6] == 1);
    assert_true(brd.pawn_control[BLACK][b6] == 1);
    assert_true(brd.pawn_control[BLACK][c6] == 2);
    assert_true(brd.pawn_control[BLACK][d6] == 1);
    assert_true(brd.pawn_control[BLACK][e6] == 2);
    assert_true(brd.pawn_control[BLACK][f6] == 2);
    assert_true(brd.pawn_control[BLACK][g6] == 2);
    assert_true(brd.pawn_control[BLACK][h6] == 1);
    assert_true(brd.pawn_control[BLACK][b4] == 1);
    assert_true(brd.pawn_control[BLACK][d4] == 1);


}


// en passant
void test_fen_parsing_general_layout_3()
{

    // this is the initial board setup
    char *test_fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 1";

    struct board brd = init_game(test_fen);

    //print_board(brd);

    assert_true(brd.side_to_move == WHITE);
    assert_true(brd.castle_perm == (WKCA | WQCA | BKCA | BQCA));

    assert_true(brd.en_passant == c6);
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
    assert_true(CHECK_BIT(test_brd, 22));
    assert_true(CHECK_BIT(test_brd, 23));
    clear_bit(&test_brd, 22);
    assert_true(CHECK_BIT(test_brd, 23));
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

    for (int i = 0; i < NUM_SQUARES; i++) {
        set_bit(&test_brd, i);
        assert_true(CHECK_BIT(test_brd, i));

        clear_bit(&test_brd, i);
        assert_false(CHECK_BIT(test_brd, i));
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
    assert_false(CHECK_BIT(brd, a1));
}

void board_test_fixture(void)
{

    test_fixture_start();	// starts a fixture

    run_test(test_initial_board_placement);

    run_test(test_setting_bits_in_a_board);
    run_test(test_checking_bits_in_a_board);
    run_test(test_clearing_bits_in_a_board);

    run_test(test_fen_parsing_initial_board_layout);
    run_test(test_fen_parsing_general_layout_1);
    run_test(test_fen_parsing_general_layout_2);
    run_test(test_fen_parsing_general_layout_3);
    run_test(test_bit_counting);
    run_test(test_LSB_clear);

    test_fixture_end();	// ends a fixture
}
