/*
 * hashkeys.c
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chess is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "types.h"
#include "utils.h"
#include "pieces.h"
#include "board.h"
#include "board_utils.h"

#include "hashkeys.h"

U64 get_castle_key(unsigned int castle_map);
U64 get_side_key();
U64 get_piece_key(piece_id_t piece, square_t square);

//----- hashkeys for positions
U64 piece_keys[NUM_PIECE_TYPES][NUM_SQUARES];
U64 side_to_move_key;
U64 castle_keys[16];	// 16 because of 4 bits being used for castle enum
//-----

/*
 * Initialises hashkeys with random numbers
 * name: init_hash_keys
 * @param
 * @return
 * 
 */
void init_hash_keys(){
	
	for (int pce = 0; pce < NUM_PIECE_TYPES; pce++){
		for(int sq = 0; sq < NUM_SQUARES; sq++){
			piece_keys[pce][sq] = RAND64;
		}
	}
	
	
	side_to_move_key = RAND64;
	
	for(int i = 0; i < 16; i++){
		castle_keys[i] = RAND64;
	}	
}

/* Returns the castle hashkey for a given castle permission map
 * 
 * name: 	get_castle_key
 * @param: 	castle_map - the map of castle options 
 * @return:	the U64 hashkey
 * 
 */
inline U64 get_castle_key(unsigned int castle_map){
	assert(castle_map < 16);
	
	return castle_keys[castle_map];
}

/* Returns the side hashkey
 * 
 * name: 	get_side_key
 * @param: 	 
 * @return:	the U64 hashkey
 * 
 */
inline U64 get_side_key(){
	return side_to_move_key;
}



/* Returns the hashkey for a particular piece on a given square
 * 
 * name: 	get_piece_key
 * @param: 	piece and square
 * @return:	the U64 hashkey
 * 
 */
inline U64 get_piece_key(piece_id_t piece, square_t square){
	return piece_keys[piece][square];
}


/*Given a board, return a positon hashkey 
 * 
 * name: 	get_position_hashkey
 * @param:	ptr ot a board struct
 * @return:	the position hashkey
 * 
 */

U64 get_position_hashkey(board_container_t *brd){
	
	U64 retval = 0;
	
	for(int sq = 0; sq < NUM_SQUARES; sq++){
		piece_t pce = get_piece_at_square(brd, sq);
		
		if (pce >= 0){
			retval ^= get_piece_key(pce, sq);
		}		
	}
	
	if (brd->side_to_move == WHITE){
		retval ^= get_side_key();
	}
	
	// TODO - figure out how to use this
	if (brd->en_passant != NO_SQUARE){
		retval ^= brd->en_passant;
	}
	
	retval ^= get_castle_key(brd->castle_perm);
	
	return retval;
}




