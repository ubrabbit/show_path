/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-26 22:02:35
 *
 */

#include "c_heap.h"


static void Heap_Swap(Heap_Base* heap,int idx1,int idx2){
	Heap_Unit *tmp;

	tmp=heap->UnitList[idx2];
	heap->UnitList[idx2]=heap->UnitList[idx1];
	heap->UnitList[idx1]=tmp;

	//fprintf(stdout, "swap: %d %d\n",idx1,idx2);
}


static void Heap_Adjust(Heap_Base* heap,int idx,int size){

	Heap_Unit *node,*lnode,*rnode;
	int min,lchild,rchild;
	//fprintf(stdout, "Heap_Adjust %d\t%d\t%d\n",idx,lchild,rchild);
	while(idx!=-1){
		min=idx;
		lchild=idx*2;
		rchild=idx*2+1;

		//叶子节点不需要调整
		if(idx>size/2)
			break;

		lnode=heap->UnitList[lchild];
		rnode=heap->UnitList[rchild];
		node=heap->UnitList[idx];
		if(heap->CmpFunc(node,lnode)==CMP_BIGGER){
			min=lchild;
			node=lnode;
		}
		if(rchild<=size && (heap->CmpFunc(node,rnode)==CMP_BIGGER) ){
			min=rchild;
			node=rnode;
		}

		if(min!=idx){
			Heap_Swap(heap,idx,min);
			idx=min;
		}
		else{
			idx=-1;
		}
	}
}


Heap_Base* CreateHeap(int maxsize,FuncPtr func){
	Heap_Base *heap;

	int size=maxsize+HEAP_SIZE_OFFSET;

	heap=(Heap_Base*)malloc(sizeof(Heap_Base));
	heap->size=maxsize+1;
	heap->idx=0;
	heap->UnitList=(Heap_Unit**)malloc( size* sizeof(Heap_Unit));
	memset(heap->UnitList,'\0',size* sizeof(Heap_Unit));
	heap->CmpFunc=func;

	return heap;
}


int Heap_IsEmpty(Heap_Base* heap){
	return (heap->idx==0)?1:0;
}


void Heap_Reset(Heap_Base *heap){
	heap->idx=0;
	memset(heap->UnitList,'\0',heap->size* sizeof(Heap_Unit));
}


void BuildHeap(Heap_Base *heap){
	int size=heap->idx;
	int i;

	for(i=size/2;i>=HEAP_IDX_FIRST;i--){
		Heap_Adjust(heap,i,heap->idx);
	}
}


void Heap_Sort(Heap_Base* heap){
	int i;

	BuildHeap(heap);
	for(i=heap->idx;i>=HEAP_IDX_FIRST;i--){
		Heap_Swap(heap,i,HEAP_IDX_FIRST);
		Heap_Adjust(heap,HEAP_IDX_FIRST,i-1);
	}
}


void* Heap_GetTop(Heap_Base* heap){
	if(Heap_IsEmpty(heap))	return NULL;
	return heap->UnitList[HEAP_IDX_FIRST]->data_ptr;
}


void Heap_Insert(Heap_Base* heap,void *value){
	Heap_Unit *node;

	node=(Heap_Unit*)malloc(sizeof(Heap_Unit));
	node->data_ptr=value;

	heap->idx++;
	heap->UnitList[heap->idx]=node;

	BuildHeap(heap);

	return;
}


Heap_Unit* Heap_Pop(Heap_Base* heap){
	Heap_Unit *node;

	if(Heap_IsEmpty(heap)){
		return NULL;
	}

	node=heap->UnitList[HEAP_IDX_FIRST];

	heap->UnitList[HEAP_IDX_FIRST]=heap->UnitList[heap->idx];
	heap->UnitList[heap->idx]=NULL;
	heap->idx--;

	Heap_Adjust(heap,HEAP_IDX_FIRST,heap->idx);

	return node;
}


void Heap_Unit_Free(Heap_Unit *node){
	free(node);
}


void Heap_Pop_Free(Heap_Base* heap){
	Heap_Unit *node;

	node=Heap_Pop(heap);
	if(node) Heap_Unit_Free(node);
}


void Heap_Free(Heap_Base* heap){
	Heap_Unit *node;

	while( heap->idx >= 0 ){
		node=Heap_Index( heap,heap->idx-- );
		Heap_Unit_Free(node);
	}
	heap->CmpFunc=NULL;
	free(heap->UnitList);
	heap->UnitList=NULL;
	free(heap);
}


void* Heap_Index(Heap_Base *heap,int idx){
	void* value;

	if(idx > heap->idx) return NULL;

	value=heap->UnitList[idx]->data_ptr;

	return value;
}
