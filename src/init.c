/*
 * init.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: code to init a board from a FEN string
 * ---------------------------------------------------------------------
 *
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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "board.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "hashkeys.h"
#include "attack.h"
#include "init.h"
#include "board_utils.h"

struct board init_game(char *fen)
{
	init_hash_keys();
	init_move_gen_framework();
	init_attack_framework();	

	struct board brd;
	init_board(fen, &brd);

	ASSERT_BOARD_OK(&brd);

	return brd;
}
