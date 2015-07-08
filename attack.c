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






/**
 * checks for overlap moves of rook and queen
 * (horizontal and vertical moves)
 */
inline bool is_rook_or_queen_attacking_square_rank_and_file(square_t sq, piece_id_t attacking_piece, const board_container_t * brd){
	square_t s = sq;
	board_t sq_bb = 0;

	
	// get the bitboard representing all the attacking piece types
	board_t att_bb = brd->bitboards[attacking_piece];


	int sq_rank = GET_RANK(sq);
	int sq_file = GET_FILE(sq);
	
	while(att_bb != 0){
		// square containing the attacking piece
		square_t att_pce_sq = POP(&att_bb);
		
		int att_pce_rank = GET_RANK(att_pce_sq);
		int att_pce_file = GET_FILE(att_pce_sq);
		
		assert(att_pce_sq != sq);

		// if piece is NOT on same rank or file as target square, then 
		// it's not attacking
		if(att_pce_rank != sq_rank && att_pce_file != sq_file)
			continue;
		

		if (att_pce_rank == sq_rank){
			// on same rank, find which way to look
			if (sq_file < att_pce_file){
				// check left for blocking pieces
				//printf("sq is < piece\n");
				for (int file = sq_file; file <= att_pce_file; file++){
					s = GET_SQUARE(att_pce_rank, file);
					//printf("test sq %d\n", s);
					
					if (sq == s){
						// hit on dest square...
						return true;
					} 
					sq_bb = 0;
					set_bit(&sq_bb, s);

					if (is_square_occupied(sq_bb, s)){
						// intervening square occupied, blocking attack
						// ...keep looking
						continue;
					}					
				} 				
			} else {
				// check right
				for (int file = att_pce_file; file <= sq_file; file++){
					s = GET_SQUARE(att_pce_rank, file);
					if (sq == s){
						// hit on dest square...
						return true;
					} 
					sq_bb = 0;
					set_bit(&sq_bb, s);

					if (is_square_occupied(sq_bb, s)){
						// intervening square occupied, blocking attack
						// ...keep looking
						continue;
					}					
				}
			}
		} else if (att_pce_file == sq_file){
			if (sq_rank < att_pce_rank){
				// see if file is empty between piece and square
				for (int rank = sq_rank; rank <= att_pce_rank; rank++){
					s = GET_SQUARE(rank, att_pce_file);
					if (sq == s){
						// hit on dest square...
						return true;
					} 
					sq_bb = 0;
					set_bit(&sq_bb, s);

					if (is_square_occupied(sq_bb, s)){
						// intervening square occupied, blocking attack
						// ...keep looking
						continue;
					}					
				} 				
			} else {
				// see if file between pieces is empty
				for (int rank = att_pce_rank; rank <= sq_rank; rank++){
					s = GET_SQUARE(rank, att_pce_file);
					if (sq == s){
						// hit on dest square...
						return true;
					} 
					sq_bb = 0;
					set_bit(&sq_bb, s);

					if (is_square_occupied(sq_bb, s)){
						// intervening square occupied, blocking attack
						// ...keep looking
						continue;
					}					
				} 				
			}
	
		} else {
			assert((att_pce_file != sq_file) && (att_pce_file != sq_file));
		}			
	}
	return false;
}








/**
 * checks for overlap moves of rook and queen
 * (horizontal and vertical moves)
 */
