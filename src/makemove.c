/*
 * makemove.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: code associated with making and retracting moves
 * on the board.
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
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "pieces.h"
#include "board.h"
#include "attack.h"
#include "makemove.h"
#include "board_utils.h"
#include "utils.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "hashkeys.h"





static inline void add_black_pawn_info(struct board *brd, enum square sq);
static inline void add_white_pawn_info(struct board *brd, enum square sq);
static inline void remove_black_pawn_info(struct board *brd, enum square sq);
static inline void remove_white_pawn_info(struct board *brd, enum square sq);
static inline void update_black_pawn_control(struct board *brd, enum square sq, int val);
static inline void update_white_pawn_control(struct board *brd, enum square sq, int val);


//bit mask for castle permissions
static const uint8_t castle_permission_mask[NUM_SQUARES] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    7, 15, 15, 15, 3, 15, 15, 11
};


void move_piece(struct board *brd, enum square from, enum square to)
{	

    enum piece pce = brd->pieces[from];
    enum colour pce_col = GET_COLOUR(pce);

#ifdef ENABLE_ASSERTS
	assert(pce != NO_PIECE);
	assert(from != to);
#endif
#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
	assert_material_correct(brd);
#endif
	
	//print_board(brd);
	
	
#ifdef ENABLE_ASSERTS
	assert_material_correct(brd);
#endif
	
	
    // adjust the hash
    brd->board_hash ^= get_piece_hash(pce, from);
    brd->board_hash ^= get_piece_hash(pce, to);

    // clear up the "from" resources
    // test
    if (pce == NO_PIECE){
		assert(false);
	}
    
    brd->pieces[from] = NO_PIECE;
    clear_bit(&brd->bitboards[pce], from);
    clear_bit(&brd->board, from);

    // set up the "to" resources
    brd->pieces[to] = pce;
    set_bit(&brd->bitboards[pce], to);
    set_bit(&brd->board, to);

#ifdef ENABLE_ASSERTS
	assert_material_correct(brd);
#endif

    if (pce_col == WHITE) {
        if(pce == W_PAWN) {
            // easiest way to move a pawn
            remove_white_pawn_info(brd, from);
            add_white_pawn_info(brd, to);
        } else if (pce == W_KING) {
            brd->king_sq[WHITE] = to;
        }
    } else {
        if(pce == B_PAWN) {
            // easiest way to move a pawn
            remove_black_pawn_info(brd, from);
            add_black_pawn_info(brd, to);
        } else if (pce == B_KING) {
            brd->king_sq[BLACK] = to;
        }
    }
#ifdef ENABLE_ASSERTS
	assert_material_correct(brd);
#endif
}

// return false if move is invalid, true otherwise
bool make_move(struct board *brd, mv_bitmap mv)
{

	
    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);

	
#ifdef ENABLE_ASSERTS
	printf("VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n");	
	ASSERT_BOARD_OK(brd);
	printf("UHUHUH - ");
	print_move_details(mv);
	printf("castle move = %d\n", IS_CASTLE_MOVE(mv));
	printf("en pass move = %d\n", IS_EN_PASS_MOVE(mv));
	printf("capture move = %d\n", IS_CAPTURE_MOVE(mv));

	printf("from sq %s\n", print_square(from));
	printf("to sq %s\n", print_square(to));
	printf("pce on from : %c\n", get_piece_label(brd->pieces[from]));
	printf("pce on to : %c\n", get_piece_label(brd->pieces[to]));
#endif
	


    enum piece pce_being_moved = brd->pieces[from];
    enum colour side = brd->side_to_move;

    brd->history[brd->history_ply].board_hash = brd->board_hash;

    if (IS_EN_PASS_MOVE(mv)) {
        if (side == WHITE) {
            // must be a bp
            remove_piece_from_board(brd, B_PAWN, to - 8);
        } else {
            // must be a wp
            remove_piece_from_board(brd, W_PAWN, to + 8);
        }
    } else if (IS_CASTLE_MOVE(mv)) {
        switch (to) {
        case c1:
            move_piece(brd, a1, d1);
            break;
        case c8:
            move_piece(brd, a8, d8);
            break;
        case g1:
            move_piece(brd, h1, f1);
            break;
        case g8:
            move_piece(brd, h8, f8);
            break;
        default:
            printf("to : %s\n", print_square(to));
            assert(false);
            break;
        }

    }

    if (brd->en_passant != NO_SQUARE) {
        brd->board_hash ^= get_en_passant_hash(brd->en_passant);
    }

    brd->board_hash ^= get_castle_hash(brd->castle_perm);

    // set up history
    brd->history[brd->history_ply].move = mv;
    brd->history[brd->history_ply].fifty_move_counter = brd->fifty_move_counter;
    brd->history[brd->history_ply].en_passant = brd->en_passant;
    brd->history[brd->history_ply].castle_perm = brd->castle_perm;

    // reset castle and set again
    brd->castle_perm &= castle_permission_mask[from];
    brd->castle_perm &= castle_permission_mask[to];
    brd->board_hash ^= get_castle_hash(brd->castle_perm);

    brd->en_passant = NO_SQUARE;
    brd->fifty_move_counter++;

    if (IS_CAPTURE_MOVE(mv)) {
        enum piece capt = brd->pieces[to];
        remove_piece_from_board(brd, capt, to);
        brd->fifty_move_counter = 0;
    }

    brd->ply++;
    brd->history_ply++;


    if (IS_PAWN(pce_being_moved)) {
        brd->fifty_move_counter = 0;

        if (IS_PAWN_START(mv)) {
            if (side == WHITE) {
                brd->en_passant = from + 8;
            } else {
                brd->en_passant = from - 8;
            }
            brd->board_hash ^= get_en_passant_hash(brd->en_passant);
        }
    }

#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
	 
    if ((from == e5) && (to == c6)){
		enum piece pce_e5 = brd->pieces[e5];
		enum piece pce_c6 = brd->pieces[c6];
		
		if ((pce_e5 == W_KNIGHT) && (pce_c6 == B_PAWN))
		{
			if (IS_CAPTURE_MOVE(mv)){
				assert(false);
			}
		}
	}
#endif


    move_piece(brd, from, to);


#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
#endif


    enum piece promoted = PROMOTED_PCE(mv);
    if (promoted != NO_PIECE) {
        enum piece capt = brd->pieces[to];
        remove_piece_from_board(brd, capt, to);
        add_piece_to_board(brd, promoted, to);
    }


#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
#endif
		

    // flip side
    flip_sides(brd);

    // check if move is valid (ie, king in check)
    enum square king_sq = brd->king_sq[side];

    // side is already flipped above, so use that as the attacking side
    if (is_sq_attacked(brd, king_sq, brd->side_to_move)) {
        take_move(brd);


#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
#endif


        return false;
    } else {
		

#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
#endif
		
        return true;
    }


}

inline void take_move(struct board *brd)
{
    brd->history_ply--;
    brd->ply--;

    mv_bitmap mv = brd->history[brd->history_ply].move;

    // note: when reverting, the 'from' square will be empty and the 'to'
    // square has the piece in it.
    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);

    // hash out en passant and castle if set
    if (brd->en_passant != NO_SQUARE) {
        brd->board_hash ^= get_en_passant_hash(brd->en_passant);
    }

    brd->board_hash ^= get_castle_hash(brd->castle_perm);

    brd->castle_perm = brd->history[brd->history_ply].castle_perm;
    brd->fifty_move_counter = brd->history[brd->history_ply].fifty_move_counter;
    brd->en_passant = brd->history[brd->history_ply].en_passant;

    // now, hash back in
    if (brd->en_passant != NO_SQUARE) {
        brd->board_hash ^= get_en_passant_hash(brd->en_passant);
    }
    brd->board_hash ^= get_castle_hash(brd->castle_perm);

    // flip side
    flip_sides(brd);

    if (MFLAG_EN_PASSANT & mv) {
        if (brd->side_to_move == WHITE) {
            add_piece_to_board(brd, B_PAWN, to - 8);
        } else {
            add_piece_to_board(brd, W_PAWN, to + 8);
        }
    } else if (MFLAG_CASTLE & mv) {
        switch (to) {
        case c1:
            move_piece(brd, d1, a1);
            break;
        case c8:
            move_piece(brd, d8, a8);
            break;
        case g1:
            move_piece(brd, f1, h1);
            break;
        case g8:
            move_piece(brd, f8, h8);
            break;
        default:
            assert(false);
            break;
        }
    }
    // note: to revert move, move piece from 'to' to 'from'
    move_piece(brd, to, from);

    enum piece captured = CAPTURED_PCE(mv);
    if (captured != NO_PIECE) {
        add_piece_to_board(brd, captured, to);
    }

    enum piece promoted = PROMOTED_PCE(mv);
    if (PROMOTED_PCE(mv) != NO_PIECE) {
        enum colour prom_col = GET_COLOUR(promoted);
        remove_piece_from_board(brd, promoted, from);

        enum piece pce_to_add = (prom_col == WHITE) ? W_PAWN : B_PAWN;
        add_piece_to_board(brd, pce_to_add, from);
    }
}



inline void flip_sides(struct board *brd)
{
    // flip side
    brd->side_to_move = GET_OPPOSITE_SIDE(brd->side_to_move);
    brd->board_hash ^= get_side_hash();
}


void add_piece_to_board(struct board *brd, enum piece pce, enum square sq)
{
#ifdef ENABLE_ASSERTS
    assert(pce != NO_PIECE);
    assert(IS_VALID_PIECE(pce));
    assert(IS_VALID_SQUARE(sq));
#endif


    enum colour col = GET_COLOUR(pce);
    brd->board_hash ^= get_piece_hash(pce, sq);

    brd->pieces[sq] = pce;
    brd->material[col] += GET_PIECE_VALUE(pce);

    // set piece on bitboards
    set_bit(&brd->bitboards[pce], sq);
    set_bit(&brd->board, sq);

    switch (pce) {
    case W_PAWN:
        add_white_pawn_info(brd, sq);
        break;
    case B_PAWN:
        add_black_pawn_info(brd, sq);
        break;
    case W_KING:
    case B_KING:
        brd->king_sq[col] = sq;
        break;
    default:
        break;
    }

}



void remove_piece_from_board(struct board *brd, enum piece pce_to_remove, enum square sq)
{
#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
    assert(pce_to_remove != NO_PIECE);
    assert(IS_VALID_PIECE(pce_to_remove));
	assert(IS_VALID_SQUARE(sq));
	
	enum piece pce_on_sq = brd->pieces[sq];
	assert(pce_on_sq == pce_to_remove);
	
#endif

    enum colour col = GET_COLOUR(pce_to_remove);
    brd->board_hash ^= get_piece_hash(pce_to_remove, sq);
    brd->pieces[sq] = NO_PIECE;
    brd->material[col] -= GET_PIECE_VALUE(pce_to_remove);

    // remove piece from bitboards
    clear_bit(&brd->bitboards[pce_to_remove], sq);
    clear_bit(&brd->board, sq);

    switch (pce_to_remove) {
    case W_PAWN:
        remove_white_pawn_info(brd, sq);
        break;
    case B_PAWN:
        remove_black_pawn_info(brd, sq);
        break;
    case W_KING:
    case B_KING:
        brd->king_sq[col] = NO_SQUARE;
        break;
    default:
        break;
    }
    
#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
#endif
}




static inline void remove_black_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[BLACK][file]--;
    brd->pawns_on_rank[BLACK][rank]--;

    update_black_pawn_control(brd, sq, -1);
}



static inline void remove_white_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[WHITE][file]--;
    brd->pawns_on_rank[WHITE][rank]--;

    update_white_pawn_control(brd, sq, -1);

}




// adds a pawn to the underlying board struct
static inline void add_black_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[BLACK][file]++;
    brd->pawns_on_rank[BLACK][rank]++;

    update_black_pawn_control(brd, sq, 1);
}



// adds a pawn to the underlying board struct
static inline void add_white_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[WHITE][file]++;
    brd->pawns_on_rank[WHITE][rank]++;

    update_white_pawn_control(brd, sq, 1);

}



static inline void update_black_pawn_control(struct board *brd, enum square sq, int val)
{
    int32_t next_sq = 0;
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    if (file > FILE_A) {
        if (rank > RANK_1) {
            next_sq = (int32_t)sq + SW;
            brd->pawn_control[BLACK][next_sq] += val;
        }
    }
    if (file < FILE_H) {
        if (rank > RANK_1) {
            next_sq = (int32_t)sq + SE;
            brd->pawn_control[BLACK][next_sq] += val;
        }
    }
}

static inline void update_white_pawn_control(struct board *brd, enum square sq, int val)
{

    int32_t next_sq = 0;
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    if (file > FILE_A) {
        if (rank < RANK_8) {
            next_sq = sq + NW;
            brd->pawn_control[WHITE][next_sq] += val;
        }
    }
    if (file < FILE_H) {
        if (rank < RANK_8) {
            next_sq = sq + NE;
            brd->pawn_control[WHITE][next_sq] += val;
        }
    }
}



/*
 *
 * name: clear_bit
 * @param : board, square
 * @return : void
 *
 */
inline void clear_bit(uint64_t * brd, enum square sq)
{
    *brd = *brd & (uint64_t) (~(0x01ull << sq));
}

/*
 *
 * name: set_bit
 * @param : board, square
 * @return : void
 *
 */
inline void set_bit(uint64_t * brd, enum square sq)
{
    *brd = *brd | (uint64_t) (0x01ull << sq);
}



