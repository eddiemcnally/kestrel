/*
 * board.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for manipulating the chess board
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
#include "attack.h"
#include "types.h"
#include "fen.h"
#include "board.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "board_utils.h"
#include "tt.h"
#include "hashkeys.h"
#include "pieces.h"



static inline void add_black_pawn_info(struct board *brd, enum square sq);
static inline void add_white_pawn_info(struct board *brd, enum square sq);
static inline void remove_black_pawn_info(struct board *brd, enum square sq);
static inline void remove_white_pawn_info(struct board *brd, enum square sq);
static inline void update_pawn_control(struct board* brd, const enum colour col, const enum square sq, int8_t val);



static void assert_board_and_move(struct board *brd, mv_bitmap mv);


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

struct bitboards{
	// bitboard entry for each piece
    uint64_t pieces[NUM_PIECES];

    // The above array piece arrays overlayed into a single bitboard.
    // In effect, an bitwise OR of all elements in pieces[]
    uint64_t board;
};


/**
 * A container for holding a specific position
 */
struct board {

	// bitboards
	struct bitboards bitboards;
    
    // indexed by enum colour, contains sum of all piece values
    uint32_t material[NUM_COLOURS];

    // contains the pieces on each square
    enum piece pieces[NUM_SQUARES];

    
    // we need to look up the king position very frequently,
    // so save it for a quick lookup, rather than extracting it
    // from a bitboard each time
    enum square king_sq[NUM_COLOURS];

	// maintain separate info about the pawns to simplify the
    // evaluation of pawn structure, open files, etc
    int8_t pawns_on_file[NUM_COLOURS][NUM_FILES];
    int8_t pawns_on_rank[NUM_COLOURS][NUM_RANKS];
    int8_t pawn_control[NUM_COLOURS][NUM_SQUARES];


    // the next side to move
    enum colour side_to_move;

    // a hash of the current board
    uint64_t board_hash;

    // the square where en passent is active
    enum square en_passant;

    uint8_t fifty_move_counter;

    // keeping track of ply
    uint8_t ply;
    uint8_t history_ply;


    // the best moves from the current position
    mv_bitmap pv_line[MAX_SEARCH_DEPTH];

    // castling permissions
    uint8_t castle_perm;

    // move history
    struct undo history[MAX_GAME_MOVES];


    // move ordering
    uint32_t search_history[NUM_PIECES][NUM_SQUARES];
    mv_bitmap search_killers[NUM_KILLER_MOVES][MAX_SEARCH_DEPTH];

};


/*
 * Creates and initialises a new board. The default starting piece
 * positions are populated.
 * name: init_board
 * @param fen : the FEN string
 * @return	a new board
 *
 */
void init_board(char *fen, struct board *brd)
{
    get_clean_board(brd);
    consume_fen_notation(fen, brd);
    
#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
#endif

}

/*
 * Creates an empty board struct
 * name: get_clean_board
 * @param
 * @return	ptr to a created board struct
 *
 */
void get_clean_board(struct board *brd)
{
    memset(brd, 0, sizeof(struct board));

    for(uint8_t i = 0; i < NUM_SQUARES; i++) {
        brd->pieces[i] = NO_PIECE;
    }

    brd->king_sq[WHITE] = NO_SQUARE;
    brd->king_sq[BLACK] = NO_SQUARE;

    brd->side_to_move = WHITE;
    brd->en_passant = NO_SQUARE;

    for(uint16_t i = 0; i < MAX_SEARCH_DEPTH; i++) {
        brd->pv_line[i] = NO_MOVE;
    }

    for(uint16_t i = 0; i < MAX_GAME_MOVES; i++) {
        brd->history[i].move = NO_MOVE;
        brd->history[i].en_passant = NO_SQUARE;
        // other struct values are already set to zero with memset
    }
    
    set_castle_permission(brd, CASTLE_PERM_NONE);
}

// returns the count.
uint8_t populate_pv_line(struct board *brd, uint8_t depth)
{

    mv_bitmap mv = probe_tt(brd->board_hash);

    uint8_t count = 0;

    while((mv != NO_MOVE) && (count < depth)) {

        //assert(count < MAX_SEARCH_DEPTH);

        make_move(brd, mv);
        brd->pv_line[count++] = mv;

        mv = probe_tt(brd->board_hash);
    }

    // rollback moves
    while(brd->ply > 0) {
        take_move(brd);
    }

    return count;
}


mv_bitmap get_best_pvline(const struct board *brd){
	return get_pvline(brd, 0);
}

