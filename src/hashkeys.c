/*
 * hashkeys.c
 * 
 * ---------------------------------------------------------------------
 * Description : code associated with generating Zobrist hash codes 
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
 * 
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "types.h"
#include "hashkeys.h"


static uint64_t generate_rand64(void);
#define NUM_CASTLE_KEYS		16


struct zobrist {
	uint64_t piece_keys[NUM_PIECES][NUM_SQUARES];
	uint64_t side_to_move_key;
	// 16 combinations because of 4 bits being used for castle enum
	uint64_t castle_keys[NUM_CASTLE_KEYS];
	// could use FILE, but using squares means we don't have to 
	// calc the file in order to access the key
	uint64_t en_passant_keys[NUM_SQUARES];
};


static struct zobrist st_zobrist;

/*
 * Initialises hashkeys with random numbers
 * name: init_hash_keys
 * @param
 * @return
 *
 */
void init_hash_keys()
{
	// reset the seed for the RNG
	time_t now = time(0);
	srand((unsigned int)now);
		
	for (int pce = 0; pce < NUM_PIECES; pce++) {
		for (int sq = 0; sq < NUM_SQUARES; sq++) {
			st_zobrist.piece_keys[pce][sq] = generate_rand64();
		}
	}

	st_zobrist.side_to_move_key = generate_rand64();
	
	for (int i = 0; i < NUM_CASTLE_KEYS; i++) {
		st_zobrist.castle_keys[i] = generate_rand64();
	}
	
	for (int i = 0; i < NUM_SQUARES; i++) {
		st_zobrist.en_passant_keys[i] = generate_rand64();
	}
	
	
}

/* Returns the castle hashkey for a given castle permission map
 *
 * name: 	get_castle_hash
 * @param: 	castle_map - the map of castle options
 * @return:	the uint64_t hashkey
 *
 */
uint64_t get_castle_hash(uint8_t castle_map)
{
	return st_zobrist.castle_keys[castle_map];
}

/* Returns the side hashkey
 *
 * name: 	get_side_hash
 * @param:
 * @return:	the uint64_t hashkey
 *
 */
uint64_t get_side_hash(void)
{
	return st_zobrist.side_to_move_key;
}

/* Returns the hashkey for a particular piece on a given square
 *
 * name: 	get_piece_hash
 * @param: 	piece and square
 * @return:	the uint64_t hashkey
 *
 */
uint64_t get_piece_hash(enum piece pce, enum square sq)
{
	return st_zobrist.piece_keys[pce][sq];
}

uint64_t get_en_passant_hash(enum square sq)
{
	return st_zobrist.en_passant_keys[sq];
}

/*Given a board, return a positon hashkey
 *
 * name: 	get_position_hash
 * @param:	ptr ot a board struct
 * @return:	the position hashkey
 *
 */

uint64_t get_position_hash(const struct board * brd)
{
	uint64_t retval = 0;

	for (enum square sq = a1; sq <= h8; sq++) {
		enum piece pce = brd->pieces[sq];
		if (pce != NO_PIECE) {
			retval ^= get_piece_hash(pce, sq);
		}
	}

	if (brd->side_to_move == WHITE) {
		retval ^= get_side_hash();
	}

	if (brd->en_passant != NO_SQUARE) {
		retval ^= get_en_passant_hash(brd->en_passant);
	}

	retval ^= get_castle_hash(brd->castle_perm);
	
	return retval;
}


///////////////////////////////////
////// NOTE ///////////////////////
///////////////////////////////////
// Replace the rand() with a predictable "random" number.
// This has the advantage during debugging of always having the same
// hash values for board positions.
////////////////////////////////////////////////


/*
static uint64_t generate_rand64(void)
{
	// GNU C rand() generates a 32-bit
	uint64_t retval = (uint64_t) rand();
	retval |= (uint64_t) rand () << 32;

	return retval;
}
*/
// see https://en.wikipedia.org/wiki/Linear_congruential_generator
static uint64_t generate_rand64(void){
    static uint64_t next = 1;
 
    next = next * 1103515245 + 12345;
    return next;
}



