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
#include "move_gen.h"
#include "move_gen_utils.h"
#include "utils.h"


int32_t quiesce(struct board *brd, int32_t alpha, int32_t beta);
int32_t alpha_beta(struct board *brd, int32_t alpha, int32_t beta, uint8_t depth);
static void init_search(struct board *brd);
static void bring_best_move_to_top(uint16_t move_num, struct move_list *mvl);


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




void search_positions(struct board *brd, uint8_t depth){
	
	assert(ASSERT_BOARD_OK(brd) == true);

	init_search(brd);
	
	mv_bitmap best_move = NO_MOVE;
	int32_t score = 0;

	
	// use iterative deepening
	for(uint8_t i = 1; i < depth; i++){
		score = alpha_beta(brd, -INFINITE, INFINITE, i);
		
		uint8_t num_moves = populate_pv_line(brd, depth);
		
		best_move = brd->pv_line[0];
		
		printf("depth %d score %d best move %s, #moves %d\n", i, score, print_move(best_move), num_moves);
		printf("\t\t\tpv line :\t");
		for(uint8_t j = 0; j < num_moves; j++){
			printf(" %s", print_move(brd->pv_line[j]));
		}		
		printf("\n");
	}
}



static void init_search(struct board *brd){
	dispose_table(brd->pvtable);
	
	brd-> pvtable = create_pv_table();	
}
	

int32_t alpha_beta(struct board *brd, int32_t alpha, int32_t beta, uint8_t depth) {
	if(depth == 0){
		return quiesce(brd, alpha, beta);
	} 
	
	
	mv_bitmap best_move = NO_MOVE;
	bool is_alpha_improved = false;
	
	struct move_list mvl[1];
	memset(mvl, 0, sizeof(struct move_list));	
		
	generate_all_moves(brd, mvl);

	uint16_t num_moves = mvl[0].move_count;
	
	
	// if a move is in the PV table, then set the score to be high
	// so it gets sorted correctly
	//mv_bitmap pv_mv = find_move(brd->pvtable, brd->board_hash);
	//if (pv_mv != NO_MOVE){
		//// find in move list
		//for(uint32_t i = 0; i < mvl[0].move_count; i++){
			//if (mvl[0].moves[i].move_bitmap == pv_mv){
				//mvl[0].moves[i].score = 200000;
			//}
		//}
	//}
	
	uint8_t legal_move_cnt = 0;
	for(uint16_t i = 0; i < num_moves; i++){
		
		bring_best_move_to_top(i, mvl);
		
		struct move mv = mvl[0].moves[i];
		
		bool valid_move = make_move(brd, mv.move_bitmap);
		if (valid_move == false){
			// moved already reverted
			continue;
		}
		
		legal_move_cnt++;
		
		// note: alpha/beta are swapped, and sign is reversed
		int32_t score = -alpha_beta(brd, -beta, -alpha, (uint8_t)(depth - 1));
		
		take_move(brd);
		
		if (score >= beta){
			return beta;		// fail-hard beta cutoff
		}
		if (score > alpha){
			alpha = score;
			is_alpha_improved = true;
			best_move = mv.move_bitmap;
		}		
	}
	
	if(legal_move_cnt == 0) {
		// no legal moves....must be mate or draw
		enum piece king = NO_PIECE;
		if (brd->side_to_move == WHITE)
			king = W_KING;
		else 
			king = B_KING;
			
		uint64_t king_bb = brd->bitboards[king];
		enum square king_sq = pop_1st_bit(&king_bb);
		
		enum colour opposite_side = GET_OPPOSITE_SIDE(brd->side_to_move);
		
		if (is_sq_attacked(brd, king_sq, opposite_side)){
			return -MATE + brd->ply;
		} else {
			// draw
			return 0;
		}
	}
	
	if (is_alpha_improved){
		add_move_to_pv_table(brd->pvtable, brd->board_hash, best_move);
	}
	
	return alpha;
}


static void bring_best_move_to_top(uint16_t move_num, struct move_list *mvl){

	uint32_t best_score = 0;
	uint16_t best_move_num = move_num; 

	for(uint16_t i = move_num; i < mvl->move_count; i++){
		if (mvl->moves[i].score > best_score){
			best_score = mvl->moves[i].score;
			best_move_num = i;
		}	
	} 

	struct move temp_mv = mvl->moves[move_num];
	mvl->moves[move_num] = mvl->moves[best_move_num];
	mvl->moves[best_move_num] = temp_mv;
}

 




int32_t quiesce(struct board *brd, int32_t alpha, int32_t beta){
	
	if (is_repetition(brd) || brd->fifty_move_counter >= 100){
		return 0;
	}
	
	if (brd->ply > MAX_SEARCH_DEPTH){
		return evaluate_position(brd);
	}
	
	int32_t stand_pat_score = evaluate_position(brd);
	
	if (stand_pat_score >= beta){
		return beta;
	}
	if (stand_pat_score > alpha){
		alpha = stand_pat_score;
	}

	struct move_list mvl[1];
	memset(mvl, 0, sizeof(struct move_list));	
			
	generate_all_capture_moves(brd, mvl);
	
	//print_move_list_details(mvl);

	//validate_move_list(mvl);

	uint16_t num_moves = mvl->move_count;
	struct move best_move = {.move_bitmap = 0, .score = 0};
	bool is_alpha_improved = false;
		
	for(uint16_t i = 0; i < num_moves; i++){
				
		bring_best_move_to_top(i, mvl);		
		
		struct move mv = mvl->moves[i];
		
		bool valid_move = make_move(brd, mv.move_bitmap);
		if (valid_move == false){
			// moved already reverted
			continue;
		}
		
		// note: alpha/beta are swapped, and sign is reversed
		int32_t score = -quiesce(brd, -beta, -alpha);
		
		take_move(brd);
		
		if (score > alpha){
			if (score >= beta){
				return beta;
			}
			is_alpha_improved = true;
			alpha = score;
			best_move = mv;
		}
	}
	
	if (is_alpha_improved){
		add_move_to_pv_table(brd->pvtable, brd->board_hash, best_move.move_bitmap);
	}

	return alpha;
}

