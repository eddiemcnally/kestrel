/*
 * board_utils.c
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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "pieces.h"




/*
 *
 * name: Takes a char array as FEN notation, and returns a populated
 * board
 * @param char * representing the FEN string
 * @return a populated board
 *
 */
//board_container_t consume_fen_notation(char *fen_string){

	////example of starting poition:
	////rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

	//// step 1: ignore moves, castling, etc, just return the board

	////char *str = strdup(fen_string);  	// copy so we own the str (it's modified by strsep())
	////char *token;
	////int rank = 7;
	 						////// FEN notation starts are rank 8
	////while ((token = strsep(&str_copy, ","))) {

		////my_fn(token);

	////}
	////free(str);
	//return void;
//}
