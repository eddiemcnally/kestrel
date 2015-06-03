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



typedef char piece_t;



#define WHITE				0
#define BLACK				1

#define PAWN				(piece_t)(1 << 2)
#define ROOK				(piece_t)(2 << 2)
#define KNIGHT				(piece_t)(3 << 2)
#define BISHOP				(piece_t)(4 << 2)
#define QUEEN				(piece_t)(5 << 2)
#define KING				(piece_t)(6 << 2)

#define GET_PIECE(piece)	(piece & 0xFC)
#define GET_COLOUR(piece)   (piece & 0x01)
#define IS_WHITE(piece)		(GET_COLOUR(piece) == 0)
#define IS_BLACK(piece)		(!IS_WHITE(piece))



#define W_PAWN				(piece_t)(PAWN   | WHITE)
#define W_ROOK				(piece_t)(ROOK   | WHITE)
#define W_KNIGHT			(piece_t)(KNIGHT | WHITE)
#define W_BISHOP			(piece_t)(BISHOP | WHITE)
#define W_QUEEN				(piece_t)(QUEEN  | WHITE)
#define W_KING				(piece_t)(KING   | WHITE)

#define B_PAWN				(piece_t)(PAWN   | BLACK)
#define B_ROOK				(piece_t)(ROOK   | BLACK)
#define B_KNIGHT			(piece_t)(KNIGHT | BLACK)
#define B_BISHOP			(piece_t)(BISHOP | BLACK)
#define B_QUEEN				(piece_t)(QUEEN  | BLACK)
#define B_KING				(piece_t)(KING   | BLACK)



char get_piece_label(piece_t piece);



#endif
