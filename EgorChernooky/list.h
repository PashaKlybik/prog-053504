#ifndef UNTITLED_LIST_H
#define UNTITLED_LIST_H
#include <stdlib.h>

struct node{
    int data;
    struct node *next, *prev;
};
struct node *node(int data, struct node *next, struct node *prev);

struct list{
    struct node *head;
    struct node *tail;
    int size;
};
struct list *list();
void pushBack(struct list *l, int value);
void popBack(struct list *l);
void pushFront(struct list *l, int value);
void popFront(struct list *l);
void freeList(struct list *l);

#endif //UNTITLED_LIST_H