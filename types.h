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


typedef unsigned long long 		BITBOARD;
typedef unsigned char 			U8;
typedef unsigned long long 		U64;
typedef unsigned short 			U16;
typedef unsigned int			U32;




typedef enum {
	BLACK,
	WHITE,
	NUM_COLOURS
} COLOUR;


typedef enum {
	a1 = 0, b1, c1, d1, e1, f1, g1, h1,
	a2,	b2, c2, d2, e2, f2, g2, h2,
	a3,	b3, c3, d3, e3, f3, g3, h3,
	a4,	b4, c4, d4, e4, f4, g4, h4,
	a5,	b5, c5, d5, e5, f5, g5, h5,
	a6,	b6, c6, d6, e6, f6, g6, h6,
	a7,	b7, c7, d7, e7, f7, g7, h7,
	a8,	b8, c8, d8, e8, f8, g8, h8,
	NUM_SQUARES
} SQUARE;


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
	B_KING 		= 11,
	NUM_PIECES 	= 12
} PIECE;

// piece values, indexed into using the PIECE enum
static const int piece_values[NUM_PIECES]= {  	
	100,  		// PAWN
	550, 		// ROOK
	325, 		// BISHOP
	325, 		// KNIGHT
	1000, 		// QUEEN
	50000, 		// KING
	100, 		// PAWN
	325, 		// BISHOP
	325, 		// KNIGHT
	550, 		// ROOK
	1000, 		// QUEEN
	50000		// KING  
};				





// contains information before the current
// move was made
typedef struct {
	U16 	move;
	U8 	fifty_move_counter;
	U8 	castle_perm;
	U64 	board_hash;
	SQUARE 	en_passant;
} undo_t;



// half moves
#define MAX_GAME_MOVES 		2048
#define MAX_POSITION_MOVES	256



/**
 * A container for holding the bitboards
 */
typedef struct {
	// bitboard entry for each piece
	BITBOARD bitboards[NUM_PIECES];

	// The above array piece arrays overlayed into a single bitboard.
	// In effect, an OR of all elements in piece_boards[]
	BITBOARD board;

	// squares where the kings are
	SQUARE king_squares[NUM_COLOURS];

	// the next side to move
	COLOUR side_to_move;
	
	// any square where en passent is active
	SQUARE en_passant;

	// fifty move ounter
	U8 fifty_move_counter;
	
	// keeping track of ply
	U8 ply;
	U8 history_ply;
	
	// indexed by PIECE, contains the number of pieces of that 
	// type on the board
	U8 pce_num[NUM_PIECES];

	// indexed by COLOUR, contains number of pieces != PAWN
	U8 big_pieces[NUM_COLOURS]; 
	
	// indexed by COLOUR, contains number of ROOKs and QUEENs
	U8 major_pieces[NUM_COLOURS];
	
	// indexed by COLOUR, contains number of BISHOPs and KNIGHTs
	U8 minor_pieces[NUM_COLOURS];

	// indexed by COLOUR, contains sum of all piece values
	U8 material[NUM_COLOURS];

	// contains the pieces on each square
	PIECE pieces[NUM_SQUARES];


	// castling permissions
	U8 castle_perm;
	
	// move history
	undo_t history[MAX_GAME_MOVES];
	
	// a hash of the current board 
	U64 board_hash;
	
} BOARD;


#endif
