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
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "pieces.h"
#include "board.h"
#include "attack.h"
#include "makemove.h"
#include "board_utils.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "hashkeys.h"






enum pawn_action{
	ADD,		// signifies we're adding a pawn
	REMOVE		// removing a pawn
};

static inline void set_up_pawn_info(struct board *brd, enum colour col, 
								enum square sq, enum pawn_action action);


//bit mask for castle permissions
static const uint8_t castle_permission_mask[NUM_SQUARES] = {
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

	enum piece pce = brd->pieces[from];
	enum colour pce_col = GET_COLOUR(pce);

	//assert(IS_VALID_PIECE(pce));

	// adjust the hash
	brd->board_hash ^= get_piece_hash(pce, from);
	brd->board_hash ^= get_piece_hash(pce, to);	
	

	// clear up the "from" resources
	brd->pieces[from] = NO_PIECE;
	clear_bit(&brd->bitboards[pce], from);
	
	// set up the "to" resources
	brd->pieces[to] = pce;
	set_bit(&brd->bitboards[pce], to);

	if (pce_col == WHITE){
		clear_bit(&brd->colour_bb[WHITE], from);
		set_bit(&brd->colour_bb[WHITE], to);
		if(pce == W_PAWN){
			// easiest way to move a pawn
			set_up_pawn_info(brd, WHITE, from, REMOVE);
			set_up_pawn_info(brd, WHITE, to, ADD);
		} else if (pce == W_KING){
			brd->king_sq[WHITE] = to;
		}
	} else {
		clear_bit(&brd->colour_bb[BLACK], from);
		set_bit(&brd->colour_bb[BLACK], to);
		if(pce == B_PAWN){
			// easiest way to move a pawn
			set_up_pawn_info(brd, BLACK, from, REMOVE);
			set_up_pawn_info(brd, BLACK, to, ADD);
		} else if (pce == B_KING){
			brd->king_sq[BLACK] = to;
		}
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
			printf("to : %s\n", print_square(to));
			assert(false);
			break;
		}
	}

	if (brd->en_passant != NO_SQUARE) {
		brd->board_hash ^= get_en_passant_hash(brd->en_passant);
	}


	brd->board_hash ^= get_castle_hash(brd->castle_perm);
	
	// set up history
	brd->history[brd->history_ply].move = mv;
	brd->history[brd->history_ply].fifty_move_counter = brd->fifty_move_counter;
	brd->history[brd->history_ply].en_passant = brd->en_passant;
	brd->history[brd->history_ply].castle_perm = brd->castle_perm;

	// reset castle and set again
	brd->castle_perm &= castle_permission_mask[from];
	brd->castle_perm &= castle_permission_mask[to];
	brd->board_hash ^= get_castle_hash(brd->castle_perm);


	brd->en_passant = NO_SQUARE;
	brd->fifty_move_counter++;

	if (IS_CAPTURE_MOVE(mv)){
		remove_piece_from_board(brd, to);
		brd->fifty_move_counter = 0;
	}

	brd->ply++;
	brd->history_ply++;

	enum piece pce_being_moved = brd->pieces[from];

	if (IS_PAWN(pce_being_moved)) {
		brd->fifty_move_counter = 0;

		if (IS_PAWN_START(mv)) {
			if (side == WHITE) {
				brd->en_passant = from + 8;
			} else {
				brd->en_passant = from - 8;
			}
			brd->board_hash ^= get_en_passant_hash(brd->en_passant);
		}
	}

	move_piece(brd, from, to);

	enum piece promoted = PROMOTED_PCE(mv);
	if (promoted != NO_PIECE) {
		remove_piece_from_board(brd, to);
		add_piece_to_board(brd, promoted, to);
	}
	// flip side
	flip_sides(brd);

	// check if move is valid (ie, king in check)
	enum square king_sq = brd->king_sq[side];

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
		brd->board_hash ^= get_en_passant_hash(brd->en_passant);
	}
	
	brd->board_hash ^= get_castle_hash(brd->castle_perm);

	brd->castle_perm = brd->history[brd->history_ply].castle_perm;
	brd->fifty_move_counter = brd->history[brd->history_ply].fifty_move_counter;
	brd->en_passant = brd->history[brd->history_ply].en_passant;

	// now, hash back in
	if (brd->en_passant != NO_SQUARE) {
		brd->board_hash ^= get_en_passant_hash(brd->en_passant);
	}
	brd->board_hash ^= get_castle_hash(brd->castle_perm);

	// flip side
	flip_sides(brd);

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
	if (PROMOTED_PCE(mv) != NO_PIECE) {
		enum colour prom_col = GET_COLOUR(promoted);
		remove_piece_from_board(brd, from);

		enum piece pce_to_add = (prom_col == WHITE) ? W_PAWN : B_PAWN;
		add_piece_to_board(brd, pce_to_add, from);
	}
}



