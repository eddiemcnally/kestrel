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
#include <assert.h>
#include "types.h"
#include "board.h"
#include "board_utils.h"
#include "move.h"
#include "attack.h"
#include "occupancy_mask.h"

bool is_horizontal_or_vertical_blocked(SQUARE sq_one, SQUARE sq_two, const BOARD * brd);
bool is_diagonally_blocked(SQUARE sq_one, SQUARE sq_two, const BOARD * brd);



bool is_sq_attacked(const SQUARE sq, const COLOUR attacking_side, const BOARD * brd){
		
	if (is_knight_attacking_square(sq, attacking_side, brd))
		return true;
	if (is_pawn_attacking_square(sq, attacking_side, brd))
		return true;
	if (is_king_attacking_square(sq, attacking_side, brd))
		return true;
	if (is_queen_attacking_square(sq, attacking_side, brd))	
		return true;
	if (is_bishop_attacking_square(sq, attacking_side, brd))
		return true;
	if (is_rook_attacking_square(sq, attacking_side, brd))
		return true;
	
	return false;
}

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07
	
inline bool is_knight_attacking_square(SQUARE sq, COLOUR attacking_side, const BOARD * brd){
	// create bitboard for square under attack 
	BITBOARD sqBB = 0;
	set_bit(&sqBB, sq);
	
	//printf("*** checking sq %d\n", sq);
	
	PIECE attacking_piece;

	if (attacking_side == WHITE)
		attacking_piece = W_KNIGHT;
	else 
		attacking_piece = B_KNIGHT;

	
	// get the bitboard representing all knights on the board of 
	// this colour
	BITBOARD bbKnight = brd->bitboards[attacking_piece];
	
	
	//printf("#knights %d\n", CNT(bbKnight));

	
	while( bbKnight != 0){
		SQUARE att_pce_sq = POP(&bbKnight);
		
		// get occupancy mask for this piece and square
		BITBOARD mask = GET_KNIGHT_OCC_MASK(att_pce_sq);
		
			
		//printf("att_pce_sq %d\n", att_pce_sq);
		//printf("sq %d knight mask 0x%016llx\n", sq, mask);
		//printf("square mask 0x%016llx\n", sqBB);
		//printf("square & mask 0x%016llx\n", mask & sqBB);
	
		
		if ((mask & sqBB) != 0){
			// a Knight is attacking this square
			return true;
		}	
	}
	return false;
}



bool is_bishop_attacking_square(SQUARE sq, COLOUR attacking_side, const BOARD * brd){
	// create bitboard for square under attack 
	BITBOARD sqBB = 0;
	set_bit(&sqBB, sq);
	
	PIECE attacking_piece;

	if (attacking_side == WHITE)
		attacking_piece = W_BISHOP;
	else 
		attacking_piece = B_BISHOP;

	
	// get the bitboard representing all bishops on the board of 
	// this colour
	BITBOARD bbBishop = brd->bitboards[attacking_piece];
	
	while( bbBishop != 0){
		SQUARE att_pce_sq = POP(&bbBishop);
		
		// get occupancy mask for this piece and square
		BITBOARD mask = GET_BISHOP_OCC_MASK(att_pce_sq);
		
		//printf("bishop mask 0x%016llx\n", mask);
		//printf("square mask 0x%016llx\n", sqBB);
		
		//printf("square & mask 0x%016llx\n", mask & sqBB);
		if (mask & sqBB){
			// a bishop is possibly attacking this square
			// search for any blocking pieces
			
			//printf("doing block check\n");
			bool blocked = is_diagonally_blocked(sq, att_pce_sq, brd);
			if (!blocked)
				return true;
		}	
	}
	return false;
}



bool is_rook_attacking_square(SQUARE sq, COLOUR attacking_side, const BOARD * brd){
	// create bitboard for square under attack 
	BITBOARD sqBB = 0;
	set_bit(&sqBB, sq);
	
	PIECE attacking_piece;

	if (attacking_side == WHITE)
		attacking_piece = W_ROOK;
	else 
		attacking_piece = B_ROOK;

	
	// get the bitboard representing all rooks on the board of 
	// this colour
	BITBOARD bbRook = brd->bitboards[attacking_piece];
	
	// check all rooks
	while( bbRook != 0){
		SQUARE att_pce_sq = POP(&bbRook);
		
		// get occupancy mask for this piece and square
		BITBOARD mask = GET_ROOK_OCC_MASK(att_pce_sq);
		if (mask & sqBB){
			// a Rook is possibly attacking this square
			// search for any blocking pieces
			if (is_horizontal_or_vertical_blocked(sq, att_pce_sq, brd)){
				continue;
			} else{
				// square is being attacked by a rook
				return true;
			}
		}	
	}
	return false;
}



