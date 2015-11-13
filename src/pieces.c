/*
 * pieces.c
 * 
 * ---------------------------------------------------------------------
 * DESCRIPTION: code associated with pieces on the board
 * ---------------------------------------------------------------------
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
#include <ctype.h>
#include "types.h"
#include "board.h"
#include "pieces.h"

// offsets into this array are by type enum piece
static const char pieceToChar[NUM_PIECES] = {
	'P', 'p', 'B', 'b', 'N', 'n', 'R', 'r', 'Q', 'q', 'K', 'k'
};


/**
 * Takes a typed piece and returns a human-readable representation
 */
inline char get_piece_label(enum piece pce)
{
	return pieceToChar[pce];
}

inline U32 get_piece_value(const enum piece pce)
{
	return piece_values[pce];
}

