#include "../headers/device_list.h"
#include <stdlib.h>
#include <string.h>

DeviceNode * device_list_init_node(){
	
	DeviceNode * node = (DeviceNode *)malloc(sizeof(DeviceNode));
	return node;
}

Device * device_list_get_device(DeviceNode *node){
	return node->device;
}

int device_list_add_node(DeviceNode * node){

	if(device_list == NULL){
		device_list = node;
		device_list->previous = NULL;
		device_list->next = NULL;
		return 0;
	}
	else{
		DeviceNode * traveler = device_list;
		while(traveler->next != NULL){
			traveler = traveler->next;
		}
		/*when the loop ends I'l be at the end of the list*/
		traveler->next = node;
		node->next = NULL;
		node->previous = traveler;
		return 0;
	}

}

int device_list_size(){
	int count = 0;
	DeviceNode * traveler = (DeviceNode*)device_list;
	while(traveler != NULL){
		count++;
		traveler = traveler->next;
	}
	return count;
}

DeviceNode * device_list_find_device(char * device_name){
	
	DeviceNode * traveler = (DeviceNode*)device_list;
	
	while(traveler != NULL){
		if(strcmp(traveler->device->device_name, device_name) == 0)
			return traveler;
		traveler = traveler->next;
	}
	free(traveler);
	/*Device not found*/
	return NULL;

}