/*
 * move_gen.h
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


struct move_list {
	mv_bitmap moves[MAX_POSITION_MOVES];	// list of moves
	uint16_t move_count;					// #moves in list
};




mv_bitmap MOVE(enum square from, enum square to, enum piece capture,
	       enum piece promote, uint64_t flags, uint32_t score);
void generate_all_moves(struct board *brd, struct move_list *mvl);
void generate_all_capture_moves(struct board *brd, struct move_list *mvl);
void init_move_gen_framework(void);


void TEST_generate_white_pawn_moves(struct board *brd,
				    struct move_list *mvl);
void TEST_generate_black_pawn_moves(struct board *brd,
				    struct move_list *mvl);
void TEST_generate_knight_piece_moves(struct board *brd,
				      struct move_list *mvl, enum colour col);
void TEST_generate_king_moves(struct board *brd,
			      struct move_list *mvl, enum colour col);
void TEST_generate_castle_moves(struct board *brd,
				struct move_list *mvl, enum colour col);
void TEST_generate_sliding_horizontal_vertical_moves(struct board
						     *brd,
						     struct move_list *mvl,
						     enum colour col);
void TEST_generate_sliding_diagonal_moves(struct board *brd,
					  struct move_list *mvl,
					  enum colour col);
void TEST_generate_queen_moves(struct board *brd,
			       struct move_list *mvl, enum piece pce);
bool TEST_is_move_in_list(struct move_list *mvl, mv_bitmap mv);
void TEST_add_en_passent_move(mv_bitmap move_bitmap, struct move_list *mvlist);
void TEST_add_quiet_move(struct board *brd, mv_bitmap move_bitmap, 
			struct move_list *mvlist);
uint32_t TEST_get_move_score(enum piece victim, enum piece attacker);
struct move_list *TEST_get_empty_move_list(void);


#endif
