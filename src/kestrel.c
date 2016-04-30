/*
 * kestrel.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: main entry point for kestrel chess engine
 * ---------------------------------------------------------------------
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "types.h"
#include "board.h"
#include "evaluate.h"
#include "makemove.h"
#include "board_utils.h"
#include "hashkeys.h"
#include "init.h"
#include "occupancy_mask.h"
#include "fen.h"
#include "utils.h"
#include "tt.h"
#include "search.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "uci_protocol.h"


// sample game positions
//#define MATE_IN_TWO			"1r3rk1/1pnnq1bR/p1pp2B1/P2P1p2/1PP1pP2/2B3P1/5PK1/2Q4R w - - 0 1"
//#define MATE_IN_FIVE		"8/R7/4kPP1/3ppp2/3B1P2/1K1P1P2/8/8 w - - 0 1"
//#define MATE_IN_FOUR		"k1K5/p7/P1N5/1P6/4pP2/2p1P3/pp6/r3Q3 w - - 0 1"
//#define WAC1 				"r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"
//#define SAMPLE_POSITION		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"



#define INPUTBUFFER 5000

static void do_uci_loop(void);

int main(int argc, char **argv)
{
    if (argc > 1) {
        printf("%d", **argv);
    }

    // set process pri and cpu affinity for max performance
    set_priority_and_affinity();
    /*
    	struct board brd = init_game(SAMPLE_POSITION);

    	struct search_info si = {0};
    	si.depth = 4;
    	search_positions(&brd, &si, 64000000);
    */
    do_uci_loop();

    return 0;
}


// code courtesy of BlueFever Software (but modified/adapted)
static void do_uci_loop()
{

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char line[INPUTBUFFER];



    init_game_no_board();

    struct board brd;
    get_clean_board(&brd);

    struct search_info si;
    init_search_struct(&si);


    uci_print_hello();

    while (true) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if (!fgets(line, INPUTBUFFER, stdin)) {
            continue;
        }

        if (line[0] == '\n') {
            continue;
        }

        if (!strncmp(line, "isready", 7)) {
            uci_print_ready();
            continue;
        } else if (!strncmp(line, "position", 8)) {
            uci_parse_position(line, &brd);
        } else if (!strncmp(line, "ucinewgame", 10)) {
            uci_parse_position("position startpos\n", &brd);
//        } else if (!strncmp(line, "go", 2)) {
//            ParseGo(line, info, pos);
        } else if (!strncmp(line, "quit", 4)) {
            si.stop_search = true;
            break;
        } else if (!strncmp(line, "uci", 3)) {
            uci_print_hello();
        }
        if (si.stop_search == true) {
            break;
        }
    }
}


