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
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "makemove.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "pv_table.h"

static void init_table(struct pv_table *tab);
static struct pv_entry *get_empty_entry(void);
static void init_pv_entry(struct pv_entry *entry);
static uint64_t get_index(const uint64_t board_hash);

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

// dumps stats for the pv_table
void dump_pv_table_stats(const struct pv_table *table){
	printf("pv_table stats : \n");

	int num_empty = 0;
	int num_non_empty = 0;
	int num_items = 0;

	for(int i = 0; i < NUM_PV_ENTRIES; i++){

		struct pv_entry *entry = &table->entries[i];
		if (entry->move != NO_MOVE){
			num_non_empty++;
			num_items++;

			// count keys at this offset
			int num_keys = 1;

			while (entry->next != NULL) {
				entry = entry->next;
				num_keys++;
				num_items++;
			}
			//printf("\tindex %d \t : %d\n", i, num_keys);
		} else {
			num_empty++;
		}
	}
	printf("\tnum_empty = %d\n", num_empty);
	printf("\tnum_non_empty = %d\n", num_non_empty);
	printf("\tnum_items = %d\n", num_items);

}



void add_move_to_pv_table(const struct pv_table *table, const uint64_t board_hash,
	      const mv_bitmap move)
{
	//printf("Adding mv to brd : %s\n", print_move(move));
		
	uint64_t index = get_index(board_hash);

	struct pv_entry *entry = &table->entries[index];

	if (entry->move == NO_MOVE) {
		// empty slot, so just add the move
		entry->move = move;
		entry->hashkey = board_hash;
	} else {
		//printf("KEY Collision on add()...mv = %s, hash = %ju\n", print_move(move), board_hash);
		// key collision
		// create new pv_entry and add to list
		struct pv_entry *new = get_empty_entry();
		// set it up
		new->move = move;
		new->hashkey = board_hash;

		// add to start of list
		new->next = entry->next;
		entry->next = new;
	}
}

inline mv_bitmap find_move(const struct pv_table *table, const uint64_t board_hash)
{
	uint64_t index = get_index(board_hash);

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
	for (uint32_t i = 0; i < NUM_PV_ENTRIES; i++) {

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

static inline uint64_t get_index(const uint64_t board_hash)
{
	return board_hash % NUM_PV_ENTRIES;
}