inline void flip_sides(struct board *brd){
	// flip side
	brd->side_to_move = GET_OPPOSITE_SIDE(brd->side_to_move);
	brd->board_hash ^= get_side_hash();
}


void add_piece_to_board(struct board *brd, enum piece pce, enum square sq)
{
	enum colour col = GET_COLOUR(pce);
	brd->board_hash ^= get_piece_hash(pce, sq);

	brd->pieces[sq] = pce;
	brd->material[col] += get_piece_value(pce);

	// set piece on bitboards
	set_bit(&brd->bitboards[pce], sq);
	set_bit(&brd->board, sq);
	set_bit(&brd->colour_bb[col], sq);
	
	switch (pce){
		case W_PAWN:
		case B_PAWN:
			set_up_pawn_info(brd, col, sq, ADD);
			break;
		case W_KING:
		case B_KING:
			brd->king_sq[col] = sq;
			break;
		default:
			break;		
	}
	
}	



inline void remove_piece_from_board(struct board *brd, enum square sq)
{
	enum piece pce = brd->pieces[sq];
	enum colour col = GET_COLOUR(pce);

	brd->board_hash ^= get_piece_hash(pce, sq);

	brd->pieces[sq] = NO_PIECE;

	brd->material[col] -= get_piece_value(pce);

	// remove piece from bitboards
	clear_bit(&brd->bitboards[pce], sq);
	clear_bit(&brd->board, sq);
	clear_bit(&brd->colour_bb[col], sq);
	
	
	switch (pce){
		case W_PAWN:
		case B_PAWN:
			set_up_pawn_info(brd, col, sq, REMOVE);
			break;
		case W_KING:
		case B_KING:
			brd->king_sq[col] = NO_SQUARE;
			break;
		default:
			break;		
	}	
}




static inline void set_up_pawn_info(struct board *brd, enum colour col, enum square sq, enum pawn_action action){

	uint8_t file = GET_FILE(sq);
	uint8_t rank = GET_RANK(sq);
	
	if (action == ADD){
		brd->pawns_on_file[col][file]++;
		brd->pawns_on_rank[col][rank]++;
	} else {
		brd->pawns_on_file[col][file]--;
		brd->pawns_on_rank[col][rank]--;
	} 

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
	
	int32_t next_sq = 0;
	switch (col){
		case WHITE:
			if (file > FILE_A){
				next_sq = sq + NW;
			
				if (action == ADD) 
					brd->pawn_control[col][next_sq]++;
				else  
					brd->pawn_control[col][next_sq]--;
			}
			if (file < FILE_H){
				next_sq = sq + NE;
			
				if (action == ADD) 
					brd->pawn_control[col][next_sq]++;
				else  
					brd->pawn_control[col][next_sq]--;
			}
			break;
		
		case BLACK:
			if (file > FILE_A){
				next_sq = sq + SW;
				
				if (action == ADD) 
					brd->pawn_control[col][next_sq]++;
				else  
					brd->pawn_control[col][next_sq]--;
			}
			if (file < FILE_H){
				next_sq = sq + SE;
				
				if (action == ADD) 
					brd->pawn_control[col][next_sq]++;
				else  
					brd->pawn_control[col][next_sq]--;
			}
			break;
		default:
			assert(false);
			
	}
#pragma GCC diagnostic pop

}


/*
 *
 * name: clear_bit
 * @param : board, square
 * @return : void
 *
 */
inline void clear_bit(uint64_t * brd, enum square sq)
{
	*brd = *brd & (uint64_t) (~(0x01ull << sq));
}

/*
 *
 * name: set_bit
 * @param : board, square
 * @return : void
 *
 */
inline void set_bit(uint64_t * brd, enum square sq)
{
	*brd = *brd | (uint64_t) (0x01ull << sq);
}

inline void set_bits(uint64_t * w, uint64_t bitmask)
{
	*w = *w | bitmask;
}

