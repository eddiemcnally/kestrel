/*
 * board_new.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for manipulating pieces and the board
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

static inline void update_pawn_control(struct new_board *brd, const enum colour col, const enum square sq, enum rank rank, enum file file, int8_t val);



struct new_board {
	// bitboards
	struct bitboards bitboards;

	// contains the piece on each square
	enum piece pieces[NUM_SQUARES];


	// we need to look up the king position very frequently,
	// so save it for a quick lookup, rather than extracting it
	// from a bitboard each time
	enum square king_sq[NUM_COLOURS];

	// maintain separate info about the pawns to simplify the
	// evaluation of pawn structure, open files, etc
	uint8_t pawns_on_file[NUM_COLOURS][NUM_FILES];
	uint8_t pawns_on_rank[NUM_COLOURS][NUM_RANKS];
	uint8_t pawn_control[NUM_COLOURS][NUM_SQUARES];

	// indexed by enum colour, contains sum of all piece values
	uint32_t material[NUM_COLOURS];
};




void clear_board(struct new_board *brd)
{
	clear_bitboards(&brd->bitboards);

	for(int i = 0; i < NUM_SQUARES; i++) {
		brd->pieces[i] = NO_PIECE;
	}

	for(int i = 0; i < NUM_COLOURS; i++) {
		brd->pieces[i] = NO_SQUARE;
	}

	for(int i = 0; i < NUM_COLOURS; i++) {
		for(int j = 0; j < NUM_FILES; j++) {
			brd->pawns_on_file[i][j] = NO_PIECE;
		}
	}

	for(int i = 0; i < NUM_COLOURS; i++) {
		for(int j = 0; j < NUM_RANKS; j++) {
			brd->pawns_on_rank[i][j] = NO_PIECE;
		}
	}

	for(int i = 0; i < NUM_COLOURS; i++) {
		for(int j = 0; j < NUM_SQUARES; j++) {
			brd->pawns_on_rank[i][j] = 0;
		}
	}

	for(int i = 0; i < NUM_COLOURS; i++) {
		brd->material[i] = 0;
	}
}



void add_to_board(struct new_board *brd, enum piece pce, enum square sq)
{
	add_piece_to_bitboards(&brd->bitboards, pce, sq);

	brd->pieces[square] = pce;

	enum colour pce_col = GET_COLOUR(pce);

	// update king square
	if (IS_KING(pce)) {
		brd->king_sq[pce_col] = sq;

	}

	if (IS_PAWN(pce)) {

		enum rank r = get_rank(sq);
		enum file f = get_file(sq);

		brd->pawns_on_file[pce_col][f] += 1;
		brd->pawns_on_rank[pce_col][r] += 1;

		update_pawn_control(brd, pce+col, sq, rank, file, 1);

	}

	// update material
	brd->material[pce_col] += piece_values[pce];


}









void remove_from_board(struct new_board *brd, enum piece pce, enum square sq)
{
	remove_piece_from_bitboards(&brd->bitboards, pce, sq);

	brd->pieces[square] = NO_PIECE;

	enum colour pce_col = GET_COLOUR(pce);

	// update king square
	if (IS_KING(pce)) {
		brd->king_sq[pce_col] = NO_PIECE;
	}

	if (IS_PAWN(pce)) {
		enum rank r = get_rank(sq);
		enum file f = get_file(sq);

		brd->pawns_on_file[pce_col][f] -= 1;
		brd->pawns_on_rank[pce_col][r] -= 1;

		update_pawn_control(brd, pce_col, sq, rank, file, -1);
	}

	// update material
	brd->material[pce_col] -= piece_values[pce];
}

int32_t get_material_value(const struct new_board *brd, enum colour col)
{
	return brd->material[col];
}



enum square get_king_square(const struct new_board *brd, enum colour col)
{
	return brd->king_sq[col];
};






static inline void update_pawn_control(struct new_board *brd, const enum colour col, const enum square sq, enum rank rank, enum file file, int8_t val)
{
	int8_t next_sq = 0;

	if (col == WHITE) {
		if (file > FILE_A) {
			if (rank < RANK_8) {
				next_sq = (int8_t)(sq + NW);
				brd->pawn_control[col][next_sq] += val;
			}
		}
		if (file < FILE_H) {
			if (rank < RANK_8) {
				next_sq = (int8_t)(sq + NE);
				pos->pawn_control[col][next_sq] += val;
			}
		}
	} else {
		if (file > FILE_A) {
			if (rank > RANK_1) {
				next_sq = (int8_t)((int8_t)sq + (int8_t)SW);
				pos->pawn_control[col][next_sq] += val;
			}
		}
		if (file < FILE_H) {
			if (rank > RANK_1) {
				next_sq = (int8_t)((int8_t)sq + (int8_t)SE);
				pos->pawn_control[col][next_sq] += val;
			}
		}
	}
}




