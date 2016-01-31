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




void search_positions(struct board *brd, uint8_t search_depth){
	
	assert(ASSERT_BOARD_OK(brd) == true);

	init_search(brd);
	
	create_pv_table();
	
	mv_bitmap best_move = NO_MOVE;
	int32_t score = 0;
	
	// use iterative deepening
	for(uint8_t current_depth = 1; current_depth <= search_depth; current_depth++){
		score = alpha_beta(brd, -INFINITE, INFINITE, current_depth);
		
		uint8_t num_moves = populate_pv_line(brd, current_depth);
		
		best_move = brd->pv_line[0];
		
		printf("depth %d score %d best move %s, #moves %d\n", current_depth, score, print_move(best_move), num_moves);
		printf("\t\t\tpv line :\t");
		for(uint8_t j = 0; j < num_moves; j++){
			printf(".......%s", print_move(brd->pv_line[j]));
		}		
		printf("\n");
	}
}



static void init_search(struct board *brd){
	
	// init move ordering support
	for(uint8_t i = 0; i < NUM_PIECES; i++){
		for(uint8_t j = 0; j < NUM_SQUARES; j++){
			brd->search_history[i][j] = NO_PIECE;
		} 
	}
	
	for(uint8_t i = 0; i < SEARCH_KILLER_MOVES; i++){
		for(uint8_t j = 0; j < MAX_SEARCH_DEPTH; j++){
			brd->search_history[i][j] = NO_MOVE;
		} 
	}	
	
	brd->ply = 0;
}
	

int32_t alpha_beta(struct board *brd, int32_t alpha, int32_t beta, uint8_t depth) {
	if(depth == 0){
		return evaluate_position(brd);
		//quiesce(brd, alpha, beta);
	} 
	
	if (is_repetition(brd) || brd->fifty_move_counter >= 100){
		return 0; // a draw 
	}
	
	if (brd->ply > MAX_SEARCH_DEPTH - 1){
		return evaluate_position(brd);
	}
		
	mv_bitmap best_move = NO_MOVE;
	int32_t old_alpha = alpha;
	
	struct move_list mvl = {
		.moves = {0},
		.move_count = 0
	};
		
	generate_all_moves(brd, &mvl);

	uint16_t num_moves = mvl.move_count;
	
	
	// if a move is in the PV table, then set the score to be high
	// so it gets sorted correctly
/*	mv_bitmap pv_mv = find_move(brd->pvtable, brd->board_hash);
	if (pv_mv != NO_MOVE){
		// find in move list
		for(uint32_t i = 0; i < mvl.move_count; i++){
			if (mvl.moves[i] == pv_mv){
				add_to_score(&(mvl.moves[i]), 200000);
			}
		}
	}
	*/
	
	uint8_t legal_move_cnt = 0;
	for(uint16_t i = 0; i < num_moves; i++){
		//bring_best_move_to_top(i, &mvl);
		
		mv_bitmap mv = mvl.moves[i];
		bool valid_move = make_move(brd, mv);
		if (valid_move){
			legal_move_cnt++;
			
			// note: alpha/beta are swapped, and sign is reversed
			int32_t score = -alpha_beta(brd, -beta, -alpha, (uint8_t)(depth - 1));
			printf("after alphabeta.......score %d, alpha %d\n", score, alpha);	
			take_move(brd);
			
			if (score > alpha){
				if (score >= beta){
					printf("score %d > beta %d\n", score, beta);
					return beta;
				}
				
				printf("best score %d > alpha %d, move %s\n", score, alpha, print_move(mv));
				alpha = score;
				best_move = mv;
			}			
		}
		
	}
	
	if(legal_move_cnt == 0) {
		//printf("***no legal moves left\n");
		// no legal moves....must be mate or draw
		enum square king_sq = brd->king_sq[brd->side_to_move];	
		enum colour opposite_side = GET_OPPOSITE_SIDE(brd->side_to_move);
		
		if (is_sq_attacked(brd, king_sq, opposite_side)){
			///printf("***no legal moves left....MATE\n");
			return -MATE + brd->ply;
		} else {
			// draw
			///printf("***no legal moves left.....draw\n");
			return 0;
		}
	}
	
	if (alpha != old_alpha){
		// improved alpha, so add to pv table
		printf("Adding to pv %s, score %d\n", print_move(best_move), alpha);
		add_move_to_pv_table(brd->board_hash, best_move);
	}
	
	return alpha;
}


inline void bring_best_move_to_top(uint16_t move_num, struct move_list *mvl){

	int32_t best_score = 0;
	uint16_t best_move_num = move_num; 

	for(uint16_t i = move_num; i < mvl->move_count; i++){
		int32_t score = get_score(mvl->moves[i]);
		if (score > best_score){
			best_score = score;
			best_move_num = i;
		}	
	} 

	mv_bitmap temp_mv = mvl->moves[move_num];
	mvl->moves[move_num] = mvl->moves[best_move_num];
	mvl->moves[best_move_num] = temp_mv;
}

 


