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
#include "tt.h"
#include "board_utils.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "utils.h"


static int32_t quiescence(struct board *brd, struct search_info *si, int32_t alpha, int32_t beta);
static int32_t alpha_beta(struct board *brd, struct search_info *si, int32_t alpha, int32_t beta, uint8_t depth);
static void init_search(struct board *brd);
static inline void check_search_time_limit(struct search_info *sinfo);


// keep as power of 2
#define	EXPIRY_NODE_COUNT	1024



// checks to see if most recent move is a repetition
inline bool is_repetition(const struct board *brd)
{
	// the 50move counter is reset when a pawn moves or piece taken
	// (these are moves that can't be repeated), so we only need to
	// search the history from the last time the counter was reset

	int start = brd->history_ply - brd->fifty_move_counter;

	for (int i = start; i < brd->history_ply-1; i++) {
		if (brd->board_hash == brd->history[i].board_hash) {
			return true;
		}
	}
	return false;
}




void search_positions(struct board *brd, struct search_info *si, uint32_t tt_size_in_bytes){
	
	si->search_start_time = get_time_of_day_in_millis();
	
	assert(ASSERT_BOARD_OK(brd) == true);

	init_search(brd);
	
	create_tt_table(tt_size_in_bytes);
		
	mv_bitmap best_move = NO_MOVE;
	int32_t score = 0;
	uint8_t num_moves = 0;
	// use iterative deepening
	for(uint8_t current_depth = 1; current_depth <= si->depth; current_depth++){
		score = alpha_beta(brd, si, -INFINITE, INFINITE, current_depth);
		
		if (si->search_stopped == true){
			break;
		}
		num_moves = populate_pv_line(brd, current_depth);
		
		best_move = brd->pv_line[0];
		printf("depth %d score %d best move %s, #moves %d\n", current_depth, score, print_move(best_move), num_moves);
		printf("\t\t\tpv line :\t");
		for(uint8_t j = 0; j < num_moves; j++){
			printf(".......%s", print_move(brd->pv_line[j]));
		}		
		printf("\n");
	}

	// populate the best moves
	for(int i = 0; i < MAX_SEARCH_DEPTH; i++){
		if (i < num_moves)
			continue;
			
		brd->pv_line[i] = NO_MOVE;
	}
	// update search stats
	uint32_t elapsed_time_in_millis = (uint32_t)(get_time_of_day_in_millis() - si->search_start_time);
	si->nodes_per_second = (si->num_nodes * 1000) / elapsed_time_in_millis;
	
	printf("bestmove %s\n", print_move(best_move));
}



static void init_search(struct board *brd){
	
	for(int i = 0; i < MAX_SEARCH_DEPTH; i++){
		brd->pv_line[i] = NO_MOVE;
	}
	
	for(int i = 0; i < NUM_PIECES; i++) {
		for(int j = 0; j < NUM_SQUARES; j++) {
			brd->search_history[i][j] = NO_MOVE;
		}
	}
	
	for(int i = 0; i < NUM_KILLER_MOVES; i++) {
		for(int j = 0; j < MAX_SEARCH_DEPTH; j++) {
			brd->search_killers[i][j] = NO_MOVE;
		}
	}	
	
	brd->ply = 0;
	
}
	

static int32_t alpha_beta(struct board *brd, struct search_info *si, int32_t alpha, int32_t beta, uint8_t depth) {
	if(depth <= 0){
		return quiescence(brd, si, alpha, beta);
	} 
	
	// only check every so many nodes
	if ((si->num_nodes & (EXPIRY_NODE_COUNT - 1)) == 0){
		check_search_time_limit(si);	
	}
	
	si->num_nodes++;	
	
	if (is_repetition(brd)){
		si->repetition++;
		return 0; // a draw 
	}
	
	if (brd->fifty_move_counter >= 100){
		si->fifty_move_rule++;
		return 0; // a draw 
	}
	
	if (brd->ply > MAX_SEARCH_DEPTH - 1){
		si->max_depth_reached++;
		return evaluate_position(brd);
	}
		
	mv_bitmap best_move = NO_MOVE;
	int32_t old_alpha = alpha;
	
	struct move_list mvl = {
		.moves = {0},
		.move_count = 0
	};
		
	generate_all_moves(brd, &mvl);

	// check is position already in PV table
	mv_bitmap pv_move = probe_tt(brd->board_hash);
	if (pv_move != NO_MOVE){
		// prioritise 
		for(uint16_t i = 0; i < mvl.move_count; i++){
			if (mvl.moves[i] == pv_move){
				add_to_score(&mvl.moves[i], MOVE_ORDER_WEIGHT_PV_MOVE);
				si->move_ordering_pv_move++;
				break;
			}
		}
	}
	
	uint16_t num_moves = mvl.move_count;
	
	uint8_t legal_move_cnt = 0;
	for(uint16_t i = 0; i < num_moves; i++){
		bring_best_move_to_top(i, &mvl);
		
		// incr search stats
		si->num_nodes++;
		
		mv_bitmap mv = mvl.moves[i];
		bool valid_move = make_move(brd, mv);
		if (valid_move){
			legal_move_cnt++;
		
			// note: alpha/beta are swapped, and sign is reversed
			int32_t score = -alpha_beta(brd, si, -beta, -alpha, (uint8_t)(depth - 1));
			take_move(brd);
					
			if (si->search_stopped == true){
				return 0;
			}
										
			if (score > alpha){
				if (score >= beta){
					if (legal_move_cnt == 1){
						si->fail_high_first++;
					}
					si->fail_high++;
					
					// killer move....beta cutoff, no capture
					if (IS_CAPTURE_MOVE(mv) == false){
						si->killer_moves++;
						// shuffle down killers
						brd->search_killers[1][brd->ply] = brd->search_killers[0][brd->ply];
						brd->search_killers[0][brd->ply] = mv;
					}
										
					return beta;
				}
				alpha = score;
				best_move = mv;
				
				
				// search history....alpha cutoff, no capture
				if (IS_CAPTURE_MOVE(mv) == false){
					si->search_history++;
					enum square from_sq = FROMSQ(best_move);
					enum square to_sq = TOSQ(best_move);
					
					enum piece pce = brd->pieces[from_sq];
					brd->search_history[pce][to_sq] += depth;
				}	
				
				
			}			
		} else {
			si->invalid_moves_made++;
		}	
	}
	
	if(legal_move_cnt == 0) {
		si->zero_legal_moves++;
		//printf("***no legal moves left\n");
		// no legal moves....must be mate or draw
		enum square king_sq = brd->king_sq[brd->side_to_move];	
		enum colour opposite_side = GET_OPPOSITE_SIDE(brd->side_to_move);
		
		if (is_sq_attacked(brd, king_sq, opposite_side)){
			si->mates_detected++;
			return -MATE + brd->ply;
		} else {
			// draw
			return 0;
		}
	}
	
	if (alpha != old_alpha){
		// improved alpha, so add to tt
		add_to_tt(brd->board_hash, best_move);

		// search stats
		si->added_to_tt++;
	}
	
	return alpha;
}



