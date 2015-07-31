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
#include <assert.h>
#include "types.h"
#include "board.h"
#include "board_utils.h"
#include "pieces.h"
#include "occupancy_mask.h"
#include "move.h"
#include "utils.h"


static void add_pawn_capture_move(const struct board *brd, enum colour col,
					enum square from, enum square to,
					enum piece capture,	struct move_list *mvl);
static void add_pawn_move(const struct board *brd, enum colour col,
					enum square from, enum square to, struct move_list *mvl);
static void add_quiet_move(const struct board *brd, mv_bitmap move_bitmap, struct move_list *mvlist);
static void add_capture_move(const struct board *brd, mv_bitmap move_bitmap, struct move_list *mvlist);
static void add_en_passent_move(const struct board *brd, mv_bitmap move_bitmap, struct move_list *mvlist);
void generate_white_pawn_moves(const struct board *brd, struct move_list *mvl);
void generate_black_pawn_moves(const struct board *brd, struct move_list *mvl);
static U64 get_horizontal_mask(enum square sq);
static U64 get_vertical_mask(enum square sq);
static U64 get_positive_diagonal_mask(enum square sq);
static U64 get_negative_diagonal_mask(enum square sq);
static void generate_queen_moves(const struct board *brd, struct move_list *mvl, enum piece pce);



/* indexed using enum square
 * Represents the horizontal squares that a rook can move to, when
 * on a specific square
 */
static const U64 horizontal_move_mask [] = {
			0x00000000000000ff, 0x00000000000000ff, 0x00000000000000ff, 0x00000000000000ff,
			0x00000000000000ff, 0x00000000000000ff, 0x00000000000000ff, 0x00000000000000ff,
			0x000000000000ff00, 0x000000000000ff00, 0x000000000000ff00, 0x000000000000ff00,
			0x000000000000ff00, 0x000000000000ff00, 0x000000000000ff00, 0x000000000000ff00,
			0x0000000000ff0000, 0x0000000000ff0000, 0x0000000000ff0000, 0x0000000000ff0000,
			0x0000000000ff0000, 0x0000000000ff0000, 0x0000000000ff0000, 0x0000000000ff0000,
			0x00000000ff000000, 0x00000000ff000000, 0x00000000ff000000, 0x00000000ff000000,
			0x00000000ff000000, 0x00000000ff000000, 0x00000000ff000000, 0x00000000ff000000,
			0x000000ff00000000, 0x000000ff00000000, 0x000000ff00000000, 0x000000ff00000000,
			0x000000ff00000000, 0x000000ff00000000, 0x000000ff00000000, 0x000000ff00000000,
			0x0000ff0000000000, 0x0000ff0000000000, 0x0000ff0000000000, 0x0000ff0000000000,
			0x0000ff0000000000, 0x0000ff0000000000, 0x0000ff0000000000, 0x0000ff0000000000,
			0x00ff000000000000, 0x00ff000000000000, 0x00ff000000000000, 0x00ff000000000000,
			0x00ff000000000000, 0x00ff000000000000, 0x00ff000000000000, 0x00ff000000000000,
			0xff00000000000000, 0xff00000000000000, 0xff00000000000000, 0xff00000000000000,
			0xff00000000000000, 0xff00000000000000, 0xff00000000000000, 0xff00000000000000
};


/* indexed using enum square
 * Represents the vertical squares that a rook can move to, when
 * on a specific square
 */
static const U64 vertical_move_mask [] = {
			0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
			0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
			0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
			0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
			0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
			0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
			0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
			0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
			0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
			0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
			0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
			0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
			0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
			0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
			0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
			0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080
};


/* indexed using enum square
 * Represents the bottom-left to top-right diagonals that a bishop can move to, when
 * on a specific square
 */
