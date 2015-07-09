/*
 * occupancy_mask.c
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
#include "occupancy_mask.h"

void generate_king_occupancy_masks(board_t * occ_mask_array);
void generate_knight_occupancy_masks(board_t * occ_mask_array);
void generate_rook_occupancy_masks(board_t * occ_mask_array);
void generate_bishop_occupancy_masks(board_t * occ_mask_array);
void generate_queen_occupancy_masks(board_t * occ_mask_array);
void generate_white_pawn_occupancy_masks(board_t * occ_mask_array);
void generate_black_pawn_occupancy_masks(board_t * occ_mask_array);
void print_out_masks(board_t * masks);
void set_dest_sq_if_valid(int rank, int file, board_t *brd);


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

		// the equates to ranks and files as follows (rank/file):
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

		// left 1 file, down 1 rank
		dest_rank = rank - 1;
		dest_file = file - 1;
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
		
		//printf("rank/file: %d/%d\n", rank, file);
		
		board_t b = 0;

		// left 1 file, up 2 ranks
		dest_rank = rank + 2;
		dest_file = file - 1;
		//printf("dest_rank/dest_file: %d/%d\n", dest_rank, dest_file);
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// left 1 file, down 2 ranks
		dest_rank = rank - 2;
		dest_file = file - 1;
		//printf("dest_rank/dest_file: %d/%d\n", dest_rank, dest_file);

		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		
		// left 2 files, up 1 rank
		dest_rank = rank + 1;
		dest_file = file - 2;
		//printf("dest_rank/dest_file: %d/%d\n", dest_rank, dest_file);

		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// left 2 files, down 1 rank
		dest_rank = rank - 1;
		dest_file = file - 2;
		//printf("dest_rank/dest_file: %d/%d\n", dest_rank, dest_file);

		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		// right 1 file, up 2 ranks
		dest_rank = rank + 2;
		dest_file = file + 1;
		//printf("dest_rank/dest_file: %d/%d\n", dest_rank, dest_file);

		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// right 1 file, down 2 ranks
		dest_rank = rank - 2;
		dest_file = file + 1;
		//printf("dest_rank/dest_file: %d/%d\n", dest_rank, dest_file);

		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		
		// right 2 files, up 1 rank
		dest_rank = rank + 1;
		dest_file = file + 2;
		//printf("dest_rank/dest_file: %d/%d\n", dest_rank, dest_file);

		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// right 2 files, down 1 rank
		dest_rank = rank - 1;
		dest_file = file + 2;
		//printf("dest_rank/dest_file: %d/%d\n", dest_rank, dest_file);

		set_dest_sq_if_valid(dest_rank, dest_file, &b);

		occ_mask_array[sq] = b;
		
		//printf("****** 0x%016llx\n", b);
    }
}










/*
 * Generates the destination bit masks for a pawn for each square
 * name: generate_pawn_occupancy_masks
 * @param	pointer to 64-elem array of bitboards
 * @return	filled out array.
 * 
 * NOTE: ignores en-passent (for now!)
 */
void generate_white_pawn_occupancy_masks(board_t * occ_mask_array)
{
	
//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07
    for (square_t sq = a2; sq < a8; sq++) {
		// a2 n=> start at 2nd rank and ignore the 8th rank
		
		int dest_rank = 0;
		int dest_file = 0;
		
		// 2 destination squares are:
		// (-1 +1), (+1 +1)
		// ie, (left 1, up 1), (right 1, up 1)
		//
		// converting to ranks and files, we get:
		//  (left 1 file, up 1 rank), ( right 1 file, up 1 rank)
		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);
		
		board_t b = 0;

		// left 1 file, up 1 rank
		dest_rank = rank + 1;
		dest_file = file - 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// right 1 file, up 1 rank
		dest_rank = rank + 1;
		dest_file = file + 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		occ_mask_array[sq] = b;
    }
}



/*
 * Generates the destination bit masks for a pawn for each square
 * name: generate_pawn_occupancy_masks
 * @param	pointer to 64-elem array of bitboards
 * @return	filled out array.
 * 
 * NOTE: ignores en-passent (for now!)
 */
