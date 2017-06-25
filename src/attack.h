/*
 * attack.h
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
#include "kestrel.h"



void init_attack_framework(void);
bool is_sq_attacked(const struct board *brd, enum square sq,
                    enum colour attacking_side);

bool is_attacked_horizontally_or_vertically(const struct board *brd, enum square sq_one, enum square sq_two);
bool is_attacked_diagonally(const struct board *brd, enum square attacking_sq, enum square target_sq);
bool is_knight_attacking_square(const struct board *brd, uint64_t sq_bb, enum piece attacking_piece);
bool is_king_attacking_square(const struct board *brd, uint64_t sq_bb, enum colour col);
