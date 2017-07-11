#ifndef LINKED_LIST
#define LINKED_LIST

#ifndef TYPES
#include "types.h"
#endif

static Node *head;

/*Function Prototypes*/
Node * init_node(Node *);
Device * get_device(Node *);
int add_node(Node *);
int size();
Node * find_device(char *);

Node * init_node(Node *node){
	
	node = malloc(sizeof(Node));
	return node;
}

Device * get_device(Node *node){
	return node->device;
}

int add_node(Node * node){

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
		node->next = NULL;
		node->previous = traveler;
		free(traveler);
		return 0;
	}

}

int size(){
	int count = 0;
	Node * traveler = head;
	while(traveler != NULL){
		count++;
		traveler = traveler->next;
	}
	return count;
}

Node * find_device(char * device_name){
	
	Node * traveler = head;
	
	while(traveler != NULL){
		if(strcmp(traveler->device->device_name, device_name) == 0)
			return traveler;
		traveler = traveler->next;
	}
	free(traveler);
	/*Device not found*/
	return NULL;

}


#endif
