#ifndef LINKED_LIST
#include "linked_list.h"
#endif

#ifndef TYPES
#include "types.h"
#endif

static Node *head;
Device * get_device(Node *);
Node * dlist_find_device(char *);