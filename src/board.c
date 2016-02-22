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

static void get_clean_board(struct board *brd);

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
}

/*
 * Creates an empty board struct
 * name: get_clean_board
 * @param
 * @return	ptr to a created board struct
 *
 */
static void get_clean_board(struct board *brd)
{
	memset(brd, 0, sizeof(struct board));

	for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		brd->pieces[sq] = NO_PIECE;
	}
}

// returns the count.
uint8_t populate_pv_line(struct board *brd, uint8_t depth){

	mv_bitmap mv = probe_tt(brd->board_hash);

	uint8_t count = 0;

	while((mv != NO_MOVE) && (count < depth)){
		
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



inline uint64_t overlay_white_pieces(const struct board * brd)
{
	return brd->bitboards[W_PAWN] | brd->bitboards[W_BISHOP]
	    | brd->bitboards[W_KNIGHT] | brd->bitboards[W_ROOK]
	    | brd->bitboards[W_QUEEN] | brd->bitboards[W_KING];
}

inline uint64_t overlay_black_pieces(const struct board * brd)
{
	return brd->bitboards[B_PAWN] | brd->bitboards[B_BISHOP]
	    | brd->bitboards[B_KNIGHT] | brd->bitboards[B_ROOK]
	    | brd->bitboards[B_QUEEN] | brd->bitboards[B_KING];
}


inline bool is_piece_on_square(const struct board *brd, enum piece pce, enum square sq){
	enum piece on_board = brd->pieces[sq];
	return (pce == on_board);
}


inline uint64_t square_to_bitboard(enum square sq)
{
	uint64_t retval = 0;
	set_bit(&retval, sq);
	return retval;
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
