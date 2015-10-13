
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


#ifndef _MOVE_TEST_FIXTURE_H
#define _MOVE_TEST_FIXTURE_H


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
void test_add_piece(void);
void test_move_piece(void);


#endif
