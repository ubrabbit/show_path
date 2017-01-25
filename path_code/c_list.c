#include "c_list.h"

Link_List* CreateList(void){
	Link_List* list;
	list=(Link_List*)malloc(sizeof(Link_List));
	list->head=NULL;
	list->tail=NULL;
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


void* ListPopNode(Link_List* list){

	List_Node* tmp;
	void *ptr;

	tmp=list->head;
	if( NULL!=tmp ){
		list->head=tmp->next;
		list->head->prev=NULL;
	}

	ptr=tmp->data_ptr;
	ListFreeNode(tmp);
	return tmp;
}

