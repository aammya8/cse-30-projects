#include <stdlib.h>
#include <stdio.h>

#include "linked_list_subs.h"

struct node *creatNode(int data1, int data2, struct node *link)
{
    struct node *ptr = malloc(sizeof(*ptr));
    if (ptr != NULL) {
        ptr->data1 = data1;
        ptr->data2 = data2;
        ptr->next = link;
    }
    return ptr;
}

struct node *insertEnd(int data1, int data2,struct node *head)
{
    struct node *ptr = head;
    struct node *prev = head;
    struct node *new;

    if ((new = creatNode(data1, data2, NULL)) == NULL)
        return NULL;

    while (ptr != NULL) {
        prev = ptr;
        ptr = ptr->next;
    }
    if (prev == NULL)
        return new;
    prev->next = new;
    return head;
}

struct node *findNode(int data, struct node *ptr)
{
    while (ptr != NULL) {
        if (ptr->data1 == data)
            break;
        ptr = ptr->next;
    }
    return ptr;
}

struct node *deleteNode(int data1, struct node *head)
{
    struct node *ptr = head;
    struct node *prev = head;

    while (ptr != NULL) {
        if (ptr->data1 == data1)
        break;
        prev = ptr;
        ptr = ptr->next;
    }
    if (ptr == NULL) // not found return head
        return head;
    if (ptr == head)
        head = ptr->next;
    else
        prev->next = ptr->next;
    free(ptr);
    return head;
}

void freeLinkedList(struct node* head)
{
   struct node* next;

   while (head != NULL)
    {
       next = head;
       head = head->next;
       free(next);
    }
    
    printf("\nHead pointer after freeing the list: %lu\n", (unsigned long)head);

}
