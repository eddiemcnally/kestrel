/*
 * utils.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: misc utilities for the application
 * ---------------------------------------------------------------------
 *
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
#define _GNU_SOURCE
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <sched.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "types.h"
#include "utils.h"



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


void
print_stacktrace (void)
{
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = (size_t)backtrace (array, 10);
    strings = backtrace_symbols (array, (int)size);

    printf ("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++) {
        printf ("%s\n", strings[i]);
    }

    free (strings);
}



uint64_t get_time_of_day_in_millis(void)
{
    struct timeval tp;

    int errno = gettimeofday(&tp, NULL);

    if (errno == 0) {
        return (uint64_t) (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    }

    return 0;
}

uint64_t get_elapsed_time_in_millis(uint64_t start_time)
{
    uint64_t now_in_millis = get_time_of_day_in_millis();
    return (now_in_millis - start_time);
}

