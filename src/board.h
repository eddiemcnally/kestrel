/*
 * board.h
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

#pragma once

#include <stdbool.h>
#include "types.h"
#include "board.h"


#define	BOARD_EMPTY	((uint64_t)0x0ull)

#define NUM_RANKS 	8
#define NUM_FILES	8

enum rank {
    RANK_1 = 0, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

enum file {
    FILE_A = 0, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

#define GET_PIECE_MASK(square)		((uint64_t)(0x01ull << (int)(square)))

#define IS_VALID_RANK(rank)		((rank >= RANK_1) && (rank <= RANK_8))
#define IS_VALID_FILE(file)		((file >= FILE_A) && (file <= FILE_H))
#define IS_VALID_SQUARE(sq)		((sq >= a1) && (sq <= h8))





//              56 57 58 59 60 61 62 63
//              48 49 50 51 52 53 54 55
//              40 41 42 43 44 45 46 47
//              32 33 34 35 36 37 38 39
//              24 25 26 27 28 29 30 31
//              16 17 18 19 20 21 22 23
//              08 09 10 11 12 13 14 15
//              00 01 02 03 04 05 06 07
// board directions
enum {
    NORTH 	= 8,
    NN 		= (NORTH + NORTH),
    SOUTH 	= -8,
    SS 		= (SOUTH + SOUTH),
    EAST 	= 1,
    WEST 	= -1,
    NE 		= 9,
    SW 		= -9,
    NW 		= 7,
    SE 		= -7
};


// castling permissions
enum castle_perm {
	CASTLE_PERM_NONE = 0,
    WKCA = 0x01,		// white, king-side
    WQCA = 0x02,		// white, queen-side
    BKCA = 0x04,		// black, king-side
    BQCA = 0x08			// black, queen-side
};




void move_piece(struct board *brd, enum square from, enum square to);
void remove_piece_from_board(struct board *brd,  enum piece pce_to_remove, enum square sq);
void add_piece_to_board(struct board *brd, enum piece pce, enum square sq);
uint64_t get_bitboard(const struct board *brd, enum piece pce);
uint64_t get_bitboard_for_king(const struct board *brd, enum colour piece_col);
uint64_t get_bitboard_all_pieces(const struct board *brd);
uint64_t get_bitboard_combined(const struct board *brd, enum piece piece_1, enum piece piece_2);
int32_t get_material_value(const struct board *brd, enum colour col);
enum square get_king_square(const struct board *brd, enum colour col);

enum colour get_side_to_move(const struct board *brd);
void set_side_to_move(struct board *brd, enum colour side);

void set_castle_permission(struct board *brd, enum castle_perm perm);
enum castle_perm get_castle_permissions(const struct board *brd);

void set_en_passant_sq(struct board *brd, enum square sq);
enum square get_en_passant_sq(const struct board *brd);

void update_board_hash(struct board *brd);
uint64_t get_board_hash(struct board *brd);

enum piece get_piece_on_square(const struct board *brd, enum square sq);

void push_history(struct board *brd, mv_bitmap move);
mv_bitmap pop_history(struct board *brd);

uint8_t get_ply(struct board *brd);
void set_ply(struct board *brd, uint8_t ply);


mv_bitmap get_best_pvline(struct board *brd);
mv_bitmap get_pvline(struct board *brd, uint8_t search_depth);
void set_pvline(struct board *brd, uint8_t search_depth, mv_bitmap move);


uint8_t get_fifty_move_counter(struct board *brd);


bool is_pawn_controlling_sq(const struct board *brd, enum colour col, enum square sq);

void init_board(char *fen, struct board *brd);
void get_clean_board(struct board *brd);
uint8_t populate_pv_line(struct board *brd, uint8_t depth);
void set_piece_material(struct board *brd);
uint64_t overlay_white_piece_bitboards(const struct board * brd);
uint64_t overlay_black_piece_bitboards(const struct board * brd);
void overlay_boards(struct board *the_board);
uint8_t count_bits(uint64_t bb);
uint64_t square_to_bitboard(enum square sq);
bool is_piece_on_square(const struct board *brd, enum piece pce, enum square sq);
bool is_square_occupied(uint64_t bitboard, enum square sq);
bool is_repetition(const struct board *brd);

uint8_t get_rank(enum square sq);
uint8_t get_file(enum square sq);
enum square get_square(uint8_t rank, uint8_t file);
