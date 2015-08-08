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
#include "attack.h"
#include "board_utils.h"
#include "move.h"
#include "hashkeys.h"




void update_piece_hash(struct board *brd, enum piece pce, enum square sq);
void update_castle_hash(struct board *brd);
void update_side_hash(struct board *brd);
void update_EP_hash(struct board *brd);

//bit mask for castle permissions
static const U8 castle_permission_mask[NUM_SQUARES] = {
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

	brd->pieces[sq] = pce;

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

	enum piece pce = get_piece_at_square(brd, from);

	update_piece_hash(brd, pce, from);
	brd->pieces[from] = NO_PIECE;
	clear_bit(&brd->bitboards[pce], from);
	clear_bit(&brd->board, from);


	update_piece_hash(brd, pce, to);
	brd->pieces[to] = pce;
	set_bit(&brd->bitboards[pce], to);
	set_bit(&brd->board, to);

}



// return false if move is invalid, true otherwise
bool make_move(struct board *brd, mv_bitmap mv){
	ASSERT_BOARD_OK(brd);

	enum square from = FROMSQ(mv);
	enum square to = TOSQ(mv);

	enum colour side = brd->side_to_move;

	brd->history[brd->history_ply].board_hash  = brd->board_hash;

	if(mv & MFLAG_EN_PASSANT) {
        if(side == WHITE) {
            clear_piece(brd, to-8);
        } else {
            clear_piece(brd, to+8);
        }
    } else if (mv & MFLAG_CASTLE) {
        switch(to) {
            case c1:
                move_piece(brd, a1, d1);
				break;
            case c8:
				move_piece(brd, a8, d8);
				break;
            case g1:
				move_piece(brd, h1, f1);
				break;
            case g8:
				move_piece(brd, h8, f8);
				break;
            default:
				assert(false);
				break;
        }
    }


	if (brd->en_passant != NO_SQUARE){
		update_EP_hash(brd);
	}
	update_castle_hash(brd);

	// set up history
	brd->history[brd->history_ply].move = mv;
	brd->history[brd->history_ply].fifty_move_counter = brd->fifty_move_counter;
	brd->history[brd->history_ply].en_passant = brd->en_passant;
	brd->history[brd->history_ply].castle_perm = brd->castle_perm;

	brd->castle_perm &= castle_permission_mask[from];
	brd->castle_perm &= castle_permission_mask[to];
	brd->en_passant = NO_SQUARE;

	update_castle_hash(brd);

	enum piece captured = CAPTURED(mv);
	brd->fifty_move_counter++;

	if (captured != NO_PIECE){
		clear_piece(brd, to);
		brd->fifty_move_counter = 0;
	}

	brd->ply++;
	brd->history_ply++;

	enum piece pce_being_moved = brd->pieces[from];

	if (is_pawn(pce_being_moved)){
		brd->fifty_move_counter = 0;

		if (mv & MFLAG_PAWN_START){
			if (side == WHITE){
				brd->en_passant = from + 8;
			} else {
				brd->en_passant = from - 8;
			}
			update_EP_hash(brd);
		}
	}

	move_piece(brd, from, to);

	enum piece promoted = PROMOTED(mv);
	if (promoted != NO_PIECE){
		clear_piece(brd, to);
		add_piece(brd, promoted, to);
	}

	// flip side
	brd->side_to_move = FLIP_SIDE(brd->side_to_move);
	update_side_hash(brd);

	// check if move is valid (ie, king in check)
	enum piece king = (side == BLACK) ? B_KING : W_KING;

	U64 bb_king = brd->bitboards[king];

	enum square king_sq = POP(&bb_king);


	// side is already flipped above, so use that as the attacking side
	if (is_sq_attacked(king_sq, brd->side_to_move, brd)){
		return false;
	}

	return true;

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
