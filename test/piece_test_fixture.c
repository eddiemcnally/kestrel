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

void test_asserting()
{
	//assert_true( 1 == 1);
	//assert_false(1 == 2);
	//assert_int_equal(1, 1);
}

void test_assert_fails()
{
	//assert_true( 1 == 2);
}

void piece_test_fixture( void )
{
	test_fixture_start();
	//run_test(test_asserting);
	//run_test(test_assert_fails);
	test_fixture_end();
}
