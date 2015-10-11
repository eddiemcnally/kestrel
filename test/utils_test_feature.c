/*
 * utils_test_fixture.c
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
#include "attack.h"
#include "pv_table.h"
#include "board_utils.h"
#include "utils.h"

void test_bit_reversal(void);
void utils_test_fixture(void);
void test_clear_MSB(void);

void test_bit_reversal(void)
{

	U64 test_word = 0xFA340A7314DA;
	U64 reversed = reverse_bits(test_word);
	assert_true(reversed == 0x5B28CE502C5F0000);

	test_word = 0x6F43DA3E2;
	reversed = reverse_bits(test_word);
	assert_true(reversed == 0x47C5BC2F60000000);

	// swap the above to confirm
	test_word = 0x47C5BC2F60000000;
	reversed = reverse_bits(test_word);
	assert_true(reversed == 0x6F43DA3E2);

	test_word = 0x3AD1FDE008934A;
	reversed = reverse_bits(test_word);
	assert_true(reversed == 0x52C91007BF8B5C00);

	test_word = 0x00000000000000AA;
	reversed = reverse_bits(test_word);
	assert_true(reversed == 0x5500000000000000);

}

void test_clear_MSB(void)
{
	U64 bb = 0x54673;
	clear_MSB_to_inclusive_bit(&bb, 10);

	assert_true(bb == 0x273);
}

void test_find_LSB()
{
	U64 test_val = 0x526200817;
	U8 b = get_LSB_index(test_val);
	assert_true(b == 0);

	test_val = 0x29310040;
	b = get_LSB_index(test_val);
	assert_true(b == 6);

	test_val = 0x1;
	b = get_LSB_index(test_val);
	assert_true(b == 0);

}

void test_find_MSB()
{
	U64 test_val = 0x526200817;
	U8 b = get_MSB_index(test_val);
	assert_true(b == 34);

	test_val = 0x29310040;
	b = get_MSB_index(test_val);
	assert_true(b == 29);

	test_val = 0x8000000000000000;
	b = get_MSB_index(test_val);
	assert_true(b == 63);

	test_val = 0x1;
	b = get_MSB_index(test_val);
	assert_true(b == 0);

}

void test_clear_LSB(void)
{
	U64 bb = 0x54673;
	clear_LSB_to_inclusive_bit(&bb, 5);
	assert_true(bb == 0x54640);
}

void test_flip_side()
{

	enum colour fs = FLIP_SIDE(WHITE);
	assert_true(fs == BLACK);

	fs = FLIP_SIDE(fs);
	assert_true(fs == WHITE);

	fs = FLIP_SIDE(fs);
	assert_true(fs == BLACK);
}



void test_pv_table(){

	struct pv_table *table = create_pv_table();
	
	// add a multiple of the table size to force key collisions
	for (U64 i = 0; i < (4 * NUM_PV_ENTRIES); i++){
		add_move(table, i, (U32)((i + 1)));
	}
	
	// now verify all additions
	for (U64 i = 0; i < (4 * NUM_PV_ENTRIES); i++){
		mv_bitmap mv = find_move(table, i);
		
		assert_true(mv == (i + 1));
	}
	
	dispose_table(table);
	
}




void utils_test_fixture(void)
{

	test_fixture_start();

	run_test(test_find_LSB);
	run_test(test_find_MSB);
	run_test(test_bit_reversal);
	run_test(test_clear_MSB);
	run_test(test_clear_LSB);
	run_test(test_flip_side);
	run_test(test_pv_table);

	test_fixture_end();
}





