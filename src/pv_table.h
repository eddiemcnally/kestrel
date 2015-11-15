/*
 * pv_table.h
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
#ifndef _PV_TABLE_H_
#define _PV_TABLE_H_

#include "types.h"

#define		NUM_PV_ENTRIES	(1024 * 1024)

struct pv_table *create_pv_table(void);
void dump_pv_table_stats(const struct pv_table *table);
uint8_t populate_pv_line(const struct pv_table *table, struct board *brd,
		      const uint8_t depth);
void add_move_to_pv_table(const struct pv_table *table, const uint64_t board_hash,
	      const mv_bitmap move);
mv_bitmap find_move(const struct pv_table *table, const uint64_t board_hash);
void dispose_table(struct pv_table *table);

#endif
