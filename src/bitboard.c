/*
 * bitboard.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for manipulating the bitboards
 * ---------------------------------------------------------------------
 *
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
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "bitboard.h"
#include "pieces.h"



void add_piece_to_bitboards(struct bitboards *bb, enum piece pce, enum square sq){
	// set pieces[] bb
	uint64_t pce_bb = bb->pieces[pce];
	set_bit(&pce_bb, sq);
	bb->pieces[pce] = pce_bb;
	
	// set overall bb
	set_bit(&bb->board, sq);
	
	// set the colour bb
	if (IS_BLACK(pce)){
		set_bit(&bb->colour_bb[BLACK], sq);
	} 
	else
	{
		set_bit(&bb->colour_bb[WHITE], sq);
	}	
}


void remove_piece_from_bitboards(struct bitboards *bb, enum piece pce, enum square sq){
	// clear pieces[] bb
	uint64_t pce_bb = bb->pieces[pce];
	clear_bit(&pce_bb, sq);
	bb->pieces[pce] = pce_bb;
	
	// set overall bb
	clear_bit(&bb->board, sq);
	
	// set the colour bb
	if (IS_BLACK(pce)){
		clear_bit(&bb->colour_bb[BLACK], sq);
	} 
	else
	{
		clear_bit(&bb->colour_bb[WHITE], sq);
	}
}




uint64_t square_to_bitboard(enum square sq)
{
    uint64_t retval = 0;
    set_bit(&retval, sq);
    return retval;
}



uint64_t get_bitboard(const struct bitboards *bb, enum piece pce){
	return bb->pieces[pce];
}

uint64_t get_bitboard_for_king(const struct bitboards *bb, enum colour piece_col){
	if (piece_col == WHITE){
		return bb->pieces[W_KING];
	} else{
		return bb->pieces[B_KING];
	}
}


uint64_t get_bitboard_combined(const struct bitboards *bb, enum piece pce_1, enum piece pce_2){
	return bb->pieces[pce_1] | bb->pieces[pce_2];
}



/*
 *
 * name: clear_bit
 * @param : board, square
 * @return : void
 *
 */
void clear_bit(uint64_t * brd, enum square sq)
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
void set_bit(uint64_t * brd, enum square sq)
{
    *brd = *brd | (uint64_t) (0x01ull << sq);
}



/*
 * Counts set bits in a uint64_t
 * name: count_bits
 * @param 	the board
 * @return	the number of set bits
 *
 */
uint8_t count_bits(const uint64_t bb)
{
    return (uint8_t) __builtin_popcountll(bb);
}




uint64_t get_bitboard_all_pieces(const struct bitboards *bb){
	return bb->board;
}



uint64_t get_bitboard_for_colour(const struct bitboards *bb, enum colour col)
{
	return bb->colour_bb[col];	
}

bool bitboard_stucts_are_same(const struct bitboards *bb1, const struct bitboards *bb2){
	
	for(int i = 0; i < NUM_PIECES; i++){
		if(bb1->pieces[i] != bb2->pieces[i]){
			return false;
		}
	}
	
	if (bb1->board != bb2->board){
		return false;
	}
		
	for(int i = 0; i < NUM_COLOURS; i++){
		if(bb1->colour_bb[i] != bb2->colour_bb[i]){
			return false;
		}
	}
	return true;		
}
