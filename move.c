/*
 * move.c
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
#include <ctype.h>
#include <assert.h>
#include "types.h"
#include "board.h"
#include "pieces.h"
#include "move.h"


void print_out_masks(board_t * masks);
void set_dest_sq_if_valid(int rank, int file, board_t * brd);


/*
 * Generates the occupancy bit masks for a king on each square on the board 
 * name: 	generate_king_occupancy_masks
 * @param	pointer to 64-elem array of bitboards
 * @return	filled out array.
 * 
 */
void generate_king_occupancy_masks(board_t * occ_mask_array)
{
	
    for (square_t sq = 0; sq < NUM_SQUARES; sq++) {
		// valid king moves, and resulting distance vector
		// +7, +8, +9
		// -1,  K, +1
		// -9, -8, -7

		// the equates to ranks as files as follows (rank/file):
		//	(+1, -1), 	(+1, 0), 	(+1, +1)
		//  (0, -1), 	XXX, 		(0, +1)
		// etc
		int dest_rank = 0;
		int dest_file = 0;
		
		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);
		
		board_t b = 0;

		// left 1 file, up 1 rank
		dest_rank = rank + 1;
		dest_file = file - 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		// no file move, up 1 rank
		dest_rank = rank + 1;
		dest_file = file;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		// right 1 file, up 1 rank
		dest_rank = rank + 1;
		dest_file = file + 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

///////////////////////////////////


		// left 1 file, no rank change
		dest_rank = rank;
		dest_file = file - 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);


		// right 1 file, no rank change
		dest_rank = rank;
		dest_file = file + 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

//////////////////////////////////////

		// right 1 file, down 1 rank
		dest_rank = rank - 1;
		dest_file = file + 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		// no file move, down 1 rank
		dest_rank = rank - 1;
		dest_file = file;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		// right 1 file, down 1 rank
		dest_rank = rank - 1;
		dest_file = file + 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		occ_mask_array[sq] = b;
    }
}

/*
 * Generates the destination bit masks for a knight for each square
 * name: generate_knight_occupancy_masks
 * @param	pointer to 64-elem array of bitboards
 * @return	filled out array.
 * 
 */
void generate_knight_occupancy_masks(board_t * occ_mask_array)
{
	
//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07
    for (square_t sq = 0; sq < NUM_SQUARES; sq++) {
		int dest_rank = 0;
		int dest_file = 0;
		
		// 8 destination squares are:
		// (-2 +8), (-1 + 16), (+1 + 16) (+2 + 8), 
		// (-2 -8), (-1 - 16), (+1 - 16) (+2 - 8),

		// ie, (left 2, up 8), (left 2, down 8), etc
		//
		// converting to ranks and files, we get:
		//  (left 2 files, up 1 rank), ( left 2 files, down 1 rank), etc
		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);
		
		board_t b = 0;

		// left 1 file, up 2 ranks
		dest_rank = rank + 2;
		dest_file = file - 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// left 1 file, down 2 ranks
		dest_rank = rank - 2;
		dest_file = file - 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		
		// left 2 files, up 1 rank
		dest_rank = rank + 1;
		dest_file = file - 2;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// left 2 files, down 1 rank
		dest_rank = rank - 1;
		dest_file = file - 2;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		// right 1 file, up 2 ranks
		dest_rank = rank + 2;
		dest_file = file + 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// right 1 file, down 2 ranks
		dest_rank = rank - 2;
		dest_file = file + 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		
		// right 2 files, up 1 rank
		dest_rank = rank + 1;
		dest_file = file + 2;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// right 2 files, down 1 rank
		dest_rank = rank - 1;
		dest_file = file + 2;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		occ_mask_array[sq] = b;
    }
}




void generate_rook_occupancy_masks(board_t * occ_mask_array)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    for (square_t sq = 0; sq < NUM_SQUARES; sq++) {
		
		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);
		
		board_t b = 0;
		
		// move up the ranks of this file
		for (int rank_iter = RANK_1; rank_iter <= RANK_8; rank_iter++){
			if (rank == rank_iter){
				// skip the rank we're on
				continue;
			}			
			set_dest_sq_if_valid(rank_iter, file, &b);
		}
			
		// move along the files of this rank
		for (int file_iter = FILE_A; file_iter <= FILE_H; file_iter++){
			if (file == file_iter){
				// skip the file we're on
				continue;
			}			
			set_dest_sq_if_valid(rank, file_iter, &b);
		}
			
			
		occ_mask_array[sq] = b;
    }
}



