
/*
 * piece_test_fixture.h
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

#ifndef _PIECE_TEST_FIXTURE_H
#define _PIECE_TEST_FIXTURE_H


void test_rank_file_macros(void);
void test_piece_colour_macro(void);
void test_piece_classification_IS_BIG_PIECE(void);
void test_piece_classification_IS_MAJOR_PIECE(void);
void test_piece_classification_IS_MINOR_PIECE(void);
void test_piece_to_char_conversion(void);
void piece_test_fixture(void);

#endif