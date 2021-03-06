/*
 * kestrel.h
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
#pragma once

#include <stdbool.h>
#include <stdint.h>


#define ENGINE_NAME	"Kestrel v0.9"
#define	AUTHOR		"Eddie McNally"




#define NUM_RANKS 	8
#define NUM_FILES	8


/*
 *
 * The 'mv_bitmap' field is bitmapped as follows:
 * 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 1111 1111 1111 1111 1111 1111 -> Move Score
 * 0000 0000 0000 0000 0000 0000 0011 1111 0000 0000 0000 0000 0000 0000 0000 0000 -> From Square
 * 0000 0000 0000 0000 0000 1111 1100 0000 0000 0000 0000 0000 0000 0000 0000 0000 -> To Square
 * 0000 0000 0000 0000 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 -> Captured piece
 * 0000 0000 0000 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 -> Promoted Piece
 * 0000 0000 0001 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 -> Is En passant move
 * 0000 0000 0010 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 -> Is Pawn Start (1st pawn move)
 * 0000 0000 0100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 -> Is Castle Move
 * 0000 0000 1000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 -> Is Capture Move
 * XXXX XXXX 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 -> spare/unused
 */
typedef uint64_t mv_bitmap;

// bit mask offsets for the above bitmap
#define MV_MASK_OFF_SCORE			0
#define MV_MASK_OFF_FROM_SQ			32
#define MV_MASK_OFF_TO_SQ			38
#define MV_MASK_OFF_CAPTURED_PCE	44
#define MV_MASK_OFF_PROMOTED_PCE	48


//--- macros for setting the 'move' field in the MOVE struct
#define FROMSQ(m) 			(((m) >> MV_MASK_OFF_FROM_SQ) & 0x3F)
#define TOSQ(m) 			(((m) >> MV_MASK_OFF_TO_SQ) & 0x3F)
#define CAPTURED_PCE(m) 	(((m) >> MV_MASK_OFF_CAPTURED_PCE) & 0xF)
#define PROMOTED_PCE(m) 	(((m) >> MV_MASK_OFF_PROMOTED_PCE) & 0xF)


#define MFLAG_EN_PASSANT 	0x0010000000000000
#define MFLAG_PAWN_START 	0x0020000000000000
#define MFLAG_CASTLE 		0x0040000000000000
#define MFLAG_CAPTURE 		0x0080000000000000
#define MFLAG_NONE			0x0

#define MV_MASK_SCORE		0x00000000FFFFFFFF
#define MV_MASK_MOVE		0xFFFFFFFF00000000

#define	IS_EN_PASS_MOVE(mv)		((mv & MFLAG_EN_PASSANT) != 0)
#define IS_CAPTURE_MOVE(mv)		((mv & MFLAG_CAPTURE) != 0)
#define IS_CASTLE_MOVE(mv)		((mv & MFLAG_CASTLE) != 0)
#define IS_PAWN_START(mv)		((mv & MFLAG_PAWN_START) != 0)
#define IS_PAWN_START(mv)		((mv & MFLAG_PAWN_START) != 0)





#define NO_MOVE				0



enum square {
    a1 = 0, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    NUM_SQUARES = 64,
    NO_SQUARE	= 100
};



enum colour {
    WHITE = 0,
    BLACK = 1,
    NUM_COLOURS = 2
};

// white is even, black is odd
enum piece {
    W_PAWN 		= 0,
    B_PAWN 		= 1,
    W_BISHOP	= 2,
    B_BISHOP	= 3,
    W_KNIGHT	= 4,
    B_KNIGHT	= 5,
    W_ROOK 		= 6,
    B_ROOK		= 7,
    W_QUEEN 	= 8,
    B_QUEEN		= 9,
    W_KING		= 10,
    B_KING		= 11
};
#define NUM_PIECES	12
#define	NO_PIECE	(0x0F)


// contains information before the current
// move was made
struct undo {
    mv_bitmap move;
    uint8_t fifty_move_counter;
    uint8_t castle_perm;
    uint64_t board_hash;
    enum square en_passant;
};


// half moves
#define MAX_GAME_MOVES 		2048
#define MAX_POSITION_MOVES	256
#define MAX_SEARCH_DEPTH	64

#define NUM_KILLER_MOVES	2


struct position;

struct perft_stats {
    uint64_t num_ep;
    uint64_t num_captures;
};
