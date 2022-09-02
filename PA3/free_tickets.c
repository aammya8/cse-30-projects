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
 * free_tickets
 * 
 * usage: Frees all allocated memory in database. Iterates through each hash
 *        chain and frees all the vehicles and the tickets for each vehicle.
 *        Count and prints total number of tickets freed in the database.
 * 
 * args:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 */

void
free_tickets(struct vehicle **hashtab, uint32_t tabsz)
{
    if (hashtab == NULL)
        return;
    
    unsigned long cnt = 0UL;        /* total freed tickets count */
    struct vehicle *cur_vh = NULL;  /* pointer to current vehicle in hashtab */
    struct vehicle *next_vh = NULL; /* pointer to next vehicle in hashtab */
    struct ticket *cur_tk = NULL;   /* pointer to current ticket of vehicle */
    struct ticket *next_tk = NULL;  /* pointer to next ticket of vehicle */

    /* 
     * Iterate hashtable. At every hash table index, iterate vehicle chain.
     * For each vehicle, iterate through all tickets. Free memory in the
     * order 1) ticket chain, 2) vehicle chain, 3) hash table at index i 
     * (in order from first freed to last freed) 
     */
    for (uint32_t i = 0; i < tabsz; i++) {
        cur_vh = hashtab[i];
        while (cur_vh != NULL)
        {
            cur_tk = cur_vh->head;
            while (cur_tk != NULL)
            {
                next_tk = cur_tk->next;
                free(cur_tk);
                cnt++;
                cur_tk = next_tk;
            }
            cur_vh->head = NULL;

            next_vh = cur_vh->next;
            free(cur_vh->state);
            cur_vh->state = NULL;
            free(cur_vh->plate);
            cur_vh->plate = NULL;
            free(cur_vh);
            cur_vh = next_vh;
        }
        hashtab[i] = NULL;
    }

    free(hashtab);
    hashtab = NULL;

    printf("Total tickets freed: %lu\n", cnt);
    return;
} 

#endif
