/*
 * tt.c
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
#include "tt.h"



static void init_table(void);
static void set_tt_size(uint32_t size_in_bytes);



struct tt_entry {
	uint64_t hashkey;
	mv_bitmap move;
	uint32_t score;
	uint8_t flags;
	uint8_t depth;
};

static uint32_t tt_size = 0;
static struct tt_entry *tt = NULL;


void create_tt_table(uint32_t size_in_bytes)
{
	if (tt != NULL){
		dispose_tt_table();
	}
	
	set_tt_size(size_in_bytes);
	
	init_table();
}



void add_to_tt(const uint64_t board_hash, const mv_bitmap move)
{
	
    struct tt_entry * entry = &tt[board_hash & tt_size];
	
	entry->hashkey = board_hash;
	entry->move = move;
	
}

inline mv_bitmap probe_tt(const uint64_t board_hash)
{
    struct tt_entry * entry = &tt[board_hash & tt_size];

	if (entry->hashkey == board_hash){
		return entry->move;
	}
	return NO_MOVE;
}

void dispose_tt_table()
{
	if (tt != NULL){
		free(tt);
	}	
}

static inline void init_table()
{
	for(uint32_t i = 0; i < tt_size; i++){
	    struct tt_entry * entry = &tt[i];
		entry->hashkey = 0;
		entry->move = NO_MOVE;
		entry->score = 0;
		entry->depth = 0;
		entry->flags = 0;
	}
}

static void set_tt_size(uint32_t size_in_bytes) {

	if (tt != NULL){
		dispose_tt_table();
	}

	// round down to nearest power of 2
    if (size_in_bytes & (size_in_bytes - 1)) {
        size_in_bytes--;
        for (int i=1; i<32; i=i*2)
            size_in_bytes |= size_in_bytes >> i;
        size_in_bytes++;
        size_in_bytes>>=1;
    }

    if (size_in_bytes < sizeof(struct tt_entry)) {
        tt_size = 0;
    }

    tt_size = (uint32_t)((size_in_bytes / sizeof(struct tt_entry)) -1);
    tt = (struct tt_entry *) malloc(size_in_bytes);
}


