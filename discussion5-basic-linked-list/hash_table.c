#include <stdlib.h>
#include <stdio.h>

#include "linked_list_subs.h"

#define TBSZ 3


int main(void)
{
    struct node *ptr;
    struct node **table;
    u_int32_t index;

    if ((table = calloc(TBSZ, sizeof(*table))) == NULL) {
        fprintf(stderr,"Cannot allocate hash table\n");
        return EXIT_FAILURE;
    }

    index = 1933 % TBSZ; // hash(1933) % TBSZ
    if ((ptr = insertEnd(1933, 1, table[index])) != NULL)
        table[index] = ptr;

    index = 1955 % TBSZ;
    if ((ptr = insertEnd(1955, 3, table[index])) != NULL)
        table[index] = ptr;

    index = 2020 % TBSZ;
    if ((ptr = insertEnd(2020, 5, table[index])) != NULL)
        table[index] = ptr;

    index = 1956 % TBSZ;
    if ((ptr = insertEnd(1956, 4, table[index])) != NULL)
        table[index] = ptr;

    printf("\nDumping All Data after inserting\n");
    for (index = 0U; index < TBSZ; index++) {
        ptr = table[index];
        printf("chain: %d\n", index);

        while (ptr != NULL) {
            printf("data1: %d data2: %d\n", ptr->data1, ptr->data2);
            ptr = ptr->next;
        }
    }

    index = 2020 % TBSZ; 
    if ((ptr = findNode(2020, table[index])) != NULL)
        printf("\nFound data1: %d data2: %d\n", ptr->data1, ptr->data2);
    else
        printf("\nNot Found 2020\n");

    index = 1933 % TBSZ; 
    if ((ptr = deleteNode(1933, table[index])) != NULL)
        table[index] = ptr;
    printf("\nDumping All Data after deleting 1933\n");
    for (index = 0U; index < TBSZ; index++) {
        ptr = table[index];
        printf("chain: %d\n", index);

        while (ptr != NULL) {
            printf("data1: %d data2: %d\n", ptr->data1, ptr->data2);
            ptr = ptr->next;
        }
    }

    printf("\nFreeing the entire table\n");
    for (index = 0U; index < TBSZ; index++) {
        ptr = table[index];
        printf("chain: %d\n", index);

        freeLinkedList(ptr);
    }
    free(table);

    return 0;
}
