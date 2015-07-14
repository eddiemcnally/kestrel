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


#define NO_PIECE	((unsigned int)(-1))

#define IS_WHITE(piece)		(piece <= W_KING)
#define IS_BLACK(piece)		(IS_WHITE(piece) == false)


char get_piece_label(PIECE piece);
COLOUR get_colour(PIECE piece);

bool is_big_piece(PIECE pce);
bool is_major_piece(PIECE pce);
bool is_minor_piece(PIECE pce);

bool isRQ(PIECE pce);
bool isKi(PIECE pce);
bool isKn(PIECE pce);
bool isBQ(PIECE pce);
bool isR(PIECE pce);
bool isB(PIECE pce);







#endif
