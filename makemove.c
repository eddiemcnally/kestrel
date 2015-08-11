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
#include "makemove.h"
#include "board_utils.h"
#include "move.h"
#include "hashkeys.h"




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

	overlay_colours(brd, GET_COLOUR(pce));
	overlay_boards(brd);
}



// return false if move is invalid, true otherwise
bool make_move(struct board *brd, mv_bitmap mv){

	ASSERT_BOARD_OK(brd);

	enum square from = FROMSQ(mv);
	enum square to = TOSQ(mv);

	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	print_move_details(mv, 0);
	printf("from : %s\n", print_square(from));
	printf("to   : %s\n", print_square(to));

	//print_board(brd);




	assert(from >= a1 && from <= h8);
	assert(to >= a1 && to <= h8);


	enum piece pce = get_piece_at_square(brd, from);
	enum colour side = brd->side_to_move;

	assert(GET_COLOUR(pce) == side);

	brd->history[brd->history_ply].board_hash  = brd->board_hash;

	if(mv & MFLAG_EN_PASSANT) {
        if(side == WHITE) {
            remove_piece_from_board(brd, pce, to-8);
        } else {
            remove_piece_from_board(brd, pce, to+8);
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
		remove_piece_from_board(brd, captured, to);
		brd->fifty_move_counter = 0;
	}

	brd->ply++;
	brd->history_ply++;

	enum piece pce_being_moved = brd->pieces[from];

	if (IS_PAWN(pce_being_moved)){
		brd->fifty_move_counter = 0;

		if (mv & MFLAG_PAWN_START){
			if (side == WHITE){
				brd->en_passant = from + 8;
				assert(GET_RANK(from + 8) == RANK_3);
			} else {
				brd->en_passant = from - 8;
				assert(GET_RANK(from - 8) == RANK_6);
			}
			update_EP_hash(brd);
		}
	}

	move_piece(brd, from, to);

	enum piece promoted = PROMOTED(mv);
	if (promoted != NO_PIECE){
		remove_piece_from_board(brd, pce_being_moved, to);
		add_piece_to_board(brd, promoted, to);
	}

	// flip side
	brd->side_to_move = FLIP_SIDE(brd->side_to_move);
	update_side_hash(brd);


	ASSERT_BOARD_OK(brd);

	// check if move is valid (ie, king in check)
	enum piece king = (side == BLACK) ? B_KING : W_KING;
	U64 bb_king = brd->bitboards[king];
	enum square king_sq = POP(&bb_king);

	assert(king_sq >= a1 && king_sq <= h8);

	// side is already flipped above, so use that as the attacking side
	if (is_sq_attacked(king_sq, brd->side_to_move, brd)){

		printf("---------------- invalid move....reverting ------------------\n");
		printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

		take_move(brd);
		return false;
	} else {
		return true;
	}
}




void take_move(struct board *brd){

	ASSERT_BOARD_OK(brd);

	brd->history_ply--;
	brd->ply--;

	mv_bitmap mv = brd->history[brd->history_ply].move;
	enum square from = FROMSQ(mv);
	enum square to = TOSQ(mv);

	assert(from >= a1 && from <= h8);
	assert(to >= a1 && to <= h8);



	// hash out en passant and castle if set
	if (brd->en_passant != NO_SQUARE)
		update_EP_hash(brd);
	update_castle_hash(brd);

    brd->castle_perm = brd->history[brd->history_ply].castle_perm;
    brd->fifty_move_counter = brd->history[brd->history_ply].fifty_move_counter;
    brd->en_passant = brd->history[brd->history_ply].en_passant;

	// now, hash back in
	if (brd->en_passant != NO_SQUARE)
		update_EP_hash(brd);
	update_castle_hash(brd);

	// flip side
	brd->side_to_move = FLIP_SIDE(brd->side_to_move);
	update_side_hash(brd);

	if (MFLAG_EN_PASSANT & mv){
		if (brd->side_to_move == WHITE){
			add_piece_to_board(brd, B_PAWN, to - 8);
		} else{
			add_piece_to_board(brd, W_PAWN, to + 8);
		}
	} else if (MFLAG_CASTLE & mv){
		switch(to){
			case c1:
				move_piece(brd, d1, a1);
				break;
			case c8:
				move_piece(brd, d8, a8);
				break;
			case g1:
				move_piece(brd, f1, h1);
				break;
			case g8:
				move_piece(brd, f8, h8);
				break;
			default:
				assert(false);
				break;
		}
	}

	move_piece(brd, to, from);

	enum piece captured = CAPTURED(mv);
	if (captured != NO_PIECE){
		add_piece_to_board(brd, captured, to);
	}

	enum piece promoted = PROMOTED(mv);
	if (promoted != NO_PIECE){
		enum colour prom_col = GET_COLOUR(promoted);
		remove_piece_from_board(brd, brd->pieces[from], from);

		enum piece pce_to_add = (prom_col == WHITE) ? W_PAWN : B_PAWN;
		add_piece_to_board(brd, pce_to_add, from);
	}

	ASSERT_BOARD_OK(brd);
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
