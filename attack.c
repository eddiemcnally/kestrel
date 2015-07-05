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
#include <stdbool.h>
#include "attack.h"


bool is_sq_attacked(const square_t sq, const colour_t attacking_side, const board_container_t * brd){
	
	// create bitboard for square 
	board_t sqBB = 0;
	set_bit(&sqBB, sq);
	
	piece_id_t attacking_piece;
	
	// ------------------------
	// check bishops
	// ------------------------
	if (attacking_side == WHITE)
		attacking_piece = W_BISHOP;
	else
		attacking_piece = B_BISHOP;
	
	board_t bbBishop = brd->bitboards[attacking_piece];
	while( bbBishop != 0){
		square_t sqBB = POP(&bbBishop);
		
		// get occupancy mask for this piece
		
			
			
			
		}
			
		
	}
		
	
	
	
	
	
	
	
	
	
	
	
}






