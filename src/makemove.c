/*
 * makemove.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: code associated with making and retracting moves
 * on the board.
 * ---------------------------------------------------------------------
 *
 *
 *
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
	7, 15, 15, 15, 3, 15, 15, 11
};

inline void move_piece(struct board *brd, enum square from, enum square to)
{

	//ASSERT_BOARD_OK(brd);

	enum piece pce = get_piece_at_square(brd, from);
	enum colour pce_col = GET_COLOUR(pce);

	//assert(IS_VALID_PIECE(pce));

	// clear up the "from" resources
	update_piece_hash(brd, pce, from);
	brd->pieces[from] = NO_PIECE;
	clear_bit(&brd->bitboards[pce], from);

	// set up the "to" resources
	update_piece_hash(brd, pce, to);
	brd->pieces[to] = pce;
	set_bit(&brd->bitboards[pce], to);

	// only need to update whichever colour has moved
	if (pce_col == WHITE){
		// only need to update individual bits
		clear_bit(&brd->colour_bb[WHITE], from);
		set_bit(&brd->colour_bb[WHITE], to);
	} else {
		clear_bit(&brd->colour_bb[BLACK], from);
		set_bit(&brd->colour_bb[BLACK], to);
	}

	brd->board = brd->colour_bb[WHITE] | brd->colour_bb[BLACK];

	//ASSERT_BOARD_OK(brd);

}

// return false if move is invalid, true otherwise
bool make_move(struct board *brd, mv_bitmap mv)
{
	//ASSERT_BOARD_OK(brd);

	enum square from = FROMSQ(mv);
	enum square to = TOSQ(mv);

	enum colour side = brd->side_to_move;

	brd->history[brd->history_ply].board_hash = brd->board_hash;

	if (mv & MFLAG_EN_PASSANT) {
		if (side == WHITE) {
			remove_piece_from_board(brd, to - 8);
		} else {
			remove_piece_from_board(brd, to + 8);
		}
	} else if (mv & MFLAG_CASTLE) {
		switch (to) {
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

	if (brd->en_passant != NO_SQUARE) {
		update_EP_hash(brd);
	}
	update_castle_hash(brd);

	// set up history
	brd->history[brd->history_ply].move = mv;
	brd->history[brd->history_ply].fifty_move_counter =
	    brd->fifty_move_counter;
	brd->history[brd->history_ply].en_passant = brd->en_passant;
	brd->history[brd->history_ply].castle_perm = brd->castle_perm;

	brd->castle_perm &= castle_permission_mask[from];
	brd->castle_perm &= castle_permission_mask[to];
	brd->en_passant = NO_SQUARE;

	update_castle_hash(brd);

	enum piece captured = CAPTURED_PCE(mv);
	brd->fifty_move_counter++;

	if (captured != NO_PIECE) {
		remove_piece_from_board(brd, to);
		brd->fifty_move_counter = 0;
	}

	brd->ply++;
	brd->history_ply++;

	enum piece pce_being_moved = brd->pieces[from];

	if (IS_PAWN(pce_being_moved)) {
		brd->fifty_move_counter = 0;

		if (mv & MFLAG_PAWN_START) {
			if (side == WHITE) {
				brd->en_passant = from + 8;
			} else {
				brd->en_passant = from - 8;
			}
			update_EP_hash(brd);
		}
	}

	move_piece(brd, from, to);

	enum piece promoted = PROMOTED_PCE(mv);
	if (promoted != NO_PIECE) {
		remove_piece_from_board(brd, to);
		add_piece_to_board(brd, promoted, to);
	}
	// check if move is valid (ie, king in check)
	enum piece king = (side == BLACK) ? B_KING : W_KING;
	U64 bb_king = brd->bitboards[king];
	enum square king_sq = pop_1st_bit(&bb_king);

	// flip side
	brd->side_to_move = GET_OPPOSITE_SIDE(brd->side_to_move);
	update_side_hash(brd);

	// side is already flipped above, so use that as the attacking side
	if (is_sq_attacked(brd, king_sq, brd->side_to_move)) {
		take_move(brd);
		return false;
	} else {
		return true;
	}
}

inline void take_move(struct board *brd)
{
	brd->history_ply--;
	brd->ply--;

	mv_bitmap mv = brd->history[brd->history_ply].move;

	// note: when reverting, the 'from' square will be empty and the 'to'
	// square has the piece in it.
	enum square from = FROMSQ(mv);
	enum square to = TOSQ(mv);

	// hash out en passant and castle if set
	if (brd->en_passant != NO_SQUARE) {
		update_EP_hash(brd);
	}
	update_castle_hash(brd);

	brd->castle_perm = brd->history[brd->history_ply].castle_perm;
	brd->fifty_move_counter =
	    brd->history[brd->history_ply].fifty_move_counter;
	brd->en_passant = brd->history[brd->history_ply].en_passant;

	// now, hash back in
	if (brd->en_passant != NO_SQUARE) {
		update_EP_hash(brd);
	}
	update_castle_hash(brd);

	// flip side
	brd->side_to_move = GET_OPPOSITE_SIDE(brd->side_to_move);
	update_side_hash(brd);

	if (MFLAG_EN_PASSANT & mv) {
		if (brd->side_to_move == WHITE) {
			add_piece_to_board(brd, B_PAWN, to - 8);
		} else {
			add_piece_to_board(brd, W_PAWN, to + 8);
		}
	} else if (MFLAG_CASTLE & mv) {
		switch (to) {
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
	// note: to revert move, move piece from 'to' to 'from'
	move_piece(brd, to, from);

	enum piece captured = CAPTURED_PCE(mv);
	if (captured != NO_PIECE) {
		add_piece_to_board(brd, captured, to);
	}

	enum piece promoted = PROMOTED_PCE(mv);
	if (promoted != NO_PIECE) {
		enum colour prom_col = GET_COLOUR(promoted);
		remove_piece_from_board(brd, from);

		enum piece pce_to_add = (prom_col == WHITE) ? W_PAWN : B_PAWN;
		add_piece_to_board(brd, pce_to_add, from);
	}
}




void add_piece_to_board(struct board *brd, enum piece pce, enum square sq)
{
	enum colour col = GET_COLOUR(pce);
	update_piece_hash(brd, pce, sq);

	brd->pieces[sq] = pce;
	brd->material[col] += get_piece_value(pce);
/*
	printf("added piece to board %c, value %d, total %d\n",
				get_piece_label(pce),
				get_piece_value(pce),
				brd->material[col]);
*/

	// set piece on bitboards
	set_bit(&brd->bitboards[pce], sq);
	set_bit(&brd->board, sq);

	if (col == WHITE)
		set_bit(&brd->colour_bb[WHITE], sq);
	else
		set_bit(&brd->colour_bb[BLACK], sq);

}



