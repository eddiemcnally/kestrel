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
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "kestrel.h"
#include "fen/fen.h"
#include "board_utils.h"
#include "move_gen_utils.h"
#include "uci_protocol.h"
#include "board.h"
#include "utils.h"

struct timeval tv;
struct timezone tz;


// NOTE : the code in this file was taken from BlueFever Software
// and modified/adapter. Thanks guys :-)


/*
 * Prints best move in UCI format
 */
void uci_print_bestmove(mv_bitmap mv)
{
    printf("bestmove %s\n", print_move(mv));
}

void uci_print_info_score(int32_t best_score, uint8_t depth, uint32_t nodes, uint64_t time_in_ms, uint8_t num_pv_moves, mv_bitmap *pv_line)
{
    //printf("info score cp %d depth %d nodes %d time %jd ", best_score, depth, nodes,time_in_ms);
    //printf("pv ");
    //for(uint8_t i = 0; i < num_pv_moves; i++) {
    //    printf(" %s", print_move(pv_line[i]));
   // }
    //printf("\n");
}

void uci_print_hello()
{
    printf("id name %s\n", ENGINE_NAME);
    printf("id author %s\n", AUTHOR);
    printf("uciok\n");
}

// parses the UCI "position" command which is of the format
// 		position [fen <fenstring> | startpos ]  moves <move1> .... <movei>
// The line argument points to the start of the string, and includes
// the "position" characters
void uci_parse_position(char *line, struct position *pos)
{

    // skip over the "position "
    line += 9;
    char *pc = line;

    if(strncmp(line, "startpos", 8) == 0) {
        consume_fen_notation(STARTING_FEN, pos);
    } else {
        pc = strstr(line, "fen");
        if(pc == NULL) {
            consume_fen_notation(STARTING_FEN, pos);
        } else {
            // skip over "fen "
            pc += 4;
            consume_fen_notation(pc, pos);
        }
    }

    // now, process the moves
    pc = strstr(line, "moves");
    mv_bitmap mv;

    if(pc != NULL) {
        // skip over "moves "
        pc += 6;
        while(*pc) {
            mv = parse_move(pc, pos);
            if(mv == NO_MOVE) {
                break;
            }

            make_move(pos, mv);
            set_ply(pos, 0);

            while(*pc && *pc != ' ') {
                pc++;
            }
            pc++;
        }
    }

}

#define NUM_BYTES	256
void read_input(struct search_info *si)
{
    ssize_t bytes;

    char input[NUM_BYTES] = "", *endc;

    if (uci_check_input_buffer()) {
        si->stop_search = true;
        do {
            bytes=read(fileno(stdin),input,NUM_BYTES);
        } while (bytes<0);

        endc = strchr(input,'\n');
        if (endc) {
            *endc=0;
        }

        if (strlen(input) > 0) {
            if (!strncmp(input, "quit", 4))    {
                si->exit = true;
            }
        }
        return;
    }
}


/*
 * Parses the "go" command.
 *
 * The spec for the go command is as follows:
 *
 * go
	start calculating on the current position set up with the "position" command.
	There are a number of commands that can follow this command, all will be sent in the same string.
	If one command is not sent its value should be interpreted as it would not influence the search.
	* searchmoves <move1> .... <movei>
		restrict search to this moves only
		Example: After "position startpos" and "go infinite searchmoves e2e4 d2d4"
		the engine should only search the two moves e2e4 and d2d4 in the initial position.
	* ponder
		start searching in pondering mode.
		Do not exit the search in ponder mode, even if it's mate!
		This means that the last move sent in in the position string is the ponder move.
		The engine can do what it wants to do, but after a "ponderhit" command
		it should execute the suggested move to ponder on. This means that the ponder move sent by
		the GUI can be interpreted as a recommendation about which move to ponder. However, if the
		engine decides to ponder on a different move, it should not display any mainlines as they are
		likely to be misinterpreted by the GUI because the GUI expects the engine to ponder
	   on the suggested move.
	* wtime <x>
		white has x msec left on the clock
	* btime <x>
		black has x msec left on the clock
	* winc <x>
		white increment per move in mseconds if x > 0
	* binc <x>
		black increment per move in mseconds if x > 0
	* movestogo <x>
      there are x moves to the next time control,
		this will only be sent if x > 0,
		if you don't get this and get the wtime and btime it's sudden death
	* depth <x>
		search x plies only.
	* nodes <x>
	   search x nodes only,
	* mate <x>
		search for a mate in x moves
	* movetime <x>
		search exactly x mseconds
	* infinite
		search until the "stop" command. Do not exit the search without being told so in this mode!

 */

void uci_parse_go(char *line, struct search_info *si, struct position *pos)
{

    int32_t depth = -1;
    int32_t move_time = -1;
    int32_t time = -1;
    int32_t incr = -1;
    int32_t moves_to_go = -1;
    char *ptr = NULL;

    si->search_time_set = false;

    if ((ptr = strstr(line,"infinite"))) {
        ;
    }

    // black incr per move in ms
    if ((ptr = strstr(line,"binc")) && get_side_to_move(pos) == BLACK) {
        incr = atoi(ptr + 5);	// skip over "binc "
    }
    // white incr per move in ms
    if ((ptr = strstr(line,"winc")) && get_side_to_move(pos) == WHITE) {
        incr = atoi(ptr + 5);	// skip over "winc "
    }
    // white's remaining time in ms
    if ((ptr = strstr(line,"wtime")) && get_side_to_move(pos) == WHITE) {
        time = atoi(ptr + 6); 	// skip over "wtime "
    }

    // black's remaining time in ms
    if ((ptr = strstr(line,"btime")) && get_side_to_move(pos) == BLACK) {
        time = atoi(ptr + 6);	// skip over "btime "
    }

    if ((ptr = strstr(line,"movestogo"))) {
        moves_to_go = atoi(ptr + 10);	// skip over "movestogo "
    }
    // time allowed for searching in ms
    if ((ptr = strstr(line,"movetime"))) {
        move_time = atoi(ptr + 9);		// skip over "movetime "
    }
    // number of plies to search
    if ((ptr = strstr(line,"depth"))) {
        depth = atoi(ptr + 6);		// skip over "depth "
    }

    if(move_time != -1) {
        time = move_time;
        moves_to_go = 1;
    }

    si->search_start_time = get_time_of_day_in_millis();
    si->depth = (uint8_t)depth;

    if(time != -1) {
        si->search_time_set = true;
        time /= moves_to_go;
        time -= 50;
        si->search_expiry_time = si->search_start_time
                                 + (uint64_t)time
                                 + (uint64_t)incr;
    }

    if(depth == -1) {
        si->depth = MAX_SEARCH_DEPTH;
    }

    printf("time:%d start:%jd stop:%jd depth:%d timeset:%i\n",
           time,si->search_start_time,si->search_expiry_time, si->depth,
           (int)si->search_time_set);
    search_positions(pos, si, 640000000);
}



// code taken from http://home.arcor.de/dreamlike/chess/
int uci_check_input_buffer()
{

    fd_set readfds;

    FD_ZERO (&readfds);
    FD_SET (fileno(stdin), &readfds);
    tv.tv_sec=0;
    tv.tv_usec=0;
    select(16, &readfds, 0, 0, &tv);

    return (FD_ISSET(fileno(stdin), &readfds));
}

void uci_print_ready()
{
    printf("readyok\n");
}