void generate_queen_occupancy_masks(board_t * occ_mask_array){
	// a queen is a rook + bishop
	generate_bishop_occupancy_masks(occ_mask_array);
	generate_rook_occupancy_masks(occ_mask_array);
}


// ignores first move and en passant
void generate_pawn_occupancy_masks(colour_t side, board_t * occ_mask_array){

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    for (square_t sq = 0; sq < NUM_SQUARES; sq++) {
		
		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);

		board_t b = 0;

		if (side == WHITE){
			int dest_rank = rank + 1;
			int dest_file_left = file - 1;
			int dest_file_right = file + 1;
			
			set_dest_sq_if_valid(dest_rank, dest_file_left, &b);
			set_dest_sq_if_valid(dest_rank, dest_file_right, &b);
		} else {
			int dest_rank = rank - 1;
			int dest_file_left = file - 1;
			int dest_file_right = file + 1;
			
			set_dest_sq_if_valid(dest_rank, dest_file_left, &b);
			set_dest_sq_if_valid(dest_rank, dest_file_right, &b);
		}
		occ_mask_array[sq] = b;
			
	}	
}




void generate_bishop_occupancy_masks(board_t * occ_mask_array)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

    for (square_t sq = 0; sq < NUM_SQUARES; sq++) {
						
		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);
		
		int dest_rank = 0;
		int dest_file = 0;
		board_t b = 0;
		
		// move left and down
		dest_rank = rank;
		dest_file = file;		
		while(IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)){
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank--;
			dest_file--;
		}
		
		// move left and up
		dest_rank = rank;
		dest_file = file;		
		while(IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)){
			set_dest_sq_if_valid(dest_rank, dest_file, &b);			
			dest_rank++;
			dest_file--;
		}
		
		// move right and down
		dest_rank = rank;
		dest_file = file;		
		while(IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)){
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank--;
			dest_file++;
		}
		
		// move right and up
		dest_rank = rank;
		dest_file = file;		
		while(IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)){
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank++;
			dest_file++;
		}

		
		// clear our square
		clear_bit(&b, sq);
					
		occ_mask_array[sq] = b;
    }
}









void set_dest_sq_if_valid(int rank, int file, board_t *brd){
	if ( IS_VALID_FILE(file) && IS_VALID_RANK(rank) ){
		int dest_sq = GET_SQUARE(rank, file);
		set_bit(brd, dest_sq);
	}
}



/*
 * Prints out occupancy masks as padded hex
 * name: print_rook_occupancy_masks
 * @param	-
 * @return 	-
 * 
 */
void print_rook_occupancy_masks(void)
{
    board_t *masks = malloc(sizeof(board_t) * NUM_SQUARES);
    memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

    generate_rook_occupancy_masks(masks);

    print_out_masks(masks);

    free(masks);
}


/*
 * Prints out occupancy masks as padded hex
 * name: print_knight_occupancy_masks
 * @param	-
 * @return 	-
 * 
 */
void print_knight_occupancy_masks(void)
{
    board_t *masks = malloc(sizeof(board_t) * NUM_SQUARES);
    memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

    generate_knight_occupancy_masks(masks);

    print_out_masks(masks);

    free(masks);
}

/*
 * Prints out occupancy masks as padded hex
 * name: print_king_occupancy_masks
 * @param	-
 * @return 	-
 * 
 */
void print_king_occupancy_masks(void)
{
    board_t *masks = malloc(sizeof(board_t) * NUM_SQUARES);
    memset(masks, 0, sizeof(board_t) * NUM_SQUARES);

    generate_king_occupancy_masks(masks);

    print_out_masks(masks);

    free(masks);
}

void print_out_masks(board_t * masks)
{
    for (int i = 0; i < NUM_SQUARES; i++) {
		printf("0x%016llx\n", masks[i]);
    }
}
