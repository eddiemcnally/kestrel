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
bool is_repetition(const struct board *brd);

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

// searches using iterative deepening
void search_positions(struct board *brd, struct search_info *si)
{
	mv_bitmap best_move = NO_MOVE;
	I32 best_score = -INFINITE;
	U8 current_depth = 0;
	U8 num_pv_moves = 0;

	// initialise for search
	reset_search_history(brd, si);

	for (current_depth = 1; current_depth <= si->depth; current_depth++) {
		best_score =
		    alpha_beta(-INFINITE, INFINITE, current_depth, brd, si);

		num_pv_moves = populate_pv_line(brd->pvtable, brd, current_depth);
		best_move = brd->pv_array[0];


		// debug //////////////////////////
		printf("depth %d score %d, move %s, nodes %d, pvMoves %d ",
		       (int)current_depth, (int)best_score,
		       print_move(best_move), (int)si->node_count,
		       num_pv_moves);

		printf("pv:");
		for (int pv_num = 0; pv_num < num_pv_moves; ++pv_num) {
			printf(" %s", print_move(brd->pv_array[pv_num]));
		}
		printf("\n");

		printf("ordering : %.2f\n",
		       (si->fail_high_first / si->fail_high));
		/////////////////////////////////////
	}

	dump_pv_table_stats(brd->pvtable);

}

static void reset_search_history(struct board *brd, struct search_info *si)
{
	for (int i = 0; i < NUM_PIECES; i++) {
		for (int j = 0; j < NUM_SQUARES; j++) {
			brd->search_history[i][j] = NO_MOVE;
		}
	}

	for (int i = 0; i < NUM_COLOURS; i++) {
		for (int j = 0; j < MAX_SEARCH_DEPTH; j++) {
			brd->search_killers[i][j] = NO_MOVE;
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
	/*printf("alpha %d, beta %d, depth %d, :: ", alpha, beta, depth);
	   print_compressed_board(brd);
	   printf("\n");
	 */

	if (depth == 0) {
		si->node_count++;
		return evaluate_position(brd);
	}

	si->node_count++;

	if ((is_repetition(brd) || brd->fifty_move_counter >= 100)
				&& brd->ply ==0){
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

	U16 num_legal_moves = 0;
	I32 orig_alpha = alpha;
	mv_bitmap best_move = NO_MOVE;
	I32 score = -INFINITE;

	for (U16 i = 0; i < mv_list.move_count; i++) {

		mv_bitmap mv = mv_list.moves[i].move_bitmap;

		if (!make_move(brd, mv)) {
			// invalid move
			continue;
		}

		num_legal_moves++;

		// swap alpha/beta
		score =
		    -alpha_beta(-beta, -alpha, (U8)(depth - 1), brd, si);

		// revert move
		take_move(brd);

		if (score > alpha) {
			if (score >= beta) {
				// a beta cut-off
				if (num_legal_moves == 1) {
					si->fail_high_first++;
				}
				si->fail_high++;
				return beta;
			}

			alpha = score;
			best_move = mv;
		}
	}

	if (num_legal_moves == 0) {
		// no legal moves, so find out where king is, and see if it's being attacked
		enum piece king;
		if (brd->side_to_move == WHITE) {
			king = W_KING;
		} else {
			king = B_KING;
		}

		U64 bb_king = brd->bitboards[king];
		enum square king_sq = pop_1st_bit(&bb_king);

		enum colour opp_side = GET_OPPOSITE_SIDE(brd->side_to_move);

		if (is_sq_attacked(brd, king_sq, opp_side)) {
			// no legal moves AND we're in mate, implied checkmate
			return (-MATE + brd->ply);
		} else {
			// stalemate and draw
			return 0;
		}
	}

	if (alpha != orig_alpha) {
		// alpha is improved....better move
		add_move_to_pv_table(brd->pvtable, brd->board_hash, best_move);
	}

	return alpha;
}
