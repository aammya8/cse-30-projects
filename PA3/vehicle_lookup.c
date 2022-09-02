#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * vehicle_lookup
 *
 * usage:    Searches for vehicle match in the database.
 *           Vehicle must match both plate and state strings.
 * args:
 *  hashtab  pointer to hashtable (pointer to an array of pointers)
 *  tabsz    number of elements in the hash table
 *  plate    plate id string to be found
 *  state    state id string to be found
 *  argv     for error printing
 *
 * returns:  pointer to vehicle if found 
 *           NULL otherwise
 */

struct vehicle *
vehicle_lookup(struct vehicle **hashtab, uint32_t tabsz, char *plate,
    char *state, char **argv)
{
    uint32_t hash_idx;      /* hash value of vehicle */
    struct vehicle *cur_vh; /* ptr to first vehicle at hash_idx */

    hash_idx = hash(plate, argv) % tabsz;
    cur_vh = hashtab[hash_idx];

    /* 
     * Iterate through vehicles in vehicle chain at hashtab[hash_idx].
     * If vehicle match (state, plate) found, return pointer to vehicle.
     */
    while (cur_vh != NULL) {
        if ((strcmp(cur_vh->state, state) == 0) &&
            (strcmp(cur_vh->plate, plate) == 0)) {
            return cur_vh;
        }
        cur_vh = cur_vh->next;
    }
    return NULL;
}

#endif
