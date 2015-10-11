/*
 * utils.h
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

#ifndef _UTILS_H_
#define _UTILS_H_

#include "types.h"

U64 generate_rand64(void);
void set_priority_and_affinity(void);
U64 get_time_in_millis(void);
U64 get_elapsed_time_in_millis(U64 start_time);


// These are the same, but use different name to aid reading if code
#define IS_SQUARE_OCCUPIED(BB, SQ)	( ( (BB >> SQ) & 0x01ull) != 0)
#define CHECK_BIT(BB, SQ)				(IS_SQUARE_OCCUPIED(BB, SQ))

#endif
