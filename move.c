/*
 * move.c
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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "pieces.h"


/*
 * Generates the occupancy bit masks for a king on each square on the board 
 * name: 	generate_king_occupancy_masks
 * @param	pointer to 64-elem array of bitboards
 * @return	filled out array.
 * 
 */
void generate_king_occupancy_masks(board_t *occ_mask_array){
	for(int i = 0; i < NUM_SQUARES; i++){
		// +7, +8, +9
		// -1,  K, +1
		// -9, -8, -7
		
		board_t b = 0;
		
		if (i + 7 < NUM_SQUARES)
			b |= (0x1ull << (i + 7));
		if (i + 8 < NUM_SQUARES)
			b |= (0x1ull << (i + 8));
		if (i + 9 < NUM_SQUARES)
			b |= (0x1ull << (i + 9));
				
		if (i - 1 >= 0)
			b |= (0x1ull << (i - 1));
		if (i + 1 < NUM_SQUARES)
			b |= (0x1ull << (i + 1));
		
		if (i - 9 >= 0)
			b |= (0x1ull << (i - 9));
		if (i - 8 >= 0)
			b |= (0x1ull << (i - 8));
		if (i - 7 >= 0)
			b |= (0x1ull << (i - 7));

		occ_mask_array[i] = b;
	}	
}

/*
 * Generates the destination bit masks for a knight for each square
 * name: generate_knight_occupancy_masks
 * @param	pointer to 64-elem array of bitboards
 * @return	filled out array.
 * 
 */
void generate_knight_occupancy_masks(board_t *occ_mask_array){
	for(int i = 0; i < NUM_SQUARES; i++){
		// 8 destination squares are:
		// (-2 +8), (-1 + 16), (+1 + 16) (+2 + 8), 
		// (-2 -8), (-1 - 16), (+1 - 16) (+2 - 8),
		
		board_t b = 0;
		
		if (i + 6 < NUM_SQUARES)
			b |= (0x1ull << (i + 6));
		if (i + 15 < NUM_SQUARES)
			b |= (0x1ull << (i + 15));
		if (i + 17 < NUM_SQUARES)
			b |= (0x1ull << (i + 17));
		if (i + 10 < NUM_SQUARES)
			b |= (0x1ull << (i + 10));
		
		if (i - 10 >= 0)
			b |= (0x1ull << (i - 10));
		if (i - 17 >= 0)
			b |= (0x1ull << (i - 17));
		if (i - 15 >= 0)
			b |= (0x1ull << (i - 15));
		if (i - 6 >= 0)
			b |= (0x1ull << (i - 6));

		occ_mask_array[i] = b;
	}	
}

void print_knight_occupancy_masks(){
	board_t * masks = malloc(sizeof(board_t) * NUM_SQUARES);
	memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

	generate_knight_occupancy_masks(masks);

	for(int i = 0; i < NUM_SQUARES; i++){
		printf("0x%016llx\n", masks[i]);
	}

	free(masks);
}


void print_king_occupancy_masks(){
	board_t * masks = malloc(sizeof(board_t) * NUM_SQUARES);
	memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

	generate_king_occupancy_masks(masks);

	for(int i = 0; i < NUM_SQUARES; i++){
		printf("0x%016llx\n", masks[i]);
	}

	free(masks);
}



