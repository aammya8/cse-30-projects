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
 * del_ticket
 *      
 * usage: Removes ticket (summons) from the database. Looks up the summons for
 *        a vehicle (state, plate) and pays it once found (deletes the ticket
 *        from the ticket chain). Updates tot_fine and cnt_tickets fields in
 *        struct vehicle to account for ticket removal. If vehicle no longer 
 *        has any tickets, removes the vehicle from hash chain.  
 *
 * args:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  fineTab table mapping fine code to description and fine   
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *  argv    for error printing
 *
 * returns:  0 if ticket sucessfully inserted without error
 *          -1 for all errors or if ticket not found
 */

int
del_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
    char *plate, char *state, char *summ, char **argv)
{
    struct vehicle *vh_ptr;   /* ptr to vehicle associated to target ticket */
    vh_ptr = vehicle_lookup(hashtab, tabsz, plate, state, argv);

    if (vh_ptr == NULL) 
        return -1;
    
    unsigned long long summ_id;  /* ticket summons id */
    if (strtosumid(summ, &summ_id, argv) != 0)
        return -1;
    
    struct ticket *cur_tk = vh_ptr->head; /* ptr to head of ticket chain */
    struct ticket *prev_tk = NULL;        /* ptr to ticket prev of cur_tk */
    int found = 0;                        /* 1 if ticket found, 0 otherwise */

    /* 
     * Iterate through ticket list and delete ticket with summons number
     * of summ_id, if found. 
     */
    while (cur_tk != NULL) {

        if (cur_tk->summons == summ_id) {

            found = 1;
            vh_ptr->tot_fine -= fineTab[cur_tk->code].fine;
            vh_ptr->cnt_ticket -= 1;

            /* 
             * ========= cur_tk not head of ticket list ========= 
             */
            if (cur_tk != vh_ptr->head) {
                prev_tk->next = cur_tk->next;
                free(cur_tk);
                cur_tk = NULL;
                break;
            }


            /*
             * ========= cur_tk is head of ticket list ==========
             */
 
            /*  
             * Fix vehicle chain pointers and remove vehicle from hashtable if
             * cur_tk is the only ticket associated with vehicle pointed to by
             * vh_ptr.
             * 
             */
            if(cur_tk->next == NULL) {
                uint32_t hash_idx;       /* hash value of vh_ptr */
                struct vehicle *cur_vh;  /* ptr to first vehicle at hash_idx */
                struct vehicle *prev_vh; /* ptr to first vehicle at hash_idx */

                hash_idx = hash(vh_ptr->plate, argv) % tabsz;
                cur_vh = hashtab[hash_idx];
                prev_vh = NULL;
                
                /* 
                 * Find pointer to previous vehicle of vehicle to be removed.
                 * Remove target vehicle node from the vehicle linked list by
                 * fixing pointers
                 */
                while(cur_vh != NULL) {
                    if ((strcmp(cur_vh->state, state) == 0) &&
                        (strcmp(cur_vh->plate, plate) == 0)) {
                        if (prev_vh == NULL) {
                            hashtab[hash_idx] = cur_vh->next;
                            break;
                        }
                        prev_vh->next = cur_vh->next;
                        break;
                    }
                    prev_vh = cur_vh; 
                    cur_vh = cur_vh->next;                
                }
     
                /* 
                 * free memory and set pointers to NULL
                 */
                free(cur_tk);
                cur_tk = NULL;
                free(vh_ptr->state);
                vh_ptr->state = NULL;
                free(vh_ptr->plate);
                vh_ptr->plate = NULL;
                free(vh_ptr);
                vh_ptr = NULL;
                break;
            }

            /* 
             * If cur_tk is NOT the only ticket associated with vehicle pointed
             * to by vh_ptr, then remove ticket from ticket chain by fixing the
             * head pointer of vh_ptr. Then, free memory.
             */
            vh_ptr->head = cur_tk->next;
            free(cur_tk);
            cur_tk = NULL;
            break;
        }

        /* 
         * summ_id match not yet found, update pointers to continue iteration
         */
        else {
            prev_tk = cur_tk;
            cur_tk = cur_tk->next;
        }
    }

    if (found == 0)
        return -1;
    
    return 0;
} 

#endif
