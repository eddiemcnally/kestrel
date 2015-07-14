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

#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>
#include "types.h"
#include "board.h"


#define	BOARD_EMPTY	((U64)0x0ull)

#define NUM_RANKS 	8
#define NUM_FILES	8

enum {
	RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

enum {
	FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};






#define	NO_SQUARE	((enum square)(-1))

#define GET_PIECE_MASK(square)	((U64)(0x01ull << (int)(square)))


// zero-based
#define GET_RANK(square)	((square) / 8)
#define GET_FILE(square)	((square) % 8)
#define GET_SQUARE(RANK, FILE)	((RANK * 8) + FILE)

#define IS_VALID_RANK(rank)	((rank >= RANK_1) && (rank <= RANK_8))
#define IS_VALID_FILE(file)	((file >= FILE_A) && (file <= FILE_H))




// castling permissions
enum {
	WKCA = 0x01,	// white, king-side
	WQCA = 0x02,	// white, queen-side
	BKCA = 0x04,	// black, king-side
	BQCA = 0x08		// black, queen-side
};




// some shortcut macros
#define POP(bb)			pop_1st_bit(bb)
#define	CNT(bb)			count_bits(bb)




struct board * init_board(void);
struct board *get_clean_board(void);
enum piece get_piece_at_square(struct board * the_board, enum square square);
bool add_piece_to_board(struct board * board, enum piece piece, enum square square);
bool is_square_occupied(U64 board, enum square square);
void set_bit(U64 * brd, enum square sq);
void clear_bit(U64 * brd, enum square sq);
bool check_bit(U64 * brd, enum square sq);
U8 count_bits(U64 bb);
U8 pop_1st_bit(U64 *bb);
void update_piece_material(struct board * brd);


#endif