mv_bitmap get_pvline(const struct board *brd, uint8_t search_depth){
	return brd->pv_line[search_depth];
}

void set_pvline(struct board *brd, uint8_t search_depth, mv_bitmap move){
	brd->pv_line[search_depth] = move;
}



inline uint64_t overlay_white_piece_bitboards(const struct board * brd)
{
    return brd->bitboards.pieces[W_PAWN] | brd->bitboards.pieces[W_BISHOP]
           | brd->bitboards.pieces[W_KNIGHT] | brd->bitboards.pieces[W_ROOK]
           | brd->bitboards.pieces[W_QUEEN] | brd->bitboards.pieces[W_KING];
}

inline uint64_t overlay_black_piece_bitboards(const struct board * brd)
{
    return brd->bitboards.pieces[B_PAWN] | brd->bitboards.pieces[B_BISHOP]
           | brd->bitboards.pieces[B_KNIGHT] | brd->bitboards.pieces[B_ROOK]
           | brd->bitboards.pieces[B_QUEEN] | brd->bitboards.pieces[B_KING];
}


void update_board_hash(struct board *brd){
	brd->board_hash = get_position_hash(brd);
}

uint64_t get_board_hash(const struct board *brd){
	return brd->board_hash;
}

enum piece get_piece_on_square(const struct board *brd, enum square sq){
	return brd->pieces[sq];
}

void set_en_passant_sq(struct board *brd, enum square sq){
	brd->en_passant = sq;
}

enum square get_en_passant_sq(const struct board *brd){
	return brd->en_passant;
}




uint8_t get_fifty_move_counter(const struct board *brd){
	return brd->fifty_move_counter;
}

uint8_t get_ply(const struct board *brd){
	return brd->ply;
}

void set_ply(struct board *brd, uint8_t ply){
	brd->ply = ply;
}

uint8_t get_history_ply(const struct board *brd){
	return brd->history_ply;
}
void set_history_ply(struct board *brd, uint8_t hist_ply){
	brd->history_ply = hist_ply;
}



/* Compares two boards and checks for equality
 *
 * name: assert_boards_are_equal
 * @param
 * @return
 *
 */
void assert_boards_are_equal(const struct board *brd1, const struct board *brd2)
{

    for (int i = 0; i < NUM_PIECES; i++) {
        assert(get_bitboard(brd1, i) == get_bitboard(brd2, i));
    }

    assert(get_bitboard_all_pieces(brd1) == get_bitboard_all_pieces(brd2));

    assert(get_side_to_move(brd1)  == get_side_to_move(brd2));

    assert(get_en_passant_sq(brd1) == get_en_passant_sq(brd2));

    assert(get_fifty_move_counter(brd1) == get_fifty_move_counter(brd2));

    assert(get_ply(brd1) == get_ply(brd2));
    assert(get_history_ply(brd1) == get_history_ply(brd2));

    assert(get_material_value(brd1, WHITE) == get_material_value(brd2, WHITE));
    assert(get_material_value(brd1, BLACK) == get_material_value(brd2, BLACK));

    for (int i = 0; i < NUM_SQUARES; i++) {
        assert(get_piece_on_square(brd1, i) == get_piece_on_square(brd2, i));
    }

    assert(get_castle_permissions(brd1) == get_castle_permissions(brd2));

    // already verified that brd1->history_ply == brd2_history_ply
    for (int i = 0; i < brd1->history_ply; i++) {
        assert(brd1->history[i].move == brd2->history[i].move);
        assert(brd1->history[i].fifty_move_counter ==
               brd2->history[i].fifty_move_counter);
        assert(brd1->history[i].castle_perm ==
               brd2->history[i].castle_perm);
        assert(brd1->history[i].board_hash ==
               brd2->history[i].board_hash);
        assert(brd1->history[i].en_passant ==
               brd2->history[i].en_passant);
    }

    assert(get_board_hash(brd1) == get_board_hash(brd2));

}


/*
 * Clones the given board. Returns malloc'ed memory that needs to be free'd
 * name: clone_board
 * @param
 * @return
 *
 */
void clone_board(const struct board *board_to_clone, struct board *cloned)
{
    memcpy(cloned, board_to_clone, sizeof(struct board));

}




inline bool is_piece_on_square(const struct board *brd, enum piece pce, enum square sq)
{
    enum piece on_board = brd->pieces[sq];
    return (pce == on_board);
}


inline uint64_t square_to_bitboard(enum square sq)
{
    uint64_t retval = 0;
    set_bit(&retval, sq);
    return retval;
}

uint64_t get_bitboard(const struct board *brd, enum piece pce){
	return brd->bitboards.pieces[pce];
}

