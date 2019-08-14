#include "../headers/thread_list.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

ThreadNode * thread_list_init_node(){
	
	ThreadNode *node = (ThreadNode *)malloc(sizeof(ThreadNode));
	return node;
}

pthread_t * thread_list_get_thread(ThreadNode *node){
	return node->thread;
}

int thread_list_add_node(ThreadNode * node){

	if(thread_list == NULL){
		thread_list = node;
		thread_list->previous = NULL;
		thread_list->next = NULL;
		return 0;
	}
	else{
		ThreadNode * traveler = thread_list;
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

int thread_list_size(){
	int count = 0;
	ThreadNode * traveler = (ThreadNode*)thread_list;
	while(traveler != NULL){
		count++;
		traveler = traveler->next;
	}
	return count;
}

ThreadNode * thread_list_find_thread(char * thread_name){
	
	ThreadNode * traveler = (ThreadNode*)thread_list;
	size_t size = 16;
	char * t_name;
	
	while(traveler != NULL){
		pthread_getname_np(*(traveler->thread), t_name, size);
		if(strcmp(t_name, thread_name) == 0){
			free(t_name);
			return traveler;
		}
		free(t_name);
		traveler = traveler->next;
	}
	free(traveler);
	/*Thread not found*/
	return NULL;

}