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

#include <stdbool.h>
#include "types.h"




struct move {
    mv_bitmap	move_bitmap;
    U32 score;
};

struct move_list {
    struct move moves[MAX_POSITION_MOVES];
    U16 move_count;
};


//--- macros for setting the 'move' field in the MOVE struct
#define FROMSQ(m) 		((m) & 0x7F)
#define TOSQ(m) 		(((m)>>7) & 0x7F)
#define CAPTURED(m) 	(((m)>>14) & 0xF)
#define PROMOTED(m) 	(((m)>>20) & 0xF)

// creates a bitmapped int for the move attributes
// f -> from
// t -> to
// ca -> captured
// pro -> promoted
//
//#define MOVE(from,to,capture,promote,fl) ( (from) | ((to) << 7) | ( (capture) << 14 ) | ( (promote) << 20 ) | (fl))


#define MFLAG_EN_PASSANT 	0x0040000
#define MFLAG_PAWN_START 	0x0080000
#define MFLAG_CASTLE 		0x1000000

//#define MFLAG_CAPTURED 		0x007C000	// En Passant | Captures
//#define MFLAG_PROMOTED		0x0F00000
//---

char *print_move(mv_bitmap move_bitmap);
void print_move_details(U32 move_bitmap, U32 score);
void print_move_list(const struct move_list *list);
void print_move_list_details(const struct move_list *list);
void generate_all_moves(const struct board *brd, struct move_list *mvl);
void validate_move_list(struct move_list *mvl);
mv_bitmap MOVE(enum square from, enum square to, enum piece capture, enum piece promote, U32 fl);

void TEST_generate_white_pawn_moves(const struct board *brd, struct move_list *mvl);
void TEST_generate_black_pawn_moves(const struct board *brd, struct move_list *mvl);
void TEST_generate_knight_piece_moves(const struct board *brd, struct move_list *mvl, enum colour col);
void TEST_generate_king_moves(const struct board *brd, struct move_list *mvl, enum colour col);
void TEST_generate_castle_moves(const struct board *brd, struct move_list *mvl, enum colour col);
void TEST_generate_sliding_horizontal_vertical_moves(const struct board *brd, struct move_list *mvl, enum piece pce);
void TEST_generate_sliding_diagonal_moves(const struct board *brd, struct move_list *mvl, enum piece pce);
void TEST_generate_queen_moves(const struct board *brd, struct move_list *mvl, enum piece pce);
bool TEST_is_move_in_list(struct move_list *mvl, mv_bitmap mv);
void TEST_add_quiet_move(mv_bitmap move_bitmap, struct move_list *mvlist);
void TEST_add_en_passent_move(mv_bitmap move_bitmap, struct move_list *mvlist);
void TEST_add_capture_move(mv_bitmap move_bitmap, struct move_list *mvlist);
struct move_list * TEST_get_empty_move_list(void);

#endif
