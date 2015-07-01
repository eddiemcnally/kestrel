/*
 * pieces.c
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chess is distributed in the hope that it will be useful, but
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


// offsets into this array are by type piece_id_t 
const char pieceToChar[NUM_PIECE_TYPES] = {
    'P', 'R', 'N', 'B', 'Q', 'K', 'p', 'r', 'n', 'b', 'q', 'k'
};


/**
 * Takes a typed piece and returns a human-readable representation
 */
inline char get_piece_label(piece_id_t piece)
{
    return pieceToChar[piece];
}


inline colour_t get_colour(piece_id_t piece){
	if (IS_WHITE(piece))
		return WHITE;
	return BLACK;	
}