static const U64 positive_diagonal_masks [] = {
			0x8040201008040200,0x0080402010080400,0x0000804020100800,0x0000008040201000,
			0x0000000080402000,0x0000000000804000,0x0000000000008000,0x0000000000000000,
			0x4020100804020000,0x8040201008040001,0x0080402010080002,0x0000804020100004,
			0x0000008040200008,0x0000000080400010,0x0000000000800020,0x0000000000000040,
			0x2010080402000000,0x4020100804000100,0x8040201008000201,0x0080402010000402,
			0x0000804020000804,0x0000008040001008,0x0000000080002010,0x0000000000004020,
			0x1008040200000000,0x2010080400010000,0x4020100800020100,0x8040201000040201,
			0x0080402000080402,0x0000804000100804,0x0000008000201008,0x0000000000402010,
			0x0804020000000000,0x1008040001000000,0x2010080002010000,0x4020100004020100,
			0x8040200008040201,0x0080400010080402,0x0000800020100804,0x0000000040201008,
			0x0402000000000000,0x0804000100000000,0x1008000201000000,0x2010000402010000,
			0x4020000804020100,0x8040001008040201,0x0080002010080402,0x0000004020100804,
			0x0200000000000000,0x0400010000000000,0x0800020100000000,0x1000040201000000,
			0x2000080402010000,0x4000100804020100,0x8000201008040201,0x0000402010080402,
			0x0000000000000000,0x0001000000000000,0x0002010000000000,0x0004020100000000,
			0x0008040201000000,0x0010080402010000,0x0020100804020100,0x0040201008040201
};

/* indexed using enum square
 * Represents the top-left to bottom-right diagonals that a bishop can move to, when
 * on a specific square
 */
static const U64 negative_diagonal_masks [] = {
			0x0000000000000000, 0x0000000000000100, 0x0000000000010200, 0x0000000001020400,
			0x0000000102040800, 0x0000010204081000, 0x0001020408102000, 0x0102040810204000,
			0x0000000000000002, 0x0000000000010004, 0x0000000001020008, 0x0000000102040010,
			0x0000010204080020, 0x0001020408100040, 0x0102040810200080, 0x0204081020400000,
			0x0000000000000204, 0x0000000001000408, 0x0000000102000810, 0x0000010204001020,
			0x0001020408002040, 0x0102040810004080, 0x0204081020008000, 0x0408102040000000,
			0x0000000000020408, 0x0000000100040810, 0x0000010200081020, 0x0001020400102040,
			0x0102040800204080, 0x0204081000408000, 0x0408102000800000, 0x0810204000000000,
			0x0000000002040810, 0x0000010004081020, 0x0001020008102040, 0x0102040010204080,
			0x0204080020408000, 0x0408100040800000, 0x0810200080000000, 0x1020400000000000,
			0x0000000204081020, 0x0001000408102040, 0x0102000810204080, 0x0204001020408000,
			0x0408002040800000, 0x0810004080000000, 0x1020008000000000, 0x2040000000000000,
			0x0000020408102040, 0x0100040810204080, 0x0200081020408000, 0x0400102040800000,
			0x0800204080000000, 0x1000408000000000, 0x2000800000000000, 0x4000000000000000,
			0x0002040810204080, 0x0004081020408000, 0x0008102040800000, 0x0010204080000000,
			0x0020408000000000, 0x0040800000000000, 0x0080000000000000, 0x0000000000000000
};



/* man function for taking a board and returning a populated
 * move list for all pieces
 *
 * name: generate_all_moves
 * @param
 * @return
 *
 */
void generate_all_moves(const struct board *brd, struct move_list *mvl)
{
    if (brd->side_to_move == WHITE) {
		generate_white_pawn_moves(brd, mvl);
		generate_knight_piece_moves(brd, mvl, WHITE);
		generate_king_moves(brd, mvl, WHITE);
		generate_sliding_horizontal_vertical_moves(brd, mvl, W_ROOK);
		generate_sliding_diagonal_moves(brd, mvl, W_BISHOP);
		generate_queen_moves(brd, mvl, W_QUEEN);
    } else {
		generate_black_pawn_moves(brd, mvl);
		generate_knight_piece_moves(brd, mvl, BLACK);
		generate_king_moves(brd, mvl, BLACK);
		generate_sliding_horizontal_vertical_moves(brd, mvl, B_ROOK);
		generate_sliding_diagonal_moves(brd, mvl, B_BISHOP);
		generate_queen_moves(brd, mvl, B_QUEEN);
	}

}


