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

#define IS_WHITE(piece)			(piece <= W_KING)
#define IS_BLACK(piece)			(IS_WHITE(piece) == false)

#define FLIP_SIDE(side)			(side ^ 1)

#define	GET_COLOUR(piece)		(IS_WHITE(piece) ? WHITE : BLACK )

#define IS_BISHOP(pce)			((pce == W_BISHOP) || (pce == B_BISHOP))
#define IS_KNIGHT(pce)			((pce == W_KNIGHT) || (pce == B_KNIGHT))
#define IS_ROOK(pce)			((pce == W_ROOK) || (pce == B_ROOK))
#define IS_PAWN(pce)			((pce == W_PAWN) || (pce == B_PAWN))
#define IS_KING(pce)			((pce == W_KING) || (pce == B_KING))


char get_piece_label(enum piece piece);

bool is_sliding_piece(enum piece pce);

bool is_valid_piece(enum piece pce);

U32 get_piece_value(const enum piece pce);
#endif
