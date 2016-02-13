/*
 * init.h
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
#include "types.h"
#include "move_gen.h"


struct search_info {
	// ---- inputs to search
	uint8_t depth;					// search depth
	
	
	// ---- search stats
	uint32_t num_nodes;				// num nodes searched
	uint32_t added_to_tt;			// num moves added to transposition table
	uint32_t invalid_moves_made;	// num moves that needed to be reverted
	uint32_t zero_legal_moves;		// num times we hit zero legal moves
	uint32_t repetition;			// num repetitions detected
	uint32_t fifty_move_rule;		// num fifty move rule limits detected
	uint32_t max_depth_reached;		// num times max search depth reached
	uint32_t fail_high;				// num beta cut-offs
	uint32_t fail_high_first;		// found best legal move first
	uint32_t stand_pat_cutoff;		// num times stand pat is better than beta in Quiescence
	uint32_t stand_pat_improvement;	// num times stand pat improves alpha
	uint32_t mates_detected;		// num mate moves detected
	
};


void search_positions(struct board *brd, struct search_info *si, uint32_t tt_size_in_bytes);
bool is_repetition(const struct board *brd);
void bring_best_move_to_top(uint16_t move_num, struct move_list *mvl);
void dump_search_info(struct search_info *si);

