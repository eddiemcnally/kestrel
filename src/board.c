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
#include "types.h"
#include "fen.h"
#include "board.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "board_utils.h"
#include "tt.h"
#include "makemove.h"
#include "hashkeys.h"
#include "pieces.h"



static inline void add_black_pawn_info(struct board *brd, enum square sq);
static inline void add_white_pawn_info(struct board *brd, enum square sq);
static inline void remove_black_pawn_info(struct board *brd, enum square sq);
static inline void remove_white_pawn_info(struct board *brd, enum square sq);
static inline void update_pawn_control(struct board* brd, const enum colour col, const enum square sq, int8_t val);


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


mv_bitmap get_best_pvline(struct board *brd){
	return get_pvline(brd, 0);
}

mv_bitmap get_pvline(struct board *brd, uint8_t search_depth){
	return brd->pv_line[search_depth];
}

void set_pvline(struct board *brd, uint8_t search_depth, mv_bitmap move){
	brd->pv_line[search_depth] = move;
}



inline uint64_t overlay_white_piece_bitboards(const struct board * brd)
{
    return brd->bitboards[W_PAWN] | brd->bitboards[W_BISHOP]
           | brd->bitboards[W_KNIGHT] | brd->bitboards[W_ROOK]
           | brd->bitboards[W_QUEEN] | brd->bitboards[W_KING];
}

inline uint64_t overlay_black_piece_bitboards(const struct board * brd)
{
    return brd->bitboards[B_PAWN] | brd->bitboards[B_BISHOP]
           | brd->bitboards[B_KNIGHT] | brd->bitboards[B_ROOK]
           | brd->bitboards[B_QUEEN] | brd->bitboards[B_KING];
}


void update_board_hash(struct board *brd){
	brd->board_hash = get_position_hash(brd);
}

uint64_t get_board_hash(struct board *brd){
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




uint8_t get_fifty_move_counter(struct board *brd){
	return brd->fifty_move_counter;
}

uint8_t get_ply(struct board *brd){
	return brd->ply;
}

void set_ply(struct board *brd, uint8_t ply){
	brd->ply = ply;
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
	return brd->bitboards[pce];
}

uint64_t get_bitboard_for_king(const struct board *brd, enum colour piece_col){
	return brd->king_sq[piece_col];
}

uint64_t get_bitboard_all_pieces(const struct board *brd){
	return brd->board;
}


bool is_pawn_controlling_sq(const struct board *brd, enum colour col, enum square sq){
	return brd->pawn_control[col][sq] > 0; 
}

uint64_t get_bitboard_combined(const struct board *brd, enum piece pce_1, enum piece pce_2){
	return brd->bitboards[pce_1] | brd->bitboards[pce_2];
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

#ifdef ENABLE_ASSERTS
	assert(pce != NO_PIECE);
	printf("------------_> pce = %c\n", get_piece_label(pce));
	assert(from != to);
	ASSERT_BOARD_OK(brd);
	assert_material_correct(brd);
#endif
	
    // adjust the hash
    brd->board_hash ^= get_piece_hash(pce, from);
    brd->board_hash ^= get_piece_hash(pce, to);

    brd->pieces[from] = NO_PIECE;
    clear_bit(&brd->bitboards[pce], from);
    clear_bit(&brd->board, from);

    // set up the "to" resources
    brd->pieces[to] = pce;
    set_bit(&brd->bitboards[pce], to);
    set_bit(&brd->board, to);

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

#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
	assert_material_correct(brd);
#endif
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
#ifdef ENABLE_ASSERTS
	ASSERT_BOARD_OK(brd);
    assert(pce_to_remove != NO_PIECE);
    assert(IS_VALID_PIECE(pce_to_remove));
	assert(IS_VALID_SQUARE(sq));
	
	enum piece pce_on_sq = brd->pieces[sq];
	assert(pce_on_sq != NO_PIECE);
	if (pce_on_sq != pce_to_remove){
		printf("*****");
	}
	
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

    update_pawn_control(brd, BLACK, sq, -1);
}



static inline void remove_white_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[WHITE][file]--;
    brd->pawns_on_rank[WHITE][rank]--;

    update_pawn_control(brd, WHITE, sq, -1);

}




// adds a pawn to the underlying board struct
static inline void add_black_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[BLACK][file]++;
    brd->pawns_on_rank[BLACK][rank]++;

    update_pawn_control(brd, BLACK, sq, 1);
}



// adds a pawn to the underlying board struct
static inline void add_white_pawn_info(struct board *brd, enum square sq)
{
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

    brd->pawns_on_file[WHITE][file]++;
    brd->pawns_on_rank[WHITE][rank]++;

    update_pawn_control(brd, WHITE, sq, 1);

}


static inline void update_pawn_control(struct board* brd, const enum colour col, const enum square sq, int8_t val)
{

    int8_t next_sq = 0;
    uint8_t file = GET_FILE(sq);
    uint8_t rank = GET_RANK(sq);

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
