/*
 * pv_table.c
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
#include <stdbool.h>
#include "types.h"
#include "move.h"
#include "pv_table.h"

#define MAX_TABLE_RAM 	1024 * 1024 * 2

static void clear_table(const struct pv_table *tab);
static U64 get_index(const struct board *brd);


void init_pv_table(struct pv_table *tab)
{
	U32 num_entries = MAX_TABLE_RAM / sizeof(struct pv_entry);

	tab->num_entries = num_entries;
	tab->table = (struct pv_entry *)malloc(MAX_TABLE_RAM);

	clear_table(tab);
}

static void clear_table(const struct pv_table *tab)
{
	struct pv_entry *elem = tab->table;

	for (U32 i = 0; i < tab->num_entries; i++) {
		elem->move = NO_MOVE;
		elem->hashkey = 0ull;
		elem++;
	}
}


void add_move(const struct board *brd, const mv_bitmap move){
	U64 index = get_index(brd);
	
	brd->pvtable->table[index].move = move;
	brd->pvtable->table[index].hashkey = brd->board_hash;
}


mv_bitmap find_move(const struct board *brd){
	U64 index = get_index(brd);
	
	if(brd->pvtable->table[index].hashkey == brd->board_hash) {
		return brd->pvtable->table[index].move;
	}
	return NO_MOVE;
}

static inline U64 get_index(const struct board *brd){
	return brd->board_hash % brd->pvtable->num_entries;
}
