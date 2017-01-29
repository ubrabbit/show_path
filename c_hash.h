/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-26 22:02:35
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "c_list.h"

#ifndef TYPEDEF_HASH
#define TYPEDEF_HASH 1

#define HASH_KEY_TYPE long
#define SIZE_OFFSET 4

typedef struct Hash_Unit{
        HASH_KEY_TYPE   key;
        void *data_ptr;
}Hash_Unit;

typedef struct Hash_Table Hash_Table;
typedef HASH_KEY_TYPE (*Hash_Map_Func)(Hash_Table* table, HASH_KEY_TYPE key);

struct Hash_Table
{
        long    size;
        Link_List**     KeyList;
        Hash_Map_Func   Map_Func;
};


Hash_Table* Create_Hash_Table(long size, Hash_Map_Func map_func);

void Hash_Node_Free(Hash_Unit * unit);

void Hash_Table_Free(Hash_Table *table);

long Hash_Map_Key(HASH_KEY_TYPE key);

Hash_Unit * Hash_Node_By_Key(Hash_Table* table, HASH_KEY_TYPE key);

int Hash_Add_Node(Hash_Table* table, HASH_KEY_TYPE key, void *ptr);

void*  Hash_Del_Node(Hash_Table* table, HASH_KEY_TYPE key);

#endif