inline bool is_pawn_attacking_square(SQUARE sq, COLOUR attacking_side, const BOARD * brd){
		
	PIECE attacking_piece;
	
	// create bitboard for square under attack 
	BITBOARD sqBB = 0;
	set_bit(&sqBB, sq);
	
	// ------------------------
	// check pwhich pawn
	// ------------------------
	if (attacking_side == WHITE)
		attacking_piece = W_PAWN;
	else
		attacking_piece = B_PAWN;
	
	
	// get the bitboard representing all pawns on the board of 
	// this colour
	BITBOARD bbPawn = brd->bitboards[attacking_piece];
	while( bbPawn != 0){
		SQUARE att_pce_sq = POP(&bbPawn);
		
		// get occupancy mask for this piece
		BITBOARD mask = 0;
		if (attacking_side == WHITE)
			mask = GET_WHITE_PAWN_OCC_MASK(att_pce_sq);
		else
			mask = GET_BLACK_PAWN_OCC_MASK(att_pce_sq);
		
		if (mask & sqBB){
			// a pawn is attacking this square
			return true;
		}	
	}
	return false;
}




inline bool is_queen_attacking_square(SQUARE sq, COLOUR attacking_side, const BOARD * brd){
	
	// create bitboard for square under attack 
	BITBOARD sqBB = 0;
	set_bit(&sqBB, sq);
	
	PIECE attacking_piece;
		
			
	// ------------------------
	// check king
	// ------------------------
	if (attacking_side == WHITE)
		attacking_piece = W_QUEEN;
	else
		attacking_piece = B_QUEEN;

	
	// get the bitboard representing all queens on the board of 
	// this colour
	BITBOARD bbQueen = brd->bitboards[attacking_piece];
	
	//printf("#queens %d\n", CNT(bbQueen));

	// check *all* queens to see if blocked.
	while( bbQueen != 0){
		SQUARE att_pce_sq = POP(&bbQueen);
		
		// get occupancy mask for this square
		BITBOARD mask = GET_QUEEN_OCC_MASK(att_pce_sq);
		
		//printf("att_pce_sq %d\n", att_pce_sq);
		//printf("sq %d queen mask 0x%016llx\n", sq, mask);
		//printf("square mask 0x%016llx\n", sqBB);
		//printf("square & mask 0x%016llx\n", mask & sqBB);
	
				
		if (mask & sqBB){
			// the queen is potentially attacking this square.
			// Need to see if any blocking pieces
			
			if (is_diagonally_blocked(sq, att_pce_sq, brd)){
				//printf("diag blocked: sq %d, att_pce_sq %d\n", sq, att_pce_sq);
				continue;
			} else if (is_horizontal_or_vertical_blocked(sq, att_pce_sq, brd)){
				//printf("vert/hor blocked: sq %d, att_pce_sq %d\n", sq, att_pce_sq);
				continue;
			} else{
				// not blocked => a queen is attacking square
				return true;
			}				
		} 
	}
	
	return false;

}




inline bool is_king_attacking_square(SQUARE sq, COLOUR attacking_side, const BOARD * brd){
	
	// create bitboard for square under attack 
	BITBOARD sqBB = 0;
	set_bit(&sqBB, sq);
	
	PIECE attacking_piece;
		
			
	// ------------------------
	// check king
	// ------------------------
	if (attacking_side == WHITE)
		attacking_piece = W_KING;
	else
		attacking_piece = B_KING;
	
	// get the bitboard representing the king on the board of 
	// this colour
	BITBOARD bbPawn = brd->bitboards[attacking_piece];
	while( bbPawn != 0){
		SQUARE att_pce_sq = POP(&bbPawn);
		
		// get occupancy mask for this square
		BITBOARD mask = GET_KING_OCC_MASK(att_pce_sq);
		if (mask & sqBB){
			// a king is attaching this square
			return true;
		}	
	}
	return false;
}



