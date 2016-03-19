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
#include <string.h>
#include "types.h"
#include "fen.h"
#include "board_utils.h"
#include "move_gen_utils.h"
#include "uci_protocol.h"


// NOTE : the code in this file was taken from BlueFever Software
// and modified/adapter. Thanks guys :-)


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

// parses the UCI "position" command which is of the format
// 		position [fen <fenstring> | startpos ]  moves <move1> .... <movei>
// The line argument points to the start of the string, and includes 
// the "position" characters
void uci_parse_position(char *line, struct board *brd){

	// skip over the "position "
	line += 9;
	char *pc = line;
		
	if(strncmp(line, "startpos", 8) == 0){
        consume_fen_notation(STARTING_FEN, brd);
    } else {
        pc = strstr(line, "fen");
        if(pc == NULL) {
            consume_fen_notation(STARTING_FEN, brd);
        } else {
			// skip over "fen "
            pc += 4;
            consume_fen_notation(pc, brd);
        }
    }
	
	// now, process the moves
	pc = strstr(line, "moves");
	mv_bitmap mv;
	
	if(pc != NULL) {
		// skip over "moves "
        pc += 6;
        while(*pc) {
              mv = parse_move(pc, brd);
			  if(mv == NO_MOVE) 
				break;
			  
			  make_move(brd, mv);
              brd->ply=0;
              
              while(*pc && *pc != ' ') 
				pc++;
              pc++;
        }
    }

}



/*
void uci_parse_go(char *line, struct search_info *si, struct board *brd){
    
	int depth = -1, movestogo = 30,movetime = -1;
	int time = -1, inc = 0;
    char *ptr = NULL;
	info->timeset = FALSE;
	
	if ((ptr = strstr(line,"infinite"))) {
		;
	} 
	
	if ((ptr = strstr(line,"binc")) && pos->side == BLACK) {
		inc = atoi(ptr + 5);
	}
	
	if ((ptr = strstr(line,"winc")) && pos->side == WHITE) {
		inc = atoi(ptr + 5);
	} 
	
	if ((ptr = strstr(line,"wtime")) && pos->side == WHITE) {
		time = atoi(ptr + 6);
	} 
	  
	if ((ptr = strstr(line,"btime")) && pos->side == BLACK) {
		time = atoi(ptr + 6);
	} 
	  
	if ((ptr = strstr(line,"movestogo"))) {
		movestogo = atoi(ptr + 10);
	} 
	  
	if ((ptr = strstr(line,"movetime"))) {
		movetime = atoi(ptr + 9);
	}
	  
	if ((ptr = strstr(line,"depth"))) {
		depth = atoi(ptr + 6);
	} 
	
	if(movetime != -1) {
		time = movetime;
		movestogo = 1;
	}
	
	info->starttime = GetTimeMs();
	info->depth = depth;
	
	if(time != -1) {
		info->timeset = TRUE;
		time /= movestogo;
		time -= 50;		
		info->stoptime = info->starttime + time + inc;
	} 
	
	if(depth == -1) {
		info->depth = MAXDEPTH;
	}
	
	printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
		time,info->starttime,info->stoptime,info->depth,info->timeset);
	SearchPosition(pos, info);
}

*/





void uci_print_ready(){
    printf("readyok\n");
}

