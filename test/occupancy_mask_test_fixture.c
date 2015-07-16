/*
 * occupancy_mask_test_fixture.c
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
#include "board.h"
#include "pieces.h"
#include "board_utils.h"
#include "occupancy_mask.h"

void test_king_occupancy_mask(void);
void test_rook_occupancy_mask(void);
void test_knight_occupancy_mask(void);
void test_bishop_occupancy_mask(void);
void test_queen_occupancy_mask(void);
void test_white_pawn_occupancy_mask(void);
void test_black_pawn_occupancy_mask(void);
void occupancy_mask_test_fixture(void);

/**
 * Tests the occupancy masks for a bishop
 */
void test_king_occupancy_mask(void)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    U64 mask = GET_KING_OCC_MASK(a3);
    assert_true(check_bit(&mask, a4));
    assert_true(check_bit(&mask, a2));
    assert_true(check_bit(&mask, b2));
    assert_true(check_bit(&mask, b3));
    assert_true(check_bit(&mask, b4));
    assert_true(5 == CNT(mask));

    mask = GET_KING_OCC_MASK(h1);
    assert_true(check_bit(&mask, g1));
    assert_true(check_bit(&mask, g2));
    assert_true(check_bit(&mask, h2));
    assert_true(3 == CNT(mask));

    mask = GET_KING_OCC_MASK(d1);
    assert_true(check_bit(&mask, c1));
    assert_true(check_bit(&mask, e1));
    assert_true(check_bit(&mask, c2));
    assert_true(check_bit(&mask, d2));
    assert_true(check_bit(&mask, e2));
    assert_true(5 == CNT(mask));

    mask = GET_KING_OCC_MASK(d5);
    assert_true(check_bit(&mask, c4));
    assert_true(check_bit(&mask, d4));
    assert_true(check_bit(&mask, e4));
    assert_true(check_bit(&mask, c5));
    assert_true(check_bit(&mask, e5));
    assert_true(check_bit(&mask, c6));
    assert_true(check_bit(&mask, d6));
    assert_true(check_bit(&mask, e6));
    assert_true(8 == CNT(mask));

    mask = GET_KING_OCC_MASK(a8);
    assert_true(check_bit(&mask, a7));
    assert_true(check_bit(&mask, b7));
    assert_true(check_bit(&mask, b8));
    assert_true(3 == CNT(mask));

    mask = GET_KING_OCC_MASK(h8);
    assert_true(check_bit(&mask, g7));
    assert_true(check_bit(&mask, h7));
    assert_true(check_bit(&mask, g8));
    assert_true(3 == CNT(mask));
}

void test_rook_occupancy_mask(void)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    U64 mask = GET_ROOK_OCC_MASK(a1);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, a2));
    assert_true(check_bit(&mask, a3));
    assert_true(check_bit(&mask, a4));
    assert_true(check_bit(&mask, a5));
    assert_true(check_bit(&mask, a5));
    assert_true(check_bit(&mask, a7));
    assert_true(check_bit(&mask, a8));
    assert_true(check_bit(&mask, b1));
    assert_true(check_bit(&mask, c1));
    assert_true(check_bit(&mask, d1));
    assert_true(check_bit(&mask, e1));
    assert_true(check_bit(&mask, f1));
    assert_true(check_bit(&mask, g1));
    assert_true(check_bit(&mask, h1));
    assert_true(14 == CNT(mask));

    mask = GET_ROOK_OCC_MASK(h1);
    assert_true(check_bit(&mask, h2));
    assert_true(check_bit(&mask, h3));
    assert_true(check_bit(&mask, h4));
    assert_true(check_bit(&mask, h5));
    assert_true(check_bit(&mask, h5));
    assert_true(check_bit(&mask, h7));
    assert_true(check_bit(&mask, h8));
    assert_true(check_bit(&mask, a1));
    assert_true(check_bit(&mask, b1));
    assert_true(check_bit(&mask, c1));
    assert_true(check_bit(&mask, d1));
    assert_true(check_bit(&mask, e1));
    assert_true(check_bit(&mask, f1));
    assert_true(check_bit(&mask, g1));
    assert_true(14 == CNT(mask));

    mask = GET_ROOK_OCC_MASK(d5);
    assert_true(check_bit(&mask, a5));
    assert_true(check_bit(&mask, b5));
    assert_true(check_bit(&mask, c5));
    assert_true(check_bit(&mask, e5));
    assert_true(check_bit(&mask, f5));
    assert_true(check_bit(&mask, g5));
    assert_true(check_bit(&mask, h5));
    assert_true(check_bit(&mask, d1));
    assert_true(check_bit(&mask, d2));
    assert_true(check_bit(&mask, d3));
    assert_true(check_bit(&mask, d4));
    assert_true(check_bit(&mask, d6));
    assert_true(check_bit(&mask, d7));
    assert_true(check_bit(&mask, d8));
    assert_true(14 == CNT(mask));

}

void test_white_pawn_occupancy_mask(void)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    U64 mask = GET_WHITE_PAWN_OCC_MASK(a2);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, b3));
    assert_true(1 == CNT(mask));

    mask = GET_WHITE_PAWN_OCC_MASK(h2);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, g3));
    assert_true(1 == CNT(mask));

    mask = GET_WHITE_PAWN_OCC_MASK(d2);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, c3));
    assert_true(check_bit(&mask, e3));
    assert_true(2 == CNT(mask));

    mask = GET_WHITE_PAWN_OCC_MASK(g6);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, f7));
    assert_true(check_bit(&mask, h7));
    assert_true(2 == CNT(mask));

    mask = GET_WHITE_PAWN_OCC_MASK(c7);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, b8));
    assert_true(check_bit(&mask, d8));
    assert_true(2 == CNT(mask));

}

