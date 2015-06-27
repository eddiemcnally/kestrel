/*
 * pieces.h
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

#ifndef _PIECES_H_
#define _PIECES_H_


typedef enum {
    W_PAWN 		= 0,
    W_ROOK 		= 1,
    W_KNIGHT 	= 2,
    W_BISHOP 	= 3,
    W_QUEEN 	= 4,
    W_KING 		= 5,
    B_PAWN 		= 6,
    B_ROOK 		= 7,
    B_KNIGHT 	= 8,
    B_BISHOP 	= 9,
    B_QUEEN 	= 10,
    B_KING 		= 11
} piece_id_t;


typedef char piece_t;


#define NO_PIECE	-1

char get_piece_label(piece_id_t piece);



#endif