uint64_t get_bitboard_for_king(const struct board *brd, enum colour piece_col){
	return brd->king_sq[piece_col];
}

uint64_t get_bitboard_all_pieces(const struct board *brd){
	return brd->bitboards.board;
}


bool is_pawn_controlling_sq(const struct board *brd, enum colour col, enum square sq){
	return brd->pawn_control[col][sq] > 0; 
}

uint64_t get_bitboard_combined(const struct board *brd, enum piece pce_1, enum piece pce_2){
	return brd->bitboards.pieces[pce_1] | brd->bitboards.pieces[pce_2];
}


int32_t get_material_value(const struct board *brd, enum colour col){
	return (int32_t)brd->material[col];
}

enum colour get_side_to_move(const struct board *brd){
	return brd->side_to_move;
}

void set_side_to_move(struct board *brd, enum colour side){
	brd->side_to_move = side;
}

void set_castle_permission(struct board *brd, enum castle_perm perm){
	brd->castle_perm |= (uint8_t)perm;
}

enum castle_perm get_castle_permissions(const struct board *brd){
	return brd->castle_perm;
}



enum square get_king_square(const struct board *brd, enum colour col){
	return brd->king_sq[col];
}


inline bool is_square_occupied(uint64_t bitboard, enum square sq){
	return ((bitboard >> sq) & 0x01ull) != 0;
}

inline uint8_t get_rank(enum square sq){
	return (uint8_t)(sq >> 3);
}

inline uint8_t get_file(enum square sq){
	return (uint8_t)(sq % 8);
}

inline enum square get_square(uint8_t rank, uint8_t file){
	return (enum square)((rank << 3) + file);
}



void push_history(struct board *brd, mv_bitmap move){
    // set up history
    brd->history[brd->history_ply].move = move;
    brd->history[brd->history_ply].fifty_move_counter = brd->fifty_move_counter;
    brd->history[brd->history_ply].en_passant = brd->en_passant;
    brd->history[brd->history_ply].castle_perm = brd->castle_perm;
    brd->history[brd->history_ply].board_hash = brd->board_hash;

    brd->ply++;
    brd->history_ply++;
}

mv_bitmap pop_history(struct board *brd){

    brd->ply--;
    brd->history_ply--;

    brd->fifty_move_counter = brd->history[brd->history_ply].fifty_move_counter;
    brd->en_passant = brd->history[brd->history_ply].en_passant;
    brd->castle_perm = brd->history[brd->history_ply].castle_perm;
    brd->board_hash = brd->history[brd->history_ply].board_hash;

	return brd->history[brd->history_ply].move;
}


void move_piece(struct board *brd, enum square from, enum square to)
{	
    enum piece pce = brd->pieces[from];

    // adjust the hash
    brd->board_hash ^= get_piece_hash(pce, from);
    brd->board_hash ^= get_piece_hash(pce, to);

    brd->pieces[from] = NO_PIECE;
    clear_bit(&brd->bitboards.pieces[pce], from);
    clear_bit(&brd->bitboards.board, from);

    // set up the "to" resources
    brd->pieces[to] = pce;
    set_bit(&brd->bitboards.pieces[pce], to);
    set_bit(&brd->bitboards.board, to);

	switch(pce){
		case W_PAWN:
            // easiest way to move a pawn
            remove_white_pawn_info(brd, from);
            add_white_pawn_info(brd, to);
			break;
		case B_PAWN:
            // easiest way to move a pawn
            remove_black_pawn_info(brd, from);
            add_black_pawn_info(brd, to);
            break;
		case W_KING:
            brd->king_sq[WHITE] = to;
			break;
		case B_KING:
			brd->king_sq[BLACK] = to;
			break;
		default:
			break;
    }
}



