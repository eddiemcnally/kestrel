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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "seatest.h"
#include "types.h"
#include "board.h"
#include "pieces.h"
#include "board_utils.h"
#include "utils.h"
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

	uint64_t mask = GET_KING_OCC_MASK(a3);
	assert_true(CHECK_BIT(mask, a4));
	assert_true(CHECK_BIT(mask, a2));
	assert_true(CHECK_BIT(mask, b2));
	assert_true(CHECK_BIT(mask, b3));
	assert_true(CHECK_BIT(mask, b4));
	assert_true(5 == count_bits(mask));

	mask = GET_KING_OCC_MASK(h1);
	assert_true(CHECK_BIT(mask, g1));
	assert_true(CHECK_BIT(mask, g2));
	assert_true(CHECK_BIT(mask, h2));
	assert_true(3 == count_bits(mask));

	mask = GET_KING_OCC_MASK(d1);
	assert_true(CHECK_BIT(mask, c1));
	assert_true(CHECK_BIT(mask, e1));
	assert_true(CHECK_BIT(mask, c2));
	assert_true(CHECK_BIT(mask, d2));
	assert_true(CHECK_BIT(mask, e2));
	assert_true(5 == count_bits(mask));

	mask = GET_KING_OCC_MASK(d5);
	assert_true(CHECK_BIT(mask, c4));
	assert_true(CHECK_BIT(mask, d4));
	assert_true(CHECK_BIT(mask, e4));
	assert_true(CHECK_BIT(mask, c5));
	assert_true(CHECK_BIT(mask, e5));
	assert_true(CHECK_BIT(mask, c6));
	assert_true(CHECK_BIT(mask, d6));
	assert_true(CHECK_BIT(mask, e6));
	assert_true(8 == count_bits(mask));

	mask = GET_KING_OCC_MASK(a8);
	assert_true(CHECK_BIT(mask, a7));
	assert_true(CHECK_BIT(mask, b7));
	assert_true(CHECK_BIT(mask, b8));
	assert_true(3 == count_bits(mask));

	mask = GET_KING_OCC_MASK(h8);
	assert_true(CHECK_BIT(mask, g7));
	assert_true(CHECK_BIT(mask, h7));
	assert_true(CHECK_BIT(mask, g8));
	assert_true(3 == count_bits(mask));
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

	uint64_t mask = GET_ROOK_OCC_MASK(a1);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, a2));
	assert_true(CHECK_BIT(mask, a3));
	assert_true(CHECK_BIT(mask, a4));
	assert_true(CHECK_BIT(mask, a5));
	assert_true(CHECK_BIT(mask, a5));
	assert_true(CHECK_BIT(mask, a7));
	assert_true(CHECK_BIT(mask, a8));
	assert_true(CHECK_BIT(mask, b1));
	assert_true(CHECK_BIT(mask, c1));
	assert_true(CHECK_BIT(mask, d1));
	assert_true(CHECK_BIT(mask, e1));
	assert_true(CHECK_BIT(mask, f1));
	assert_true(CHECK_BIT(mask, g1));
	assert_true(CHECK_BIT(mask, h1));
	assert_true(14 == count_bits(mask));

	mask = GET_ROOK_OCC_MASK(h1);
	assert_true(CHECK_BIT(mask, h2));
	assert_true(CHECK_BIT(mask, h3));
	assert_true(CHECK_BIT(mask, h4));
	assert_true(CHECK_BIT(mask, h5));
	assert_true(CHECK_BIT(mask, h5));
	assert_true(CHECK_BIT(mask, h7));
	assert_true(CHECK_BIT(mask, h8));
	assert_true(CHECK_BIT(mask, a1));
	assert_true(CHECK_BIT(mask, b1));
	assert_true(CHECK_BIT(mask, c1));
	assert_true(CHECK_BIT(mask, d1));
	assert_true(CHECK_BIT(mask, e1));
	assert_true(CHECK_BIT(mask, f1));
	assert_true(CHECK_BIT(mask, g1));
	assert_true(14 == count_bits(mask));

	mask = GET_ROOK_OCC_MASK(d5);
	assert_true(CHECK_BIT(mask, a5));
	assert_true(CHECK_BIT(mask, b5));
	assert_true(CHECK_BIT(mask, c5));
	assert_true(CHECK_BIT(mask, e5));
	assert_true(CHECK_BIT(mask, f5));
	assert_true(CHECK_BIT(mask, g5));
	assert_true(CHECK_BIT(mask, h5));
	assert_true(CHECK_BIT(mask, d1));
	assert_true(CHECK_BIT(mask, d2));
	assert_true(CHECK_BIT(mask, d3));
	assert_true(CHECK_BIT(mask, d4));
	assert_true(CHECK_BIT(mask, d6));
	assert_true(CHECK_BIT(mask, d7));
	assert_true(CHECK_BIT(mask, d8));
	assert_true(14 == count_bits(mask));

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

	uint64_t mask = GET_WHITE_PAWN_OCC_MASK(a2);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, b3));
	assert_true(1 == count_bits(mask));

	mask = GET_WHITE_PAWN_OCC_MASK(h2);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, g3));
	assert_true(1 == count_bits(mask));

	mask = GET_WHITE_PAWN_OCC_MASK(d2);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, c3));
	assert_true(CHECK_BIT(mask, e3));
	assert_true(2 == count_bits(mask));

	mask = GET_WHITE_PAWN_OCC_MASK(g6);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, f7));
	assert_true(CHECK_BIT(mask, h7));
	assert_true(2 == count_bits(mask));

	mask = GET_WHITE_PAWN_OCC_MASK(c7);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, b8));
	assert_true(CHECK_BIT(mask, d8));
	assert_true(2 == count_bits(mask));

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

	uint64_t mask = GET_BLACK_PAWN_OCC_MASK(a7);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, b6));
	assert_true(1 == count_bits(mask));

	mask = GET_BLACK_PAWN_OCC_MASK(h2);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, g1));
	assert_true(1 == count_bits(mask));

	mask = GET_BLACK_PAWN_OCC_MASK(d2);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, c1));
	assert_true(CHECK_BIT(mask, e1));
	assert_true(2 == count_bits(mask));

	mask = GET_BLACK_PAWN_OCC_MASK(g6);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, f5));
	assert_true(CHECK_BIT(mask, h5));
	assert_true(2 == count_bits(mask));

	mask = GET_BLACK_PAWN_OCC_MASK(c7);
	//printf("0x%016llx\n", mask);  
	assert_true(CHECK_BIT(mask, b6));
	assert_true(CHECK_BIT(mask, d6));
	assert_true(2 == count_bits(mask));

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

	uint64_t mask = GET_KNIGHT_OCC_MASK(a1);
	assert_true(CHECK_BIT(mask, c2));
	assert_true(CHECK_BIT(mask, b3));
	assert_true(2 == count_bits(mask));

	mask = GET_KNIGHT_OCC_MASK(d1);
	assert_true(CHECK_BIT(mask, b2));
	assert_true(CHECK_BIT(mask, c3));
	assert_true(CHECK_BIT(mask, e3));
	assert_true(CHECK_BIT(mask, f2));
	assert_true(4 == count_bits(mask));

	mask = GET_KNIGHT_OCC_MASK(h1);
	assert_true(CHECK_BIT(mask, f2));
	assert_true(CHECK_BIT(mask, g3));
	assert_true(2 == count_bits(mask));

	mask = GET_KNIGHT_OCC_MASK(d5);
	assert_true(CHECK_BIT(mask, c3));
	assert_true(CHECK_BIT(mask, b4));
	assert_true(CHECK_BIT(mask, b6));
	assert_true(CHECK_BIT(mask, c7));
	assert_true(CHECK_BIT(mask, e7));
	assert_true(CHECK_BIT(mask, f6));
	assert_true(CHECK_BIT(mask, e3));
	assert_true(CHECK_BIT(mask, f4));
	assert_true(8 == count_bits(mask));

	mask = GET_KNIGHT_OCC_MASK(a8);
	assert_true(CHECK_BIT(mask, b6));
	assert_true(CHECK_BIT(mask, c7));
	assert_true(2 == count_bits(mask));

	mask = GET_KNIGHT_OCC_MASK(h8);
	assert_true(CHECK_BIT(mask, f7));
	assert_true(CHECK_BIT(mask, g6));
	assert_true(2 == count_bits(mask));
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

	uint64_t mask = GET_QUEEN_OCC_MASK(h8);
	assert_true(CHECK_BIT(mask, h7));
	assert_true(CHECK_BIT(mask, h6));
	assert_true(CHECK_BIT(mask, h5));
	assert_true(CHECK_BIT(mask, h4));
	assert_true(CHECK_BIT(mask, h3));
	assert_true(CHECK_BIT(mask, h2));
	assert_true(CHECK_BIT(mask, h1));
	assert_true(CHECK_BIT(mask, g8));
	assert_true(CHECK_BIT(mask, f8));
	assert_true(CHECK_BIT(mask, e8));
	assert_true(CHECK_BIT(mask, d8));
	assert_true(CHECK_BIT(mask, c8));
	assert_true(CHECK_BIT(mask, b8));
	assert_true(CHECK_BIT(mask, a8));
	assert_true(CHECK_BIT(mask, g7));
	assert_true(CHECK_BIT(mask, f6));
	assert_true(CHECK_BIT(mask, e5));
	assert_true(CHECK_BIT(mask, d4));
	assert_true(CHECK_BIT(mask, c3));
	assert_true(CHECK_BIT(mask, b2));
	assert_true(CHECK_BIT(mask, a1));
	assert_true(21 == count_bits(mask));

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

	uint64_t mask = GET_BISHOP_OCC_MASK(h8);
	assert_true(CHECK_BIT(mask, g7));
	assert_true(CHECK_BIT(mask, f6));
	assert_true(CHECK_BIT(mask, e5));
	assert_true(CHECK_BIT(mask, d4));
	assert_true(CHECK_BIT(mask, c3));
	assert_true(CHECK_BIT(mask, b2));
	assert_true(CHECK_BIT(mask, a1));
	assert_true(7 == count_bits(mask));

	mask = GET_BISHOP_OCC_MASK(d5);
	assert_true(CHECK_BIT(mask, c4));
	assert_true(CHECK_BIT(mask, b3));
	assert_true(CHECK_BIT(mask, a2));
	assert_true(CHECK_BIT(mask, e6));
	assert_true(CHECK_BIT(mask, f7));
	assert_true(CHECK_BIT(mask, g8));

	assert_true(CHECK_BIT(mask, c6));
	assert_true(CHECK_BIT(mask, b7));
	assert_true(CHECK_BIT(mask, a8));
	assert_true(CHECK_BIT(mask, e4));
	assert_true(CHECK_BIT(mask, f3));
	assert_true(CHECK_BIT(mask, g2));
	assert_true(CHECK_BIT(mask, h1));

	assert_true(13 == count_bits(mask));

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

	test_fixture_end();	// ends a fixture
}
