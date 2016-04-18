/*
 * occupancy_mask.c
 * 
 * ---------------------------------------------------------------------
 * DESCRIPTION: code for generating bitbard occupancy masks
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
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "types.h"
#include "board.h"
#include "pieces.h"
#include "utils.h"
#include "makemove.h"
#include "occupancy_mask.h"

void generate_king_occupancy_masks(uint64_t * occ_mask_array);
void generate_knight_occupancy_masks(uint64_t * occ_mask_array);
void generate_rook_occupancy_masks(uint64_t * occ_mask_array);
void generate_bishop_occupancy_masks(uint64_t * occ_mask_array);
void generate_queen_occupancy_masks(uint64_t * occ_mask_array);
void generate_white_pawn_occupancy_masks(uint64_t * occ_mask_array);
void generate_black_pawn_occupancy_masks(uint64_t * occ_mask_array);
void print_out_masks(const uint64_t * masks);
void set_dest_sq_if_valid(int rank, int file, uint64_t * brd);
uint64_t get_occupancy_mask(enum piece pce, enum square sq);

uint64_t get_occupancy_mask(enum piece pce, enum square sq)
{
	switch (pce) {
	case W_BISHOP:
	case B_BISHOP:
		return GET_BISHOP_OCC_MASK(sq);
	case W_ROOK:
	case B_ROOK:
		return GET_ROOK_OCC_MASK(sq);
	case W_QUEEN:
	case B_QUEEN:
		return GET_QUEEN_OCC_MASK(sq);
	case W_KNIGHT:
	case B_KNIGHT:
		return GET_KNIGHT_OCC_MASK(sq);
	case W_KING:
	case B_KING:
		return GET_KING_OCC_MASK(sq);
	case W_PAWN:
	case B_PAWN:
	
	default:
		//assert(pce == NO_PIECE);
		return 0;
	}
}

void generate_rank_and_file_masks(void)
{

	uint64_t r_masks[8] = { 0 };
	uint64_t f_masks[8] = { 0 };

	for (int i = RANK_1; i <= RANK_8; i++) {
		uint64_t mask = 0;
		for (int j = FILE_A; j <= FILE_H; j++) {

			enum square sq = GET_SQUARE(i, j);
			set_bit(&mask, sq);
		}
		r_masks[i] = mask;
	}

	for (int i = FILE_A; i <= FILE_H; i++) {
		uint64_t mask = 0;
		for (int j = RANK_1; j <= RANK_8; j++) {

			enum square sq = GET_SQUARE(j, i);
			set_bit(&mask, sq);
		}
		f_masks[i] = mask;
	}

	printf("RANK_MASKS\n");
	for (int i = 0; i < 8; i++) {
		printf("%jx\n", r_masks[i]);
	}

	printf("FILE_MASKS\n");
	for (int i = 0; i < 8; i++) {
		printf("%jx\n", f_masks[i]);
	}

}

/*
 * Generates the occupancy bit masks for a king on each square on the board
 * name: 	generate_king_occupancy_masks
 * @param	pointer to 64-elem array of bitboards
 * @return	filled out array.
 *
 */
void generate_king_occupancy_masks(uint64_t * occ_mask_array)
{

	for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		// valid king moves, and resulting distance vector
		// +7, +8, +9
		// -1,  K, +1
		// -9, -8, -7

		// the equates to ranks and files as follows (rank/file):
		//      (+1, -1),       (+1, 0),        (+1, +1)
		//  (0, -1),    XXX,            (0, +1)
		// etc
		int dest_rank = 0;
		int dest_file = 0;

		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);

		uint64_t b = 0;

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
void generate_knight_occupancy_masks(uint64_t * occ_mask_array)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

	for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
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

		uint64_t b = 0;

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
void generate_white_pawn_occupancy_masks(uint64_t * occ_mask_array)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

	for (enum square sq = a2; sq < a8; sq++) {
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

		uint64_t b = 0;

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
void generate_black_pawn_occupancy_masks(uint64_t * occ_mask_array)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07
	for (enum square sq = h7; sq >= a2; sq--) {
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

		uint64_t b = 0;

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

void generate_rook_occupancy_masks(uint64_t * occ_mask_array)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

	for (enum square sq = 0; sq < NUM_SQUARES; sq++) {

		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);

		uint64_t b = 0;

		// move up the ranks of this file
		for (int i = RANK_1; i <= RANK_8; i++) {
			set_dest_sq_if_valid(i, file, &b);
		}

		// move along the files of this rank
		for (int i = FILE_A; i <= FILE_H; i++) {
			set_dest_sq_if_valid(rank, i, &b);
		}

		// clear the square we're on
		clear_bit(&b, sq);

		occ_mask_array[sq] = b;
	}
}