void add_piece_to_board(struct board *brd, enum piece pce, enum square sq)
{
    enum colour col = GET_COLOUR(pce);
    brd->board_hash ^= get_piece_hash(pce, sq);

    brd->pieces[sq] = pce;
    brd->material[col] += GET_PIECE_VALUE(pce);

    // set piece on bitboards
    set_bit(&brd->bitboards.pieces[pce], sq);
    set_bit(&brd->bitboards.board, sq);

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

// checks to see if most recent move is a repetition
inline bool is_repetition(const struct board *brd)
{
    // the 50move counter is reset when a pawn moves or piece taken
    // (these are moves that can't be repeated), so we only need to
    // search the history from the last time the counter was reset

    int start = brd->history_ply - brd->fifty_move_counter;

    for (int i = start; i < brd->history_ply-1; i++) {
        if (brd->board_hash == brd->history[i].board_hash) {
            return true;
        }
    }
    return false;
}




void remove_piece_from_board(struct board *brd, enum piece pce_to_remove, enum square sq)
{
    enum colour col = GET_COLOUR(pce_to_remove);
    brd->board_hash ^= get_piece_hash(pce_to_remove, sq);
    brd->pieces[sq] = NO_PIECE;
    brd->material[col] -= GET_PIECE_VALUE(pce_to_remove);

    // remove piece from bitboards
    clear_bit(&brd->bitboards.pieces[pce_to_remove], sq);
    clear_bit(&brd->bitboards.board, sq);

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
}




static inline void remove_black_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

    brd->pawns_on_file[BLACK][file]--;
    brd->pawns_on_rank[BLACK][rank]--;

    update_pawn_control(brd, BLACK, sq, -1);
}



static inline void remove_white_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

    brd->pawns_on_file[WHITE][file]--;
    brd->pawns_on_rank[WHITE][rank]--;

    update_pawn_control(brd, WHITE, sq, -1);

}




// adds a pawn to the underlying board struct
static inline void add_black_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

    brd->pawns_on_file[BLACK][file]++;
    brd->pawns_on_rank[BLACK][rank]++;

    update_pawn_control(brd, BLACK, sq, 1);
}



// adds a pawn to the underlying board struct
static inline void add_white_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

    brd->pawns_on_file[WHITE][file]++;
    brd->pawns_on_rank[WHITE][rank]++;

    update_pawn_control(brd, WHITE, sq, 1);

}


static inline void update_pawn_control(struct board* brd, const enum colour col, const enum square sq, int8_t val)
{
    int8_t next_sq = 0;
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

	if (col == WHITE){
		if (file > FILE_A) {
			if (rank < RANK_8) {
				next_sq = (int8_t)(sq + NW);				
				brd->pawn_control[col][next_sq] += val;
			}
		} 
		if (file < FILE_H) {
			if (rank < RANK_8) {
				next_sq = (int8_t)(sq + NE);
				brd->pawn_control[col][next_sq] += val;
			}
		}
	} else {
		if (file > FILE_A) {
			if (rank > RANK_1) {
				next_sq = (int8_t)(sq + SW);
				brd->pawn_control[col][next_sq] += val;
			}
		} 
		if (file < FILE_H) {
			if (rank > RANK_1) {
				next_sq = (int8_t)(sq + SE);
				brd->pawn_control[col][next_sq] += val;
			}
		}
	}
}


static void assert_board_and_move(struct board *brd, mv_bitmap mv){
    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);
	
	printf("VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n");	
	print_board(brd);
	ASSERT_BOARD_OK(brd);
	printf("UHUHUH - ");
	print_move_details(mv);
	printf("castle move = %d\n", IS_CASTLE_MOVE(mv));
	printf("en pass move = %d\n", IS_EN_PASS_MOVE(mv));
	printf("capture move = %d\n", IS_CAPTURE_MOVE(mv));

	printf("from sq %s\n", print_square(from));
	printf("to sq %s\n", print_square(to));
	printf("pce on from : %c\n", get_piece_label(get_piece_on_square(brd, from)));
	if (IS_CAPTURE_MOVE(mv)){
		printf("pce on to : %c\n", get_piece_label(get_piece_on_square(brd, to)));
	} else{
		printf("pce on to : - \n");
	}

}


// return false if move is invalid, true otherwise
bool make_move(struct board *brd, mv_bitmap mv)
{	
    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);
	
    enum piece pce_being_moved = get_piece_on_square(brd, from);
    enum colour side = get_side_to_move(brd);

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

    move_piece(brd, from, to);

    enum piece promoted = PROMOTED_PCE(mv);
    if (promoted != NO_PIECE) {
        enum piece capt = brd->pieces[to];
        remove_piece_from_board(brd, capt, to);
        add_piece_to_board(brd, promoted, to);
    }

    // flip side
    flip_sides(brd);

    // check if move is valid (ie, king in check)
    enum square king_sq = brd->king_sq[side];

    // side is already flipped above, so use that as the attacking side
    if (is_sq_attacked(brd, king_sq, brd->side_to_move)) {
        take_move(brd);
        return false;
    } else {
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



/*
 * Counts set bits in a uint64_t
 * name: count_bits
 * @param 	the board
 * @return	the number of set bits
 *
 */
inline uint8_t count_bits(uint64_t bb)
{
    return (uint8_t) __builtin_popcountll(bb);
}
