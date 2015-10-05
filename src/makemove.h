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
#ifndef _MAKE_MOVE_H_
#define _MAKE_MOVE_H_

bool make_move(struct board *brd, mv_bitmap mv);
void take_move(struct board *brd);

void move_piece(struct board *brd, enum square from, enum square to);

void update_piece_hash(struct board *brd, enum piece pce, enum square sq);
void update_castle_hash(struct board *brd);
void update_side_hash(struct board *brd);
void update_EP_hash(struct board *brd);

#endif
