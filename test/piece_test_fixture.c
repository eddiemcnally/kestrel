/*
 * piece_test_fixture.c
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chess is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */  
#include "seatest.h"
#include "board.h"
#include "pieces.h"
void test_piece_to_char_conversion()
{
			assert_true('P' == get_piece_label(W_PAWN));
	assert_true('p' == get_piece_label(B_PAWN));
	assert_true('R' == get_piece_label(W_ROOK));
	assert_true('r' == get_piece_label(B_ROOK));
	assert_true('B' == get_piece_label(W_BISHOP));
	assert_true('b' == get_piece_label(B_BISHOP));
	assert_true('N' == get_piece_label(W_KNIGHT));
	assert_true('n' == get_piece_label(B_KNIGHT));
	assert_true('Q' == get_piece_label(W_QUEEN));
	assert_true('q' == get_piece_label(B_QUEEN));
	assert_true('K' == get_piece_label(W_KING));
	assert_true('k' == get_piece_label(B_KING));
} void piece_test_fixture(void) 
{
    test_fixture_start();
    run_test(test_piece_to_char_conversion);
    test_fixture_end();
} 
