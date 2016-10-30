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
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "seatest.h"
#include "types.h"
#include "board.h"
#include "pieces.h"
#include "attack.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "tt.h"
#include "board_utils.h"
#include "utils.h"

void test_bit_reversal(void);
void utils_test_fixture(void);
void test_clear_MSB(void);
void test_flip_side(void);

void test_bit_reversal(void)
{

    uint64_t test_word = 0xFA340A7314DA;
    uint64_t reversed = reverse_bits(test_word);
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

void test_flip_side(void)
{
    enum colour fs = GET_OPPOSITE_SIDE(WHITE);
    assert_true(fs == BLACK);

    fs = GET_OPPOSITE_SIDE(fs);
    assert_true(fs == WHITE);

    fs = GET_OPPOSITE_SIDE(fs);
    assert_true(fs == BLACK);
}






void utils_test_fixture(void)
{

    test_fixture_start();

    run_test(test_bit_reversal);
    run_test(test_flip_side);

    test_fixture_end();
}
