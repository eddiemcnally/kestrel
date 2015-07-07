/*
 * attack.c
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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"
#include "board.h"
#include "board_utils.h"
#include "move.h"
#include "attack.h"


bool is_sq_attacked(const square_t sq, const colour_t attacking_side, const board_container_t * brd){
	
	
	if (is_knight_attacking_square(sq, attacking_side, brd))
		return true;
	if (is_pawn_attacking_square(sq, attacking_side, brd))
		return true;
	if (is_king_attacking_square(sq, attacking_side, brd))
		return true;
	
	
	return false;
}


inline bool is_knight_attacking_square(square_t sq, colour_t attacking_side, const board_container_t * brd){
	// create bitboard for square under attack 
	board_t sqBB = 0;
	set_bit(&sqBB, sq);
	
	piece_id_t attacking_piece;

	if (attacking_side == WHITE){
		attacking_piece = W_KNIGHT;
		//printf("attacking pce = W_KNIGHT");
	} else {
		attacking_piece = B_KNIGHT;
		//printf("attacking pce = B_KNIGHT");
	}
	
	// get the bitboard representing all knights on the board of 
	// this colour
	board_t bbKnight = brd->bitboards[attacking_piece];
	
	//printf("BB for knight : 0x%016llx\n", bbKnight);
	
	while( bbKnight != 0){
		square_t att_pce_sq = POP(&bbKnight);
		
		// get occupancy mask for this piece
		board_t mask = get_occ_mask(attacking_piece, att_pce_sq);
		if (mask & sqBB){
			// a Knight is attacking this square
			return true;
		}	
	}
	return false;
}



inline bool is_pawn_attacking_square(square_t sq, colour_t attacking_side, const board_container_t * brd){
	
	// create bitboard for square under attack 
	board_t sqBB = 0;
	set_bit(&sqBB, sq);
	
	piece_id_t attacking_piece;
		
			
	// ------------------------
	// check pawns
	// ------------------------
	if (attacking_side == WHITE)
		attacking_piece = W_PAWN;
	else
		attacking_piece = B_PAWN;
	
	// get the bitboard representing all pawns on the board of 
	// this colour
	board_t bbPawn = brd->bitboards[attacking_piece];
	while( bbPawn != 0){
		square_t att_pce_sq = POP(&bbPawn);
		
		// get occupancy mask for this piece
		board_t mask = get_occ_mask(attacking_piece, att_pce_sq);
		if (mask & sqBB){
			// a pawn is attaching this square
			return true;
		}	
	}
	return false;
}




inline bool is_king_attacking_square(square_t sq, colour_t attacking_side, const board_container_t * brd){
	
	// create bitboard for square under attack 
	board_t sqBB = 0;
	set_bit(&sqBB, sq);
	
	piece_id_t attacking_piece;
		
			
	// ------------------------
	// check king
	// ------------------------
	if (attacking_side == WHITE)
		attacking_piece = W_KING;
	else
		attacking_piece = B_KING;
	
	// get the bitboard representing all pawns on the board of 
	// this colour
	board_t bbPawn = brd->bitboards[attacking_piece];
	while( bbPawn != 0){
		square_t att_pce_sq = POP(&bbPawn);
		
		// get occupancy mask for this piece
		board_t mask = get_occ_mask(attacking_piece, att_pce_sq);
		if (mask & sqBB){
			// a king is attaching this square
			return true;
		}	
	}
	return false;
}