struct move_list * get_empty_move_list(){
    struct move_list *list = malloc(sizeof(struct move_list));
	memset(list, 0, sizeof(struct move_list));
	return list;
}


static void generate_queen_moves(const struct board *brd, struct move_list *mvl, enum piece pce){
	generate_sliding_horizontal_vertical_moves(brd, mvl, pce);
	generate_sliding_diagonal_moves(brd, mvl, pce);

}



bool is_move_in_list(struct move_list *mvl, mv_bitmap mv){

	for(int i = 0; i < mvl->move_count; i++){
		if (mvl->moves[i].move_bitmap == mv){
			return true;
		}
	}
	return false;
}





static inline void add_quiet_move(const struct board *brd, mv_bitmap move_bitmap,
		    struct move_list *mvlist)
{
    mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
    mvlist->moves[mvlist->move_count].score = 0;
    mvlist->move_count++;
}

static inline void add_capture_move(const struct board *brd, mv_bitmap move_bitmap,
		      struct move_list *mvlist)
{
	mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
    mvlist->moves[mvlist->move_count].score = 0;
    mvlist->move_count++;
}

static inline void add_en_passent_move(const struct board *brd, mv_bitmap move_bitmap,
			 struct move_list *mvlist)
{
    mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
    mvlist->moves[mvlist->move_count].score = 0;
    mvlist->move_count++;
}


static inline void add_pawn_capture_move(const struct board *brd,
					enum colour col, enum square from, enum square to,
					enum piece capture,
					struct move_list *mvl)
{
	if (col == WHITE){
		if (GET_RANK(from) == RANK_7) {
			// pawn can promote to 4 pieces
			add_capture_move(brd, MOVE(from, to, capture, W_QUEEN, 0), mvl);
			add_capture_move(brd, MOVE(from, to, capture, W_ROOK, 0), mvl);
			add_capture_move(brd, MOVE(from, to, capture, W_BISHOP, 0), mvl);
			add_capture_move(brd, MOVE(from, to, capture, W_KNIGHT, 0), mvl);
		} else {
			add_capture_move(brd, MOVE(from, to, capture, NO_PIECE, 0), mvl);
		}
    } else {
	    if (GET_RANK(from) == RANK_2) {
			// pawn can promote to 4 pieces
			add_capture_move(brd, MOVE(from, to, capture, B_QUEEN, 0), mvl);
			add_capture_move(brd, MOVE(from, to, capture, B_ROOK, 0), mvl);
			add_capture_move(brd, MOVE(from, to, capture, B_BISHOP, 0), mvl);
			add_capture_move(brd, MOVE(from, to, capture, B_KNIGHT, 0), mvl);
		} else {
			add_capture_move(brd, MOVE(from, to, capture, NO_PIECE, 0), mvl);
		}
	}
}

static inline void add_pawn_move(const struct board *brd, enum colour col,
				enum square from, enum square to, struct move_list *mvl)
{
	if (col == WHITE){
		if (GET_RANK(from) == RANK_7) {
			// pawn can promote to 4 pieces
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, W_QUEEN, 0), mvl);
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, W_ROOK, 0), mvl);
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, W_BISHOP, 0), mvl);
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, W_KNIGHT, 0), mvl);
		} else {
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, NO_PIECE, 0), mvl);
		}
    } else{
		if (GET_RANK(from) == RANK_2) {
			// pawn can promote to 4 pieces
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, B_QUEEN, 0), mvl);
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, B_ROOK, 0), mvl);
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, B_BISHOP, 0), mvl);
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, B_KNIGHT, 0), mvl);
		} else {
			add_quiet_move(brd, MOVE(from, to, NO_PIECE, NO_PIECE, 0), mvl);
		}
	}
}



