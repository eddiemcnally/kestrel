/*
 * occupancy_mask_test_fixture.h
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
#ifndef _OCCUPANCY_MASK_TEST_FIXTURE_H
#define _OCCUPANCY_MASK_TEST_FIXTURE_H


void test_king_occupancy_mask(void);
void test_rook_occupancy_mask(void);
void test_knight_occupancy_mask(void);
void test_bishop_occupancy_mask(void);
void test_queen_occupancy_mask(void);
void test_white_pawn_occupancy_mask(void);
void test_black_pawn_occupancy_mask(void);
void occupancy_mask_test_fixture(void);


#endif
