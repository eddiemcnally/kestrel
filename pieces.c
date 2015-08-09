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
// additional at end needed to support NO_PIECE and NUM_PIECES
static const char pieceToChar[PIECE_ENUM_SIZE] = {
    'P', 'R', 'N', 'B', 'Q', 'K', 'p', 'r', 'n', 'b', 'q', 'k', '#', '-'
};



// piece values, indexed into using the enum piece enum
static const U32 piece_values[NUM_PIECES] = {
    100,			// PAWN
    550,			// ROOK
    325,			// BISHOP
    325,			// KNIGHT
    1000,			// QUEEN
    50000,			// KING
    100,			// PAWN
    325,			// BISHOP
    325,			// KNIGHT
    550,			// ROOK
    1000,			// QUEEN
    50000			// KING
};



/**
 * Takes a typed piece and returns a human-readable representation
 */
inline char get_piece_label(enum piece piece)
{
    return pieceToChar[piece];
}


inline U32 get_piece_value(const enum piece pce){
	return piece_values[pce];
}


bool is_valid_piece(enum piece pce){
	return (pce >= 0 && pce <= NUM_PIECES) || (pce == NO_PIECE);
}


inline enum colour swap_colour(enum colour col){
	if (col == WHITE)
		return BLACK;
	return WHITE;
}


inline enum colour get_colour(enum piece piece)
{
    if (IS_WHITE(piece))
		return WHITE;
    return BLACK;
}

inline bool is_big_piece(enum piece pce)
{
    switch (pce) {
		case W_PAWN:
		case B_PAWN:
			return false;
		default:
			return true;
    }
}

inline bool is_major_piece(enum piece pce)
{
    switch (pce) {
		case W_ROOK:
		case W_QUEEN:
		case B_ROOK:
		case B_QUEEN:
			return true;
		default:
			return false;
    }
}

inline bool is_minor_piece(enum piece pce)
{
    switch (pce) {
		case W_BISHOP:
		case W_KNIGHT:
		case B_BISHOP:
		case B_KNIGHT:
			return true;
		default:
			return false;
    }
}

inline bool isRQ(enum piece pce)
{
    switch (pce) {
		case W_ROOK:
		case W_QUEEN:
		case B_ROOK:
		case B_QUEEN:
			return true;
		default:
			return false;
    }
}


inline bool is_pawn(enum piece pce)
{
    if ((pce == B_PAWN) || (pce == W_PAWN))
		return true;
    return false;
}

inline bool is_king(enum piece pce)
{
    if ((pce == B_KING) || (pce == W_KING))
		return true;
    return false;
}

inline bool isKn(enum piece pce)
{
    if ((pce == B_KNIGHT) || (pce == W_KNIGHT))
		return true;
    return false;
}

inline bool isR(enum piece pce)
{
    switch (pce) {
		case W_ROOK:
		case B_ROOK:
			return true;
		default:
			return false;
    }
}

inline bool isB(enum piece pce)
{
    switch (pce) {
		case W_BISHOP:
		case B_BISHOP:
			return true;
		default:
			return false;
    }
}

inline bool is_sliding_piece(enum piece pce){
	switch(pce){
		case W_BISHOP:
		case W_QUEEN:
		case W_ROOK:
		case B_BISHOP:
		case B_QUEEN:
		case B_ROOK:
			return true;
		default:
			return false;
	}
}


inline bool isBQ(enum piece pce)
{
    switch (pce) {
		case W_BISHOP:
		case W_QUEEN:
		case B_BISHOP:
		case B_QUEEN:
			return true;
		default:
			return false;
    }
}
