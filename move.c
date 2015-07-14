/*
 * move.c
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
#include "types.h"
#include "board.h"
#include "pieces.h"
#include "move.h"



/*
 * Prints out the algebraic notatio of a move (eg, a2a4)
 * name: print_move
 * @param
 * @return
 * 
 */
char * print_move(const MOVE * m) {

	static char move_string[6];
	
	int from_file = GET_FILE(FROMSQ(m->move));
	int from_rank = GET_RANK(FROMSQ(m->move));
	
	int to_file = GET_FILE(TOSQ(m->move));
	int to_rank = GET_RANK(TOSQ(m->move));
	
	PIECE promoted_pce = PROMOTED(m->move);
	
	if(promoted_pce > 0) {
		char pchar = 'q';	
		if(isKn(promoted_pce)) {
			pchar = 'n';
		} else if(isR(promoted_pce))  {
			pchar = 'r';
		} else if(isB(promoted_pce))  {
			pchar = 'b';
		}
		sprintf(move_string, "%c%c%c%c%c", ('a'+from_file), ('1'+from_rank), ('a'+to_file), ('1'+to_rank), pchar);
	} else {
		sprintf(move_string, "%c%c%c%c", ('a'+from_file), ('1'+from_rank), ('a'+to_file), ('1'+to_rank));
	}
	
	return move_string;
}
