#ifndef PHOUND_TYPES
#include "phound_types.h"
#endif

static ThreadNode *thread_list;
ThreadNode * thread_list_init_node();
int thread_list_add_node(ThreadNode *);
int thread_list_size();
pthread_t * thread_list_get_thread(ThreadNode *);
ThreadNode * thread_list_find_thread(char *);