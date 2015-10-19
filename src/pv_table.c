/*
 * pv_table.c
 * 
 * ---------------------------------------------------------------------
 * DESCRIPTION: Maintains a hashtable of principle variation moves.
 * The hashtable is implemented as an array of linked lists.
 * --------------------------------------------------------------------- 
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
#include <stdbool.h>
#include "types.h"
#include "makemove.h"
#include "move.h"
#include "pv_table.h"

static void init_table(struct pv_table *tab);
static struct pv_entry *get_empty_entry(void);
static void init_pv_entry(struct pv_entry *entry);
static U64 get_index(const U64 board_hash);

/* Create and initialise a PV Table
 * 
 * name: create_pv_table
 * @param
 * @return
 * 
 */

struct pv_table *create_pv_table(void)
{
	struct pv_table *retval = malloc(sizeof(struct pv_table));

	struct pv_entry *entries =
	    malloc(NUM_PV_ENTRIES * sizeof(struct pv_entry));

	retval->entries = entries;

	init_table(retval);

	return retval;
}

void add_move(const struct pv_table *table, const U64 board_hash,
	      const mv_bitmap move)
{
	U64 index = get_index(board_hash);

	struct pv_entry *entry = &table->entries[index];

	if (entry->move == NO_MOVE) {
		// empty slot
		entry->move = move;
		entry->hashkey = board_hash;
	} else {
		// key collision
		// create new pv_entry and add to list
		struct pv_entry *new = get_empty_entry();
		// set it up
		new->move = move;
		new->hashkey = board_hash;

		// append to end of linked list
		while (entry->next != NULL) {
			entry = entry->next;
		}
		entry->next = new;
	}
}

mv_bitmap find_move(const struct pv_table *table, const U64 board_hash)
{
	U64 index = get_index(board_hash);

	struct pv_entry *entry = &table->entries[index];

	if (entry->move == NO_MOVE) {
		// slot empty
		return NO_MOVE;
	}

	if (entry->hashkey == board_hash) {
		// found entry
		return entry->move;
	}
	// see if there is a key collision and traverse any list
	while (entry->next != NULL) {
		entry = entry->next;
		if (entry->hashkey == board_hash) {
			// found entry
			return entry->move;
		}
	}

	return NO_MOVE;
}

void dispose_table(struct pv_table *table)
{
	// dispose of linked lists
	for (U32 i = 0; i < NUM_PV_ENTRIES; i++) {

		struct pv_entry *entry = &table->entries[i];
		if (entry->next != NULL) {
			// scan down lined list and free up each entry's memory
			struct pv_entry *pve = entry->next;
			while (pve != NULL) {
				entry = pve;

				pve = pve->next;
				free(entry);
			}
		}
	}

	// now dispose of array
	free(table->entries);

	// dispose table
	free(table);
}

I32 get_pv_line(const struct pv_table *table, struct board *brd,
		      const U8 depth)
{	
	mv_bitmap move = find_move(table, brd->board_hash);

	I32 count = 0;
	while (move != NO_MOVE && count < depth) {
		make_move(brd, move);
		brd->pv_array[count++] = move;

		move = find_move(table, brd->board_hash);
	}

	while (brd->ply > 0) {
		take_move(brd);
	}
	
	return count;
}

static inline void init_table(struct pv_table *tab)
{
	struct pv_entry *elem = &tab->entries[0];
	for (int i = 0; i < NUM_PV_ENTRIES; i++) {
		init_pv_entry(elem);
		elem++;
	}
}

static inline struct pv_entry *get_empty_entry()
{

	struct pv_entry *retval = malloc(sizeof(struct pv_entry));
	init_pv_entry(retval);
	return retval;
}

static inline void init_pv_entry(struct pv_entry *entry)
{
	entry->move = NO_MOVE;
	entry->hashkey = 0;
	entry->next = NULL;
}

static inline U64 get_index(const U64 board_hash)
{
	return board_hash % NUM_PV_ENTRIES;
}
