/*
 * main.c
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chess is distributed in the hope that it will be useful, but
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
#include "board.h"
#include "board_utils.h"
#include "hashkeys.h"
#include "init.h"
#include "fen.h"


#define FEN1 	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN2  	"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN3	"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"

int main(int argc, char **argv)
{
	init_hash_keys();
	
	
	board_container_t * the_board = get_clean_board();
    consume_fen_notation(FEN1, the_board);
    print_board(the_board);
    
    
	the_board = get_clean_board();
    consume_fen_notation(FEN2, the_board);
    print_board(the_board);
    
    
	the_board = get_clean_board();
    consume_fen_notation(FEN3, the_board);
    print_board(the_board);
    
    return 0;
}
