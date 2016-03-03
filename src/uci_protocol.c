/*
 * uci_protocol.c
 * 
 * ---------------------------------------------------------------------
 * Description : code associated with handling the UCI protocol 
 * --------------------------------------------------------------------- 
 * 
 *  
 * 
 * Copyright (C) 2016 Eddie McNally <emcn@gmx.com>
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
 * 
 */
#include <stdio.h>
#include "types.h"
#include "move_gen_utils.h"
#include "uci_protocol.h"


/*
 * Prints best move in UCI format
 */
void uci_print_bestmove(mv_bitmap mv){
	printf("bestmove %s\n", print_move(mv));
}

void uci_print_info_score(int32_t best_score, uint8_t depth, uint32_t nodes, uint64_t time_in_ms, uint8_t num_pv_moves, mv_bitmap *pv_line){
	printf("info score cp %d depth %d nodes %d time %jd ", best_score, depth, nodes,time_in_ms);
	printf("pv ");
	for(uint8_t i = 0; i < num_pv_moves; i++){
		printf(" %s", print_move(pv_line[i]));
	}
	printf("\n");
}

void uci_print_hello(){
    printf("id name %s\n", ENGINE_NAME);
    printf("id author %s\n", AUTHOR);
    printf("uciok\n");
}

void uci_print_ready(){
    printf("readyok\n");
}

