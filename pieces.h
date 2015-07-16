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

#define IS_WHITE(piece)		(piece <= W_KING)
#define IS_BLACK(piece)		(IS_WHITE(piece) == false)

char get_piece_label(enum piece piece);
enum colour get_colour(enum piece piece);

bool is_big_piece(enum piece pce);
bool is_major_piece(enum piece pce);
bool is_minor_piece(enum piece pce);

bool isRQ(enum piece pce);
bool isKi(enum piece pce);
bool isKn(enum piece pce);
bool isBQ(enum piece pce);
bool isR(enum piece pce);
bool isB(enum piece pce);

#endif
