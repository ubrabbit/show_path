#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifndef DEFINED_LIST
#define DEFINED_LIST

#define CMP_BIGGER 1
#define CMP_SMALLER -1
#define CMP_EQUAL 0


typedef struct list_node
{
        struct list_node* next;
        struct list_node* prev;
        void* data_ptr;
}List_Node;

typedef int (*List_Cmp_Func)(void* data_1, void* data_2);

typedef struct  link_list
{
        struct  list_node* head;
        struct  list_node* tail;
        List_Cmp_Func   Node_Cmp_Func;
}Link_List;


Link_List* CreateList(List_Cmp_Func func);

int ListIsEmpty(Link_List *list);

void ListAddNode(Link_List* list, void* ptr);

void* ListIndexNode(Link_List* list, long idx);

int ListDelNode(Link_List* list, void *ptr);

void* ListPopNode(Link_List* list);

void ListFreeAll(Link_List *list);

void ListFreeNode(List_Node* node);

#endif
