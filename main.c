/*
 * main.c
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
#include <assert.h>
#include <string.h>
#include "types.h"
#include "move.h"
#include "board.h"
#include "board_utils.h"
#include "hashkeys.h"
#include "init.h"
#include "occupancy_mask.h"
#include "fen.h"
#include "move.h"

#define FEN1 	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN2  	"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN3	"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN4	"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN5 	"8/8/8/R1P3p1/8/8/8/8 w - - 0 1"
#define FEN6	"rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define FEN7	"rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"

int main(int argc, char **argv)
{
    if (argc > 0) {
		printf("%d", **argv);
    }

    init_hash_keys();

    //struct board *the_board = get_clean_board();
    //consume_fen_notation(FEN5, the_board);

    //U64 rook = GET_ROOK_OCC_MASK(a5);
    //print_occupancy_mask_as_board(&rook, W_ROOK, a5);

   // printf("ROOK : 0x%016llx\n", rook);

    //U64 pawn = 0;
    //set_bit(&pawn, g5);
    // printf("PAWN : 0x%016llx\n", pawn);

    //printf("AND'ed : 0x%016llx\n", pawn & rook);

    //print_board(the_board);
    //ASSERT_BOARD_OK(the_board);

    //print_occupancy_masks(B_KNIGHT);

    //the_board = get_clean_board();
    //consume_fen_notation(FEN2, the_board);
    //print_board(the_board);
    //ASSERT_BOARD_OK(the_board);

    //the_board = get_clean_board();
    //consume_fen_notation(FEN3, the_board);
    //print_board(the_board);
    //ASSERT_BOARD_OK(the_board);

    //the_board = get_clean_board();
    //consume_fen_notation(FEN4, the_board);
    //print_board(the_board);
    //ASSERT_BOARD_OK(the_board);

    //the_board = get_clean_board();
    //consume_fen_notation(FEN6, the_board);
	//print_board(the_board);
	//printf("creating movelist\n");



    //struct move_list *list = malloc(sizeof(struct move_list));
	//memset(list, 0, sizeof(struct move_list));
	//printf("calling gen move list\n");

    //generate_all_moves(the_board, list);

    //printf("calling print moves\n");
    //print_move_list(list);




    //the_board = get_clean_board();
    //consume_fen_notation(FEN7, the_board);
	//print_board(the_board);
	//printf("creating movelist\n");



    //list = malloc(sizeof(struct move_list));
	//memset(list, 0, sizeof(struct move_list));
	//printf("calling gen move list\n");

    //generate_all_moves(the_board, list);

    //printf("calling print moves\n");
    //print_move_list(list);


/*
	char * sliding_test = "K7/1rp5/5R1P/6p1/7P/1k3p1P/1P1p2r1/4R3 w - - 0 1";
    struct board *brd= get_clean_board();
    //print_board(brd);
    consume_fen_notation(sliding_test, brd);

    struct move_list *list = malloc(sizeof(struct move_list));
	memset(list, 0, sizeof(struct move_list));

	generate_sliding_horizontal_vertical_moves(brd, list, B_ROOK);


	generate_sliding_horizontal_vertical_moves(brd, list, W_ROOK);
*/



    return 0;
}
