/*
 * search.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for searching for moves and determining
 * the best ones
 * ---------------------------------------------------------------------
 *
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
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "search.h"
#include "attack.h"
#include "evaluate.h"
#include "board.h"
#include "pieces.h"
#include "makemove.h"
#include "pv_table.h"
#include "board_utils.h"
#include "move.h"
#include "utils.h"


// checks to see if most recent move is a repetition
inline bool is_repetition(const struct board *brd)
{
	// the 50move counter is reset when a pawn moves or piece taken
	// (these are moves that can't be repeated), so we only need to
	// search the history from the last time the counter was reset

	int start = brd->history_ply - brd->fifty_move_counter;

	for (int i = start; i < brd->history_ply; i++) {
		if (brd->board_hash == brd->history[i].board_hash) {
			return true;
		}
	}
	return false;
}