void generate_black_pawn_occupancy_masks(board_t * occ_mask_array)
{
	
//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47 
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07
    for (square_t sq = h7; sq >= a2; sq--) {
		// ignore top and bottom rank
		
		int dest_rank = 0;
		int dest_file = 0;
		
		// 2 destination squares are:
		// (-1 11), (+1 +1)
		// ie, (left 1, down 1), (right 1, down 1)
		//
		// converting to ranks and files, we get:
		//  (left 1 file, down 1 rank), ( right 1 file, down 1 rank)
		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);
		
		board_t b = 0;

		// left 1 file, down 1 rank
		dest_rank = rank - 1;
		dest_file = file - 1;
		set_dest_sq_if_valid(dest_rank, dest_file, &b);
		
		// right 1 file, down 1 rank
		dest_rank = rank - 1;
		dest_file = file + 1;
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
		for (int i = RANK_1; i <= RANK_8; i++){
			set_dest_sq_if_valid(i, file, &b);
		}
			
		// move along the files of this rank
		for (int i = FILE_A; i <= FILE_H; i++){
			set_dest_sq_if_valid(rank, i, &b);
		}
			
		// clear the square we're on
		clear_bit(&b, sq);
		
		occ_mask_array[sq] = b;
    }
}



void generate_queen_occupancy_masks(board_t * occ_mask_array){
	// a queen is a rook + bishop, so create a temp mask array
	board_t temp_bishop_mask [NUM_SQUARES] = {0}; 
	board_t temp_rook_mask [NUM_SQUARES] = {0}; 
		
	generate_bishop_occupancy_masks(temp_bishop_mask);
	generate_rook_occupancy_masks(temp_rook_mask);
	
	// combine masks
	for(int i = 0; i < NUM_SQUARES; i++){
		occ_mask_array[i] = temp_bishop_mask[i] | temp_rook_mask[i];
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
		
		//printf("rank/file : %d/%d\n", rank, file);
		
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
		//printf("---- OK  rank/file (sq=%d): %d/%d\n", dest_sq, rank, file);
	} else{
		//printf("XXXX bad rank/file: %d/%d\n", rank, file);
	}
}


/*
 * Prints out occupancy masks as padded hex
 * name: print_bishop_occupancy_masks
 * @param	-
 * @return 	-
 * 
 */
void print_occupancy_masks(piece_id_t pce)
{
	
    board_t masks[NUM_SQUARES] = {0};
    
	switch(pce){
		case W_KNIGHT:
		case B_KNIGHT:
			generate_knight_occupancy_masks(masks);
			break;
		case W_BISHOP:
		case B_BISHOP:
			generate_bishop_occupancy_masks(masks);
			break;
		case W_QUEEN:
		case B_QUEEN:
			generate_queen_occupancy_masks(masks);
			break;
			
		case W_ROOK:
		case B_ROOK:
			generate_rook_occupancy_masks(masks);
			break;
			
		case W_KING:
		case B_KING:
			generate_king_occupancy_masks(masks);
			break;

		case W_PAWN:
			generate_white_pawn_occupancy_masks(masks);
			break;
		case B_PAWN:
			generate_black_pawn_occupancy_masks(masks);
			break;
			
		default:
			break;
		}

    print_out_masks(masks);

}



/**
 * Thanks again to Bluefever Software for this code
 */
void print_occupancy_mask_as_board(board_t * mask, piece_id_t pce, square_t square)
{

	//int sq,piece;
	
	printf("\nOccupancy Mask:\n\n");
	
	for(int rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ",rank+1);	// enum is zero-based
		for(int file = FILE_A; file <= FILE_H; file++) {
			square_t sq = GET_SQUARE(rank, file);
			if (square == sq){
				char c = get_piece_label(pce);
				printf("%3c", c);
			} else{
				if (check_bit(mask, sq)){
					// attack square
					printf("  X");
				} else{
					printf("  -");
				}
			}
		}
		printf("\n");
	}
	
	printf("\n   ");
	for(int file = FILE_A; file <= FILE_H; file++) {
		printf("%3c",'a'+file);	
	}
	printf("\n\n");
	
}




void print_out_masks(board_t * masks)
{
    for (int i = 0; i < NUM_SQUARES; i++) {
		printf("0x%016llx\n", masks[i]);
    }
}