static int32_t quiescence(struct board *brd, struct search_info *si, int32_t alpha, int32_t beta) {
	
	// only check every so many nodes
	if ((si->num_nodes & (EXPIRY_NODE_COUNT - 1)) == 0){
		check_search_time_limit(si);	
	}
	si->num_nodes++;
	
	if (is_repetition(brd) || brd->fifty_move_counter > 100){
		// draw
		return 0;
	}

	if (brd->ply > MAX_SEARCH_DEPTH - 1){
		return evaluate_position(brd);	
	}
	
	// stand pat
	int32_t stand_pat_score = evaluate_position(brd);
	if (stand_pat_score >= beta){
		si->stand_pat_cutoff++;
		return beta;
	}	
	if (stand_pat_score > alpha){
		si->stand_pat_improvement++;
		alpha = stand_pat_score;
	}
	
	
	struct move_list mvl = {
		.moves = {0},
		.move_count = 0
	};
		
	// only the capture moves
	generate_all_capture_moves(brd, &mvl);

	uint16_t num_moves = mvl.move_count;
	
	for(uint16_t i = 0; i < num_moves; i++){
		bring_best_move_to_top(i, &mvl);
		
		mv_bitmap mv = mvl.moves[i];
		bool valid_move = make_move(brd, mv);
		if (valid_move){
		
			// note: alpha/beta are swapped, and sign is reversed
			int32_t score = -quiescence(brd, si, -beta, -alpha);
			take_move(brd);
					
			if (si->search_stopped == true){
				return 0;
			}
			
			if (score > alpha){
				if (score >= beta){
					return beta;
				}
				
				alpha = score;
			}			
		}	
	}
	return alpha;
}



static inline void check_search_time_limit(struct search_info *sinfo){
	if(sinfo->search_time_set == true){
		uint64_t curr_time_of_day = get_time_of_day_in_millis();
		if (curr_time_of_day >= sinfo->search_expiry_time){
			// search timed out
			sinfo->search_stopped = true;
		}
	}
}



inline void bring_best_move_to_top(uint16_t move_num, struct move_list *mvl){

	uint32_t best_score = 0;
	uint16_t best_move_num = move_num; 

	for(uint16_t i = move_num; i < mvl->move_count; i++){
		uint32_t score = get_score(mvl->moves[i]);
		if (score > best_score){
			best_score = score;
			best_move_num = i;
		}	
	} 

	mv_bitmap temp_mv = mvl->moves[move_num];
	mvl->moves[move_num] = mvl->moves[best_move_num];
	mvl->moves[best_move_num] = temp_mv;
}

void dump_search_info(struct search_info *si){
	printf("Search Stats :\n");
	printf("\tSearch Depth..............%d\n", si->depth);
	printf("\t#nodes....................%d\n", si->num_nodes);
	printf("\t#nodes/sec................%d\n", si->nodes_per_second);
	printf("\t#add to TT................%d\n", si->added_to_tt);
	printf("\t#invalid moves............%d\n", si->invalid_moves_made);
	printf("\t#zero legal moves.........%d\n", si->zero_legal_moves);
	printf("\t#repetitions..............%d\n", si->repetition);
	printf("\t#mate moves detected......%d\n", si->mates_detected);
	printf("\t#50-move rules............%d\n", si->fifty_move_rule);
	printf("\t#max depth reached........%d\n", si->max_depth_reached);
	printf("\tmove ordering : PV Move...%d\n", si->move_ordering_pv_move);
	printf("\tmove ordering : Killers...%d\n", si->killer_moves);
	printf("\tmove ordering : History...%d\n", si->search_history);
	printf("\tfhf/fh....................%.2f\n", ((float)si->fail_high_first/(float)si->fail_high));
	printf("\tstand-pat beta cutoff.....%d\n", si->stand_pat_cutoff);
	printf("\tstand-pat improvement.....%d\n", si->stand_pat_improvement);
}


