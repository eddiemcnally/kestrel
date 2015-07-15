/*
 * utils.c
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

#include <stdlib.h>
#include "types.h"
#include "utils.h"

U64 generate_rand64(void)
{
	U64 retval = 0;

	retval = (U64) rand();
	retval |= (U64) rand() << 15;
	retval |= (U64) rand() << 30;
	retval |= (U64) rand() << 45;
	retval |= ((U64) rand() & 0xf) << 60;

	return retval;
}
