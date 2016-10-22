/*
 * move_gen_utils.h
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

#include <stdbool.h>
#include "types.h"
#include "move_gen.h"
#include "makemove.h"

uint32_t get_score(mv_bitmap mv);
mv_bitmap get_move(mv_bitmap mv);
void add_to_score(mv_bitmap *mv, uint32_t to_add);


void validate_move_list(struct move_list *mvl);

struct move_list *get_empty_move_list(void);
bool is_move_in_list(struct move_list *mvl, mv_bitmap mv);
uint64_t reverse_bits(uint64_t word);
uint8_t pop_1st_bit(uint64_t * bb);
char *print_move(mv_bitmap move_bitmap);
void print_move_details(mv_bitmap move_bitmap);
void print_move_list(const struct move_list *list);
void print_move_list_details(const struct move_list *list);
void print_board_and_move(struct board *brd, mv_bitmap move_bitmap);



