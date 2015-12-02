/*
 * move.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: code to generate valid moves from a given board
 * position.
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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "board.h"
#include "attack.h"
#include "board_utils.h"
#include "pieces.h"
#include "makemove.h"
#include "occupancy_mask.h"
#include "move.h"
#include "utils.h"

static void add_pawn_capture_move(enum colour col,
				  enum square from, enum square to,
				  enum piece capture, struct move_list *mvl);
static void add_pawn_move(struct board *brd, enum colour col, 
					enum square from, enum square to, struct move_list *mvl);
static void add_capture_move(mv_bitmap move_bitmap,
							struct move_list *mvlist, uint32_t score);
static void add_quiet_move(struct board *brd, mv_bitmap mv, 
								struct move_list *mvlist, enum piece pce);
static void generate_white_pawn_moves(struct board *brd,
				      struct move_list *mvl, const bool only_capture_moves);
static void generate_black_pawn_moves(struct board *brd,
				      struct move_list *mvl, const bool only_capture_moves);
static uint64_t get_horizontal_mask(const enum square sq);
static uint64_t get_vertical_mask(const enum square sq);
static uint64_t get_positive_diagonal_mask(const enum square sq);
static uint64_t get_negative_diagonal_mask(const enum square sq);
static struct move_list *get_empty_move_list(void);
static void generate_knight_piece_moves(struct board *brd,
					       struct move_list *mvl,
					       enum piece knight,
					       enum colour opposite_col, 
					       const bool only_capture_moves);
static inline void generate_king_moves(struct board *brd,
				       struct move_list *mvl, enum piece pce,
				       enum colour col, enum colour opposite_col, 
				       const bool only_capture_moves);
static void generate_black_castle_moves(struct board *brd,
					struct move_list *mvl);
static void generate_white_castle_moves(struct board *brd,
					struct move_list *mvl);
static void generate_sliding_horizontal_vertical_moves(struct board
						       *brd, struct move_list
						       *mvl, enum piece pce, 
						       const bool only_capture_moves);
static void generate_sliding_diagonal_moves(struct board *brd,
					    struct move_list *mvl,
					    enum piece pce, const bool only_capture_moves);
static void generate_queen_moves(struct board *brd,
				 struct move_list *mvl, enum piece pce, 
				 const bool only_capture_moves);
static bool is_move_in_list(struct move_list *mvl, mv_bitmap mv);





/* indexed using enum square
 * Represents the horizontal squares that a rook can move to, when
 * on a specific square
 */
static const uint64_t horizontal_move_mask[] = {
	0x00000000000000ff, 0x00000000000000ff, 0x00000000000000ff,
	0x00000000000000ff,
	0x00000000000000ff, 0x00000000000000ff, 0x00000000000000ff,
	0x00000000000000ff,
	0x000000000000ff00, 0x000000000000ff00, 0x000000000000ff00,
	0x000000000000ff00,
	0x000000000000ff00, 0x000000000000ff00, 0x000000000000ff00,
	0x000000000000ff00,
	0x0000000000ff0000, 0x0000000000ff0000, 0x0000000000ff0000,
	0x0000000000ff0000,
	0x0000000000ff0000, 0x0000000000ff0000, 0x0000000000ff0000,
	0x0000000000ff0000,
	0x00000000ff000000, 0x00000000ff000000, 0x00000000ff000000,
	0x00000000ff000000,
	0x00000000ff000000, 0x00000000ff000000, 0x00000000ff000000,
	0x00000000ff000000,
	0x000000ff00000000, 0x000000ff00000000, 0x000000ff00000000,
	0x000000ff00000000,
	0x000000ff00000000, 0x000000ff00000000, 0x000000ff00000000,
	0x000000ff00000000,
	0x0000ff0000000000, 0x0000ff0000000000, 0x0000ff0000000000,
	0x0000ff0000000000,
	0x0000ff0000000000, 0x0000ff0000000000, 0x0000ff0000000000,
	0x0000ff0000000000,
	0x00ff000000000000, 0x00ff000000000000, 0x00ff000000000000,
	0x00ff000000000000,
	0x00ff000000000000, 0x00ff000000000000, 0x00ff000000000000,
	0x00ff000000000000,
	0xff00000000000000, 0xff00000000000000, 0xff00000000000000,
	0xff00000000000000,
	0xff00000000000000, 0xff00000000000000, 0xff00000000000000,
	0xff00000000000000
};

/* indexed using enum square
 * Represents the vertical squares that a rook can move to, when
 * on a specific square
 */
static const uint64_t vertical_move_mask[] = {
	0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040,
	0x8080808080808080,
	0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010, 0x2020202020202020, 0x4040404040404040,
	0x8080808080808080
};

/* indexed using enum square
 * Represents the bottom-left to top-right diagonals that a bishop can move to, when
 * on a specific square
 */