bool is_horizontal_or_vertical_blocked(SQUARE sq_one, SQUARE sq_two, const BOARD * brd){
	
	int sq_one_rank = GET_RANK(sq_one);
	int sq_one_file = GET_FILE(sq_one);
	
	int sq_two_rank = GET_RANK(sq_two);
	int sq_two_file = GET_FILE(sq_two);
	
	if (sq_one_rank == sq_two_rank){
		// search horizontally
		if (sq_one_file < sq_two_file){
			// search left
			for(int i = sq_one_file + 1; i < sq_two_file; i++){
				SQUARE s = GET_SQUARE(sq_one_rank, i);
				if (is_square_occupied(brd->board, s))
					return true;
			} 
		} else{
			// search right
			for(int i = sq_two_file + 1; i < sq_one_file; i++){
				SQUARE s = GET_SQUARE(sq_one_rank, i);
				if (is_square_occupied(brd->board, s))
					return true;
			}
		}
		return false;
	} else if (sq_one_file == sq_two_file){
		// search vertically
		if (sq_one_rank < sq_two_rank){
			// search up
			for(int i = sq_one_rank + 1; i < sq_two_rank; i++){
				SQUARE s = GET_SQUARE(i, sq_one_file);
				if (is_square_occupied(brd->board, s))
					return true;
			} 
		} else{
			// search down
			for(int i = sq_two_rank + 1; i < sq_one_rank; i++){
				SQUARE s = GET_SQUARE(i, sq_one_file);
				if (is_square_occupied(brd->board, s))
					return true;
			}
		}
		return false;
	} 
	return false;	
}



bool is_diagonally_blocked(SQUARE sq_one, SQUARE sq_two, const BOARD * brd){
	
	assert(sq_one != sq_two);
	
//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07
	
	//printf("*****sq1 %d, sq2 %d\n", sq_one, sq_two);
	
	if (sq_one < sq_two){
		//printf("111");
		if ( ((sq_two - sq_one) % 9) == 0){
			// search up and right from sq_one
			SQUARE sq = sq_one + 9;
			int sq_rank = GET_RANK(sq);
			int sq_file = GET_FILE(sq);
			
			while ((sq < sq_two) && (sq_file <= FILE_H) && (sq_rank <= RANK_8)){
				//printf("111checking square %d\n", sq);
				if (is_square_occupied(brd->board, sq)){
					return true;
				}
				sq += 9;
				sq_rank = GET_RANK(sq);
				sq_file = GET_FILE(sq);
			}			
		} else{
			//printf("222");

			// search up and left
			SQUARE sq = sq_one + 7;
			int sq_rank = GET_RANK(sq);
			int sq_file = GET_FILE(sq);
			
			while ((sq < sq_two) && (sq_file >= FILE_A) && (sq_rank <= RANK_8) ){
				//printf("222checking square %d\n", sq);
				if (is_square_occupied(brd->board, sq)){
					return true;
				}
				sq += 7;
				sq_rank = GET_RANK(sq);
				sq_file = GET_FILE(sq);
				
			}
		}		
		return false;
	} else {
		if ( ((sq_one - sq_two) % 9) == 0){
			//printf("333");

			// search down and left from sq_one
			SQUARE sq = sq_one - 9;
			int sq_rank = GET_RANK(sq);
			int sq_file = GET_FILE(sq);
			
			while ((sq > sq_two) && (sq_rank >= RANK_1) && (sq_file >= FILE_A)){

				//printf("333checking square %d\n", sq);
				if (is_square_occupied(brd->board, sq)){
					//printf("333 sq %d occupied\n", sq);
					return true;
				}
				sq -= 9;
				sq_rank = GET_RANK(sq);
				sq_file = GET_FILE(sq);
			}
		} else{
			//printf("444");

			// search down and right from sq_two
			SQUARE sq = sq_one - 7;
			int sq_rank = GET_RANK(sq);
			int sq_file = GET_FILE(sq);

			while ((sq < sq_one) && (sq_file >= FILE_H) && (sq_rank >= RANK_1)){
				//printf("444checking square %d\n", sq);

				if (is_square_occupied(brd->board, sq)){
					return true;
				}
				sq -= 7;
				sq_rank = GET_RANK(sq);
				sq_file = GET_FILE(sq);
			}
		}
		return false;
	}
}


