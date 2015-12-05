/*
 * search.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for searching for moves and determining
 * the best ones
 * ---------------------------------------------------------------------
 *
 *
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
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "search.h"
#include "attack.h"
#include "evaluate.h"
#include "board.h"
#include "pieces.h"
#include "makemove.h"
#include "pv_table.h"
#include "board_utils.h"
#include "move.h"
#include "utils.h"


int32_t alpha_beta(struct board *brd, int32_t alpha, int32_t beta, int32_t depth);


// checks to see if most recent move is a repetition
inline bool is_repetition(const struct board *brd)
{
	// the 50move counter is reset when a pawn moves or piece taken
	// (these are moves that can't be repeated), so we only need to
	// search the history from the last time the counter was reset

	int start = brd->history_ply - brd->fifty_move_counter;

	for (int i = start; i < brd->history_ply; i++) {
		if (brd->board_hash == brd->history[i].board_hash) {
			return true;
		}
	}
	return false;
}






int32_t alpha_beta(struct board *brd, int32_t alpha, int32_t beta, int32_t depth) {
	if(depth == 0){
		// simple position eval until quiessence is implemented
		return evaluate_position(brd);
	} 
	
	struct move_list mvl[1];
	
	generate_all_moves(brd, mvl);

	int32_t num_moves = mvl[0].move_count;
	
	for(int32_t i = 0; i < num_moves; i++){
		struct move mv = mvl[0].moves[i];
		
		bool valid_move = make_move(brd, mv.move_bitmap);
		if (valid_move == false){
			continue;
		}
		
		int32_t score = alpha_beta(brd, -beta, -alpha, depth - 1);
		
		take_move(brd);
		
		if (score >= beta){
			return beta;		// fail-hard beta cutoff
		}
		if (score > alpha){
			alpha = score;
		}
		
	}
	return alpha;
	
}