static const uint64_t positive_diagonal_masks[] = {
	0x8040201008040200, 0x0080402010080400, 0x0000804020100800,
	0x0000008040201000,
	0x0000000080402000, 0x0000000000804000, 0x0000000000008000,
	0x0000000000000000,
	0x4020100804020000, 0x8040201008040001, 0x0080402010080002,
	0x0000804020100004,
	0x0000008040200008, 0x0000000080400010, 0x0000000000800020,
	0x0000000000000040,
	0x2010080402000000, 0x4020100804000100, 0x8040201008000201,
	0x0080402010000402,
	0x0000804020000804, 0x0000008040001008, 0x0000000080002010,
	0x0000000000004020,
	0x1008040200000000, 0x2010080400010000, 0x4020100800020100,
	0x8040201000040201,
	0x0080402000080402, 0x0000804000100804, 0x0000008000201008,
	0x0000000000402010,
	0x0804020000000000, 0x1008040001000000, 0x2010080002010000,
	0x4020100004020100,
	0x8040200008040201, 0x0080400010080402, 0x0000800020100804,
	0x0000000040201008,
	0x0402000000000000, 0x0804000100000000, 0x1008000201000000,
	0x2010000402010000,
	0x4020000804020100, 0x8040001008040201, 0x0080002010080402,
	0x0000004020100804,
	0x0200000000000000, 0x0400010000000000, 0x0800020100000000,
	0x1000040201000000,
	0x2000080402010000, 0x4000100804020100, 0x8000201008040201,
	0x0000402010080402,
	0x0000000000000000, 0x0001000000000000, 0x0002010000000000,
	0x0004020100000000,
	0x0008040201000000, 0x0010080402010000, 0x0020100804020100,
	0x0040201008040201
};

/* indexed using enum square
 * Represents the top-left to bottom-right diagonals that a bishop can move to, when
 * on a specific square
 */
static const uint64_t negative_diagonal_masks[] = {
	0x0000000000000000, 0x0000000000000100, 0x0000000000010200,
	0x0000000001020400,
	0x0000000102040800, 0x0000010204081000, 0x0001020408102000,
	0x0102040810204000,
	0x0000000000000002, 0x0000000000010004, 0x0000000001020008,
	0x0000000102040010,
	0x0000010204080020, 0x0001020408100040, 0x0102040810200080,
	0x0204081020400000,
	0x0000000000000204, 0x0000000001000408, 0x0000000102000810,
	0x0000010204001020,
	0x0001020408002040, 0x0102040810004080, 0x0204081020008000,
	0x0408102040000000,
	0x0000000000020408, 0x0000000100040810, 0x0000010200081020,
	0x0001020400102040,
	0x0102040800204080, 0x0204081000408000, 0x0408102000800000,
	0x0810204000000000,
	0x0000000002040810, 0x0000010004081020, 0x0001020008102040,
	0x0102040010204080,
	0x0204080020408000, 0x0408100040800000, 0x0810200080000000,
	0x1020400000000000,
	0x0000000204081020, 0x0001000408102040, 0x0102000810204080,
	0x0204001020408000,
	0x0408002040800000, 0x0810004080000000, 0x1020008000000000,
	0x2040000000000000,
	0x0000020408102040, 0x0100040810204080, 0x0200081020408000,
	0x0400102040800000,
	0x0800204080000000, 0x1000408000000000, 0x2000800000000000,
	0x4000000000000000,
	0x0002040810204080, 0x0004081020408000, 0x0008102040800000,
	0x0010204080000000,
	0x0020408000000000, 0x0040800000000000, 0x0080000000000000,
	0x0000000000000000
};

#define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
#define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
#define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )

static const unsigned char BitReverseTable256[256] =
{
    R6(0), R6(2), R6(1), R6(3)
};



// -------------------------------
// define lookup array for MVV LVA
//
// indexed by "enum piece"
static const uint16_t victim_score[NUM_PIECES] = {
	100,	// W_PAWN
	400,	// W_ROOK
	200,	// W_KNIGHT
	300,	// W_BISHOP
	500,	// W_QUEEN
	600,	// W_KING
	100,	// B_PAWN
	400,	// B_ROOK
	200,	// B_KNIGHT
	300,	// B_BISHOP
	500,	// B_QUEEN
	600		// B_KING
};

//                      	[victim]    [attacker]
static uint32_t mvv_lva_score[NUM_PIECES][NUM_PIECES];

// pawn capture score is symmetrical (ie, same regardless of 
// white or black attacker/victim. Save the score since we use it a 
// lot (saves array lookup at runtime)
static uint32_t pawn_v_pawn_score = 0;
////////////////////////////////////////////////



void init_mvv_lva_scores(void){
	for(uint8_t attacker = W_PAWN; attacker <= B_KING; attacker++) {
		for(uint8_t victim = W_PAWN; victim <= B_KING; victim++) {
			mvv_lva_score[victim][attacker] = (uint32_t)(victim_score[victim]
								+ 6 - ( victim_score[attacker] / 100));
		}
	}
	pawn_v_pawn_score = mvv_lva_score[W_PAWN][B_PAWN];
}






/* man function for taking a board and returning a populated
 * move list for all pieces
 *
 * name: generate_all_moves
 * @param
 * @return
 *
 */
void generate_all_moves(struct board *brd, struct move_list *mvl)
{
	//ASSERT_BOARD_OK(brd);

	if (brd->side_to_move == WHITE) {
		generate_white_pawn_moves(brd, mvl, false);
		generate_knight_piece_moves(brd, mvl, W_KNIGHT, BLACK, false);
		generate_king_moves(brd, mvl, W_KING, WHITE, BLACK, false);
		generate_sliding_horizontal_vertical_moves(brd, mvl, W_ROOK, false);
		generate_sliding_diagonal_moves(brd, mvl, W_BISHOP, false);
		generate_queen_moves(brd, mvl, W_QUEEN, false);
	} else {
		generate_black_pawn_moves(brd, mvl, false);
		generate_knight_piece_moves(brd, mvl, B_KNIGHT, WHITE, false);
		generate_king_moves(brd, mvl, B_KING, BLACK, WHITE, false);
		generate_sliding_horizontal_vertical_moves(brd, mvl, B_ROOK, false);
		generate_sliding_diagonal_moves(brd, mvl, B_BISHOP, false);
		generate_queen_moves(brd, mvl, B_QUEEN, false);
	}

}


