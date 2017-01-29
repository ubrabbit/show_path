/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-26 22:02:35
 *
 */

#include "c_hash.h"


Hash_Unit * Hash_Node_By_Key(Hash_Table* table, HASH_KEY_TYPE key){
        Hash_Unit *unit;
        long idx,i;

        idx=table->Map_Func(table,key);
        i=0;
        unit=(Hash_Unit *)ListIndexNode(table->KeyList[idx],i++);
        while( NULL != unit ){
                if( unit->key==key ) break;
                unit=(Hash_Unit *)ListIndexNode(table->KeyList[idx],i++);
        }

        if( NULL == unit ) return NULL;

        return unit;
}


Hash_Table* Create_Hash_Table(long size, Hash_Map_Func map_func){
        Hash_Table* table;
        int i;

        assert( NULL != map_func );

        table=(Hash_Table*)malloc(sizeof(Hash_Table));
        table->KeyList=(Link_List**)malloc( sizeof(Link_List*) * ( size + SIZE_OFFSET ) );
        for( i=0; i< ( size + SIZE_OFFSET ); i++ ){
                table->KeyList[i]=CreateList(NULL);
        }
        table->size=size;
        table->Map_Func=map_func;
        return table;
}


void Hash_Node_Free(Hash_Unit * unit){
        if( unit->data_ptr ){
                free( unit->data_ptr );
        }
        unit->data_ptr=NULL;
        free(unit);
}


void Hash_Table_Free(Hash_Table *table){
        Hash_Unit *unit;
        Link_List* list;
        long i;

        for( i=0; i< ( table->size + SIZE_OFFSET ); i++ ){
                list=table->KeyList[i];

                unit=(Hash_Unit *)ListPopNode(list);
                while( NULL != unit ){
                        unit->data_ptr=NULL;
                        Hash_Node_Free(unit);
                        unit=(Hash_Unit *)ListPopNode(list);
                } 

                ListFreeAll(list);
                table->KeyList[i]=NULL;
        }
        free(table->KeyList);
        table->KeyList=NULL;
        table->Map_Func=NULL;
        free(table);
}


int Hash_Add_Node(Hash_Table* table, HASH_KEY_TYPE key, void *ptr){
        long idx;
        Hash_Unit *unit;

        unit=(Hash_Unit*)malloc(sizeof(Hash_Unit));
        unit->key=key;
        unit->data_ptr=ptr;

        idx=table->Map_Func(table,key);
        ListAddNode(table->KeyList[idx],(void *)unit);
        return 1;
}


void*  Hash_Del_Node(Hash_Table* table, HASH_KEY_TYPE key){
        long idx;
        Hash_Unit *unit;
        void *ptr;

        unit=Hash_Node_By_Key(table,key);
        if( NULL == unit ) return NULL;

        idx=table->Map_Func(table,key);
        assert(ListDelNode(table->KeyList[idx],(void*)unit) == 1);

        ptr=unit->data_ptr;
        Hash_Node_Free(unit);
 
        return ptr;
}

