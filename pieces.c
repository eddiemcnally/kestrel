/*
 * pieces.c
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
	'P', 'R', 'N', 'B', 'Q', 'K', 'p', 'r', 'n', 'b', 'q', 'k'
};


/**
 * Takes a typed piece and returns a human-readable representation
 */
inline char get_piece_label(enum piece piece)
{
	return pieceToChar[piece];
}


inline enum colour get_colour(enum piece piece){
	if (IS_WHITE(piece))
		return WHITE;
	return BLACK;	
}


inline bool is_big_piece(enum piece pce){
	switch(pce){
		case W_PAWN:
		case B_PAWN:
			return false;
		default:
			return true;
	}
}

inline bool is_major_piece(enum piece pce){
	switch(pce){
		case W_ROOK:
		case W_QUEEN:
		case B_ROOK:
		case B_QUEEN:
			return true;
		default:
			return false;
	}
}


inline bool is_minor_piece(enum piece pce){
	switch(pce){
		case W_BISHOP:
		case W_KNIGHT:
		case B_BISHOP:
		case B_KNIGHT:
			return true;
		default:
			return false;
	}
}


inline bool isRQ(enum piece pce){
	switch(pce){
		case W_ROOK:
		case W_QUEEN:
		case B_ROOK:
		case B_QUEEN:
			return true;
		default:
			return false;
	}
}


inline bool isKi(enum piece pce){
	if ( (pce == B_KING) || (pce == W_KING))
		return true;
	return false;
}



inline bool isKn(enum piece pce){
	if ( (pce == B_KNIGHT) || (pce == W_KNIGHT))
		return true;
	return false;
}


inline bool isR(enum piece pce){
	switch(pce){
		case W_ROOK:
		case B_ROOK:
			return true;
		default:
			return false;
	}
}


inline bool isB(enum piece pce){
	switch(pce){
		case W_BISHOP:
		case B_BISHOP:
			return true;
		default:
			return false;
	}
}

inline bool isBQ(enum piece pce){
	switch(pce){
		case W_BISHOP:
		case W_QUEEN:
		case B_BISHOP:
		case B_QUEEN:
			return true;
		default:
			return false;
	}
}


