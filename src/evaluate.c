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
#include <assert.h>
#include "types.h"
#include "board_utils.h"
#include "board.h"
#include "evaluate.h"
#include "pieces.h"
#include "move_gen.h"
#include "move_gen_utils.h"


static int32_t eval_piece(const struct board *brd, enum piece pce, const int8_t *pt);
static int32_t eval_pawn_dependant_pieces(const struct board *brd, enum piece target_pce, enum piece pawn, const int32_t *adj_vals);
static int32_t eval_paired_pieces(const struct board *brd, enum piece pce);
static int32_t eval_pawn_shield(const struct board *brd, enum piece king);


/*****************************************************
 * Piece tables 
 * 
 * These tables are from White's POV and [0] is a1
 * 
 * Values taken from BlueFever Software (youtube)
 * 
 * 
 * 
 */

static const int8_t PAWN_PT[NUM_SQUARES] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

static const int8_t KNIGHT_PT[NUM_SQUARES] = {
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0		
};

static const int8_t BISHOP_PT[NUM_SQUARES] = {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

static const int8_t ROOK_PT[NUM_SQUARES] = {
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0		
};


static const int8_t QUEEN_PT[NUM_SQUARES] ={
-20	,	-10	,	-10	, 	-5	, 	-5	,	-10	,	-10	,	-20	,
-10	,  	0	,  	5	,  	0	,  	0	,  	0	,  	0	,	-10	,
-10	,  	5	,  	5	,  	5	,  	5	,  	5	,  	0	,	-10	,
  0	,  	0	,  	5	,  	5	,  	5	,  	5	,  	0	, 	-5	,	
 -5	,  	0	,  	5	,  	5	,  	5	,  	5	,  	0	, 	-5	,
-10	,  	0	,  	5	,  	5	,  	5	,  	5	,  	0	,	-10	,
-10	,  	0	,  	0	,  	0	,  	0	,  	0	,  	0	,	-10	,
-20	,	-10	,	-10	, 	-5	, 	-5	,	-10	,	-10	,	-20
};



// a mirror array to allow for looking up the 
// piece tables for BLACK

static const int8_t MIRROR_PT[NUM_SQUARES] ={
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};

#define MIRROR_SQUARE(sq)	(MIRROR_PT[sq])

// knights are less valuable when there are fewer pawns
// numbers obtained from :
// http://chessprogramming.wikispaces.com/CPW-Engine_eval
static const int32_t knight_adj[8] = { -16, -12, -8, -4,  0,  4,  8, 12};

// rooks are less valuable when there are more pawns
static const int32_t rook_adj[8] =   {12,  9,  6,  3,  0, -3, -6, -9};

// define additional score values based on having paired pieces
// these values are taken from:
// http://chessprogramming.wikispaces.com/CPW-Engine_eval_init
enum paired_piece_val{
	BISHOP_PAIR_ADJ 	= 30,
	KNIGHT_PAIR_ADJ  	= 8,
	ROOK_PAIR_ADJ		= 16
};

// value of having a pawn shield
// these values are taken from:
// http://chessprogramming.wikispaces.com/CPW-Engine_eval_init
enum pawn_shield_val{
	PAWN_SHIELD_1 	= 10,	// on rank immediately in front of king
    PAWN_SHIELD_2 	= 5		// moved off immediate rank in front of king
};

// holds all the individual elements used for evaluating the
// position. Done mainly to allow for debugging of the 
// individual eveluation elements and their effect on the overall
// score.
struct eval_info{
	int32_t 	material[NUM_COLOURS];			// plain material count
	int32_t		piece_position[NUM_COLOURS];	// piece position on the board
	int32_t		knight[NUM_COLOURS];			// knight, based on # pawns present
	int32_t		rook[NUM_COLOURS];				// rook, based on # pawns present
	int32_t		paired_pieces[NUM_COLOURS];		// bonus based on piece pairs
	int32_t		pawn_shield[NUM_COLOURS];		// benefits of pawn shield
	
};

// TODO:
// additional eval elements to be added:
//		- mobility
//		- adjust for middle game and end game
// 		- # pieces attacking squares surrounding king
// 		- blockages (eg, bad bishops)

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


	// adjust for piece position
	// =========================
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

	// adjust value of knights based on number of pawns
	// ================================================
	score += eval_pawn_dependant_pieces(brd, W_KNIGHT, W_PAWN, knight_adj);
	score -= eval_pawn_dependant_pieces(brd, B_KNIGHT, B_PAWN, knight_adj);

	// now adjust value of rooks based on number of pawns
	// ==================================================
	score += eval_pawn_dependant_pieces(brd, W_ROOK, W_PAWN, rook_adj);
	score -= eval_pawn_dependant_pieces(brd, B_ROOK, B_PAWN, rook_adj);

	// adjust score based on paired pieces
	// ===================================
	score += eval_paired_pieces(brd, W_BISHOP);
	score += eval_paired_pieces(brd, W_KNIGHT);
	score += eval_paired_pieces(brd, W_ROOK);
	score -= eval_paired_pieces(brd, B_BISHOP);
	score -= eval_paired_pieces(brd, B_KNIGHT);
	score -= eval_paired_pieces(brd, B_ROOK);
	
	// adjust for benefits of having a pawn shield in front of king
	// ============================================================
	score += eval_pawn_shield(brd, W_KING);
	score -= eval_pawn_shield(brd, B_KING);

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


// knights lose value as pawns disappear, and rooks gain value
inline static int32_t eval_pawn_dependant_pieces(const struct board *brd, 
					enum piece target_pce, enum piece pawn, const int32_t *adj_vals){

	// for each target, adjust the score based on the number of friendly pawns
	// -------
	
	uint64_t target_bb = brd->bitboards[target_pce];
	if (target_bb == 0){
		// no pieces of this type, so nothing to adjust by
		return 0;
	}
	uint8_t num_targets = count_bits(target_bb);

	uint64_t pawn_bb = brd->bitboards[pawn];
	uint8_t num_pawns = count_bits(pawn_bb);

	int32_t adj_val = *(adj_vals + num_pawns);
	
	int32_t retval = 0;
	for(int i = 0; i < num_targets; i++){
		retval += adj_val;
	}

	return retval;
} 


inline static int32_t eval_paired_pieces(const struct board *brd, enum piece pce){
	uint64_t pce_bb = brd->bitboards[pce];
	uint8_t num_pieces = count_bits(pce_bb);
	
	if (num_pieces <= 1){
		// no adjustment
		return 0;
	}
	
	switch(pce){
		case W_BISHOP:
		case B_BISHOP:
			return BISHOP_PAIR_ADJ;
		case W_KNIGHT:
		case B_KNIGHT:
			return KNIGHT_PAIR_ADJ;
		case W_ROOK:
		case B_ROOK:
			return ROOK_PAIR_ADJ;
		default:
			assert(false);			
	}
	assert(false);
}

inline static int32_t eval_pawn_shield(const struct board *brd, enum piece king){

	enum colour col = GET_COLOUR(king);
	
	enum square king_sq = brd->king_sq[col];
	uint8_t king_file = GET_FILE(king_sq);

	int32_t retval = 0;

	if (col == WHITE){
		if (king_file > FILE_E ) {
			if (is_piece_on_square(brd, W_PAWN, f2))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, W_PAWN, f3))
				retval += PAWN_SHIELD_2;

			if (is_piece_on_square(brd, W_PAWN, g2))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, W_PAWN, g3))
				retval += PAWN_SHIELD_2;

			if (is_piece_on_square(brd, W_PAWN, h2))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, W_PAWN, h3))
				retval += PAWN_SHIELD_2;
				
		} else if (king_file < FILE_D){
			if (is_piece_on_square(brd, W_PAWN, a2))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, W_PAWN, a3))
				retval += PAWN_SHIELD_2;

			if (is_piece_on_square(brd, W_PAWN, b2))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, W_PAWN, b3))
				retval += PAWN_SHIELD_2;

			if (is_piece_on_square(brd, W_PAWN, c2))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, W_PAWN, c3))
				retval += PAWN_SHIELD_2;
		}

	} else {
		if (king_file > FILE_E){
			if (is_piece_on_square(brd, B_PAWN, f7))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, B_PAWN, f6))
				retval += PAWN_SHIELD_2;

			if (is_piece_on_square(brd, B_PAWN, g7))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, B_PAWN, g6))
				retval += PAWN_SHIELD_2;

			if (is_piece_on_square(brd, B_PAWN, h7))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, B_PAWN, h6))
				retval += PAWN_SHIELD_2;
				
		} else if (king_file < FILE_D){
			if (is_piece_on_square(brd, B_PAWN, a7))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, B_PAWN, a6))
				retval += PAWN_SHIELD_2;

			if (is_piece_on_square(brd, B_PAWN, b7))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, B_PAWN, b6))
				retval += PAWN_SHIELD_2;

			if (is_piece_on_square(brd, B_PAWN, c7))
				retval += PAWN_SHIELD_1;
			else if (is_piece_on_square(brd, B_PAWN, c6))
				retval += PAWN_SHIELD_2;
		}
	}

	return retval;
}





