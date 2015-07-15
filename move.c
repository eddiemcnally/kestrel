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

void add_quiet_move(struct board *brd, int move_bitmap,
		    struct move_list *mvlist)
{
	mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
	mvlist->moves[mvlist->move_count].score = 0;
	mvlist->move_count++;
}

void add_capture_move(struct board *brd, int move_bitmap,
		      struct move_list *mvlist)
{
	mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
	mvlist->moves[mvlist->move_count].score = 0;
	mvlist->move_count++;
}

void add_en_passent_move(struct board *brd, int move_bitmap,
			 struct move_list *mvlist)
{
	mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
	mvlist->moves[mvlist->move_count].score = 0;
	mvlist->move_count++;
}

/*
 * Prints out the algebraic notatio of a move (eg, a2a4)
 * name: print_move
 * @param
 * @return
 * 
 */
char *print_move(const U32 move_bitmap)
{

	static char move_string[6];

	int from_file = GET_FILE(FROMSQ(move_bitmap));
	int from_rank = GET_RANK(FROMSQ(move_bitmap));

	int to_file = GET_FILE(TOSQ(move_bitmap));
	int to_rank = GET_RANK(TOSQ(move_bitmap));

	enum piece promoted_pce = PROMOTED(move_bitmap);

	if (promoted_pce > 0) {
		char pchar = 'q';
		if (isKn(promoted_pce)) {
			pchar = 'n';
		} else if (isR(promoted_pce)) {
			pchar = 'r';
		} else if (isB(promoted_pce)) {
			pchar = 'b';
		}
		sprintf(move_string, "%c%c%c%c%c", ('a' + from_file),
			('1' + from_rank), ('a' + to_file), ('1' + to_rank),
			pchar);
	} else {
		sprintf(move_string, "%c%c%c%c", ('a' + from_file),
			('1' + from_rank), ('a' + to_file), ('1' + to_rank));
	}

	return move_string;
}

/*
 * Prints out the move list
 * name: print_move_list
 * @param
 * @return
 * 
 */

void print_move_list(const struct move_list *list)
{
	printf("MoveList:\n");

	for (int i = 0; i < list->move_count; i++) {
		U32 move = list->moves[i].move_bitmap;
		U32 score = list->moves[i].score;

		printf("Move:%d > %s (score:%d)\n", (i + 1), print_move(move),
		       score);
	}
	printf("MoveList Total %d Moves:\n\n", list->move_count);
}
