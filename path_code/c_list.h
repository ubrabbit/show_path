#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef DEFINED_LIST
#define DEFINED_LIST

typedef struct list_node
{
        struct list_node* next;
        struct list_node* prev;
        void* data_ptr;
}List_Node;


typedef struct  link_list
{
        struct  list_node* head;
        struct  list_node* tail;
}Link_List;


Link_List* CreateList(void);

int ListIsEmpty(Link_List *list);

void ListAddNode(Link_List* list, void* ptr);

void* ListPopNode(Link_List* list);

void ListFreeAll(Link_List *list);

void ListFreeNode(List_Node* node);

#endif
