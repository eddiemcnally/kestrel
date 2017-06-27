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
#include "kestrel.h"
#include "fen/fen.h"
#include "board.h"
#include "bitboard.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "board_utils.h"
#include "tt.h"
#include "hashkeys.h"
#include "pieces.h"



static inline void add_pawn_info(struct position *pos, enum colour col, enum square sq);
static inline void remove_black_pawn_info(struct position *pos, enum square sq);
static inline void remove_white_pawn_info(struct position *pos, enum square sq);
static inline void update_pawn_control(struct position*pos, const enum colour col, const enum square sq, int8_t val);
static void init_board(struct position *pos);
static void get_clean_board(struct position *pos);
static void make_pawn_move(struct position *pos, mv_bitmap mv);
static void make_castle_move(struct position *pos, mv_bitmap mv);



static void assert_board_and_move(struct position *pos, mv_bitmap mv);


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


/**
 * A container for holding a specific position
 */
struct position {

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
    uint8_t pawns_on_file[NUM_COLOURS][NUM_FILES];
    uint8_t pawns_on_rank[NUM_COLOURS][NUM_RANKS];
    uint8_t pawn_control[NUM_COLOURS][NUM_SQUARES];


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

//////////////////////////////////////////////////////////////////
//
// 							Initialisation
//
//////////////////////////////////////////////////////////////////

struct position* allocate_board(void){
	struct position *pos  = (struct position *)malloc(sizeof(struct position));
	memset(pos, 0, sizeof(struct position));
	init_board(pos);
	return pos;
}

void free_board(struct position *pos){
	free(pos);
}




/*
 * Creates and initialises a new board. The default starting piece
 * positions are populated.
 * name: init_board
 * @param fen : the FEN string
 * @return	a new board
 *
 */
static void init_board(struct position *pos)
{
    get_clean_board(pos);

    init_hash_keys();
    init_move_gen_framework();
    init_attack_framework();

}


/*
 * Creates an empty board struct
 * name: get_clean_board
 * @param
 * @return	ptr to a created board struct
 *
 */
static void get_clean_board(struct position *pos)
{

    for(uint8_t i = 0; i < NUM_SQUARES; i++) {
        pos->pieces[i] = NO_PIECE;
    }

    pos->king_sq[WHITE] = NO_SQUARE;
    pos->king_sq[BLACK] = NO_SQUARE;

    pos->side_to_move = WHITE;
    pos->en_passant = NO_SQUARE;

    for(uint16_t i = 0; i < MAX_SEARCH_DEPTH; i++) {
        pos->pv_line[i] = NO_MOVE;
    }

    for(uint16_t i = 0; i < MAX_GAME_MOVES; i++) {
        pos->history[i].move = NO_MOVE;
        pos->history[i].en_passant = NO_SQUARE;
        // other struct values are already set to zero with memset
    }

    set_castle_permission(pos, CASTLE_PERM_NONE);
}


void init_search_history(struct position *pos){

    for(int i = 0; i < NUM_PIECES; i++) {
        for(int j = 0; j < NUM_SQUARES; j++) {
            pos->search_history[i][j] = NO_MOVE;
        }
    }
}


void init_search_killers(struct position *pos){

    for(int i = 0; i < NUM_KILLER_MOVES; i++) {
        for(int j = 0; j < MAX_SEARCH_DEPTH; j++) {
            pos->search_killers[i][j] = NO_MOVE;
        }
    }
}

///////////////////////////////////////////////////////////////////////






// returns the count.
uint8_t populate_pv_line(struct position *pos, uint8_t depth)
{

    mv_bitmap mv = probe_tt(pos->board_hash);

    uint8_t count = 0;

    while((mv != NO_MOVE) && (count < depth)) {

        //assert(count < MAX_SEARCH_DEPTH);

        make_move(pos, mv);
        pos->pv_line[count++] = mv;

        mv = probe_tt(pos->board_hash);
    }

    // rollback moves
    while(pos->ply > 0) {
        take_move(pos);
    }

    return count;
}


mv_bitmap get_best_pvline(const struct position *pos){
	return get_pvline(pos, 0);
}

mv_bitmap get_pvline(const struct position *pos, uint8_t search_depth){
	return pos->pv_line[search_depth];
}

void set_pvline(struct position *pos, uint8_t search_depth, mv_bitmap move){
	pos->pv_line[search_depth] = move;
}


void update_board_hash(struct position *pos){
	pos->board_hash = get_position_hash(pos);
}

uint64_t get_board_hash(const struct position *pos){
	return pos->board_hash;
}

enum piece get_piece_on_square(const struct position *pos, enum square sq){
	return pos->pieces[sq];
}

void set_en_passant_sq(struct position *pos, enum square sq){
	pos->en_passant = sq;
}

enum square get_en_passant_sq(const struct position *pos){
	return pos->en_passant;
}




uint8_t get_fifty_move_counter(const struct position *pos){
	return pos->fifty_move_counter;
}

uint8_t get_ply(const struct position *pos){
	return pos->ply;
}

void set_ply(struct position *pos, uint8_t ply){
	pos->ply = ply;
}

uint8_t get_history_ply(const struct position *pos){
	return pos->history_ply;
}
void set_history_ply(struct position *pos, uint8_t hist_ply){
	pos->history_ply = hist_ply;
}

const struct bitboards * get_bitboard_struct(const struct position *pos){
	return &pos->bitboards;
}



void shuffle_search_killers(struct position *pos, mv_bitmap mv){

	pos->search_killers[1][pos->ply] = pos->search_killers[0][pos->ply];
    pos->search_killers[0][pos->ply] = mv;

}

void add_to_search_history(struct position *pos, enum piece pce, enum square to_sq, uint8_t depth){
	pos->search_history[pce][to_sq] += depth;
}


/* Compares two boards and checks for equality
 *
 * name: assert_boards_are_equal
 * @param
 * @return
 *
 */
void assert_boards_are_equal(const struct position *pos1, const struct position *pos2)
{
	// check the bitboards
	assert(bitboard_stucts_are_same(&pos1->bitboards, &pos2->bitboards));

    assert(get_side_to_move(pos1)  == get_side_to_move(pos2));

    assert(get_en_passant_sq(pos1) == get_en_passant_sq(pos2));

    assert(get_fifty_move_counter(pos1) == get_fifty_move_counter(pos2));

    assert(get_ply(pos1) == get_ply(pos2));
    assert(get_history_ply(pos1) == get_history_ply(pos2));

    assert(get_material_value(pos1, WHITE) == get_material_value(pos2, WHITE));
    assert(get_material_value(pos1, BLACK) == get_material_value(pos2, BLACK));

    for (enum square sq = a1; sq < NUM_SQUARES; sq++) {
        assert(get_piece_on_square(pos1, sq) == get_piece_on_square(pos2, sq));
    }

    assert(get_castle_permissions(pos1) == get_castle_permissions(pos2));

    // already verified that pos1->history_ply == brd2_history_ply
    for (int i = 0; i < pos1->history_ply; i++) {
        assert(pos1->history[i].move == pos2->history[i].move);
        assert(pos1->history[i].fifty_move_counter ==
               pos2->history[i].fifty_move_counter);
        assert(pos1->history[i].castle_perm ==
               pos2->history[i].castle_perm);
        assert(pos1->history[i].board_hash ==
               pos2->history[i].board_hash);
        assert(pos1->history[i].en_passant ==
               pos2->history[i].en_passant);
    }

    assert(get_board_hash(pos1) == get_board_hash(pos2));

}


mv_bitmap get_search_killer(struct position *pos, uint8_t killer_move_num, uint8_t ply){
	return pos->search_killers[killer_move_num][ply];
}

uint32_t get_search_history(struct position *pos, enum piece pce, enum square sq){
	return pos->search_history[pce][sq];
}


/*
 * Clones the given board. Returns malloc'ed memory that needs to be free'd
 * name: clone_board
 * @param
 * @return
 *
 */
void clone_board(const struct position *board_to_clone, struct position *cloned)
{
    memcpy(cloned, board_to_clone, sizeof(struct position));

}




inline bool is_piece_on_square(const struct position *pos, enum piece pce, enum square sq)
{
    enum piece on_board = pos->pieces[sq];
    return (pce == on_board);
}



bool is_pawn_controlling_sq(const struct position *pos, enum colour col, enum square sq){
	return pos->pawn_control[col][sq] > 0;
}


int32_t get_material_value(const struct position *pos, enum colour col){
	return (int32_t)pos->material[col];
}

enum colour get_side_to_move(const struct position *pos){
	return pos->side_to_move;
}

void set_side_to_move(struct position *pos, enum colour side){
	pos->side_to_move = side;
}

void set_castle_permission(struct position *pos, enum castle_perm perm){
	pos->castle_perm |= (uint8_t)perm;
}

enum castle_perm get_castle_permissions(const struct position *pos){
	return pos->castle_perm;
}



enum square get_king_square(const struct position *pos, enum colour col){
	return pos->king_sq[col];
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

inline enum square get_square(enum rank r, enum file f){
	return (enum square)((r << 3) + f);
}


uint8_t get_num_pawns_on_rank(const struct position *pos, enum colour col, enum rank rank){
	return pos->pawns_on_rank[col][rank];
}

uint8_t get_num_pawns_on_file(const struct position *pos, enum colour col, enum file file){
	return pos->pawns_on_file[col][file];
}

uint8_t get_num_squares_under_pawn_ctl(const struct position *pos, enum colour col, enum square sq){
	return pos->pawn_control[col][sq];
}


void push_history(struct position *pos, mv_bitmap move){
    // set up history
    pos->history[pos->history_ply].move = move;
    pos->history[pos->history_ply].fifty_move_counter = pos->fifty_move_counter;
    pos->history[pos->history_ply].en_passant = pos->en_passant;
    pos->history[pos->history_ply].castle_perm = pos->castle_perm;
    pos->history[pos->history_ply].board_hash = pos->board_hash;

    pos->ply++;
    pos->history_ply++;
}

mv_bitmap pop_history(struct position *pos){

    pos->ply--;
    pos->history_ply--;

    pos->fifty_move_counter = pos->history[pos->history_ply].fifty_move_counter;
    pos->en_passant = pos->history[pos->history_ply].en_passant;
    pos->castle_perm = pos->history[pos->history_ply].castle_perm;
    pos->board_hash = pos->history[pos->history_ply].board_hash;

	return pos->history[pos->history_ply].move;
}


void move_piece(struct position *pos, enum square from, enum square to)
{
    enum piece pce = pos->pieces[from];

    // adjust the hash
    pos->board_hash ^= get_piece_hash(pce, from);
    pos->board_hash ^= get_piece_hash(pce, to);

    pos->pieces[from] = NO_PIECE;
    pos->pieces[to] = pce;

	// adjust bitboards
	remove_piece_from_bitboards(&pos->bitboards, pce, from);
	add_piece_to_bitboards(&pos->bitboards, pce, to);

	switch(pce){
		case W_PAWN:
            // easiest way to move a pawn
            remove_white_pawn_info(pos, from);
            add_pawn_info(pos, WHITE, to);
			break;
		case B_PAWN:
            // easiest way to move a pawn
            remove_black_pawn_info(pos, from);
            add_pawn_info(pos, BLACK, to);
            break;
		case W_KING:
            pos->king_sq[WHITE] = to;
			break;
		case B_KING:
			pos->king_sq[BLACK] = to;
			break;
		default:
			break;
    }
}



void add_piece_to_board(struct position *pos, enum piece pce, enum square sq)
{
    enum colour col = GET_COLOUR(pce);
    pos->board_hash ^= get_piece_hash(pce, sq);

    pos->pieces[sq] = pce;
    pos->material[col] += GET_PIECE_VALUE(pce);

    // set piece on bitboards
    add_piece_to_bitboards(&pos->bitboards, pce, sq);

    switch (pce) {
    case W_PAWN:
        add_pawn_info(pos, WHITE, sq);
        break;
    case B_PAWN:
        add_pawn_info(pos, BLACK, sq);
        break;
    case W_KING:
    case B_KING:
        pos->king_sq[col] = sq;
        break;
    default:
        break;
    }

}

// checks to see if most recent move is a repetition
inline bool is_repetition(const struct position *pos)
{
    // the 50move counter is reset when a pawn moves or piece taken
    // (these are moves that can't be repeated), so we only need to
    // search the history from the last time the counter was reset

    int start = pos->history_ply - pos->fifty_move_counter;

    for (int i = start; i < pos->history_ply-1; i++) {
        if (pos->board_hash == pos->history[i].board_hash) {
            return true;
        }
    }
    return false;
}




void remove_piece_from_board(struct position *pos, enum piece pce_to_remove, enum square sq)
{
    enum colour col = GET_COLOUR(pce_to_remove);
    pos->board_hash ^= get_piece_hash(pce_to_remove, sq);
    pos->pieces[sq] = NO_PIECE;
    pos->material[col] -= GET_PIECE_VALUE(pce_to_remove);

    // remove piece from bitboards
    remove_piece_from_bitboards(&pos->bitboards, pce_to_remove, sq);

    switch (pce_to_remove) {
    case W_PAWN:
        remove_white_pawn_info(pos, sq);
        break;
    case B_PAWN:
        remove_black_pawn_info(pos, sq);
        break;
    case W_KING:
    case B_KING:
        pos->king_sq[col] = NO_SQUARE;
        break;
    default:
        break;
    }
}




static inline void remove_black_pawn_info(struct position *pos, enum square sq)
{
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

    pos->pawns_on_file[BLACK][file]--;
    pos->pawns_on_rank[BLACK][rank]--;

    update_pawn_control(pos, BLACK, sq, -1);
}



static inline void remove_white_pawn_info(struct position *pos, enum square sq)
{
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

    pos->pawns_on_file[WHITE][file]--;
    pos->pawns_on_rank[WHITE][rank]--;

    update_pawn_control(pos, WHITE, sq, -1);

}






// adds a pawn to the underlying board struct
static inline void add_pawn_info(struct position *pos, enum colour col, enum square sq)
{
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

    pos->pawns_on_file[col][file]++;
    pos->pawns_on_rank[col][rank]++;

    update_pawn_control(pos, col, sq, 1);
}


static inline void update_pawn_control(struct position *pos, const enum colour col, const enum square sq, int8_t val)
{
    int8_t next_sq = 0;
    uint8_t file = get_file(sq);
    uint8_t rank = get_rank(sq);

	if (col == WHITE){
		if (file > FILE_A) {
			if (rank < RANK_8) {
				next_sq = (int8_t)(sq + NW);
				pos->pawn_control[col][next_sq] += val;
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


static void assert_board_and_move(struct position *pos, mv_bitmap mv){
    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);


	print_board(pos);
	ASSERT_BOARD_OK(pos);
	print_move_details(mv);
	printf("castle move = %d\n", IS_CASTLE_MOVE(mv));
	printf("en pass move = %d\n", IS_EN_PASS_MOVE(mv));
	printf("capture move = %d\n", IS_CAPTURE_MOVE(mv));

	printf("from sq %s\n", print_square(from));
	printf("to sq %s\n", print_square(to));
	printf("pce on from : %c\n", get_piece_label(get_piece_on_square(pos, from)));
	if (IS_CAPTURE_MOVE(mv)){
		printf("pce on to : %c\n", get_piece_label(get_piece_on_square(pos, to)));
	} else{
		printf("pce on to : - \n");
	}

}


// return false if move is invalid, true otherwise
bool make_move(struct position *pos, mv_bitmap mv)
{
    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);

    enum piece pce_being_moved = get_piece_on_square(pos, from);
    enum colour side = get_side_to_move(pos);

    push_history(pos, mv);

    if (IS_PAWN(pce_being_moved)){
        make_pawn_move(pos, mv);
    }

    if (IS_CASTLE_MOVE(mv)) {
        make_castle_move(pos, mv);
    }


    pos->en_passant = NO_SQUARE;
    pos->fifty_move_counter++;

    if (IS_CAPTURE_MOVE(mv)) {
        enum piece capt = pos->pieces[to];
        remove_piece_from_board(pos, capt, to);
        pos->fifty_move_counter = 0;
    }

    pos->ply++;
    pos->history_ply++;


    move_piece(pos, from, to);


    // flip side
    flip_sides(pos);

    // check if move is valid (ie, king in check)
    enum square king_sq = pos->king_sq[side];

    // side is already flipped above, so use that as the attacking side
    if (is_sq_attacked(pos, king_sq, pos->side_to_move)) {
        take_move(pos);
        return false;
    } else {
        return true;
    }
}


static void make_castle_move(struct position *pos, mv_bitmap mv){

    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);

    // hash out existing castle permissions
    pos->board_hash ^= get_castle_hash(pos->castle_perm);

    switch (to) {
    case c1:
        move_piece(pos, a1, d1);
        break;
    case c8:
        move_piece(pos, a8, d8);
        break;
    case g1:
        move_piece(pos, h1, f1);
        break;
    case g8:
        move_piece(pos, h8, f8);
        break;
    default:
        printf("to : %s\n", print_square(to));
        assert(false);
        break;
    }
    // re-hash back in the new castle permissions
    pos->castle_perm &= castle_permission_mask[from];
    pos->castle_perm &= castle_permission_mask[to];
    pos->board_hash ^= get_castle_hash(pos->castle_perm);
}


static void make_pawn_move(struct position *pos, mv_bitmap mv){

    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);
    enum colour side = get_side_to_move(pos);

    pos->fifty_move_counter = 0;

    if (IS_EN_PASS_MOVE(mv)) {
        if (side == WHITE) {
	        // must be a bp
            remove_piece_from_board(pos, B_PAWN, to - 8);
        } else {
            // must be a wp
            remove_piece_from_board(pos, W_PAWN, to + 8);
        }
    }

    if (IS_PAWN_START(mv)) {
        if (side == WHITE) {
            pos->en_passant = from + 8;
        } else {
            pos->en_passant = from - 8;
        }
        pos->board_hash ^= get_en_passant_hash(pos->en_passant);
    }
    enum piece promoted = PROMOTED_PCE(mv);
    if (promoted != NO_PIECE) {
        enum piece capt = pos->pieces[to];
        remove_piece_from_board(pos, capt, to);
        add_piece_to_board(pos, promoted, to);
    }
}




inline void take_move(struct position *pos)
{
    pos->history_ply--;
    pos->ply--;

    mv_bitmap mv = pos->history[pos->history_ply].move;

    // note: when reverting, the 'from' square will be empty and the 'to'
    // square has the piece in it.
    enum square from = FROMSQ(mv);
    enum square to = TOSQ(mv);

    // hash out en passant and castle if set
    if (pos->en_passant != NO_SQUARE) {
        pos->board_hash ^= get_en_passant_hash(pos->en_passant);
    }

    pos->board_hash ^= get_castle_hash(pos->castle_perm);

    pos->castle_perm = pos->history[pos->history_ply].castle_perm;
    pos->fifty_move_counter = pos->history[pos->history_ply].fifty_move_counter;
    pos->en_passant = pos->history[pos->history_ply].en_passant;

    // now, hash back in
    if (pos->en_passant != NO_SQUARE) {
        pos->board_hash ^= get_en_passant_hash(pos->en_passant);
    }
    pos->board_hash ^= get_castle_hash(pos->castle_perm);

    // flip side
    flip_sides(pos);

    if (MFLAG_EN_PASSANT & mv) {
        if (pos->side_to_move == WHITE) {
            add_piece_to_board(pos, B_PAWN, to - 8);
        } else {
            add_piece_to_board(pos, W_PAWN, to + 8);
        }
    } else if (MFLAG_CASTLE & mv) {
        switch (to) {
        case c1:
            move_piece(pos, d1, a1);
            break;
        case c8:
            move_piece(pos, d8, a8);
            break;
        case g1:
            move_piece(pos, f1, h1);
            break;
        case g8:
            move_piece(pos, f8, h8);
            break;
        default:
            assert(false);
            break;
        }
    }
    // note: to revert move, move piece from 'to' to 'from'
    move_piece(pos, to, from);

    enum piece captured = CAPTURED_PCE(mv);
    if (captured != NO_PIECE) {
        add_piece_to_board(pos, captured, to);
    }

    enum piece promoted = PROMOTED_PCE(mv);
    if (PROMOTED_PCE(mv) != NO_PIECE) {
        enum colour prom_col = GET_COLOUR(promoted);
        remove_piece_from_board(pos, promoted, from);

        enum piece pce_to_add = (prom_col == WHITE) ? W_PAWN : B_PAWN;
        add_piece_to_board(pos, pce_to_add, from);
    }
}





inline void flip_sides(struct position *pos)
{
    // flip side
    pos->side_to_move = GET_OPPOSITE_SIDE(pos->side_to_move);
    pos->board_hash ^= get_side_hash();
}

