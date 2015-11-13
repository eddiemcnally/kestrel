/*
 * types.h
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
#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdbool.h>


typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long int U64;
typedef signed char I8;
typedef signed short I16;
typedef signed int I32;
typedef signed long long int I64;







enum colour {
	BLACK = 0,
	WHITE = 1,
	NUM_COLOURS = 2
};

/*
 *
 * The 'move' field is bitmapped as follows:
 *
 * 0000 0000 0000 0000 0000 0000 0111 1111 -> From Square
 * 0000 0000 0000 0000 0011 1111 1000 0000 -> To Square
 * 0000 0000 0000 0011 1100 0000 0000 0000 -> Captured piece
 * 0000 0000 0000 0100 0000 0000 0000 0000 -> En passant move
 * 0000 0000 0000 1000 0000 0000 0000 0000 -> Pawn Start
 * 0000 0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece
 * 0000 0001 0000 0000 0000 0000 0000 0000 -> Castle
 * XXXX XXX0 0000 0000 0000 0000 0000 0000 -> spare/unused
 */
typedef U32 mv_bitmap;

//--- macros for setting the 'move' field in the MOVE struct
#define FROMSQ(m) 			((m) & 0x7F)
#define TOSQ(m) 			((m>>7) & 0x7F)
#define CAPTURED_PCE(m) 	(((m)>>14) & 0xF)
#define PROMOTED_PCE(m) 		(((m)>>20) & 0xF)

// creates a bitmapped int for the move attributes
// f -> from
// t -> to
// ca -> captured
// pro -> promoted
//

#define MFLAG_EN_PASSANT 	0x0040000
#define MFLAG_PAWN_START 	0x0080000
#define MFLAG_CASTLE 		0x1000000
#define MFLAG_NONE			0x0

#define	IS_EN_PASS_MOVE(mv)	((mv & MFLAG_EN_PASSANT) != 0)


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
	NUM_SQUARES
};

#define	NO_SQUARE	(100)

enum piece {
	W_PAWN = 0,
	W_ROOK = 1,
	W_KNIGHT = 2,
	W_BISHOP = 3,
	W_QUEEN = 4,
	W_KING = 5,
	B_PAWN = 6,
	B_ROOK = 7,
	B_KNIGHT = 8,
	B_BISHOP = 9,
	B_QUEEN = 10,
	B_KING = 11
};
#define NO_PIECE	0x0F
#define NUM_PIECES	12


#define IS_VALID_PIECE(pce)		((pce >= W_PAWN) && (pce <= B_KING))

// contains information before the current
// move was made
struct undo {
	mv_bitmap move;
	U8 fifty_move_counter;
	U8 castle_perm;
	U64 board_hash;
	enum square en_passant;
};

// half moves
#define MAX_GAME_MOVES 		2048
#define MAX_POSITION_MOVES	256
#define MAX_SEARCH_DEPTH	64






// principle variation table structs form a linked list
struct pv_entry {
	U64 hashkey;
	mv_bitmap move;
	struct pv_entry *next;
};


struct pv_table {
	struct pv_entry *entries;
	U32 num_entries;
};





/**
 * A container for holding a specific position
 */
struct board {
	// bitboard entry for each piece
	U64 bitboards[NUM_PIECES];

	// The above array piece arrays overlayed into a single bitboard.
	// In effect, an OR of all elements in bitboards[]
	U64 board;

	// a bitboard for each colour, representing where all the pieces
	// of each colour are on the board
	U64 colour_bb[NUM_COLOURS];


	// the next side to move
	enum colour side_to_move;

	// the square where en passent is active
	enum square en_passant;

	U8 fifty_move_counter;

	// keeping track of ply
	U8 ply;
	U8 history_ply;

	// indexed by enum colour, contains sum of all piece values
	U32 material[NUM_COLOURS];

	// contains the pieces on each square
	enum piece pieces[NUM_SQUARES];

	// principle variation table
	struct pv_table * pvtable;

	mv_bitmap pv_array[MAX_SEARCH_DEPTH];

	// castling permissions
	U8 castle_perm;

	// move history
	struct undo history[MAX_GAME_MOVES];

	// a hash of the current board
	U64 board_hash;

	mv_bitmap search_history[NUM_PIECES][NUM_SQUARES];
	mv_bitmap search_killers[NUM_COLOURS][MAX_SEARCH_DEPTH];

};


// holds info associated with searching
struct search_info {
	// start time of search
	U64 start_time;
	// stop time of search
	U64 stop_time;
	// depth to search to
	U8 depth;
	// count of moved remaining
	U32 moves_remaining;
	// count of nodes
	U64 node_count;

	// quit command has been issued
	bool quit_set;
	// indicates infinite search is activated
	bool infinite;
	// flag indicating depth is set
	bool depth_set;
	// flag indicating time is set
	bool time_set;
	// search is stopped
	bool stopped;


	float fail_high;
	float fail_high_first;
};






#endif