/*
 * Generates moves for sliding pieces of a given colour
 *
 * name: generate_sliding_piece_moves
 * @param
 * @return
 *
 */
/*
void generate_sliding_piece_moves(const struct board *brd, struct move_list *mvl, enum colour col){
	const int NUM_SLIDERS = 3;
	static enum piece white_pieces[] = {W_ROOK, W_BISHOP, W_QUEEN};
	static enum piece black_pieces[] = {B_ROOK, B_BISHOP, B_QUEEN};

	// select the colour piece array
	enum piece *pieces = (col == WHITE) ? white_pieces : black_pieces;

	// a bitboard representing a pieces of this colour
	U64 all_pieces_bb = overlay_colours(brd, col);


	for (int i = 0; i < NUM_SLIDERS; i++){
		enum piece pce = pieces[i];

		// a bb representing all pieces of this type on the board
		U64 bb = brd->bitboards[pce];
		printf("bb %d %d", (int)bb, (int)all_pieces_bb);

		// iterate over pieces of this type
		while (bb != 0) {
			enum square sq = POP(&bb);

			// get the occupancy mask for this piece
			U64 occ_mask = get_occupancy_mask(pce, sq);
			printf("occ mask %d", (int)occ_mask);


		}
	}
}

*/

/*
 * Generates moves for knight pieces of a given colour
 *
 * name: generate_knight_piece_moves
 * @param
 * @return
 *
 */
