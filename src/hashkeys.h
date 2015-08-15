/*
 * hashkeys.h
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
#ifndef _HASHKEYS_H_
#define _HASHKEYS_H_

void init_hash_keys(void);
U64 get_position_hash(const struct board *brd);
U64 get_castle_hash(U8 castle_map);
U64 get_side_hash(void);
U64 get_en_passant_hash(enum square sq);
U64 get_piece_hash(enum piece pce, enum square sq);

#endif
