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
#include "pieces.h"
#include "board.h"
#include "hashkeys.h"




void update_piece_hash(struct board *brd, enum piece pce, enum square sq);
void update_castle_hash(struct board *brd);
void update_side_hash(struct board *brd);
void update_EP_hash(struct board *brd);

//bit mask for castle permissions
static const int castle_permission_mask[NUM_SQUARES] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11
};



void clear_piece(struct board *brd, enum square sq){

	enum piece pce = brd->pieces[sq];
	enum colour col = IS_WHITE(pce) ? WHITE : BLACK;

	update_piece_hash(brd, pce, sq);

	brd->pieces[sq] = NO_PIECE;

	brd->material[col] -= get_piece_value(pce);

	if (is_big_piece(pce)){
		brd->big_pieces[col]--;

		if (is_major_piece(pce)){
			brd->major_pieces[col]--;
		}
		if (is_minor_piece(pce)){
			brd->minor_pieces[col]--;
		}
	}

	// remove piece from bitboards
	clear_bit(&brd->bitboards[pce], sq);
	clear_bit(&brd->board, sq);

	brd->pce_num[pce]--;

	U64 new_hash = get_position_hashkey(brd);

	brd->board_hash = new_hash;
}



void add_piece(struct board *brd, enum piece pce, enum square sq){

	enum colour col = IS_WHITE(pce) ? WHITE : BLACK;

	update_piece_hash(brd, pce, sq);

	brd->pieces[sq] = sq;

	brd->material[col] += get_piece_value(pce);

	if (is_big_piece(pce)){
		brd->big_pieces[col]++;
		if (is_major_piece(pce)){
			brd->major_pieces[col]++;
		}
		if (is_minor_piece(pce)){
			brd->minor_pieces[col]++;
		}
	}

	// set piece on bitboards
	set_bit(&brd->bitboards[pce], sq);
	set_bit(&brd->board, sq);

	brd->pce_num[pce]++;

	U64 new_hash = get_position_hashkey(brd);
	brd->board_hash = new_hash;

}



 void move_piece(struct board *brd, enum square from, enum square to){

	// TODO
	// optimise this function, rather than call clear+add
	enum piece pce = get_piece_at_square(brd, from);

	clear_piece(brd, from);
	add_piece(brd, pce, to);

}








inline void update_piece_hash(struct board *brd, enum piece pce, enum square sq){
	brd->board_hash ^= get_hash(pce, sq);
}

inline void update_castle_hash(struct board *brd){
	brd->board_hash ^= get_castle_key(brd->castle_perm);
}


inline void update_side_hash(struct board *brd){
	brd->board_hash ^= get_side_key();
}

inline void update_EP_hash(struct board *brd){
	brd->board_hash ^= get_side_key();
}