inline void remove_piece_from_board(struct board *brd, enum square sq)
{
	enum piece pce = get_piece_at_square(brd, sq);
	enum colour col = GET_COLOUR(pce);

	update_piece_hash(brd, pce, sq);

	brd->pieces[sq] = NO_PIECE;

	brd->material[col] -= get_piece_value(pce);
/*
	printf("removed piece from board %c, value %d, total %d\n",
				get_piece_label(pce),
				get_piece_value(pce),
				brd->material[col]);
*/


	// remove piece from bitboards
	clear_bit(&brd->bitboards[pce], sq);
	clear_bit(&brd->board, sq);

	if (col == WHITE)
		clear_bit(&brd->colour_bb[WHITE], sq);
	else
		clear_bit(&brd->colour_bb[BLACK], sq);

}


/*
 *
 * name: clear_bit
 * @param : board, square
 * @return : void
 *
 */
inline void clear_bit(U64 * brd, enum square sq)
{
	*brd = *brd & (U64) (~(0x01ull << sq));
}

/*
 *
 * name: set_bit
 * @param : board, square
 * @return : void
 *
 */
inline void set_bit(U64 * brd, enum square sq)
{
	*brd = *brd | (U64) (0x01ull << sq);
}

inline void update_piece_hash(struct board *brd, enum piece pce, enum square sq)
{
	brd->board_hash ^= get_piece_hash(pce, sq);
}

inline void update_castle_hash(struct board *brd)
{
	brd->board_hash ^= get_castle_hash(brd->castle_perm);
}

inline void update_side_hash(struct board *brd)
{
	brd->board_hash ^= get_side_hash();
}

inline void update_EP_hash(struct board *brd)
{
	brd->board_hash ^= get_en_passant_hash(brd->en_passant);
}
