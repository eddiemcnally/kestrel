/*
 * makemove.h
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
#pragma once

bool make_move(struct board *brd, mv_bitmap mv);
void take_move(struct board *brd);
void flip_sides(struct board *brd);
void set_bit(uint64_t * brd, enum square sq);
void set_bits(uint64_t * brd, uint64_t bitmask);
void clear_bit(uint64_t * brd, enum square sq);
void move_piece(struct board *brd, enum square from, enum square to);

void remove_piece_from_board(struct board *brd, enum square sq);
void add_piece_to_board(struct board *brd, enum piece pce, enum square sq);


