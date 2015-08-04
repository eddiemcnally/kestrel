/*
 * makemove.c
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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "hashkeys.h"




static void update_piece_hash(struct board *brd, enum piece pce, enum square sq);
static void update_castle_hash(struct board *brd);
static void update_side_hash(struct board *brd);
static void update_EP_hash(struct board *brd);

//bit mask for castle permissions
static const int castle_permission_mask[NUM_SQUARES] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15
};



void clear_piece(struct board *brd, enum square sq){

	enum piece pce = brd->pieces[sq];
	enum colour col = IS_WHITE(pce) ? WHITE : BLACK);

	update_piece_hash(brd, pce, sq);

	brd->pieces[sq] = NO_PIECE;

	bld->material[col] -= piece_values[pce];

	if (is_big_piece(pce)){
		bld->big_pieces[col]--;
		if (is_major_piece(pce)){
			bld->major_pieces[col]--;
		} else {
			bld->minor_pieces[col]--;
		}
	}

	clear_bit(&brd->bitboards[pce], sq);
	clear_bit(&brd->board, sq);

	brd->pce_num[pce]--;

	U64 new_hash = get_position_hashkey(brd);

	brd->board_hash = new_hash;
}
















static inline void update_piece_hash(struct board *brd, enum piece pce, enum square sq){
	brd->board_hash ^= get_hash(pce, sq);
}

static inline void update_castle_hash(struct board *brd){
	brd->board_hash ^= get_castle_key(brd->castle_perm);
}


static inline void update_side_hash(struct board *brd){
	brd->b oard_hash ^= get_side_key();
}

static inline void update_EP_hash(struct board *brd){
	brd->board_hash ^= get_side_key();
}