/* man function for taking a board and returning a populated
 * move list of all captures
 *
 * name: generate_all_capture_moves
 * @param
 * @return
 *
 */
void generate_all_capture_moves(struct board *brd, struct move_list *mvl)
{
	//ASSERT_BOARD_OK(brd);

	if (brd->side_to_move == WHITE) {
		generate_white_pawn_moves(brd, mvl, true);
		generate_knight_piece_moves(brd, mvl, W_KNIGHT, BLACK, true);
		generate_king_moves(brd, mvl, W_KING, WHITE, BLACK, true);
		generate_sliding_horizontal_vertical_moves(brd, mvl, W_ROOK, true);
		generate_sliding_diagonal_moves(brd, mvl, W_BISHOP, true);
		generate_queen_moves(brd, mvl, W_QUEEN, true);
	} else {
		generate_black_pawn_moves(brd, mvl, true);
		generate_knight_piece_moves(brd, mvl, B_KNIGHT, WHITE, true);
		generate_king_moves(brd, mvl, B_KING, BLACK, WHITE, true);
		generate_sliding_horizontal_vertical_moves(brd, mvl, B_ROOK, true);
		generate_sliding_diagonal_moves(brd, mvl, B_BISHOP, true);
		generate_queen_moves(brd, mvl, B_QUEEN, true);
	}

}





void validate_move_list(struct move_list *mvl)
{
	assert(mvl->move_count < MAX_POSITION_MOVES);

	for (int i = 0; i < mvl->move_count; i++) {
		struct move m = mvl->moves[i];

		enum square from = FROMSQ(m.move_bitmap);
		enum square to = TOSQ(m.move_bitmap);
		enum piece capt = CAPTURED_PCE(m.move_bitmap);
		enum piece promote = PROMOTED_PCE(m.move_bitmap);

		assert(is_valid_piece(capt));
		assert(is_valid_piece(promote));

		assert(from >= a1 && from <= h8);
		assert(to >= a1 && to <= h8);

		assert(m.score == 0);
	}
}

// function to map move attributes to a bitmapped field
// see typdef for mv_bitmap for a description
inline mv_bitmap MOVE(enum square from, enum square to, enum piece capture,
		      enum piece promote, uint32_t flags)
{
	return (  (from 	<< MV_MASK_OFF_FROM_SQ) 
			| (to 		<< MV_MASK_OFF_TO_SQ) 	 
			| (capture 	<< MV_MASK_OFF_CAPTURED_PCE)  
			| (promote 	<< MV_MASK_OFF_PROMOTED_PCE) 
			| flags
	  );
}

/*
 * Returns the piece type on the given square
 *
 * name: 	get_piece_at_square
 * @param	the board container and the square to test
 * @return	the piece or NO_PIECE
 *
 */

inline enum piece get_piece_at_square(const struct board *the_board,
				      enum square sq)
{
	return the_board->pieces[sq];
}

static inline struct move_list *get_empty_move_list()
{
	struct move_list *list = malloc(sizeof(struct move_list));
	memset(list, 0, sizeof(struct move_list));
	return list;
}

static inline void
generate_queen_moves(struct board *brd, struct move_list *mvl,
		     enum piece pce, const bool only_capture_moves)
{
	generate_sliding_horizontal_vertical_moves(brd, mvl, pce, only_capture_moves);
	generate_sliding_diagonal_moves(brd, mvl, pce, only_capture_moves);

}

static inline bool is_move_in_list(struct move_list *mvl, mv_bitmap mv)
{
	for (int i = 0; i < mvl->move_count; i++) {
		if (mvl->moves[i].move_bitmap == mv) {
			return true;
		}
	}
	return false;
}


static inline void
add_capture_move(mv_bitmap move_bitmap, struct move_list *mvlist, uint32_t score)
{
	if (IS_EN_PASS_MOVE(move_bitmap) == false){
		assert(CAPTURED_PCE(move_bitmap) != NO_PIECE);
	}
	
	mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
	mvlist->moves[mvlist->move_count].score = score + SCORE_ADJ_CAPTURE;
	mvlist->move_count++;
}


static inline void
add_quiet_move(struct board *brd, mv_bitmap mv, struct move_list *mvlist, 
									enum piece pce)
{
	assert(CAPTURED_PCE(mv) == NO_PIECE);
	assert(pce != NO_PIECE);
	assert(brd != NULL);

	mvlist->moves[mvlist->move_count].score = 0;
	mvlist->moves[mvlist->move_count].move_bitmap = mv;
	mvlist->move_count++;
}





static inline void add_pawn_capture_move(enum colour col, enum square from,
					 enum square to, enum piece capture,
					 struct move_list *mvl)
{
	assert(capture != NO_PIECE);
	
	mv_bitmap mv = 0;
	
