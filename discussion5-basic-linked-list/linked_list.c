#include <stdlib.h>
#include <stdio.h>

#include "linked_list_subs.h"

int main() {
    struct node *head = NULL; // insert at front
    struct node *ptr;

    if ((ptr = creatNode(2020, 1, head)) != NULL)
        head = ptr; // error handling not shown
    if ((ptr = creatNode(2019, 2, head)) != NULL)
        head = ptr;
    if ((ptr = creatNode(2018, 3, head)) != NULL)
        head = ptr;

    printf("\nDumping All Data after inserting at front\n");
    ptr = head;
    while (ptr != NULL) {
        printf("data1: %d data2: %d\n", ptr->data1, ptr->data2);
        ptr = ptr->next;
    }

    if ((ptr = insertEnd(2021, 4, head)) != NULL)
        head = ptr;
    if ((ptr = insertEnd(2022, 5, head)) != NULL)
        head = ptr;
    if ((ptr = insertEnd(2023, 6, head)) != NULL)
        head = ptr;

    printf("\nDumping All Data after inserting at end\n");
    ptr = head;
    while (ptr != NULL) {
        printf("data1: %d data2: %d\n", ptr->data1, ptr->data2);
        ptr = ptr->next;
    }

    struct node *found;

    if ((found = findNode(2005, head)) != NULL)
        printf("\nFound data data1: %d data2: %d\n", found->data1, found->data2);

    if ((found = findNode(2020, head)) != NULL)
        printf("\nFound data. data1: %d data2: %d\n", found->data1, found->data2);

    head = deleteNode(1955, head);
    head = deleteNode(2018, head);
    head = deleteNode(2019, head);

    printf("\nDumping All Data after deleting 1955, 2018, 2019\n");
    ptr = head;
    while (ptr != NULL) {
        printf("data1: %d data2: %d\n", ptr->data1, ptr->data2);
        ptr = ptr->next;
    }

    freeLinkedList(head);

    return 0;
}