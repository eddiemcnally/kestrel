/*
 * occupancy_mask_gen.h
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

#pragma once

#include "types.h"

//uint64_t get_occ_mask(enum piece pce, enum square sq);
void print_occupancy_masks(enum piece pce);
void print_mask_as_board(const uint64_t * mask);
void print_out_masks(const uint64_t * masks);
void generate_diagonal_occupancy_masks(void);
uint64_t get_occupancy_mask(enum piece pce, enum square sq);
void generate_rank_and_file_masks(void);

