#include "headers/device_list.h"

void * init_node(void *node){
	
	node = (Node *)malloc(sizeof(Node));
	return (void *)node;
}

Device * get_device(Node *node){
	return node->device;
}

int add_node(void * node){

	if(head == NULL){
		head = node;
		head->previous = NULL;
		head->next = NULL;
		return 0;
	}
	else{
		Node * traveler = head;
		while(traveler->next != NULL){
			traveler = traveler->next;
		}
		/*when the loop ends I'l be at the end of the list*/
		traveler->next = node;
		((Node*)node)->next = NULL;
		((Node*)node)->previous = traveler;
		return 0;
	}

}

int size(){
	int count = 0;
	Node * traveler = (Node*)head;
	while(traveler != NULL){
		count++;
		traveler = traveler->next;
	}
	return count;
}

Node * dlist_find_device(char * device_name){
	
	Node * traveler = (Node*)head;
	
	while(traveler != NULL){
		if(strcmp(traveler->device->device_name, device_name) == 0)
			return traveler;
		traveler = traveler->next;
	}
	free(traveler);
	/*Device not found*/
	return NULL;

}