void generate_queen_occupancy_masks(uint64_t * occ_mask_array)
{
	// a queen is a rook + bishop, so create a temp mask array
	uint64_t temp_bishop_mask[NUM_SQUARES] = { 0 };
	uint64_t temp_rook_mask[NUM_SQUARES] = { 0 };

	generate_bishop_occupancy_masks(temp_bishop_mask);
	generate_rook_occupancy_masks(temp_rook_mask);

	// combine masks
	for (int i = 0; i < NUM_SQUARES; i++) {
		occ_mask_array[i] = temp_bishop_mask[i] | temp_rook_mask[i];
	}
}

void generate_bishop_occupancy_masks(uint64_t * occ_mask_array)
{

//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 40 05 06 07

	for (enum square sq = 0; sq < NUM_SQUARES; sq++) {

		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);

		//printf("rank/file : %d/%d\n", rank, file);

		int dest_rank = 0;
		int dest_file = 0;
		uint64_t b = 0;

		// move left and down
		dest_rank = rank;
		dest_file = file;
		while (IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)) {
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank--;
			dest_file--;
		}

		// move left and up
		dest_rank = rank;
		dest_file = file;
		while (IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)) {
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank++;
			dest_file--;
		}

		// move right and down
		dest_rank = rank;
		dest_file = file;
		while (IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)) {
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank--;
			dest_file++;
		}

		// move right and up
		dest_rank = rank;
		dest_file = file;
		while (IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)) {
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank++;
			dest_file++;
		}

		// clear our square
		clear_bit(&b, sq);

		occ_mask_array[sq] = b;
	}
}

void set_dest_sq_if_valid(int rank, int file, uint64_t * brd)
{
	if (IS_VALID_FILE(file) && IS_VALID_RANK(rank)) {
		int dest_sq = GET_SQUARE(rank, file);
		set_bit(brd, (enum square)dest_sq);
		//printf("---- OK  rank/file (sq=%d): %d/%d\n", dest_sq, rank, file);
	} else {
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
void print_occupancy_masks(enum piece pce)
{

	uint64_t masks[NUM_SQUARES] = { 0 };

	switch (pce) {
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

// when checking to see if a queen or bishop can attack a
// square, having a localised occupancy mask can be useful
void generate_diagonal_occupancy_masks(void)
{

	uint64_t diagonal[NUM_SQUARES] = { 0 };	// bottom left to upper right
	uint64_t antidiagonal[NUM_SQUARES] = { 0 };	// top left to bottom right

	for (enum square sq = 0; sq < NUM_SQUARES; sq++) {

		int rank = GET_RANK(sq);
		int file = GET_FILE(sq);

		//printf("rank/file : %d/%d\n", rank, file);

		int dest_rank = 0;
		int dest_file = 0;
		uint64_t b = 0;

		// move SW
		dest_rank = rank;
		dest_file = file;
		while (IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)) {
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank--;
			dest_file--;
		}
		// clear our square
		clear_bit(&b, sq);
		diagonal[sq] |= b;

		// move NW
		b = 0;
		dest_rank = rank;
		dest_file = file;
		while (IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)) {
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank++;
			dest_file--;
		}
		// clear our square
		clear_bit(&b, sq);
		antidiagonal[sq] |= b;

		// move SE
		b = 0;
		dest_rank = rank;
		dest_file = file;
		while (IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)) {
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank--;
			dest_file++;
		}
		// clear our square
		clear_bit(&b, sq);
		antidiagonal[sq] |= b;

		// move NE
		b = 0;
		dest_rank = rank;
		dest_file = file;
		while (IS_VALID_FILE(dest_file) && IS_VALID_RANK(dest_rank)) {
			set_dest_sq_if_valid(dest_rank, dest_file, &b);
			dest_rank++;
			dest_file++;
		}
		// clear our square
		clear_bit(&b, sq);
		diagonal[sq] |= b;

	}

	printf("DIAGONAL\n");
	print_out_masks(diagonal);
	printf("ANTI_DIAGONAL\n");
	print_out_masks(antidiagonal);

}

/**
 * Thanks again to Bluefever Software for this code
 */
void print_mask_as_board(const uint64_t * mask)
{
	uint64_t m = *mask;
	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ", rank + 1);	// enum is zero-based
		for (int file = FILE_A; file <= FILE_H; file++) {
			enum square sq = GET_SQUARE(rank, file);
			if (CHECK_BIT(m, sq)) {
				// attack square
				printf("  X");
			} else {
				printf("  -");
			}
		}
		printf("\n");
	}

	printf("\n   ");
	for (int file = FILE_A; file <= FILE_H; file++) {
		printf("%3c", 'a' + file);
	}
	printf("\n\n");

}

void print_out_masks(const uint64_t * masks)
{
	for (int i = 0; i < NUM_SQUARES; i++) {
		printf("%jx\n", masks[i]);

		//print_mask_as_board(&masks[i], W_QUEEN, i);

	}
}
