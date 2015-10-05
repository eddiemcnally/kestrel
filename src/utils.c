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
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sched.h>
#include <sys/times.h>
#include <sys/resource.h>
#include "types.h"
#include "utils.h"

static U8 reverse_bits_in_byte(U8 bits);

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

void set_priority_and_affinity(void)
{
	// set up CPU affinity
	cpu_set_t my_set;
	CPU_ZERO(&my_set);
	CPU_SET(1, &my_set);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &my_set) > 0) {
		printf("affinity error");
		exit(-1);
	}
	// set process priority to max
	if (setpriority(PRIO_PROCESS, 0, PRIO_MAX) != 0) {
		printf("process priority error");
		exit(-1);
	}

}

/* Reverse the bits in a word
 *
 * name: reverse_bits
 * @param
 * @return
 *
 */
U64 reverse_bits(U64 word)
{

	U64 retval = 0;

	U8 *p_in = (U8 *) & word;
	U8 *p_out = (U8 *) & retval;

	for (int i = 0; i < 8; i++) {
		*p_out = reverse_bits_in_byte(*p_in);
		p_out++;
		p_in++;
	}

	return __builtin_bswap64(retval);
}

/* Reverses the bits in a byte
 * Taken from https://graphics.stanford.edu/~seander/bithacks.html
 * name: reverse_bits_in_byte
 * @param
 * @return
 *
 */
static inline U8 reverse_bits_in_byte(U8 bits)
{
	return (U8) ((bits * 0x0202020202ULL & 0x010884422010ULL) % 1023);
}
