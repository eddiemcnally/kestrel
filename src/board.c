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
#include <string.h>
#include <assert.h>
#include "types.h"
#include "fen.h"
#include "board.h"
#include "move.h"
#include "board_utils.h"
#include "pv_table.h"
#include "makemove.h"
#include "hashkeys.h"
#include "pieces.h"

static struct board *get_clean_board(void);

/*
 * Creates and initialises a new board. The default starting piece
 * positions are populated.
 * name: init_board
 * @param fen : the FEN string
 * @return	a new board
 *
 */
struct board *init_board(char *fen)
{
	struct board *brd = get_clean_board();
	consume_fen_notation(fen, brd);
	return brd;
}

/*
 * Creates an empty board struct
 * name: get_clean_board
 * @param
 * @return	ptr to a created board struct
 *
 */
static struct board *get_clean_board(void)
{
	struct board *brd = malloc(sizeof(struct board));
	memset(brd, 0, sizeof(struct board));

	for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		brd->pieces[sq] = NO_PIECE;
	}

	brd->pvtable = create_pv_table();

	return brd;
}

inline U64 overlay_white_pieces(const struct board * brd)
{
	return brd->bitboards[W_PAWN] | brd->bitboards[W_BISHOP]
	    | brd->bitboards[W_KNIGHT] | brd->bitboards[W_ROOK]
	    | brd->bitboards[W_QUEEN] | brd->bitboards[W_KING];
}

inline U64 overlay_black_pieces(const struct board * brd)
{
	return brd->bitboards[B_PAWN] | brd->bitboards[B_BISHOP]
	    | brd->bitboards[B_KNIGHT] | brd->bitboards[B_ROOK]
	    | brd->bitboards[B_QUEEN] | brd->bitboards[B_KING];
}



inline U64 square_to_bitboard(enum square sq)
{
	U64 retval = 0;
	set_bit(&retval, sq);
	return retval;
}


/*
 * Counts set bits in a U64
 * name: count_bits
 * @param 	the board
 * @return	the number of set bits
 *
 */
inline U8 count_bits(U64 bb)
{
	return (U8) __builtin_popcountll(bb);
}
