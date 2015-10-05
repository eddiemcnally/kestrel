/*
 * search.c
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
#include <stdbool.h>
#include "types.h"
#include "search.h"
#include "board.h"
#include "board_utils.h"
#include "utils.h"

// checks to see if most recent move is a repetition
bool is_repetition(const struct board * brd)
{
	// only have to look since last fifty move counter was reset
	U8 start = (U8) (brd->history_ply - brd->fifty_move_counter);
	for (U8 i = start; i < brd->history_ply - 1; i++) {
		if (brd->board_hash == brd->history[i].board_hash) {
			return true;
		}
	}
	return false;
}
