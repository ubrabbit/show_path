/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-26 22:02:35
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifndef HEAP_IDX_FIRST
#define HEAP_IDX_FIRST	1
#define HEAP_SIZE_OFFSET	10

#define CMP_BIGGER	1
#define CMP_SMALLER	-1
#define CMP_EQUAL	0
#endif

#ifndef TYPEDEF_HEAP
#define TYPEDEF_HEAP 1
typedef struct Heap_Unit{
	void *data_ptr;
}Heap_Unit;

typedef int (*FuncPtr)(Heap_Unit *node1,Heap_Unit *node2);

typedef struct Heap_Base{
	int size;
	int idx;
	Heap_Unit** UnitList;
	FuncPtr CmpFunc;
}Heap_Base;


Heap_Base* CreateHeap(int maxsize,FuncPtr func);

int Heap_IsEmpty(Heap_Base* heap);

void Heap_Reset(Heap_Base *heap);

void BuildHeap(Heap_Base *heap);

void Heap_Sort(Heap_Base* heap);

void* Heap_GetTop(Heap_Base* heap);

void Heap_Insert(Heap_Base* heap,void *value);

Heap_Unit* Heap_Pop(Heap_Base* heap);

void Heap_Unit_Free(Heap_Unit *node);

void Heap_Pop_Free(Heap_Base* heap);

void Heap_Free(Heap_Base* heap);

void* Heap_Index(Heap_Base *heap,int idx);

#endif
