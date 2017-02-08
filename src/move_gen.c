/*
 * move_gen.c
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
#include "occupancy_mask.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "utils.h"

static void init_mvv_lva_lookup(void);
static void do_gen_moves(struct board *brd, struct move_list *mvl, const bool captures_only);
static void add_capture_move(mv_bitmap move_bitmap, struct move_list *mvlist, enum piece attacker, enum piece victim);
static void add_quiet_move(struct board *brd, mv_bitmap mv, struct move_list *mvlist, enum piece piece_being_moved);
static void add_en_passant_move(mv_bitmap mv, struct move_list *mvlist);

static void generate_white_pawn_moves(struct board *brd,
                                      struct move_list *mvl, const bool only_capture_moves);
static void generate_black_pawn_moves(struct board *brd,
                                      struct move_list *mvl, const bool only_capture_moves);
static void generate_knight_piece_moves(struct board *brd,
                                        struct move_list *mvl,
                                        enum piece knight,
                                        enum colour opposite_col,
                                        const bool only_capture_moves);
static inline void generate_king_moves(struct board *brd,
                                       struct move_list *mvl,
                                       enum colour col,	enum piece king, enum colour opposite_col,
                                       const bool only_capture_moves);
static void generate_black_castle_moves(struct board *brd,
                                        struct move_list *mvl);
static void generate_white_castle_moves(struct board *brd,
                                        struct move_list *mvl);
static void generate_sliding_horizontal_vertical_moves (struct board *brd,
        struct move_list *mvl,
        enum colour col, const bool only_capture_moves);
static void generate_sliding_diagonal_moves(struct board *brd,
        struct move_list *mvl,
        enum colour col, const bool only_capture_moves);

#ifdef ENABLE_ASSERTS
static void assert_add_quiet_move(struct board *brd, mv_bitmap mv, enum piece pce_being_moved);
static void assert_add_capture_move(struct board *brd, mv_bitmap mv);
#endif

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







#define GET_VERTICAL_MASK(sq) 		(vertical_move_mask[sq])
#define GET_HORIZONTAL_MASK(sq)		(horizontal_move_mask[sq])
#define GET_DIAGONAL_MASK(sq)		(positive_diagonal_masks[sq])
#define GET_ANTI_DIAGONAL_MASK(sq)	(negative_diagonal_masks[sq])

////////////////////////////////////////////////

// a mask for the B1, C1 and D1 squares
const uint64_t B1C1D1_MASK 	= 0x000000000000000E;
const uint64_t B8C8D8_MASK 	= 0x0E00000000000000;

const uint64_t F8G8_MASK 	= 0x6000000000000000;
const uint64_t F1G1_MASK 	= 0x0000000000000060;



// using BleuFever's approach for now
// TODO : look at alternative approach (eg, (v << 8 | a)
const uint32_t victim_score[NUM_PIECES] = { 100, 100, 200, 200, 300, 300, 400, 400, 500, 500, 600, 600 };
static uint32_t mvv_lva_score[NUM_PIECES][NUM_PIECES];



void init_move_gen_framework(void)
{

    init_mvv_lva_lookup();

}


static void init_mvv_lva_lookup()
{
    for(uint32_t att = W_PAWN; att <= B_KING; att++) {
        for(uint32_t vict = W_PAWN; vict <= B_KING; vict++) {
            mvv_lva_score[vict][att] = victim_score[vict] + 6 - ( victim_score[att] / 100);
        }
    }
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
    do_gen_moves(brd, mvl, false);
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
    do_gen_moves(brd, mvl, true);
}



static inline void do_gen_moves(struct board *brd, struct move_list *mvl, const bool captures_only)
{
		ASSERT_BOARD_OK(brd);
		//printf("generating moves...\n");
		//print_board(brd);
		
		
    if (get_side_to_move(brd) == WHITE) {
        generate_white_pawn_moves(brd, mvl, captures_only);
        generate_knight_piece_moves(brd, mvl, W_KNIGHT, BLACK, captures_only);
        generate_king_moves(brd, mvl, WHITE, W_KING, BLACK, captures_only);
        // generate rook and queen horizontal moves
        generate_sliding_horizontal_vertical_moves(brd, mvl, WHITE, captures_only);
        // generate bishop and queen diagonal moves
        generate_sliding_diagonal_moves(brd, mvl, WHITE, captures_only);
    } else {
        generate_black_pawn_moves(brd, mvl, captures_only);
        generate_knight_piece_moves(brd, mvl, B_KNIGHT, WHITE, captures_only);
        generate_king_moves(brd, mvl,BLACK, B_KING, WHITE, captures_only);
        // generate rook and queen horizontal moves
        generate_sliding_horizontal_vertical_moves(brd, mvl, BLACK, captures_only);
        // generate bishop and queen diagonal moves
        generate_sliding_diagonal_moves(brd, mvl, BLACK, captures_only);
    }
    	ASSERT_BOARD_OK(brd);
}



static inline void 
add_en_passant_move(mv_bitmap mv, struct move_list *mvlist){

#ifdef ENABLE_ASSERTS
		assert(IS_EN_PASS_MOVE(mv));
#endif
	
	
    mvlist->moves[mvlist->move_count] = mv;
    mvlist->move_count++;
}

static inline void
add_capture_move(mv_bitmap mv, struct move_list *mvlist, enum piece attacker, enum piece victim)
{
    // add mvv-lva assessment of score
    uint32_t mvvlva = mvv_lva_score[victim][attacker];

    // add weight for sorting
    mvvlva += MOVE_ORDER_WEIGHT_CAPTURE;

    add_to_score(&mv, mvvlva);

    mvlist->moves[mvlist->move_count] = mv;
    mvlist->move_count++;
}

#ifdef ENABLE_ASSERTS
static inline void
assert_add_capture_move(struct board *brd, mv_bitmap mv)
{
	assert(IS_CAPTURE_MOVE(mv));
	assert(IS_EN_PASS_MOVE(mv) == false);

    if (IS_EN_PASS_MOVE(mv) == false) {
        enum square to_sq = TOSQ(mv);
        enum piece pce = get_piece_on_square(brd, to_sq);
        assert(pce != NO_PIECE);

        enum square from_sq = FROMSQ(mv);
        pce = get_piece_on_square(brd, from_sq);
        assert(pce != NO_PIECE);
    } else {
        // "to square" should be empty for an en passant move
        enum square to_sq = TOSQ(mv);
        enum piece pce = get_piece_on_square(brd, to_sq);
        assert(pce == NO_PIECE);
    }
}

#endif

static inline void
add_quiet_move(struct board *brd,
               mv_bitmap mv,
               struct move_list *mvlist,
               enum piece piece_being_moved)
{
#ifdef ENABLE_ASSERTS
    assert_add_quiet_move(brd, mv, piece_being_moved);
#endif

    // adjust score by killer moves and search history
    if(get_search_killer(brd, 0, get_ply(brd)) == mv) {
        add_to_score(&mv, MOVE_ORDER_WEIGHT_KILLER_0);
    } else if(get_search_killer(brd, 1, get_ply(brd)) == mv) {
        add_to_score(&mv, MOVE_ORDER_WEIGHT_KILLER_1);
    } else {
        enum square to_sq = TOSQ(mv);

        add_to_score(&mv, get_search_history(brd, piece_being_moved, to_sq));
    }

    mvlist->moves[mvlist->move_count] = mv;
    mvlist->move_count++;
}

#ifdef ENABLE_ASSERTS
static inline void
assert_add_quiet_move(struct board *brd, mv_bitmap mv, enum piece piece_being_moved)
{
    if (IS_CAPTURE_MOVE(mv)) {
        assert(false);
    }

	enum square to_sq = TOSQ(mv);
    enum piece pce = get_piece_on_square(brd, to_sq);

    assert(piece_being_moved != NO_PIECE);

	// quiet move, so nothing on to square
	assert(pce == NO_PIECE);

    if (IS_EN_PASS_MOVE(mv) == false) {
        // to-square is empty
        assert(pce == NO_PIECE);

        enum square from_sq = FROMSQ(mv);
        pce = get_piece_on_square(brd, from_sq);

        assert(pce != NO_PIECE);

    } else {
        // en passant isn't a quiet move
        assert(false);
    }

}

#endif



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
    uint64_t knight_bb = get_bitboard(brd, knight);

    // iterate over all knights of this colour on the board
    while (knight_bb != 0) {

        enum square knight_sq = pop_1st_bit(&knight_bb);

        // get occupancy mask for knight at this square
        uint64_t mask = get_knight_occ_mask(knight_sq);

        // AND'ing with opposite colour pieces, will give all
        // pieces that can be captured
        uint64_t opp_pieces = get_bitboard_for_colour(brd, opposite_col);
        uint64_t capture_squares = mask & opp_pieces;


        while (capture_squares != 0) {
            // loop creating capture moves
            enum square cap_sq = pop_1st_bit(&capture_squares);
            enum piece p = get_piece_on_square(brd, cap_sq);

            mv_bitmap mv = MOVE(knight_sq, cap_sq, p, NO_PIECE, MFLAG_CAPTURE);
            add_capture_move(mv, mvl, knight, p);
        }

        if (only_capture_moves == false) {
            // find all quiet moves (ie, moves to empty squares)
            uint64_t all_pieces = get_bitboard_all_pieces(brd);
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
                                       struct move_list *mvl,
                                       enum colour col, enum piece king,
                                       enum colour opposite_col,
                                       const bool only_capture_moves)
{
    enum square king_sq = get_king_square(brd, col);

    // get occupancy mask for this piece and square
    uint64_t mask = get_king_occ_mask(king_sq);

    // AND'ing with opposite colour pieces, will give all
    // pieces that can be captured
    uint64_t opp_pieces = get_bitboard_for_colour(brd, opposite_col);
    uint64_t capture_squares = mask & opp_pieces;

    while (capture_squares != 0) {
        // loop creating capture moves
        enum square cap_sq = pop_1st_bit(&capture_squares);
        enum piece p = get_piece_on_square(brd, cap_sq);
        mv_bitmap mv = MOVE(king_sq, cap_sq, p, NO_PIECE, MFLAG_CAPTURE);
        add_capture_move(mv, mvl, king, p);
    }

    if (only_capture_moves == false) {
        // find all quiet moves
        uint64_t all_pieces = get_bitboard_all_pieces(brd);
        uint64_t empty_squares = ~all_pieces & mask;
        while (empty_squares != 0) {
            // loop creating quiet moves
            enum square empty_sq = pop_1st_bit(&empty_squares);

            mv_bitmap mv = MOVE(king_sq, empty_sq, NO_PIECE, NO_PIECE, MFLAG_NONE);

            add_quiet_move(brd, mv, mvl, king);
        }

        // check for castling moves
        if (get_castle_permissions(brd) != CASTLE_PERM_NONE) {
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
    if (get_castle_permissions(brd) & WKCA) {
		uint64_t brd_bb= get_bitboard_all_pieces(brd);
		
		bool f1g1_occupied = ((brd_bb & F1G1_MASK) != 0);
		
        if (!f1g1_occupied) {
            if (!is_sq_attacked(brd, e1, BLACK)
                    && !is_sq_attacked(brd, f1, BLACK)) {

                mv_bitmap mv = MOVE(e1, g1, NO_PIECE,
                                    NO_PIECE, MFLAG_CASTLE);
                add_quiet_move(brd, mv, mvl, W_KING);
            }
        }
    }

    if (get_castle_permissions(brd) & WQCA) {
 		uint64_t brd_bb= get_bitboard_all_pieces(brd);
 		
		bool b1c1d1_occupied = ((brd_bb & B1C1D1_MASK) != 0);

        if (!b1c1d1_occupied) {
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
    if (get_castle_permissions(brd) & BKCA) {
		uint64_t brd_bb= get_bitboard_all_pieces(brd);
		
		bool f8g8_occupied = ((brd_bb & F8G8_MASK) != 0);
		
        if (!f8g8_occupied) {
            if (!is_sq_attacked(brd, e8, WHITE)
                    && !is_sq_attacked(brd, f8, WHITE)) {

                mv_bitmap mv = MOVE(e8, g8, NO_PIECE,
                                    NO_PIECE, MFLAG_CASTLE);
                add_quiet_move(brd, mv, mvl, B_KING);
            }
        }
    }

    if (get_castle_permissions(brd) & BQCA) {
		uint64_t brd_bb= get_bitboard_all_pieces(brd);

		bool b8c8d8_not_occupied = ((brd_bb & B8C8D8_MASK) == 0);

        if (b8c8d8_not_occupied) {
            if (!is_sq_attacked(brd, e8, WHITE)
                    && !is_sq_attacked(brd, d8, WHITE)) {

                mv_bitmap mv = MOVE(e8, c8, NO_PIECE, NO_PIECE, MFLAG_CASTLE);
                add_quiet_move(brd, mv, mvl, B_KING);
            }
        }
    }
}

static inline void
generate_white_pawn_moves(struct board *brd, struct move_list *mvl,
                          const bool only_capture_moves)
{
	//print_board(brd);
	
	ASSERT_BOARD_OK(brd);
	
    // get the bitboard representing all WHITE pawns
    // on the board
    uint64_t pawn_bb = get_bitboard(brd, W_PAWN);

    uint64_t bb_black_pieces = get_bitboard_for_colour(brd, BLACK);

    while (pawn_bb != 0) {
        mv_bitmap mv = 0;
        enum square pawn_sq = pop_1st_bit(&pawn_bb);

        uint8_t pawn_file = get_file(pawn_sq);
        uint8_t pawn_rank = get_rank(pawn_sq);
        enum square north_sq = pawn_sq + NORTH;

        // check for moving 1 and 2 squares forward
        // ========================================
        if (only_capture_moves == false) {
            if (is_square_occupied(get_bitboard_all_pieces(brd), north_sq) == false) {
                if (pawn_rank == RANK_7) {
                    // pawn can promote to 4 pieces
                    mv = MOVE(pawn_sq, north_sq, NO_PIECE, W_QUEEN, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, W_PAWN);

                    mv = MOVE(pawn_sq, north_sq, NO_PIECE, W_ROOK, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, W_PAWN);

                    mv = MOVE(pawn_sq, north_sq, NO_PIECE, W_BISHOP, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, W_PAWN);

                    mv = MOVE(pawn_sq, north_sq, NO_PIECE, W_KNIGHT, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, W_PAWN);
                } else {
                    mv = MOVE(pawn_sq, north_sq, NO_PIECE, NO_PIECE, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, W_PAWN);
                }

                if (pawn_rank == RANK_2) {
                    enum square north_x2 = pawn_sq + NN;
                    if (is_square_occupied(get_bitboard_all_pieces(brd), north_x2) == false) {
                        mv = MOVE(pawn_sq, north_x2, NO_PIECE, NO_PIECE, MFLAG_PAWN_START);
                        add_quiet_move(brd, mv, mvl, W_PAWN);
                    }
                }
                      
            }
        }
        // check for capture left
        // ======================
        if (pawn_file > FILE_A) {
            enum square northwest = pawn_sq + NW;

            if (is_square_occupied(bb_black_pieces, northwest) == true) {
                enum piece capt_pce = get_piece_on_square(brd, northwest);
                
                if (pawn_rank == RANK_7) {
                    // pawn can promote to 4 pieces
                    mv = MOVE(pawn_sq, northwest, capt_pce, W_QUEEN, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, northwest, capt_pce, W_ROOK, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, northwest, capt_pce, W_BISHOP, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, northwest, capt_pce, W_KNIGHT, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);
                } else {
                    mv = MOVE(pawn_sq, northwest, capt_pce, NO_PIECE, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);
                }
            }

            if (northwest == get_en_passant_sq(brd)) {
                mv = MOVE(pawn_sq, northwest, B_PAWN, NO_PIECE, MFLAG_EN_PASSANT);
                add_en_passant_move(mv, mvl);
            }
        }
        // check for capture right
        //========================
        if (pawn_file < FILE_H) {
            enum square northeast = pawn_sq + NE;

            if (is_square_occupied(bb_black_pieces, northeast) == true) {

                enum piece capt_pce = get_piece_on_square(brd, northeast);
                if (pawn_rank == RANK_7) {
                    // pawn can promote to 4 pieces
                    mv = MOVE(pawn_sq, northeast, capt_pce, W_QUEEN, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, northeast, capt_pce, W_ROOK, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, northeast, capt_pce, W_BISHOP, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, northeast, capt_pce, W_KNIGHT, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);
                } else {
                    mv = MOVE(pawn_sq, northeast, capt_pce, NO_PIECE, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, W_PAWN, capt_pce);
                }
            }

            if (northeast == get_en_passant_sq(brd)) {
                mv = MOVE(pawn_sq, northeast, B_PAWN, NO_PIECE, MFLAG_EN_PASSANT);
                add_en_passant_move(mv, mvl);
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
    uint64_t pawn_bb = get_bitboard(brd, B_PAWN);
    uint64_t bb_white_pieces = get_bitboard_for_colour(brd, WHITE);

    while (pawn_bb != 0) {
        mv_bitmap mv = 0;
        enum square pawn_sq = pop_1st_bit(&pawn_bb);

        int pawn_file = get_file(pawn_sq);
        int pawn_rank = get_rank(pawn_sq);

        // check for moving 1 and 2 squares forward
        //=========================================
        enum square south_sq = pawn_sq + (enum square)SOUTH;
        if (only_capture_moves == false) {
            if (is_square_occupied(get_bitboard_all_pieces(brd), south_sq) == false) {
                if (pawn_rank == RANK_2) {
                    // pawn can promote to 4 pieces
                    mv = MOVE(pawn_sq, south_sq, NO_PIECE, B_QUEEN, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, B_PAWN);

                    mv = MOVE(pawn_sq, south_sq, NO_PIECE, B_ROOK, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, B_PAWN);

                    mv = MOVE(pawn_sq, south_sq, NO_PIECE, B_BISHOP, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, B_PAWN);

                    mv = MOVE(pawn_sq, south_sq, NO_PIECE, B_KNIGHT, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, B_PAWN);
                } else {
                    mv = MOVE(pawn_sq, south_sq, NO_PIECE, NO_PIECE, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, B_PAWN);
                }
                if (pawn_rank == RANK_7) {
                    enum square south_x2 = pawn_sq + (enum square)SS;	// can skip down 2 ranks

                    bool sq_2_occupied = is_square_occupied(get_bitboard_all_pieces(brd), south_x2);
                    if (sq_2_occupied == false) {
                        mv = MOVE(pawn_sq, south_x2,NO_PIECE, NO_PIECE,MFLAG_PAWN_START);
                        add_quiet_move(brd, mv, mvl, B_PAWN);
                    }
                }
                
            }
        }

        // check for capture left
        // ======================
        if (pawn_file > FILE_A) {
            enum square southwest = pawn_sq + (enum square)SW;

            if (is_square_occupied(bb_white_pieces, southwest) == true) {
                enum piece capt_pce = get_piece_on_square(brd, southwest);
                if (pawn_rank == RANK_2) {
                    // pawn can promote to 4 pieces
                    mv = MOVE(pawn_sq, southwest, capt_pce, B_QUEEN, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, southwest, capt_pce, B_ROOK, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, southwest, capt_pce, B_BISHOP, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, southwest, capt_pce, B_KNIGHT, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);
                } else {
                    mv = MOVE(pawn_sq, southwest, capt_pce, NO_PIECE, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);
                }
            }

            if (southwest == get_en_passant_sq(brd)) {
                mv = MOVE(pawn_sq, southwest, W_PAWN, NO_PIECE, MFLAG_EN_PASSANT);
                add_en_passant_move(mv, mvl);
            }
        }
        // check for capture right
        //========================
        if (pawn_file < FILE_H) {
            enum square southeast = pawn_sq + (enum square)SE;

            if (is_square_occupied(bb_white_pieces, southeast) == true) {
                enum piece capt_pce = get_piece_on_square(brd, southeast);
                if (pawn_rank == RANK_2) {
                    // pawn can promote to 4 pieces
                    mv = MOVE(pawn_sq, southeast, capt_pce, B_QUEEN, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, southeast, capt_pce, B_ROOK, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, southeast, capt_pce, B_BISHOP, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);

                    mv = MOVE(pawn_sq, southeast, capt_pce, B_KNIGHT, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);
                } else {
                    mv = MOVE(pawn_sq, southeast, capt_pce, NO_PIECE, MFLAG_CAPTURE);
                    add_capture_move(mv, mvl, B_PAWN, capt_pce);
                }
            }

            if (southeast == get_en_passant_sq(brd)) {
                mv = MOVE(pawn_sq, southeast, W_PAWN, NO_PIECE, MFLAG_EN_PASSANT);
                add_en_passant_move(mv, mvl);
            }
        }
    }

}

/* Generates sliding horizontal and vertical moves for queen and rook
 * for the given colour
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
        enum colour col, const bool only_capture_moves)
{

    // create a single bitboard for both rook nd queen
    uint64_t bb = 0;
    if (col == WHITE) {
        bb = get_bitboard_combined(brd, W_ROOK, W_QUEEN);
    } else {
        bb = get_bitboard_combined(brd, B_ROOK, B_QUEEN);
    }


    while (bb != 0) {

        enum square pce_sq = pop_1st_bit(&bb);
        enum piece piece_being_moved = get_piece_on_square(brd, pce_sq);

#ifdef ENABLE_ASSERTS
        assert(piece_being_moved != NO_PIECE);
#endif

        uint64_t hmask = GET_HORIZONTAL_MASK(pce_sq);
        uint64_t vmask = GET_VERTICAL_MASK(pce_sq);

        // create slider bb for this square
        uint64_t bb_slider = GET_PIECE_MASK(pce_sq);

        // all occupied squares (both colours)
        uint64_t occupied = get_bitboard_all_pieces(brd);

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

        // get all same colour as piece being considered so
        // they can be excluded (since they block a move)
        uint64_t col_occupied = get_bitboard_for_colour(brd, col);
        uint64_t excl_same_col = all_moves & ~col_occupied;

        while (excl_same_col != 0) {
            enum square sq = pop_1st_bit(&excl_same_col);
            enum piece mv_pce = get_piece_on_square(brd, sq);

            if (mv_pce != NO_PIECE) {
                mv_bitmap mv = MOVE(pce_sq, sq, mv_pce, NO_PIECE, MFLAG_CAPTURE);
                enum piece attacking_pce = get_piece_on_square(brd, pce_sq);
                add_capture_move(mv, mvl, attacking_pce, mv_pce);
            } else {
                if (only_capture_moves == false) {
                    mv_bitmap mv = MOVE(pce_sq, sq, NO_PIECE, NO_PIECE, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, piece_being_moved);
                }
            }
        }
    }
}

/* Generates sliding diagonal moves for bishop and queen for the given colour
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
        struct move_list *mvl, enum colour col, const bool only_capture_moves)
{

    // create single bitboard representing bishop and queen
    uint64_t bb = 0;
    if (col == WHITE) {
        bb = get_bitboard_combined(brd, W_BISHOP, W_QUEEN);
    } else {
        bb = get_bitboard_combined(brd, B_BISHOP, B_QUEEN);
    }

    while (bb != 0) {

        enum square pce_sq = pop_1st_bit(&bb);
        enum piece piece_being_moved = get_piece_on_square(brd, pce_sq);

        uint64_t posmask = GET_DIAGONAL_MASK(pce_sq);
        uint64_t negmask = GET_ANTI_DIAGONAL_MASK(pce_sq);

        // create slider bb for this square
        uint64_t bb_slider = GET_PIECE_MASK(pce_sq);

        // all occupied squares (both colours)
        uint64_t occupied = get_bitboard_all_pieces(brd);

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

        // get all same colour as piece being considered
        uint64_t col_occupied = get_bitboard_for_colour(brd, col);
        uint64_t excl_same_col = all_moves & ~col_occupied;

        while (excl_same_col != 0) {

            enum square sq = pop_1st_bit(&excl_same_col);

            enum piece mv_pce = get_piece_on_square(brd, sq);

            if (mv_pce != NO_PIECE) {
                mv_bitmap mv = MOVE(pce_sq, sq, mv_pce, NO_PIECE, MFLAG_CAPTURE);
                enum piece attacking_pce = get_piece_on_square(brd, pce_sq);
                add_capture_move(mv, mvl, attacking_pce, mv_pce);
            } else {
                if (only_capture_moves == false) {
                    mv_bitmap mv = MOVE(pce_sq, sq, NO_PIECE, NO_PIECE, MFLAG_NONE);
                    add_quiet_move(brd, mv, mvl, piece_being_moved);
                }
            }
        }
    }
}


bool move_exists(struct board *brd, mv_bitmap move_to_test)
{

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };

    generate_all_moves(brd, &mvl);

    for (uint32_t m = 0; m < mvl.move_count; m++) {
        mv_bitmap mv = mvl.moves[m];
        if (make_move(brd, mv) == false) {
            continue;
        }

        take_move(brd);

        if(mvl.moves[m] == move_to_test) {
            return true;
        }

    }
    return false;
}




// function to map move attributes to a bitmapped field
// see typdef for mv_bitmap for a description
inline mv_bitmap MOVE(enum square from, enum square to, enum piece capture,
                           enum piece promote, uint64_t flags)
{
    mv_bitmap retval = 0;

    retval =  ((mv_bitmap)from 		<< MV_MASK_OFF_FROM_SQ);
    retval |= ((mv_bitmap)to 		<< MV_MASK_OFF_TO_SQ);
    retval |= ((mv_bitmap)capture 	<< MV_MASK_OFF_CAPTURED_PCE);
    retval |= ((mv_bitmap)promote 	<< MV_MASK_OFF_PROMOTED_PCE);
    retval |= flags;

    return retval;
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
    if (col == WHITE) {
        generate_knight_piece_moves(brd, mvl, W_KNIGHT, BLACK, false);
    } else {
        generate_knight_piece_moves(brd, mvl, B_KNIGHT, WHITE, false);
    }

}

void
TEST_generate_king_moves(struct board *brd, struct move_list *mvl,
                         enum colour col)
{
    enum colour opposite_col;
    enum piece king;
    if (col == WHITE) {
        opposite_col = BLACK;
        king = W_KING;
    } else {
        king = B_KING;
        opposite_col = WHITE;
    }

    generate_king_moves(brd, mvl, col, king, opposite_col, false);
}

void
TEST_generate_castle_moves(struct board *brd, struct move_list *mvl,
                           enum colour col)
{
    if (col == WHITE) {
        generate_white_castle_moves(brd, mvl);
    } else {
        generate_black_castle_moves(brd, mvl);
    }
}

void
TEST_generate_sliding_horizontal_vertical_moves(struct board *brd,
        struct move_list *mvl,
        enum colour col)
{
    generate_sliding_horizontal_vertical_moves(brd, mvl, col, false);
}

void
TEST_generate_sliding_diagonal_moves(struct board *brd,
                                     struct move_list *mvl, enum colour col)
{
    generate_sliding_diagonal_moves(brd, mvl, col, false);
}


bool TEST_is_move_in_list(struct move_list *mvl, mv_bitmap mv)
{
    return is_move_in_list(mvl, mv);
}
