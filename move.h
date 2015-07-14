/*
 * move.h
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
#ifndef _MOVE_H_
#define _MOVE_H_

#include "types.h"


/*
 * This struct represents a piece move.
 * 
 * The 'move' field is bitmapped as follows:
 * 
 * 0000 0000 0000 0000 0000 0111 1111 -> From
 * 0000 0000 0000 0011 1111 1000 0000 -> To
 * 0000 0000 0011 1100 0000 0000 0000 -> Captured piece
 * 0000 0000 0100 0000 0000 0000 0000 -> En passant move
 * 0000 0000 1000 0000 0000 0000 0000 -> Pawn Start
 * 0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece
 * 0001 0000 0000 0000 0000 0000 0000 -> Castle
 */
typedef struct{
	U32 move;
	U32 score;
} MOVE;


typedef struct{
	MOVE 	moves[MAX_POSITION_MOVES];
	U16	move_count;
} MOVELIST;

//--- macros for setting the 'move' field in the MOVE struct
#define FROMSQ(m) 		((m) & 0x7F)
#define TOSQ(m) 		(((m)>>7) & 0x7F)
#define CAPTURED(m) 		(((m)>>14) & 0xF)
#define PROMOTED(m) 		(((m)>>20) & 0xF)

#define MFLAG_EN_PASSANT 	0x40000
#define MFLAG_PAWN_START 	0x80000
#define MFLAG_CASTLE 		0x1000000

#define MFLAG_CAPTURED 		0x7C000			// En Passant | Captures
#define MFLAG_PROMOTED		0xF00000
//---





char * print_move(const MOVE * m);


#endif
