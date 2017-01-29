/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-26 22:02:35
 *

gcc test.c c_hash.c c_list.c -g -Wall -o test

 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "c_list.h"
#include "c_hash.h"

typedef struct Test_Node{
        int key;
        char ptr[64];
}Test_Node;


Test_Node* Create_Test_Node(int key,char *ptr){
        Test_Node *node;

        node=(Test_Node*)malloc( sizeof(Test_Node) );
        node->key=key;
        memset(node->ptr,'\0',64);
        strcpy(node->ptr,ptr);
        fprintf(stdout, "node->ptr %s \n",node->ptr );
        return node;
}


void Print_List(Link_List* list, long idx){
        fprintf(stdout, "\nstart print list\n");

        List_Node* tmp;

        if(NULL==list->head){
                fprintf(stdout, "List is Empty!\n");
                return;
        }

        Hash_Unit *unit;
        Test_Node *node;
        tmp=list->head;
        while( NULL!=tmp ){
                unit=(Hash_Unit *)tmp->data_ptr;
                node=(Test_Node *)unit->data_ptr;
                fprintf(stdout, "In list idx=%ld key=%d ptr=%s \n",idx,node->key,node->ptr );
                tmp=tmp->next;
        }

        fprintf(stdout, "\n");
}


void Print_HashTable(Hash_Table* table){
        int i;
        Link_List* list;

        fprintf(stdout, "start Print_HashTable  size=%ld\n",table->size );

        for(i=0;i<table->size;i++){
                list=table->KeyList[i];
                if( ListIsEmpty(list) ) continue;
                //fprintf(stdout, ">>>>>>>>>>>>>>>>>> Print_HashTable key=%d\n",i );
                Print_List(list,i);
        }
}


void Test_Del_Hash(Hash_Table* table,int key){
        Test_Node *tmp;
        Hash_Unit *unit;

        unit=Hash_Node_By_Key(table,key);

        if( NULL == unit ){
                printf("Test_Del_Hash %d is Null \n", key);
        }
        else{
                tmp=(Test_Node*)unit->data_ptr;
                Hash_Del_Node(table,key);
                printf("Test_Del_Hash %d succ: %s \n", key,tmp->ptr);
        }
}


HASH_KEY_TYPE Test_Cmp_Func(Hash_Table* table, HASH_KEY_TYPE key){
        return key%table->size;
}


int Test_Free_Func(void* ptr){
        Test_Node       *tmp;

        tmp=(Test_Node *)ptr;
        free(tmp);

        return 1;
}


int main(void){
        Hash_Table* table;
        Test_Node *tmp;
        int i,key,size;
        char ptr[256]={'\0'};

        size=8;

        table=Create_Hash_Table(size,Test_Cmp_Func,Test_Free_Func);
        for(i=0;i<12;i++){
                sprintf(ptr,"Hash_Add_Node index: %d ",i);
                key=1000+i;
                tmp=Create_Test_Node(key,ptr);
                Hash_Add_Node(table,key,(void *)tmp);
        }

        Print_HashTable(table);

        Test_Del_Hash(table,1000);
        Test_Del_Hash(table,1002);
        Test_Del_Hash(table,1003);
        Test_Del_Hash(table,10009);
        Print_HashTable(table);

        Hash_Table_Free(table);

        return 0;
}

