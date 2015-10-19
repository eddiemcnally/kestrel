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
#include <stdbool.h>
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

static void reset_search_history(struct board *brd, struct search_info *si);
static I32 alpha_beta(I32 alpha, I32 beta, U8 depth, struct board *brd,
		      struct search_info *si);

// checks to see if most recent move is a repetition
bool is_repetition(const struct board *brd)
{
	// only have to look since last fifty move counter was reset
	U8 start = (U8) (brd->history_ply - brd->fifty_move_counter);
	for (U8 i = start; i < brd->history_ply - 1; i++) {
		if (brd->board_hash == brd->history[i].board_hash) {
			return true;
		}
	}
	return false;
}

// searches using iterative deepening
void search_positions(struct board *brd, struct search_info *si)
{
	mv_bitmap best_move = NO_MOVE;
	I32 best_score = -INFINITE;
	U8 current_depth = 0;
	I32 pv_moves = 0;

	reset_search_history(brd, si);

	for (current_depth = 1; current_depth <= si->depth; ++current_depth) {
		best_score =
		    alpha_beta(-INFINITE, INFINITE, current_depth, brd, si);

		pv_moves = get_pv_line(brd->pvtable, brd, current_depth);

		best_move = brd->pv_array[0];

		printf("depth %d score %d, move %s, nodes %d\n",
		       (int)current_depth, (int)best_score,
		       print_move(best_move), (int)si->node_count);

		pv_moves = get_pv_line(brd->pvtable, brd, current_depth);

		printf("pv:");
		for (int pv_num = 0; pv_num < pv_moves; pv_num++) {
			printf(" %s", print_move(brd->pv_array[pv_num]));
		}
		printf("\n");

		printf("ordering : %.2f\n",
		       (si->fail_high_first / si->fail_high));

	}
}

static void reset_search_history(struct board *brd, struct search_info *si)
{
	for (int i = 0; i < NUM_COLOURS; i++) {
		for (int j = 0; j < NUM_SQUARES; j++) {
			brd->search_history[i][j] = 0;
		}
	}

	for (int i = 0; i < NUM_COLOURS; i++) {
		for (int j = 0; j < MAX_SEARCH_DEPTH; j++) {
			brd->search_killers[i][j] = 0;
		}
	}

	if (brd->pvtable != NULL) {
		dispose_table(brd->pvtable);
	}
	brd->pvtable = create_pv_table();

	brd->ply = 0;

	si->start_time = get_time_in_millis();
	si->stopped = true;
	si->node_count = 0;

	si->fail_high = 0;
	si->fail_high_first = 0;

}

static inline I32 alpha_beta(I32 alpha, I32 beta, U8 depth, struct board *brd,
			     struct search_info *si)
{
	if (depth == 0) {
		si->node_count++;
		return evaluate_position(brd);
	}

	si->node_count++;

	if (is_repetition(brd) || brd->fifty_move_counter >= 100) {
		return 0;
	}

	if (brd->ply > MAX_SEARCH_DEPTH - 1) {
		return evaluate_position(brd);
	}

	struct move_list mv_list = {
		.moves = {{0, 0}},
		.move_count = 0
	};

	generate_all_moves(brd, &mv_list);

	U32 num_legal_moves = 0;
	I32 old_alpha = alpha;
	mv_bitmap best_move = NO_MOVE;
	I32 score = -INFINITE;

	mv_bitmap mv;
	for (U32 mv_num = 0; mv_num < mv_list.move_count; ++mv_num) {
		mv = mv_list.moves[mv_num].move_bitmap;
		if (!make_move(brd, mv)) {
			continue;
		}

		num_legal_moves++;;
		score = -alpha_beta(-beta, -alpha, (U8) (depth - 1), brd, si);

		take_move(brd);

		if (score > alpha) {
			if (score >= beta) {
				if (num_legal_moves == 1) {
					si->fail_high_first++;
				}
				si->fail_high++;
				return beta;
			}
			alpha = score;

			best_move = mv_list.moves[mv_num].move_bitmap;
		}
	}

	if (num_legal_moves == 0) {
		enum piece king = B_KING;
		if (brd->side_to_move == WHITE) {
			king = W_KING;
		}

		U64 bb_king = brd->bitboards[king];

		enum square king_sq = pop_1st_bit(&bb_king);
		if (is_sq_attacked(brd, king_sq, FLIP_SIDE(brd->side_to_move))) {
			return (-MATE + brd->ply);
		} else {
			return 0;
		}
	}

	if (alpha != old_alpha) {
		add_move(brd->pvtable, brd->board_hash, best_move);
	}

	return alpha;
}
