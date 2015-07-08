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
#include "types.h"
#include "move.h"
#include "board.h"
#include "board_utils.h"
#include "hashkeys.h"
#include "init.h"
#include "fen.h"


#define FEN1 	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN2  	"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN3	"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN4	"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN5 	"8/8/8/R1P3p1/8/8/8/8 w - - 0 1"
int main(int argc, char **argv)
{
	if (argc > 0){
		printf("%d", **argv);
	}
	
	
	init_hash_keys();

	board_container_t * the_board = get_clean_board();
    consume_fen_notation(FEN5, the_board);
    
    board_t rook = get_occ_mask(W_ROOK, a5);
    printf("ROOK : 0x%016llx\n", rook);
    
    board_t pawn = 0;
    set_bit(&pawn, g5);
    printf("PAWN : 0x%016llx\n", pawn);
    
    
    
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

    
    return 0;
}