	if (col == WHITE) {
		uint32_t score = mvv_lva_score[capture][W_PAWN];
		
		if (GET_RANK(from) == RANK_7) {
			// pawn can promote to 4 pieces
			mv = MOVE(from, to, capture, W_QUEEN, MFLAG_NONE);
			add_capture_move(mv, mvl, score);
			
			mv = MOVE(from, to, capture, W_ROOK, MFLAG_NONE);			
			add_capture_move(mv, mvl, score);
			
			mv = MOVE(from, to, capture, W_BISHOP, MFLAG_NONE);
			add_capture_move(mv, mvl, score);
			
			mv = MOVE(from, to, capture, W_KNIGHT, MFLAG_NONE);
			add_capture_move(mv, mvl, score);
		} else {
			mv = MOVE(from, to, capture, NO_PIECE, MFLAG_NONE);
			add_capture_move(mv, mvl, score);
		}
	} else if (col == BLACK){
		uint32_t score = mvv_lva_score[capture][B_PAWN];
		
		if (GET_RANK(from) == RANK_2) {
			// pawn can promote to 4 pieces
			mv = MOVE(from, to, capture, B_QUEEN, MFLAG_NONE);
			add_capture_move(mv, mvl, score);
			
			mv = MOVE(from, to, capture, B_ROOK, MFLAG_NONE);
			add_capture_move(mv, mvl, score);
						
			mv = MOVE(from, to, capture, B_BISHOP, MFLAG_NONE);
			add_capture_move(mv, mvl, score);

			mv = MOVE(from, to, capture, B_KNIGHT, MFLAG_NONE);
			add_capture_move(mv, mvl, score);
		} else {
			mv = MOVE(from, to, capture, NO_PIECE, MFLAG_NONE);
			add_capture_move(mv, mvl, score);
		}
	} else {
		assert(false);
	}
}

static inline void add_pawn_move(struct board *brd, enum colour col, 
					enum square from, enum square to, struct move_list *mvl)
{
	mv_bitmap mv = 0;
	if (col == WHITE) {
		if (GET_RANK(from) == RANK_7) {
			// pawn can promote to 4 pieces
			mv = MOVE(from, to, NO_PIECE, W_QUEEN, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, W_PAWN);
			
			mv = MOVE(from, to, NO_PIECE, W_ROOK, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, W_PAWN);
			
			mv = MOVE(from, to, NO_PIECE, W_BISHOP, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, W_PAWN);
			
			mv = MOVE(from, to, NO_PIECE, W_KNIGHT, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, W_PAWN);
		} else {
			mv = MOVE(from, to, NO_PIECE, NO_PIECE, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, W_PAWN);
		}
	} else if (col == BLACK){
		if (GET_RANK(from) == RANK_2) {
			// pawn can promote to 4 pieces
			mv = MOVE(from, to, NO_PIECE, B_QUEEN, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, B_PAWN);
						
			mv = MOVE(from, to, NO_PIECE, B_ROOK, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, B_PAWN);
			
			mv = MOVE(from, to, NO_PIECE, B_BISHOP, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, B_PAWN);
			
			mv = MOVE(from, to, NO_PIECE, B_KNIGHT, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, B_PAWN);
		} else {
			mv = MOVE(from, to, NO_PIECE, NO_PIECE, MFLAG_NONE);
			add_quiet_move(brd, mv, mvl, B_PAWN);
		}
	} else {
		assert(false);
	}
}

/*
 * Generates moves for knight pieces of a given colour
 *
 * name: generate_knight_piece_moves
 * @param
 * @return
 *
 */
static inline void generate_knight_piece_moves(struct board *brd,
					       struct move_list *mvl,
					       enum piece knight,
					       enum colour opposite_col,
					       const bool only_capture_moves)
{
	// get the bitboard representing all of the piece types
	// on the board
	uint64_t bbKnight = brd->bitboards[knight];

	// iterate over all knights of this colour on the board
	while (bbKnight != 0) {

		enum square knight_sq = pop_1st_bit(&bbKnight);

		// get occupancy mask for this piece and square
		uint64_t mask = GET_KNIGHT_OCC_MASK(knight_sq);


		// AND'ing with opposite colour pieces, will give all
		// pieces that can be captured
		uint64_t opp_pieces = 0;
		if (opposite_col == WHITE)
			opp_pieces = brd->colour_bb[WHITE];
		else
			opp_pieces = brd->colour_bb[BLACK];

		uint64_t capture_squares = mask & opp_pieces;
		while (capture_squares != 0) {
			// loop creating capture moves
			enum square cap_sq = pop_1st_bit(&capture_squares);
			enum piece p = get_piece_at_square(brd, cap_sq);

			//assert(p != NO_PIECE);
			
			mv_bitmap mv = MOVE(knight_sq, cap_sq, p, NO_PIECE, MFLAG_NONE);

			uint32_t score = mvv_lva_score[p][knight];
			add_capture_move(mv, mvl, score);
		}

		if (only_capture_moves == false){
			// find all quiet moves
			uint64_t all_pieces = brd->board;
			uint64_t empty_squares = ~all_pieces & mask;
			while (empty_squares != 0) {
				// loop creating quiet moves
				enum square empty_sq = pop_1st_bit(&empty_squares);

				mv_bitmap mv = MOVE(knight_sq, empty_sq, NO_PIECE, NO_PIECE, MFLAG_NONE);
				add_quiet_move(brd, mv, mvl, knight);
			}
		}
	}
}

/*
 * Generates moves for king of a given colour
 *
 * name: generate_king_moves
 * @param
 * @return
 *
 */
