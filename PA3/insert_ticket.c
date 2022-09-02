#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"
#include "readsubs.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * insert_ticket
 *
 * usage:  Adds a ticket (summons) to end of the ticket chain linked to the
 *         respective struct vehicle in the database. If the vehicle (plate, 
 *         state) is not in the database, then new struct vehicle entry is 
 *         made and inserted at the front of the collision chain in the hash
 *         table. tot_fine and cnt_tickets in the struct vehicle updated to 
 *         account for the new summons(ticket) just added.
 * 
 * args:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *          example hashval = hash(plate, argv) % tabsz;
 *  fineTab pointer to fine table; maps code number to description and cost
 *  summ    summons id string to be added 
 *  plate   plate id string to be added
 *  state   state id string to be added
 *  date    date of summons string
 *  code    summons code integer value used an an index into fine table
 *  argv    for error printing
 * 
 * returns:  0 if ticket sucessfully inserted without error
 *          -1 for all errors
 */

int
insert_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
    char *summ, char *plate, char *state, char *date, int code, char **argv)
{
    struct ticket *new_tk; /* new struct ticket entry */
    new_tk = (struct ticket*) malloc(sizeof(struct ticket));
    if (new_tk == NULL) {  
        fprintf(stderr, "%s: no memory can be allocated\n", *argv);
        return -1;
    }

    /* 
     * Initialize fields of new struct ticket 
     */
    unsigned long long summ_id;
    time_t date_val;
    if (strtosumid(summ, &summ_id, argv) != 0) 
        return -1;
    if (strtoDate(date, &date_val, argv) != 0)
        return -1;
    new_tk->summons = summ_id;
    new_tk->date = date_val;
    new_tk->code = code;
    new_tk->next = NULL;


    struct vehicle *recipient;  /* pointer to vehicle reciving ticket */
    recipient = vehicle_lookup(hashtab, tabsz, plate, state, argv);

    /* 
     * Create new struct vehicle entry and link new struct ticket entry if
     * vehicle associated to ticket does not yet exist in hash table.  
     */
    if (recipient == NULL) {
        struct vehicle *new_vh; /* new struct vehicle entry */
        uint32_t hash_index;    /* hash value of new vehicle in hashtable */

        hash_index = hash(plate, argv) % tabsz;

        new_vh = (struct vehicle*) malloc(sizeof(struct vehicle));
        if (new_vh == NULL) {  
            fprintf(stderr, "%s: no memory can be allocated\n", *argv);
            return -1;
        }

        /* 
         * Initialze fields of new struct vehicle and insert at front of
         * vehicle collision chain 
         */
        new_vh->state = strdup(state);
        new_vh->plate = strdup(plate);
        new_vh->tot_fine = fineTab[code].fine;
        new_vh->cnt_ticket = 1;
        new_vh->next = hashtab[hash_index];
        hashtab[hash_index] = new_vh; 
        new_vh->head = new_tk;

        return 0;
    }


    /* 
     * If vehicle associated to ticket already exists in hash table, link new
     * struct ticket to end of ticket chain linked to respective struct vehicle
     */
    struct ticket *last_tk = recipient->head;
    while (last_tk->next != NULL)
        last_tk = last_tk->next;
    last_tk->next = new_tk;


    /* 
     * Update tot_fine and cnt_ticket of vehicle struct to reflect new ticket
     */
    recipient->tot_fine += fineTab[code].fine;
    recipient->cnt_ticket += 1;

    return 0;
} 

#endif