void test_black_pawn_occupancy_mask(void)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    U64 mask = GET_BLACK_PAWN_OCC_MASK(a7);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, b6));
    assert_true(1 == CNT(mask));

    mask = GET_BLACK_PAWN_OCC_MASK(h2);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, g1));
    assert_true(1 == CNT(mask));

    mask = GET_BLACK_PAWN_OCC_MASK(d2);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, c1));
    assert_true(check_bit(&mask, e1));
    assert_true(2 == CNT(mask));

    mask = GET_BLACK_PAWN_OCC_MASK(g6);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, f5));
    assert_true(check_bit(&mask, h5));
    assert_true(2 == CNT(mask));

    mask = GET_BLACK_PAWN_OCC_MASK(c7);
    //printf("0x%016llx\n", mask);  
    assert_true(check_bit(&mask, b6));
    assert_true(check_bit(&mask, d6));
    assert_true(2 == CNT(mask));

}

void test_knight_occupancy_mask(void)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    U64 mask = GET_KNIGHT_OCC_MASK(a1);
    assert_true(check_bit(&mask, c2));
    assert_true(check_bit(&mask, b3));
    assert_true(2 == CNT(mask));

    mask = GET_KNIGHT_OCC_MASK(d1);
    assert_true(check_bit(&mask, b2));
    assert_true(check_bit(&mask, c3));
    assert_true(check_bit(&mask, e3));
    assert_true(check_bit(&mask, f2));
    assert_true(4 == CNT(mask));

    mask = GET_KNIGHT_OCC_MASK(h1);
    assert_true(check_bit(&mask, f2));
    assert_true(check_bit(&mask, g3));
    assert_true(2 == CNT(mask));

    mask = GET_KNIGHT_OCC_MASK(d5);
    assert_true(check_bit(&mask, c3));
    assert_true(check_bit(&mask, b4));
    assert_true(check_bit(&mask, b6));
    assert_true(check_bit(&mask, c7));
    assert_true(check_bit(&mask, e7));
    assert_true(check_bit(&mask, f6));
    assert_true(check_bit(&mask, e3));
    assert_true(check_bit(&mask, f4));
    assert_true(8 == CNT(mask));

    mask = GET_KNIGHT_OCC_MASK(a8);
    assert_true(check_bit(&mask, b6));
    assert_true(check_bit(&mask, c7));
    assert_true(2 == CNT(mask));

    mask = GET_KNIGHT_OCC_MASK(h8);
    assert_true(check_bit(&mask, f7));
    assert_true(check_bit(&mask, g6));
    assert_true(2 == CNT(mask));
}

void test_queen_occupancy_mask(void)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    U64 mask = GET_QUEEN_OCC_MASK(h8);
    assert_true(check_bit(&mask, h7));
    assert_true(check_bit(&mask, h6));
    assert_true(check_bit(&mask, h5));
    assert_true(check_bit(&mask, h4));
    assert_true(check_bit(&mask, h3));
    assert_true(check_bit(&mask, h2));
    assert_true(check_bit(&mask, h1));
    assert_true(check_bit(&mask, g8));
    assert_true(check_bit(&mask, f8));
    assert_true(check_bit(&mask, e8));
    assert_true(check_bit(&mask, d8));
    assert_true(check_bit(&mask, c8));
    assert_true(check_bit(&mask, b8));
    assert_true(check_bit(&mask, a8));
    assert_true(check_bit(&mask, g7));
    assert_true(check_bit(&mask, f6));
    assert_true(check_bit(&mask, e5));
    assert_true(check_bit(&mask, d4));
    assert_true(check_bit(&mask, c3));
    assert_true(check_bit(&mask, b2));
    assert_true(check_bit(&mask, a1));
    assert_true(21 == CNT(mask));

}

void test_bishop_occupancy_mask(void)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    U64 mask = GET_BISHOP_OCC_MASK(h8);
    assert_true(check_bit(&mask, g7));
    assert_true(check_bit(&mask, f6));
    assert_true(check_bit(&mask, e5));
    assert_true(check_bit(&mask, d4));
    assert_true(check_bit(&mask, c3));
    assert_true(check_bit(&mask, b2));
    assert_true(check_bit(&mask, a1));
    assert_true(7 == CNT(mask));

    mask = GET_BISHOP_OCC_MASK(d5);
    assert_true(check_bit(&mask, c4));
    assert_true(check_bit(&mask, b3));
    assert_true(check_bit(&mask, a2));
    assert_true(check_bit(&mask, e6));
    assert_true(check_bit(&mask, f7));
    assert_true(check_bit(&mask, g8));

    assert_true(check_bit(&mask, c6));
    assert_true(check_bit(&mask, b7));
    assert_true(check_bit(&mask, a8));
    assert_true(check_bit(&mask, e4));
    assert_true(check_bit(&mask, f3));
    assert_true(check_bit(&mask, g2));
    assert_true(check_bit(&mask, h1));

    assert_true(13 == CNT(mask));

}

void occupancy_mask_test_fixture(void)
{
    test_fixture_start();	// starts a fixture

    run_test(test_king_occupancy_mask);
    run_test(test_knight_occupancy_mask);
    run_test(test_rook_occupancy_mask);
    run_test(test_queen_occupancy_mask);
    run_test(test_bishop_occupancy_mask);
    run_test(test_white_pawn_occupancy_mask);
    run_test(test_black_pawn_occupancy_mask);

    test_fixture_end();		// ends a fixture
}