static inline void generate_king_moves(struct board *brd,
				       struct move_list *mvl, enum piece pce,
				       enum colour col, enum colour opposite_col,
				       const bool only_capture_moves)
{
	// get the bitboard representing the king
	uint64_t bbKing = brd->bitboards[pce];

	enum square king_sq = pop_1st_bit(&bbKing);

	// get occupancy mask for this piece and square
	uint64_t mask = GET_KING_OCC_MASK(king_sq);

	// AND'ing with opposite colour pieces, will give all
	// pieces that can be captured
	uint64_t opp_pieces = brd->colour_bb[opposite_col];

	uint64_t capture_squares = mask & opp_pieces;
	while (capture_squares != 0) {
		// loop creating capture moves
		enum square cap_sq = pop_1st_bit(&capture_squares);
		enum piece p = get_piece_at_square(brd, cap_sq);
		assert(p != NO_PIECE);

		mv_bitmap mv = MOVE(king_sq, cap_sq, p, NO_PIECE, MFLAG_NONE);
		uint32_t score = mvv_lva_score[p][pce];
		add_capture_move(mv, mvl, score);
	}

	if (only_capture_moves == false){
		// find all quiet moves
		uint64_t all_pieces = brd->board;
		uint64_t empty_squares = ~all_pieces & mask;
		while (empty_squares != 0) {
			// loop creating quiet moves
			enum square empty_sq = pop_1st_bit(&empty_squares);

			mv_bitmap mv = MOVE(king_sq, empty_sq, NO_PIECE, NO_PIECE, MFLAG_NONE);

			add_quiet_move(brd, mv, mvl, pce);
		}

		// check for castling moves
		if (brd->castle_perm != 0) {
			if (col == WHITE) {
				generate_white_castle_moves(brd, mvl);
			} else {
				generate_black_castle_moves(brd, mvl);
			}
		}
	}
}

static inline void generate_white_castle_moves(struct board *brd,
					       struct move_list *mvl)
{
	// check for castling
	if (brd->castle_perm & WKCA) {
		bool f1_occupied = IS_SQUARE_OCCUPIED(brd->board, f1);
		bool g1_occupied = IS_SQUARE_OCCUPIED(brd->board, g1);

		if (!f1_occupied && !g1_occupied) {
			if (!is_sq_attacked(brd, e1, BLACK)
			    && !is_sq_attacked(brd, f1, BLACK)) {

				mv_bitmap mv = MOVE(e1, g1, NO_PIECE,
											NO_PIECE, MFLAG_CASTLE);
				add_quiet_move(brd, mv, mvl, W_KING);
			}
		}
	}

	if (brd->castle_perm & WQCA) {
		bool d1_occupied = IS_SQUARE_OCCUPIED(brd->board, d1);
		bool c1_occupied = IS_SQUARE_OCCUPIED(brd->board, c1);
		bool b1_occupied = IS_SQUARE_OCCUPIED(brd->board, b1);

		if (!d1_occupied && !c1_occupied && !b1_occupied) {
			if (!is_sq_attacked(brd, e1, BLACK)
			    && !is_sq_attacked(brd, d1, BLACK)) {

				mv_bitmap mv = MOVE(e1, c1, NO_PIECE,
										NO_PIECE, MFLAG_CASTLE);
				add_quiet_move(brd, mv, mvl, W_KING);
			}
		}
	}
}

static inline void generate_black_castle_moves(struct board *brd,
					       struct move_list *mvl)
{
	if (brd->castle_perm & BKCA) {
		bool f8_occupied = IS_SQUARE_OCCUPIED(brd->board, f8);
		bool g8_occupied = IS_SQUARE_OCCUPIED(brd->board, g8);

		if (!f8_occupied && !g8_occupied) {
			if (!is_sq_attacked(brd, e8, WHITE)
			    && !is_sq_attacked(brd, f8, WHITE)) {

				mv_bitmap mv = MOVE(e8, g8, NO_PIECE,
											NO_PIECE, MFLAG_CASTLE);
				add_quiet_move(brd, mv, mvl, B_KING);
			}
		}
	}

	if (brd->castle_perm & BQCA) {
		bool d8_occupied = IS_SQUARE_OCCUPIED(brd->board, d8);
		bool c8_occupied = IS_SQUARE_OCCUPIED(brd->board, c8);
		bool b8_occupied = IS_SQUARE_OCCUPIED(brd->board, b8);

		if (!d8_occupied && !c8_occupied && !b8_occupied) {
			if (!is_sq_attacked(brd, e8, WHITE)
			    && !is_sq_attacked(brd, d8, WHITE)) {

				mv_bitmap mv = MOVE(e8, c8, NO_PIECE,
											NO_PIECE, MFLAG_CASTLE);
				add_quiet_move(brd, mv, mvl, B_KING);
			}
		}
	}
}

