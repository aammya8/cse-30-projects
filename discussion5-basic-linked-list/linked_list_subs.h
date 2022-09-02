#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct node {
  int data1;
  int data2;
  struct node *next;
};

struct node *creatNode(int data1, int data2,     struct node *link);
struct node *insertEnd(int data1, int data2,struct node *head);
struct node *findNode(int data, struct node *ptr);
struct node *deleteNode(int data1, struct node *head);
void         freeLinkedList(struct node* head);

#endif //LINKED_LIST_H