/*
 * pieces.h
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

#ifndef _PIECES_H_
#define _PIECES_H_

#include <stdbool.h>
#include "types.h"

// white is even, black is odd
#define IS_WHITE(piece)			((piece & 0x1) == 0)
#define IS_BLACK(piece)			((piece & 0x1) == 1)

#define IS_VALID_PIECE(pce)		((pce != NO_PIECE) && (pce >= W_PAWN) && (pce <= B_KING))

#define GET_OPPOSITE_SIDE(side)	(side ^ 1)

#define	GET_COLOUR(piece)		(IS_WHITE(piece) ? WHITE : BLACK )

#define IS_BISHOP(pce)			((pce == W_BISHOP) || (pce == B_BISHOP))
#define IS_QUEEN(pce)			((pce == W_QUEEN) || (pce == B_QUEEN))
#define IS_KNIGHT(pce)			((pce == W_KNIGHT) || (pce == B_KNIGHT))
#define IS_ROOK(pce)			((pce == W_ROOK) || (pce == B_ROOK))
#define IS_PAWN(pce)			((pce == W_PAWN) || (pce == B_PAWN))
#define IS_KING(pce)			((pce == W_KING) || (pce == B_KING))

#define IS_ROOK_OR_QUEEN(pce)	(IS_ROOK(pce) || IS_QUEEN(pce))
#define IS_BISHOP_OR_QUEEN(pce)	(IS_BISHOP(pce) || IS_QUEEN(pce))

// piece values, indexed into using the enum piece enum
static const uint32_t piece_values[NUM_PIECES] = {
	100,			// W_PAWN
	100,			// B_PAWN
	325,			// W_BISHOP
	325,			// B_BISHOP
	325,			// W_KNIGHT
	325,			// B_KNIGHT
	550,			// W_ROOK
	550,			// B_ROOK
	1000,			// W_QUEEN
	1000,			// B_QUEEN
	50000,			// W_KING
	50000			// B_KING
};
#define INFINITE 	((int32_t)(30000))
#define MATE 		29000




char get_piece_label(enum piece piece);
bool is_valid_piece(enum piece pce);
uint32_t get_piece_value(const enum piece pce);


#endif
