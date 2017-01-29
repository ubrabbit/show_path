#include "c_list.h"


Link_List* CreateList(List_Cmp_Func func){
        Link_List* list;
        list=(Link_List*)malloc(sizeof(Link_List));
        list->head=NULL;
        list->tail=NULL;
        list->Node_Cmp_Func=func;
        return list;
}


void ListAddNode(Link_List* list, void* ptr){
        //fprintf(stdout, "list add %d\n", node->value);

        List_Node* node;
        node=(List_Node*)malloc(sizeof(List_Node));
        node->next=NULL;
        node->prev=NULL;
        node->data_ptr=ptr;

        if(NULL==list->head){
                list->head=node;
                list->tail=node;
                node->prev=NULL;
                node->next=NULL;
                return;
        }

        list->tail->next=node;
        node->prev=list->tail;
        node->next=NULL;
        list->tail=node;

}


int ListIsEmpty(Link_List *list){
        return (NULL==list->head);
}


void ListFreeAll(Link_List *list){
        assert(ListIsEmpty(list));

        list->head=NULL;
        list->tail=NULL;
        free(list);
}


void ListFreeNode(List_Node* node){
        node->next=NULL;
        node->prev=NULL;
        free(node);
}


void* ListIndexNode(Link_List* list, long idx){
        long i;
        List_Node* node;

        if( idx <0 ) return NULL;

        i=0;
        node=list->head;
        while( i < idx && NULL != node ){
                node=node->next;
                i++;
        }

        if( NULL == node )      return NULL;

        return node->data_ptr;
}


int  ListDelNode_Step2(Link_List* list, List_Node* tmp){
        if( tmp==list->head && tmp==list->tail ){
                list->head=NULL;
                list->tail=NULL;
                return 1;
        }

        if( tmp==list->head ){
                list->head=list->head->next;
                list->head->prev=NULL;
                return 2;
        }

        if( tmp==list->tail ){
                list->tail->prev->next=NULL;
                list->tail=tmp->prev;
                return 3;
        }

        tmp->next->prev=tmp->prev;
        tmp->prev->next=tmp->next;
        return 0;
}


int ListDelNode(Link_List* list, void *ptr){
        //fprintf(stdout, "list del %d\n", value);

        int cmp;
        List_Node* tmp;
        tmp=list->head;
        cmp=CMP_SMALLER;
        while( NULL!=tmp ){
                if( NULL != list->Node_Cmp_Func )
                        cmp=list->Node_Cmp_Func(tmp->data_ptr,ptr);
                else if( tmp->data_ptr == ptr ){
                        cmp=CMP_EQUAL;
                }

                if( cmp == CMP_EQUAL ){
                        ListDelNode_Step2(list,tmp);
                        ListFreeNode(tmp);
                        return 1;
                }
                else{
                        tmp=tmp->next;
                }
        }
        return 0;
}


void* ListPopNode(Link_List* list){
        List_Node* tmp;
        void *ptr;

        if( NULL == list->head )         return NULL;

        tmp=list->head;
        ListDelNode_Step2(list,tmp);
        ptr=tmp->data_ptr;
        ListFreeNode(tmp);
        return ptr;
}