//static inline bool is_bishop_or_queen_attacking_square_diagonally(square_t sq, piece_id_t attacking_piece, const board_container_t * brd){
		
	//// get the bitboard representing all the attacking piece types
	//board_t att_bb = brd->bitboards[attacking_piece];

	//int sq_rank = GET_RANK(sq);
	//int sq_file = GET_FILE(sq);

	
	//while(att_bb != 0){
		//// square containing the attacking piece
		//square_t att_pce_sq = POP(&att_bb);

		//int att_pce_rank = GET_RANK(att_pce_sq);
		//int att_pce_file = GET_FILE(att_pce_sq);

		//assert(att_pce_sq != sq);

		//if ((sq_rank < att_pce_rank) && (sq_file < att_pce_rank)){
			//int rank = sq_rank;
			//int file = sq_file;
			
			//while( (sq_rank <= RANK_8) && (sq_file <= FILE_H)){
				//square_t s = GET_SQUARE(rank, file);
				////printf("test sq %d\n", s);					
				//if (sq == s){
					//// hit on dest square...
					//return true;
				//} 
				//board_t sq_bb = 0;
				//set_bit(&sq_bb, s);
				//if (is_square_occupied(sq_bb, s)){
					//// intervening square occupied, blocking attack
					//// ...keep looking
					//continue;
				//}
				//rank++;
				//file++;
			//}
		//} else if ((sq_rank < att_pce_rank) && (sq_file > att_pce_rank)){
			//int rank = sq_rank;
			//int file = sq_file;
			
			//while( (sq_rank <= RANK_8) && (sq_file <= FILE_H)){
				//square_t s = GET_SQUARE(rank, file);
				////printf("test sq %d\n", s);					
				//if (sq == s){
					//// hit on dest square...
					//return true;
				//} 
				//board_t sq_bb = 0;
				//set_bit(&sq_bb, s);
				//if (is_square_occupied(sq_bb, s)){
					//// intervening square occupied, blocking attack
					//// ...keep looking
					//continue;
				//}
				//rank++;
				//file++;
			//}
		//} 







		//// if piece is NOT on same rank or file as target square, then 
		//// it's not attacking
		//if(att_pce_rank != sq_rank && att_pce_file != sq_file)
			//continue;

		//if (att_pce_rank == sq_rank){
			//// on same rank, find which way to look
			//if (sq_file < att_pce_file){
				//// check left for blocking pieces
				////printf("sq is < piece\n");
				//for (int file = sq_file; file <= att_pce_file; file++){
					//square_t s = GET_SQUARE(att_pce_rank, file);
					////printf("test sq %d\n", s);
					
					//if (sq == s){
						//// hit on dest square...
						//return true;
					//} 
					//board_t sq_bb = 0;
					//set_bit(&sq_bb, s);

					//if (is_square_occupied(sq_bb, s)){
						//// intervening square occupied, blocking attack
						//// ...keep looking
						//continue;
					//}					
				//} 				
			//} else {
				//// check right
				//for (int file = att_pce_file; file <= sq_file; file++){
					//square_t s = GET_SQUARE(att_pce_rank, file);
					//if (sq == s){
						//// hit on dest square...
						//return true;
					//} 
					//board_t sq_bb = 0;
					//set_bit(&sq_bb, s);

					//if (is_square_occupied(sq_bb, s)){
						//// intervening square occupied, blocking attack
						//// ...keep looking
						//continue;
					//}					
				//}
			//}
		//} else if (att_pce_file == sq_file){
			//if (sq_rank < att_pce_rank){
				//// see if file is empty between piece and square
				//for (int rank = sq_rank; rank <= att_pce_rank; rank++){
					//square_t s = GET_SQUARE(rank, att_pce_file);
					//if (sq == s){
						//// hit on dest square...
						//return true;
					//} 
					//board_t sq_bb = 0;
					//set_bit(&sq_bb, s);

					//if (is_square_occupied(sq_bb, s)){
						//// intervening square occupied, blocking attack
						//// ...keep looking
						//continue;
					//}					
				//} 				
			//} else {
				//// see if file between pieces is empty
				//for (int rank = att_pce_rank; rank <= sq_rank; rank++){
					//square_t s = GET_SQUARE(rank, att_pce_file);
					//if (sq == s){
						//// hit on dest square...
						//return true;
					//} 
					//board_t sq_bb = 0;
					//set_bit(&sq_bb, s);

					//if (is_square_occupied(sq_bb, s)){
						//// intervening square occupied, blocking attack
						//// ...keep looking
						//continue;
					//}					
				//} 				
			//}
	
		//} else {
			//assert((att_pce_file != sq_file) && (att_pce_file != sq_file));
		//}			
	//}
	//return false;
//}








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
