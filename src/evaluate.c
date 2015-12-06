/*
 * evaluate.c
 * 
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for evaluating a position and 
 * generating a score
 * --------------------------------------------------------------------- 
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
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "board_utils.h"
#include "evaluate.h"
#include "pieces.h"
#include "move.h"


static int32_t eval_piece(const struct board *brd, enum piece pce, const int8_t *pt);



/*****************************************************
 * Piece tables 
 * 
 * These tables are from White's POV and [0] is a1
 * 
 * Values taken from :
 * 
 * http://chessprogramming.wikispaces.com/Simplified+evaluation+function#Piece-Square%20Tables
 * 
 */

static const int8_t PAWN_PT[NUM_SQUARES] = {
 0,  0,  0,  0,  0,  0,  0,  0,
50, 50, 50, 50, 50, 50, 50, 50,
10, 10, 20, 30, 30, 20, 10, 10,
 5,  5, 10, 25, 25, 10,  5,  5,
 0,  0,  0, 20, 20,  0,  0,  0,
 5, -5,-10,  0,  0,-10, -5,  5,
 5, 10, 10,-20,-20, 10, 10,  5,
 0,  0,  0,  0,  0,  0,  0,  0
};


static const int8_t KNIGHT_PT[NUM_SQUARES] ={
-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  0,  0,  0,-20,-40,
-30,  0, 10, 15, 15, 10,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 10, 15, 15, 10,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50,
};


static const int8_t BISHOP_PT[NUM_SQUARES] ={
-20,-10,-10,-10,-10,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  5,  0,  0,  0,  0,  5,-10,
-20,-10,-10,-10,-10,-10,-10,-20,
};


static const int8_t ROOK_PT[NUM_SQUARES] ={
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0	
};


static const int8_t QUEEN_PT[NUM_SQUARES] ={
-20,-10,-10, -5, -5,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5,  5,  5,  5,  0,-10,
 -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
-10,  5,  5,  5,  5,  5,  0,-10,
-10,  0,  5,  0,  0,  0,  0,-10,
-20,-10,-10, -5, -5,-10,-10,-20
};



// a mirror array to allow for looking up the 
// piece tables for BLACK

static const int8_t MIRROR_PT[NUM_SQUARES] ={
	56,	57,	58,	59,	60,	61,	62,	63,
	48,	49,	50,	51,	52,	53,	54,	55,
	40,	41,	42,	43,	44,	45,	46,	47,
	32,	33,	34,	35,	36,	37,	38,	39,
	24,	25,	26,	27,	28,	29,	30,	31,
	16,	17,	18,	19,	20,	21,	22,	23,
	8,	9,	10,	11,	12,	13,	14,	15,	
	0,	1,	2,	3,	4,	5,	6,	7
};

#define MIRROR_SQUARE(sq)	(MIRROR_PT[sq])

/*
 * 
 * name: evaluate_position
 * @param
 * @return >0 for white, <0 for black
 * 
 */
int32_t evaluate_position(const struct board *brd)
{
	// initially based on material value
	int32_t score = (int32_t) (brd->material[WHITE] - brd->material[BLACK]);

	// NOTE: black material is -ve, white is +ve
	//	
	// now adjust for piece position
	score += eval_piece(brd, W_PAWN, PAWN_PT);
	score -= eval_piece(brd, B_PAWN, PAWN_PT);

	score += eval_piece(brd, W_BISHOP, BISHOP_PT);
	score -= eval_piece(brd, B_BISHOP, BISHOP_PT);

	score += eval_piece(brd, W_KNIGHT, KNIGHT_PT);
	score -= eval_piece(brd, B_KNIGHT, KNIGHT_PT);

	score += eval_piece(brd, W_ROOK, ROOK_PT);
	score -= eval_piece(brd, B_ROOK, ROOK_PT);

	score += eval_piece(brd, W_QUEEN, QUEEN_PT);
	score -= eval_piece(brd, B_QUEEN, QUEEN_PT);

	if (brd->side_to_move == WHITE) {
		return score;
	} else {
		return -score;
	}
}

inline static int32_t eval_piece(const struct board *brd, enum piece pce,
			     const int8_t * pt)
{
	int32_t score = 0;

	uint64_t bb = brd->bitboards[pce];
	if (IS_BLACK(pce)) {
		while (bb != 0) {
			enum square sq = pop_1st_bit(&bb);
			score += pt[MIRROR_SQUARE(sq)];
		}
	} else {
		while (bb != 0) {
			enum square sq = pop_1st_bit(&bb);
			score += pt[sq];
		}
	}
	return score;
}