static inline void
generate_white_pawn_moves(struct board *brd, struct move_list *mvl,
					const bool only_capture_moves)
{
	// get the bitboard representing all WHITE pawns
	// on the board
	uint64_t bbPawn = brd->bitboards[W_PAWN];

	uint64_t bb_black_pieces = brd->colour_bb[BLACK];

	while (bbPawn != 0) {

		enum square pawn_sq = pop_1st_bit(&bbPawn);

		uint8_t pawn_file = GET_FILE(pawn_sq);
		uint8_t pawn_rank = GET_RANK(pawn_sq);
		enum square next_sq_1 = pawn_sq + 8;

		//assert(next_sq_1 <= h8);
		if (only_capture_moves == false){
			if (IS_SQUARE_OCCUPIED(brd->board, next_sq_1) == false) {
				add_pawn_move(brd, WHITE, pawn_sq, next_sq_1, mvl);

				if (pawn_rank == RANK_2) {
					enum square next_sq_2 = pawn_sq + 16;

					//assert(next_sq_2 <= h8);
					bool sq_2_occupied =
						IS_SQUARE_OCCUPIED(brd->board, next_sq_2);
					if (sq_2_occupied == false) {
						mv_bitmap mv = MOVE(pawn_sq, next_sq_2, NO_PIECE,
									NO_PIECE, MFLAG_PAWN_START);

						add_quiet_move(brd, mv, mvl, W_PAWN);
					}
				}
			}
		}
		// check for capture left
		if (pawn_file > FILE_A) {
			enum square cap_sq = pawn_sq + 7;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if (CHECK_BIT(bb_black_pieces, cap_sq) != 0){
				add_pawn_capture_move(WHITE, pawn_sq, cap_sq, pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				mv_bitmap mv = MOVE(pawn_sq, cap_sq, pce, NO_PIECE,
													MFLAG_EN_PASSANT);
				add_capture_move(mv, mvl, pawn_v_pawn_score);
			}
		}
		// check for capture right
		if (pawn_file < FILE_H) {
			enum square cap_sq = pawn_sq + 9;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if (CHECK_BIT(bb_black_pieces, cap_sq) != 0){
				add_pawn_capture_move(WHITE, pawn_sq, cap_sq,
						      pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				mv_bitmap mv = MOVE(pawn_sq, cap_sq, pce,
										NO_PIECE, MFLAG_EN_PASSANT);
				add_capture_move(mv, mvl, pawn_v_pawn_score);
			}
		}
	}

}

static inline void
generate_black_pawn_moves(struct board *brd, struct move_list *mvl, 
						const bool only_capture_moves)
{
	// get the bitboard representing all BLACK pawns
	// on the board
	uint64_t bbPawn = brd->bitboards[B_PAWN];

	uint64_t bb_white_pieces = brd->colour_bb[WHITE];

	while (bbPawn != 0) {

		enum square pawn_sq = pop_1st_bit(&bbPawn);

		//int pawn_rank = GET_RANK(pawn_sq);
		int pawn_file = GET_FILE(pawn_sq);
		int pawn_rank = GET_RANK(pawn_sq);
		enum square next_sq_1 = pawn_sq - 8;
		if (only_capture_moves == false){
			if (IS_SQUARE_OCCUPIED(brd->board, next_sq_1) == false) {
				add_pawn_move(brd, BLACK, pawn_sq, next_sq_1, mvl);

				if (pawn_rank == RANK_7) {
					enum square next_sq_2 = pawn_sq - 16;	// can skip down 2 ranks

					bool sq_2_occupied =
						IS_SQUARE_OCCUPIED(brd->board, next_sq_2);
					if (sq_2_occupied == false) {
						mv_bitmap mv = MOVE(pawn_sq, next_sq_2,
									NO_PIECE, NO_PIECE,
									MFLAG_PAWN_START);
						add_quiet_move(brd, mv, mvl, B_PAWN);
					}
				}
			}
		}
		
		// check for capture left
		if (pawn_file > FILE_A) {
			enum square cap_sq = pawn_sq - 9;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if (CHECK_BIT(bb_white_pieces, cap_sq) != 0){
				add_pawn_capture_move(BLACK, pawn_sq, cap_sq,
						      pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				mv_bitmap mv = MOVE(pawn_sq, cap_sq, pce,
						  NO_PIECE, MFLAG_EN_PASSANT);
				add_capture_move(mv, mvl, pawn_v_pawn_score);
			}
		}
		// check for capture right
		if (pawn_file < FILE_H) {
			enum square cap_sq = pawn_sq - 7;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if (CHECK_BIT(bb_white_pieces, cap_sq) != 0){
				add_pawn_capture_move(BLACK, pawn_sq, cap_sq, pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				mv_bitmap mv = MOVE(pawn_sq, cap_sq, pce,
											NO_PIECE, MFLAG_EN_PASSANT);
				add_capture_move(mv, mvl, pawn_v_pawn_score);
			}
		}
	}
}

/* Generates sliding horizontal and vertical moves (a la Rook)
 *
 * Based on the code on page:
 * 		http://chessprogramming.wikispaces.com/Efficient+Generation+of+Sliding+Piece+Attacks
 *
 * plus the video
 * 		https://www.youtube.com/watch?v=bCH4YK6oq8M
 *
 * name: generate_sliding_horizontal_vertical_moves
 * @param
 * @return
 *
 */
static inline void generate_sliding_horizontal_vertical_moves (struct board *brd,
					   struct move_list *mvl,
					   enum piece pce, const bool only_capture_moves)
{

	uint64_t bb = brd->bitboards[pce];

	while (bb != 0) {

		enum square pce_sq = pop_1st_bit(&bb);

		uint64_t hmask = get_horizontal_mask(pce_sq);
		uint64_t vmask = get_vertical_mask(pce_sq);

		// create slider bb for this square
		uint64_t bb_slider = GET_PIECE_MASK(pce_sq);

		// all occupied squares (both colours)
		uint64_t occupied = brd->board;

		uint64_t horiz1 = occupied - (2 * bb_slider);
		uint64_t horiz2 =
		    reverse_bits(reverse_bits(occupied) -
				 2 * reverse_bits(bb_slider));
		uint64_t horizontal = horiz1 ^ horiz2;

		uint64_t vert1 = (occupied & vmask) - (2 * bb_slider);
		uint64_t vert2 =
		    reverse_bits(reverse_bits(occupied & vmask) -
				 2 * reverse_bits(bb_slider));
		uint64_t vertical = vert1 ^ vert2;

		uint64_t all_moves = (horizontal & hmask) | (vertical & vmask);

		enum colour col = (IS_BLACK(pce)) ? BLACK : WHITE;

		// get all same colour as piece being considered
		uint64_t col_occupied = 0;
		if (col == WHITE)
			col_occupied = brd->colour_bb[WHITE];
		else
			col_occupied = brd->colour_bb[BLACK];

		uint64_t excl_same_col = all_moves & ~col_occupied;
		//printf("Mask Same Colour\n");
		//print_mask_as_board(&excl_same_col, pce, pce_sq);

		while (excl_same_col != 0) {

			enum square sq = pop_1st_bit(&excl_same_col);

			enum piece mv_pce = get_piece_at_square(brd, sq);

			if (mv_pce != NO_PIECE) {
				mv_bitmap mv = MOVE(pce_sq, sq, mv_pce, NO_PIECE, MFLAG_NONE);
				uint32_t score = mvv_lva_score[mv_pce][pce];
				add_capture_move(mv, mvl, score);
			} else {
				if (only_capture_moves == false){
					mv_bitmap mv = MOVE(pce_sq, sq, NO_PIECE, NO_PIECE, MFLAG_NONE);
					add_quiet_move(brd, mv, mvl, pce);
				}
			}
		}
	}
}

/* Generates sliding diagonal moves (a la Bishop)
 *
 * Based on the code on page:
 * 		http://chessprogramming.wikispaces.com/Efficient+Generation+of+Sliding+Piece+Attacks
 *
 * plus the video
 * 		https://www.youtube.com/watch?v=bCH4YK6oq8M
 *
 * name: generate_sliding_diagonal_moves
 * @param
 * @return
 *
 */

static inline void generate_sliding_diagonal_moves(struct board *brd,
				struct move_list *mvl, enum piece pce, const bool only_capture_moves)
{

	uint64_t bb = brd->bitboards[pce];

	while (bb != 0) {

		enum square pce_sq = pop_1st_bit(&bb);

		uint64_t posmask = get_positive_diagonal_mask(pce_sq);
		uint64_t negmask = get_negative_diagonal_mask(pce_sq);

		// create slider bb for this square
		uint64_t bb_slider = GET_PIECE_MASK(pce_sq);

		// all occupied squares (both colours)
		uint64_t occupied = brd->board;

		uint64_t diag1 = (occupied & posmask) - (2 * bb_slider);
		uint64_t diag2 =
		    reverse_bits(reverse_bits(occupied & posmask) -
				 (2 * reverse_bits(bb_slider)));
		uint64_t diagpos = diag1 ^ diag2;

		diag1 = (occupied & negmask) - (2 * bb_slider);
		diag2 =
		    reverse_bits(reverse_bits(occupied & negmask) -
				 (2 * reverse_bits(bb_slider)));
		uint64_t diagneg = diag1 ^ diag2;

		uint64_t all_moves = (diagpos & posmask) | (diagneg & negmask);

		enum colour col = (IS_BLACK(pce)) ? BLACK : WHITE;

		// get all same colour as piece being considered
		uint64_t col_occupied = 0;
		if (col == WHITE)
			col_occupied = brd->colour_bb[WHITE];
		else
			col_occupied = brd->colour_bb[BLACK];

		uint64_t excl_same_col = all_moves & ~col_occupied;
		//printf("Mask Same Colour\n");
		//print_mask_as_board(&excl_same_col, pce, pce_sq);

		while (excl_same_col != 0) {

			enum square sq = pop_1st_bit(&excl_same_col);

			enum piece mv_pce = get_piece_at_square(brd, sq);

			if (mv_pce != NO_PIECE) {
				mv_bitmap mv = MOVE(pce_sq, sq, mv_pce, NO_PIECE, MFLAG_NONE);
				uint32_t score = mvv_lva_score[mv_pce][pce];
				add_capture_move(mv, mvl, score);
			} else {
				if (only_capture_moves == false){
					mv_bitmap mv = MOVE(pce_sq, sq, NO_PIECE, NO_PIECE, MFLAG_NONE);
					add_quiet_move(brd, mv, mvl, pce);
				}
			}
		}
	}
}


/*
 * Clears the LSB of the board, and returns the bit # that was cleared.
 * name: pop_1st_bit
 * @param	ptr to uint64_t
 * @return	index of bit cleared.
 *
 * uses gcc built-in function (see https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)
 *
 */
inline uint8_t pop_1st_bit(uint64_t * bb)
{
	uint8_t bit = (uint8_t) __builtin_ctzll(*bb);

	// clear the bit
	clear_bit(bb, bit);
	return bit;
}



/* Reverse the bits in a word
 *
 * name: reverse_bits
 * @param
 * @return
 *
 */
inline uint64_t reverse_bits(uint64_t word)
{
	uint64_t retval = 0;

	uint8_t *p_in = (uint8_t *) & word;
	uint8_t *p_out = (uint8_t *) & retval;
	// reverse the bits in each byte
	for (int i = 0; i < 8; i++) {
		*p_out = (uint8_t)BitReverseTable256[*p_in];
		p_out++;
		p_in++;
	}

	// now reverse the bytes
	return __builtin_bswap64(retval);
}



static inline uint64_t get_vertical_mask(const enum square sq)
{
	return vertical_move_mask[sq];
}

static inline uint64_t get_horizontal_mask(const enum square sq)
{
	return horizontal_move_mask[sq];
}

static inline uint64_t get_positive_diagonal_mask(const enum square sq)
{
	return positive_diagonal_masks[sq];
}

static inline uint64_t get_negative_diagonal_mask(const enum square sq)
{
	return negative_diagonal_masks[sq];
}

/*
 * Prints out the algebraic notation of a move (eg, a2a4)
 * name: print_move
 * @param
 * @return
 *
 */
char *print_move(uint32_t move_bitmap)
{

	static char move_string[6];

	int from_file = GET_FILE(FROMSQ(move_bitmap));
	int from_rank = GET_RANK(FROMSQ(move_bitmap));

	int to_file = GET_FILE(TOSQ(move_bitmap));
	int to_rank = GET_RANK(TOSQ(move_bitmap));

	enum piece promoted_pce = PROMOTED_PCE(move_bitmap);

	if (promoted_pce != NO_PIECE) {
		char pchar = 'q';
		if (IS_KNIGHT(promoted_pce)) {
			pchar = 'n';
		} else if (IS_ROOK(promoted_pce)) {
			pchar = 'r';
		} else if (IS_BISHOP(promoted_pce)) {
			pchar = 'b';
		}
		sprintf(move_string, "%c%c%c%c%c", ('a' + from_file),
			('1' + from_rank), ('a' + to_file), ('1' + to_rank),
			pchar);
	} else {
		sprintf(move_string, "%c%c%c%c", ('a' + from_file),
			('1' + from_rank), ('a' + to_file), ('1' + to_rank));
	}

	return move_string;
}

void print_move_details(uint32_t move_bitmap, uint32_t score)
{
	int from_file = GET_FILE(FROMSQ(move_bitmap));
	int from_rank = GET_RANK(FROMSQ(move_bitmap));

	int to_file = GET_FILE(TOSQ(move_bitmap));
	int to_rank = GET_RANK(TOSQ(move_bitmap));

	enum piece captured = CAPTURED_PCE(move_bitmap);
	enum piece promoted = PROMOTED_PCE(move_bitmap);

	char c_capt = get_piece_label(captured);
	char c_promoted = get_piece_label(promoted);

	printf("%c%c%c%c, captured '%c' promote '%c' score %d\n",
	       ('a' + from_file), ('1' + from_rank), ('a' + to_file),
	       ('1' + to_rank), c_capt, c_promoted, score);

}

/*
 * Prints out the move list details
 * name: print_move_list_details
 * @param
 * @return
 *
 */

void print_move_list_details(const struct move_list *list)
{
	printf("MoveList Details:\n");

	for (int i = 0; i < list->move_count; i++) {
		uint32_t move = list->moves[i].move_bitmap;
		uint32_t score = list->moves[i].score;

		print_move_details(move, score);
	}
	printf("MoveList Total %d Moves:\n\n", list->move_count);
}

/*
 * Prints out the move list
 * name: print_move_list
 * @param
 * @return
 *
 */

void print_move_list(const struct move_list *list)
{
	printf("MoveList:\n");

	for (int i = 0; i < list->move_count; i++) {
		uint32_t move = list->moves[i].move_bitmap;
		//uint32_t score = list->moves[i].score;

		printf("%s\n", print_move(move));
	}
	//printf("MoveList Total %d Moves:\n\n", list->move_count);
}

/**
 * Test wrapper functions.
 * These wrapper functions facilitate access to unit test framework while
 * allowing the functions themselves to be static to this file
 */
void
TEST_generate_white_pawn_moves(struct board *brd, struct move_list *mvl)
{
	generate_white_pawn_moves(brd, mvl, false);
}

void
TEST_generate_black_pawn_moves(struct board *brd, struct move_list *mvl)
{
	generate_black_pawn_moves(brd, mvl, false);
}

void
TEST_generate_knight_piece_moves(struct board *brd,
				 struct move_list *mvl, enum colour col)
{
	if (col == WHITE)
		generate_knight_piece_moves(brd, mvl, W_KNIGHT, BLACK, false);
	else
		generate_knight_piece_moves(brd, mvl, B_KNIGHT, WHITE, false);

}

void
TEST_generate_king_moves(struct board *brd, struct move_list *mvl,
			 enum colour col)
{
	enum piece pce;
	enum colour opposite_col;
	if (col == WHITE){
		pce = W_KING;
		opposite_col = BLACK;
	} else {

		pce = B_KING;
		opposite_col = WHITE;
	}

	generate_king_moves(brd, mvl, pce, col, opposite_col, false);
}

void
TEST_generate_castle_moves(struct board *brd, struct move_list *mvl,
			   enum colour col)
{
	if (col == WHITE)
		generate_white_castle_moves(brd, mvl);
	else
		generate_black_castle_moves(brd, mvl);
}

void
TEST_generate_sliding_horizontal_vertical_moves(struct board *brd,
						struct move_list *mvl,
						enum piece pce)
{
	generate_sliding_horizontal_vertical_moves(brd, mvl, pce, false);
}

void
TEST_generate_sliding_diagonal_moves(struct board *brd,
				     struct move_list *mvl, enum piece pce)
{
	generate_sliding_diagonal_moves(brd, mvl, pce, false);
}

void
TEST_generate_queen_moves(struct board *brd, struct move_list *mvl,
			  enum piece pce)
{
	generate_queen_moves(brd, mvl, pce, false);
}

bool TEST_is_move_in_list(struct move_list *mvl, mv_bitmap mv)
{
	return is_move_in_list(mvl, mv);
}

uint32_t TEST_get_move_score(enum piece victim, enum piece attacker)
{
	return mvv_lva_score[victim][attacker];
}

void TEST_add_quiet_move(struct board *brd, 
		mv_bitmap move_bitmap, struct move_list *mvlist, enum piece pce)
{
	add_quiet_move(brd, move_bitmap, mvlist, pce);
}




struct move_list *TEST_get_empty_move_list(void)
{
	return get_empty_move_list();
}