void generate_knight_piece_moves(const struct board *brd, struct move_list *mvl, enum colour col){

	enum piece pce = (col == WHITE) ? W_KNIGHT : B_KNIGHT;
	enum colour pce_col = get_colour(pce);

	// get the bitboard representing all of the piece types
    // on the board
    U64 bbKnight = brd->bitboards[pce];

	// iterate over all knights of this colour on the board
    while (bbKnight != 0) {

		enum square knight_sq = POP(&bbKnight);

		// get occupancy mask for this piece and square
		U64 mask = GET_KNIGHT_OCC_MASK(knight_sq);

		while (mask != 0){
			// iterate over all the possible destination squares
			// for this knight
			enum square knight_dest_sq = POP(&mask);

			assert(knight_dest_sq != knight_sq);

			enum piece p = get_piece_at_square(brd, knight_dest_sq);
			if (p != NO_PIECE){
				// square has a piece
				enum colour pcol = get_colour(p);
				if (pcol == pce_col){
					// dest square has piece with same colour...keep looking for a move
					continue;
				} else {
					// dest square has opposing piece
					add_capture_move(brd, MOVE(knight_sq, knight_dest_sq, p, NO_PIECE, 0), mvl);
				}
			} else {
				// empty square
				add_quiet_move(brd, MOVE(knight_sq, knight_dest_sq, NO_PIECE, NO_PIECE, 0), mvl);
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
void generate_king_moves(const struct board *brd, struct move_list *mvl, enum colour col){

	enum piece pce = (col == WHITE) ? W_KING : B_KING;
	enum colour pce_col = get_colour(pce);

	// get the bitboard representing the king
    U64 bbKing = brd->bitboards[pce];

	assert(CNT(bbKing) == 1);

	enum square king_sq = POP(&bbKing);

	// get occupancy mask for this piece and square
	U64 mask = GET_KING_OCC_MASK(king_sq);

	while (mask != 0){
		// iterate over all the possible destination squares
		// for this king
		enum square king_dest_sq = POP(&mask);

		enum piece p = get_piece_at_square(brd, king_dest_sq);
		if (p != NO_PIECE){
			// square has a piece
			enum colour pcol = get_colour(p);
			if (pcol == pce_col){
				// dest square has piece with same colour...keep looking for a move
				continue;
			} else {
				// dest square has opposing piece
				add_capture_move(brd, MOVE(king_sq, king_dest_sq, p, NO_PIECE, 0), mvl);
			}
		} else {
			// empty square
			add_quiet_move(brd, MOVE(king_sq, king_dest_sq, NO_PIECE, NO_PIECE, 0), mvl);
		}
	}
}





void generate_white_pawn_moves(const struct board *brd, struct move_list *mvl)
{
    // get the bitboard representing all WHITE pawns
    // on the board
    U64 bbPawn = brd->bitboards[W_PAWN];

    while (bbPawn != 0) {

		//printf("bbPawn:\t0x%016llx\n", bbPawn);

		enum square pawn_sq = POP(&bbPawn);

		//int pawn_rank = GET_RANK(pawn_sq);
		int pawn_file = GET_FILE(pawn_sq);
		int pawn_rank = GET_RANK(pawn_sq);
		enum square next_sq_1 = pawn_sq + 8;

		if (is_square_occupied(brd->board, next_sq_1) == false) {
			add_pawn_move(brd, WHITE, pawn_sq, next_sq_1, mvl);

			if (pawn_rank == RANK_2) {
				enum square next_sq_2 = pawn_sq + 16;
				bool sq_2_occupied = is_square_occupied(brd->board, next_sq_2);
				if (sq_2_occupied == false) {
					add_quiet_move(brd,
					   MOVE(pawn_sq, next_sq_2,
						NO_PIECE, NO_PIECE,
						MFLAG_PAWN_START), mvl);
				}
			}
		}

		// check for capture left
		if (pawn_file >= FILE_B) {
			enum square cap_sq = pawn_sq + 7;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if ((pce != NO_PIECE) && (get_colour(pce) == BLACK)) {
				add_pawn_capture_move(brd, WHITE, pawn_sq, cap_sq, pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				add_pawn_capture_move(brd, WHITE, pawn_sq, cap_sq, pce, mvl);
			}
		}

		// check for capture right
		if (pawn_file <= FILE_G) {
			enum square cap_sq = pawn_sq + 9;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if ((pce != NO_PIECE) && (get_colour(pce) == BLACK)) {
				add_pawn_capture_move(brd, WHITE, pawn_sq, cap_sq, pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				add_pawn_capture_move(brd, WHITE, pawn_sq, cap_sq, pce, mvl);
			}
		}
    }
}



void generate_black_pawn_moves(const struct board *brd, struct move_list *mvl)
{
    // get the bitboard representing all BLACK pawns
    // on the board
    U64 bbPawn = brd->bitboards[B_PAWN];

    while (bbPawn != 0) {

		//printf("bbPawn:\t0x%016llx\n", bbPawn);

		enum square pawn_sq = POP(&bbPawn);

		//int pawn_rank = GET_RANK(pawn_sq);
		int pawn_file = GET_FILE(pawn_sq);
		int pawn_rank = GET_RANK(pawn_sq);
		enum square next_sq_1 = pawn_sq - 8;

		if (is_square_occupied(brd->board, next_sq_1) == false) {
			add_pawn_move(brd, BLACK, pawn_sq, next_sq_1, mvl);

			if (pawn_rank == RANK_7) {
				enum square next_sq_2 = pawn_sq - 16;
				bool sq_2_occupied = is_square_occupied(brd->board, next_sq_2);
				if (sq_2_occupied == false) {
					add_quiet_move(brd,
					   MOVE(pawn_sq, next_sq_2,
						NO_PIECE, NO_PIECE,
						MFLAG_PAWN_START), mvl);
				}
			}
		}

		// check for capture left
		if (pawn_file >= FILE_B) {
			enum square cap_sq = pawn_sq - 7;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if ((pce != NO_PIECE) && (get_colour(pce) == WHITE)) {
				add_pawn_capture_move(brd, BLACK, pawn_sq, cap_sq, pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				add_pawn_capture_move(brd, BLACK, pawn_sq, cap_sq, pce, mvl);
			}
		}

		// check for capture right
		if (pawn_file <= FILE_G) {
			enum square cap_sq = pawn_sq - 9;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if ((pce != NO_PIECE) && (get_colour(pce) == WHITE)) {
				add_pawn_capture_move(brd, BLACK, pawn_sq, cap_sq, pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				add_pawn_capture_move(brd, BLACK, pawn_sq, cap_sq, pce, mvl);
			}
		}
    }
}


/* Generates horizontal and vertical moves (a la Rook)
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
void generate_sliding_horizontal_vertical_moves(const struct board *brd, struct move_list *mvl, enum piece pce){

	assert((pce == W_ROOK) || (pce == B_ROOK) || (pce == W_QUEEN) || (pce == B_QUEEN));

	U64 bb = brd->bitboards[pce];

    while (bb != 0) {

		enum square pce_sq = POP(&bb);

		U64 hmask = get_horizontal_mask(pce_sq);
		U64 vmask = get_vertical_mask(pce_sq);

		// create slider bb for this square
		U64 bb_slider = GET_PIECE_MASK(pce_sq);

		// all occupied squares (both colours)
		U64 occupied = brd->board;

		U64 term_A = (occupied & vmask) - (2 * bb_slider);
		U64 term_B = reverse_bits(occupied & vmask);
		term_B = term_B - 2 * (reverse_bits(bb_slider));
		term_B = reverse_bits(term_B);

		// all vertical attack squares
		U64 vertical_attacks = (term_A ^ term_B) & vmask;

		term_A = (occupied & hmask) - (2 * bb_slider);
		term_B = reverse_bits(occupied & hmask);
		term_B = term_B - 2 * (reverse_bits(bb_slider));
		term_B = reverse_bits(term_B);

		// all horizontal attack squares
		U64 horizontal_attacks = (term_A ^ term_B) & hmask;

		U64 all_moves = horizontal_attacks ^ vertical_attacks;

		enum colour col = (IS_BLACK(pce)) ? BLACK : WHITE;

		// get all same colour as piece being considered
		U64 col_occupied = overlay_colours(brd, col);

		U64 excl_same_col = all_moves & ~col_occupied;
		//printf("Mask Same Colour\n");
		//print_mask_as_board(&excl_same_col, pce, pce_sq);

		while (excl_same_col != 0) {

			enum square sq = POP(&excl_same_col);

			enum piece mv_pce = get_piece_at_square(brd, sq);

			if (mv_pce != NO_PIECE) {
				add_capture_move(brd, MOVE(pce_sq, sq, mv_pce, NO_PIECE, 0), mvl);
			} else{
				add_quiet_move(brd, MOVE(pce_sq, sq, NO_PIECE, NO_PIECE, 0), mvl);
			}
		}
	}
}


/* Generates diagonal moves (a la Bishop)
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
void generate_sliding_diagonal_moves(const struct board *brd, struct move_list *mvl, enum piece pce){

	assert((pce == W_BISHOP) || (pce == B_BISHOP) || (pce == W_QUEEN) || (pce == B_QUEEN));

	U64 bb = brd->bitboards[pce];

    while (bb != 0) {

		enum square pce_sq = POP(&bb);

		U64 posmask = get_positive_diagonal_mask(pce_sq);
		U64 negmask = get_negative_diagonal_mask(pce_sq);

		// create slider bb for this square
		U64 bb_slider = GET_PIECE_MASK(pce_sq);

		// all occupied squares (both colours)
		U64 occupied = brd->board;

		U64 term_A = (occupied & posmask) - (2 * bb_slider);
		U64 term_B = reverse_bits(occupied & posmask);
		term_B = term_B - 2 * (reverse_bits(bb_slider));
		term_B = reverse_bits(term_B);

		// all positive diagonal attack squares
		U64 positive_diagonal_attacks = (term_A ^ term_B) & posmask;

		term_A = (occupied & negmask) - (2 * bb_slider);
		term_B = reverse_bits(occupied & negmask);
		term_B = term_B - 2 * (reverse_bits(bb_slider));
		term_B = reverse_bits(term_B);

		// all negative diagonal attack squares
		U64 negative_diagonal_attacks = (term_A ^ term_B) & negmask;

		U64 all_moves = positive_diagonal_attacks ^ negative_diagonal_attacks;

		enum colour col = (IS_BLACK(pce)) ? BLACK : WHITE;

		// get all same colour as piece being considered
		U64 col_occupied = overlay_colours(brd, col);

		U64 excl_same_col = all_moves & ~col_occupied;
		//printf("Mask Same Colour\n");
		//print_mask_as_board(&excl_same_col, pce, pce_sq);

		while (excl_same_col != 0) {

			enum square sq = POP(&excl_same_col);

			enum piece mv_pce = get_piece_at_square(brd, sq);

			if (mv_pce != NO_PIECE) {
				add_capture_move(brd, MOVE(pce_sq, sq, mv_pce, NO_PIECE, 0), mvl);
			} else{
				add_quiet_move(brd, MOVE(pce_sq, sq, NO_PIECE, NO_PIECE, 0), mvl);
			}
		}
	}
}







static inline U64 get_vertical_mask(enum square sq){
	return vertical_move_mask[sq];
}


static inline U64 get_horizontal_mask(enum square sq){
	return horizontal_move_mask[sq];
}


static inline U64 get_positive_diagonal_mask(enum square sq){
	return positive_diagonal_masks[sq];
}

static inline U64 get_negative_diagonal_mask(enum square sq){
	return negative_diagonal_masks[sq];
}



/*
 * Prints out the algebraic notatio of a move (eg, a2a4)
 * name: print_move
 * @param
 * @return
 *
 */
char *print_move(U32 move_bitmap)
{

    static char move_string[6];

    int from_file = GET_FILE(FROMSQ(move_bitmap));
    int from_rank = GET_RANK(FROMSQ(move_bitmap));

    int to_file = GET_FILE(TOSQ(move_bitmap));
    int to_rank = GET_RANK(TOSQ(move_bitmap));

    enum piece promoted_pce = PROMOTED(move_bitmap);

    if (promoted_pce != NO_PIECE) {
		char pchar = 'q';
		if (isKn(promoted_pce)) {
			pchar = 'n';
		} else if (isR(promoted_pce)) {
			pchar = 'r';
		} else if (isB(promoted_pce)) {
			pchar = 'b';
		}
		sprintf(move_string, "%c%c%c%c%c", ('a' + from_file),
				('1' + from_rank), ('a' + to_file), ('1' + to_rank), pchar);
	} else {
		sprintf(move_string, "%c%c%c%c", ('a' + from_file),
				('1' + from_rank), ('a' + to_file), ('1' + to_rank));
	}

    return move_string;
}


void print_move_details(U32 move_bitmap, U32 score){
	int from_file = GET_FILE(FROMSQ(move_bitmap));
    int from_rank = GET_RANK(FROMSQ(move_bitmap));

    int to_file = GET_FILE(TOSQ(move_bitmap));
    int to_rank = GET_RANK(TOSQ(move_bitmap));

	enum piece captured = CAPTURED(move_bitmap);
	enum piece promoted = PROMOTED(move_bitmap);

	char c_capt = get_piece_label(captured);
	printf("capt %c\n", c_capt);
	char c_promoted = get_piece_label(promoted);
	printf("prom %c\n", c_promoted);

	printf("move : %c%c%c%c, captured '%c' promote '%c' score %d\n", ('a' + from_file),
				('1' + from_rank), ('a' + to_file), ('1' + to_rank), c_capt, c_promoted, score);


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
		U32 move = list->moves[i].move_bitmap;
		U32 score = list->moves[i].score;

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
		U32 move = list->moves[i].move_bitmap;
		U32 score = list->moves[i].score;

		printf("Move:%d > %s (score:%d)\n", (i + 1), print_move(move), score);
    }
    printf("MoveList Total %d Moves:\n\n", list->move_count);
}
